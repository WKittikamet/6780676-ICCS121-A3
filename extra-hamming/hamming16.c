#include "hamming16.h"

/*
* Data structure
*
* +--------+--------+--------+--------+
* | ( 0)EP | ( 1)P  | ( 2)P  | ( 3)D  |  <-- row 0
* +--------+--------+--------+--------+
* | ( 4)P  | ( 5)D  | ( 6)D  | ( 7)D  |  <-- row 1
* +--------+--------+--------+--------+
* | ( 8)P  | ( 9)D  | (10)D  | (11)D  |  <-- row 2
* +--------+--------+--------+--------+
* | (12)D  | (13)D  | (14)D  | (15)D  |  <-- row 3
* +--------+--------+--------+--------+
*          ↑        ↑        ↑        ↑
*        col 0    col 1    col 2    col 3
*/

/*
 * extendedHammingEncode:
 *  - data: lower 11 bits represent the 11-bit data.
 *  - Returns a 16-bit code word with:
 *      - Extended Parity (EP) at bit 0.
 *      - Hamming parity bits at positions 1, 2, 4, 8.
 *      - Data bits at positions 3, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15.
 */
uint16_t extendedHammingEncode(uint16_t data) {
    uint16_t code = 0;

    // Insert the 11 data bits into the proper positions of the 15-bit Hamming code.
    // Extract the 11 data bits from the input word.
    uint16_t d0  = (data >> 0) & 1;
    uint16_t d1  = (data >> 1) & 1;
    uint16_t d2  = (data >> 2) & 1;
    uint16_t d3  = (data >> 3) & 1;
    uint16_t d4  = (data >> 4) & 1;
    uint16_t d5  = (data >> 5) & 1;
    uint16_t d6  = (data >> 6) & 1;
    uint16_t d7  = (data >> 7) & 1;
    uint16_t d8  = (data >> 8) & 1;
    uint16_t d9  = (data >> 9) & 1;
    uint16_t d10 = (data >> 10) & 1;

    //              ⌄Position
    code |= (d0  << 3);   
    code |= (d1  << 5);   
    code |= (d2  << 6);   
    code |= (d3  << 7);   
    code |= (d4  << 9);   
    code |= (d5  << 10);  
    code |= (d6  << 11);  
    code |= (d7  << 12);  
    code |= (d8  << 13);  
    code |= (d9  << 14);  
    code |= (d10 << 15);  

    // Compute the parity bits for positions 1, 2, 4, and 8.
    // P1 covers positions with the least significant bit set in binary: 1, 3, 5, 7, 9, 11, 13, 15
    uint16_t p1 = d0 ^ d1 ^ d3 ^ d4 ^ d6 ^ d8 ^ d10;
    
    // P2 covers positions with the second bit set in binary: 2, 3, 6, 7, 10, 11, 14, 15
    uint16_t p2 = d0 ^ d2 ^ d3 ^ d5 ^ d6 ^ d9 ^ d10;
    
    // P4 covers positions with the third bit set in binary: 4, 5, 6, 7, 12, 13, 14, 15
    uint16_t p4 = d1 ^ d2 ^ d3 ^ d7 ^ d8 ^ d9 ^ d10;
    
    // P8 covers positions with the fourth bit set in binary: 8, 9, 10, 11, 12, 13, 14, 15
    uint16_t p8 = d4 ^ d5 ^ d6 ^ d7 ^ d8 ^ d9 ^ d10;

    // Insert the computed parity bits into the code word.
    code |= (p1 << 1);
    code |= (p2 << 2);
    code |= (p4 << 4);
    code |= (p8 << 8);

    // Compute the overall parity bit (bit 0) as the XOR of bits 1 through 15. Then, set bit 0 accordingly.
    uint16_t ep = 0;
    for (int i = 1; i <= 15; i++) {
        ep ^= (code >> i) & 1;
    }
    code |= (ep << 0);

    return code;
}

/*
 * extendedHammingDecode:
 *  - code: 16-bit Extended Hamming code word.
 *  - errorPos: pointer to an integer to store the bit position of a corrected error.
 *  - Returns the original 11-bit data (in the lower 11 bits).
 *
 * The function should:
 *   1. Recompute the parity bits to form a syndrome.
 *   2. Check the overall parity (bit 0) to confirm if an error occurred.
 *   3. If a single-bit error is detected, correct the error by flipping that bit.
 *   4. Extract and return the 11-bit data.
 */
uint16_t extendedHammingDecode(uint16_t code, int *errorPos) {
    uint16_t correctedCode = code;
    int syndrome = 0;

    // TODO: Recompute parity bits for positions 1, 2, 4, and 8.
    // Combine the parity check results to form a syndrome (a number from 0 to 15).

    // TODO: Check the overall parity bit (bit 0) to decide if an error occurred.
    // If syndrome != 0 and overall parity is incorrect, then a single-bit error occurred.

    // Correct the error by flipping the bit at the position indicated by syndrome.
    // Set *errorPos to the corrected bit position (1-indexed).
    // If no error, set *errorPos to 0.
    // If error detection indicates an uncorrectable error, set *errorPos to -1.

    // TODO: Extract the original 11-bit data from the corrected code word.
    uint16_t data = 0;

    return data;
}

/*
 * introduceError:
 *  - code: a 16-bit code word.
 *  - bitPos: bit position (1-indexed, 1 to 16) to flip.
 *  - Returns the code word with the specified bit flipped.
 */
uint16_t introduceError(uint16_t code, int bitPos) {
    return code ^ (1 << (bitPos - 1));
}