#include "cacheSim.h"

// In this question, we will assume DRAM will take a 4-byte values starting from 0 to
void init_DRAM()
{
	unsigned int i=0;
	DRAM = malloc(sizeof(char) * DRAM_SIZE);
	for(i=0;i<DRAM_SIZE/4;i++)
	{
		*((unsigned int*)DRAM+i) = i;
	}
}

void printCache()
{
	int i,j,k;
	printf("===== L1 Cache Content =====\n");
	for(i=0;i<2;i++)
	{
		printf("Set %d :", i);
		for(j=0;j<2;j++)
		{
			printf(" {(TAG: 0x%x)", (unsigned int)(L1_cache[i][j].tag));
			for(k=0;k<16;k++)
				printf(" 0x%x,", (unsigned int)(L1_cache[i][j].data[k]));
			printf(" |");
		}
		printf("\n");
	}
	printf("===== L2 Cache Content =====\n");
	for(i=0;i<4;i++)
	{
		printf("Set %d :", i);
		for(j=0;j<4;j++)
		{
			printf(" {(TAG: 0x%x)", (unsigned int)(L2_cache[i][j].tag));
			for(k=0;k<16;k++)
				printf(" 0x%x,", (unsigned int)(L2_cache[i][j].data[k]));
			printf(" |");
		}
		printf("\n");
	}
}

// Global counter to track insertion order for FIFO policy
static u_int32_t fifo_counter = 1;

// Helper function for read_fifo to check a single block (ignores 4-byte crossing logic)
static int L1lookup_single(u_int32_t address) {
    unsigned int set = getL1SetID(address);
    unsigned int tag = getL1Tag(address);
    for (int i = 0; i < 2; i++) {
        if (L1_cache[set][i].tag == tag && L1_cache[set][i].timeStamp != 0) {
            return 1;
        }
    }
    return 0;
}

static int L2lookup_single(u_int32_t address) {
    unsigned int set = getL2SetID(address);
    unsigned int tag = getL2Tag(address);
    for (int i = 0; i < 4; i++) {
        if (L2_cache[set][i].tag == tag && L2_cache[set][i].timeStamp != 0) {
            return 1;
        }
    }
    return 0;
}

// Helper to fetch a 16-byte aligned block from DRAM -> L2 -> L1
static void fetch_block(u_int32_t block_addr) {
    // If it's already in L1, then return.
    if (L1lookup_single(block_addr)) return;

    // 1. Bring to L2 if not present
    if (!L2lookup_single(block_addr)) {
        unsigned int l2_set = getL2SetID(block_addr);
        unsigned int l2_tag = getL2Tag(block_addr);
        
        // Find empty way or the oldest (FIFO) in L2
        int l2_way = 0;
        u_int32_t min_time = 0xFFFFFFFF;
        for (int i = 0; i < 4; i++) {
            if (L2_cache[l2_set][i].timeStamp == 0) { 
                l2_way = i; 
                break; 
            }
            if (L2_cache[l2_set][i].timeStamp < min_time) {
                min_time = L2_cache[l2_set][i].timeStamp;
                l2_way = i;
            }
        }

        // If evicting a valid L2 block, we must back-invalidate it in L1
        if (L2_cache[l2_set][l2_way].timeStamp != 0) {
            u_int32_t evicted_addr = (L2_cache[l2_set][l2_way].tag << 6) | (l2_set << 4);
            unsigned int l1_evicted_set = getL1SetID(evicted_addr);
            unsigned int l1_evicted_tag = getL1Tag(evicted_addr);
            for (int i = 0; i < 2; i++) {
                if (L1_cache[l1_evicted_set][i].tag == l1_evicted_tag && L1_cache[l1_evicted_set][i].timeStamp != 0) {
                    L1_cache[l1_evicted_set][i].timeStamp = 0; // Invalidate
                }
            }
        }

        // Insert into L2
        L2_cache[l2_set][l2_way].tag = l2_tag;
        L2_cache[l2_set][l2_way].timeStamp = fifo_counter++;
        for (int i = 0; i < 16; i++) {
            L2_cache[l2_set][l2_way].data[i] = DRAM[block_addr + i];
        }
    }

    // 2. Bring to L1 from L2
    unsigned int l1_set = getL1SetID(block_addr);
    unsigned int l1_tag = getL1Tag(block_addr);
    
    // Find empty way or the oldest (FIFO) in L1
    int l1_way = 0;
    u_int32_t min_time_l1 = 0xFFFFFFFF;
    for (int i = 0; i < 2; i++) {
        if (L1_cache[l1_set][i].timeStamp == 0) { 
            l1_way = i; 
            break; 
        }
        if (L1_cache[l1_set][i].timeStamp < min_time_l1) {
            min_time_l1 = L1_cache[l1_set][i].timeStamp;
            l1_way = i;
        }
    }

    // Insert into L1
    L1_cache[l1_set][l1_way].tag = l1_tag;
    L1_cache[l1_set][l1_way].timeStamp = fifo_counter++;
    
    // Copy data from L2 to L1
    unsigned int l2_set_find = getL2SetID(block_addr);
    unsigned int l2_tag_find = getL2Tag(block_addr);
    for (int i = 0; i < 4; i++) {
        if (L2_cache[l2_set_find][i].tag == l2_tag_find && L2_cache[l2_set_find][i].timeStamp != 0) {
            for (int j = 0; j < 16; j++) {
                L1_cache[l1_set][l1_way].data[j] = L2_cache[l2_set_find][i].data[j];
            }
            break;
        }
    }
}

u_int32_t read_fifo(u_int32_t address)
{
	// Fetch the primary block (mask out the lowest 4 bits for a 16-byte alignment)
    u_int32_t block1_addr = address & ~0xF;
    fetch_block(block1_addr);
    
    // Check if the 4-byte access spans two blocks, fetch the second if needed
    if ((address % 16) > 12) {
        u_int32_t block2_addr = (address + 3) & ~0xF;
        fetch_block(block2_addr);
    }
    
    // Assemble the 4-byte data from L1 (Little Endian format)
    u_int32_t result = 0;
    for (int i = 0; i < 4; i++) {
        u_int32_t curr_addr = address + i;
        unsigned int set = getL1SetID(curr_addr);
        unsigned int tag = getL1Tag(curr_addr);
        unsigned int offset = curr_addr & 0xF;
        
        unsigned char byte_val = 0;
        for (int w = 0; w < 2; w++) {
            if (L1_cache[set][w].tag == tag && L1_cache[set][w].timeStamp != 0) {
                byte_val = L1_cache[set][w].data[offset];
                break;
            }
        }
        result |= ((u_int32_t)byte_val) << (i * 8);
    }
	return result;
}

int L1lookup(u_int32_t address)
{
	int hit = L1lookup_single(address);
    // If the 4-byte read crosses a 16-byte boundary, check the next block
    if ((address % 16) > 12) {
        hit = hit && L1lookup_single(address + 3);
    }
	return hit;
}

int L2lookup(u_int32_t address)
{
    int hit = L2lookup_single(address);
    // If the 4-byte read crosses a 16-byte boundary, check the next block
    if ((address % 16) > 12) {
        hit = hit && L2lookup_single(address + 3);
    }
	return hit;
}

unsigned int getL1SetID(u_int32_t address)
{
// 16-byte block (4 offset bits), 2 sets (1 bit)
return (address >> 4) & 0x1;
}

unsigned int getL2SetID(u_int32_t address)
{
// 16-byte block (4 offset bits), 4 sets (2 bits)
return (address >> 4) & 0x3;
}

unsigned int getL1Tag(u_int32_t address)
{
// Top 27 bits
return (address >> 5);
}

unsigned int getL2Tag(u_int32_t address)
{
// Top 26 bits
return (address >> 6);
}


void write(u_int32_t address, u_int32_t data)
{
	// Treat as read request first to allocate the block(s) into L1 and L2
	read_fifo(address);
	// Update byte-by-byte through DRAM, L2, and L1
    for (int i = 0; i < 4; i++) {
        u_int32_t curr_addr = address + i;
        unsigned char byte_val = (data >> (i * 8)) & 0xFF;
        
        // 1. Update DRAM
        DRAM[curr_addr] = byte_val;
        
        unsigned int offset = curr_addr & 0xF;

        // 2. Update L2
        unsigned int l2_set = getL2SetID(curr_addr);
        unsigned int l2_tag = getL2Tag(curr_addr);
        for (int w = 0; w < 4; w++) {
            if (L2_cache[l2_set][w].tag == l2_tag && L2_cache[l2_set][w].timeStamp != 0) {
                L2_cache[l2_set][w].data[offset] = byte_val;
                break;
            }
        }
        
        // 3. Update L1
        unsigned int l1_set = getL1SetID(curr_addr);
        unsigned int l1_tag = getL1Tag(curr_addr);
        for (int w = 0; w < 2; w++) {
            if (L1_cache[l1_set][w].tag == l1_tag && L1_cache[l1_set][w].timeStamp != 0) {
                L1_cache[l1_set][w].data[offset] = byte_val;
                break;
            }
        }
    }
	return;
}


int main()
{
	init_DRAM();
	cacheAccess buffer;
	int timeTaken=0;
	FILE *trace = fopen("input.trace","r");
	int L1hit = 0;
	int L2hit = 0;
	cycles = 0;
	while(!feof(trace))
	{
		fscanf(trace,"%d %x %x", &buffer.readWrite, &buffer.address, &buffer.data);
		printf("Processing the request for [R/W] = %d, Address = %x, data = %x\n", buffer.readWrite, buffer.address, buffer.data);

		// Checking whether the current access is a hit or miss so that we can advance time correctly
		if(L1lookup(buffer.address))// L1 Cache hit
		{
			timeTaken = 1;
			L1hit++;
		}
		else if(L2lookup(buffer.address))// L2 Cache hit
		{
			L2hit++;
			timeTaken = 5;
		}
		else timeTaken = 50;
		if (buffer.readWrite) write(buffer.address, buffer.data);
		else read_fifo(buffer.address);
		cycles+=timeTaken;
	}
	printCache();
	printf("Total cycles used = %ld\nL1 hits = %d, L2 hits = %d\n", cycles, L1hit, L2hit);
	fclose(trace);
	free(DRAM);
	return 0;
}




