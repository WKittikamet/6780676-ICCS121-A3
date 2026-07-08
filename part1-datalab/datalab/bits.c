/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * Waranyu Kittikamet (6780676) (github: WKittikamet)
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* 
 * sign - return 1 if positive, 0 if zero, and -1 if negative
 *  Examples: sign(130) = 1
 *            sign(-23) = -1
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 2
 */
int sign(int x) {
    int is_neg = x >> 31; // Arithmetic right shift to get -1 for negative numbers and 0 otherwise.
    int is_nonzero = !(!x); // Double ! to get 1 for any non-zero number and 0 for zero.
    return is_neg | is_nonzero; // 0 | 1 = 1, 0 | 0 = 0, -1 | 1 = -1
}
/* 
 * anyEvenBit - return 1 if any even-numbered bit in word set to 1
 *   Examples anyEvenBit(0xA) = 0, anyEvenBit(0xE) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyEvenBit(int x) {
  int mask = 0x55;          
  mask = mask | (mask << 8);  
  mask = mask | (mask << 16); 
  return !(!(x & mask));
}
/* 
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
  return ~0; // NOT 0 is 1. In two's complement, all 1s mean -1
}
/* 
 * bitMask - Generate a mask consisting of all 1's 
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {
  int all_ones = ~0;
  int mask_low = all_ones << lowbit; // Mask all 1s from 'lowbit' to 31.
  /* 
    Mask all 1s from 0 to 'highbit'. 
    << highbit << 1 to avoid violating the word-size shift limit.
  */
  int mask_high = ~(all_ones << highbit << 1);
  return mask_low & mask_high;
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  /*
    Since each byte is 8 bits, we need to shift x to the right by n * 8 positions.
    We can compute n * 8 by left-shifting n by 3 (n << 3).
  */
  int shift_amount = n << 3;
  int shifted_x = x >> shift_amount;
  return shifted_x & 0xFF;
}
/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  /*
    Create a mask by arithmetically shifting x right by 31 bits.
    If x is positive or zero, mask will be 0x00000000 (0).
    If x is negative, mask will be 0xFFFFFFFF (-1).
    For positive x: (x + 0) ^ 0 = x
    For negative x: (x + -1) ^ -1 = ~(x - 1) = -x
  */
  int mask = x >> 31;
  return (x + mask) ^ mask;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  // Construct masks for 2-bit, 4-bit, and 8-bit boundaries.
  int m1 = 0x55;
  int m2 = 0x33;
  int m3 = 0x0F;

  m1 = m1 | (m1 << 8);
  m1 = m1 | (m1 << 16); 

  m2 = m2 | (m2 << 8);
  m2 = m2 | (m2 << 16); 

  m3 = m3 | (m3 << 8);
  m3 = m3 | (m3 << 16);

  x = (x & m1) + ((x >> 1) & m1); // Count bits in each 2-bit group.
    
  x = (x & m2) + ((x >> 2) & m2); // Count bits in each 4-bit group.
    
  x = (x + (x >> 4)) & m3; // Count bits in each 8-bit group.
    
  // Add adjacent 8-bit groups, then 16-bit groups.
  x = x + (x >> 8);
  x = x + (x >> 16);
  
  /*
    The maximum possible number of bits is 32, which takes 6 bits to represent.
    Masking with 0x3F (63) isolates our final count.
  */
  return x & 0x3F;
}
/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
  int shift_n = n << 3;
  int shift_m = m << 3;
    
  int byte_n = (x >> shift_n) & 0xFF;
  int byte_m = (x >> shift_m) & 0xFF;
    
  int diff = byte_n ^ byte_m;
  return x ^ (diff << shift_n) ^ (diff << shift_m);
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  /*
    Both 0 and -0 have a sign bit of 0. For any non-zero number (including tmin), 
    either x or -x (~x + 1) will have a sign bit of 1.
    By OR-ing x and -x, the highest bit will be 1 for non-zero numbers and 0 for zero.
    We then arithmetic right shift by 31 to get -1 (all 1s) for non-zero numbers and 0 for zero.
    Adding 1 results in 0 for non-zero numbers and 1 for zero.
  */
  int minus_x = ~x + 1;
  int smeared_sign = (x | minus_x) >> 31;
  return smeared_sign + 1;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1 << 31;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  /*
    To determine if x <= y, we can check the sign of the difference (y - x).
    However, directly computing (y - x) can cause an integer overflow if x and y 
    have opposite signs. We must handle two main scenarios:
    * 1. x and y have different signs (diff_sign == -1):
      Overflow is possible if we subtract, therefore:
      If x is negative (sx == -1) and y is positive, x <= y is always true (1).
      If x is positive (sx == 0) and y is negative, x <= y is always false (0).
    * 2. x and y have the same sign (diff_sign == 0):
      Overflow is impossible when subtracting. We compute y + (~x + 1).
      If the result is non-negative (s_sub == 0), then x <= y is true (1).
      If the result is negative (s_sub == -1), then x <= y is false (0).
    We use bitwise masking to select the correct logic depending on `diff_sign`, 
    producing a final result of -1 (true) or 0 (false), then mask with 1.
  */
  int sx = x >> 31;
  int sy = y >> 31;
  int diff_sign = sx ^ sy; // -1 if signs differ, 0 if signs are the same 

  int sub = y + (~x + 1);
  int s_sub = sub >> 31;  // -1 if (y - x) < 0, 0 if (y - x) >= 0 

  /* If diff_sign is -1, we want sx. If diff_sign is 0, we want ~s_sub. */
  int ans = (diff_sign & sx) | (~diff_sign & ~s_sub);
  return ans & 1;
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
  /*
    Arithmetic right shifting naturally rounds down (toward negative infinity),
    which works for positive numbers. However, integer division requires 
    rounding toward zero. For negative numbers, we must add a bias of (2^n - 1) 
    before shifting to correct this.
    1. Smear the sign bit across a mask (0 for positive, -1 for negative).
    2. Compute the maximum bias for 2^n, which is (1 << n) - 1. We use + ~0 for -1.
    3. & the bias with the sign mask so it only applies to negative numbers.
    4. Add the bias to x and arithmetic shift right by n.
  */
  int sign = x >> 31;
  int bias = sign & ((1 << n) + ~0);
  return (x + bias) >> n;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x + 1; // The arithmetic negative of a number is the number with all of its bits interved (~) and adding 1.
}
/* 
 * greatestBitPos - return a mask that marks the position of the
 *               most significant 1 bit. If x == 0, return 0
 *   Example: greatestBitPos(96) = 0x40
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 70
 *   Rating: 4 
 */
int greatestBitPos(int x) {
  int shifted;
    
  x = x | (x >> 1);
  x = x | (x >> 2);
  x = x | (x >> 4);
  x = x | (x >> 8);
  x = x | (x >> 16);
    
  shifted = (x >> 1) & ~(1 << 31);
  return x ^ shifted;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  /*
    A number is positive if it is both non-negative AND non-zero.
    1. Check if negative: Smear the sign bit using an arithmetic right shift (x >> 31). 
    This gives -1 (all 1s) for negative numbers, and 0 for non-negative numbers.
    2. Check if zero: (!x). This gives 1 if x is 0, and 0 for any non-zero number.
    3. | these two conditions. If x is negative or zero, the result 
    will be non-zero. If x is strictly positive, both sides are 0, so the result is 0.
    4. Apply ! to invert the combined condition, returning 1 only when x > 0.
  */
  return !((x >> 31) | !x);
}
