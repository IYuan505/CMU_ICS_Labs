/* 
  i/printf("Positive part: %x\n", positive_part);
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
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
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


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

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

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
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
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
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  /* XOR of x and y is equal to (~x)&y | x&(~y) 
   * OR of x and y is equal to ~((~x)&(~y))
   * Substitude the x and y in the OR operation with (~x)&y and x&(~y),
   * we get the result of x^y back.
   */
  int not_x_and_y = (~x)&y;
  int not_y_and_x = x&(~y);
  int and_of_not_two = (~not_x_and_y)&(~not_y_and_x);
  int xor = ~and_of_not_two;
  return xor;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  /* tmin is 0x80000000
   * one in two's complement is 0x00000001
   * right shift one by 31 bits, we have 0x80000000, which is tmin
   */
  int one = 1;
  int minimum = one<<31;
  return minimum;
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  /* tmax is 0x7fffffff
   * if x is tmax, x+x=-2, and we also have to check x!=-1
   */
  int x_plus_x = x+x;
  int minus_one = ~0;
  int minus_two = minus_one+minus_one;
  int is_minus_one = !(x^minus_one);
  int is_not_minus_one = !is_minus_one;
  int sum_is_minus_two = !(x_plus_x^minus_two);
  int is_tmax = sum_is_minus_two&is_not_minus_one;
  return is_tmax;
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  /* First, I construct the all_odd_number: 0xAAAAAAAA
   * If all the odd bits in x are 1s, the xor result with 0xAAAAAAAA will result all
   * zero in the odd bits.
   * No matter what remain in the even bits, the xor result of the previous step and 0xAAAAAAAA
   * will be 0x00000000 if all odd-numbered bits in word set to 1.
   * Otherwise, return false.
   */
  int AA = 0xAA;
  int AAAA = (AA<<8)|AA;
  int AAAAAA = (AAAA<<8)|AA;
  int AAAAAAAA = (AAAAAA<<8)|AA;
  int x_on_odd_bits = x^AAAAAAAA;
  int x_intersection_A = x_on_odd_bits&AAAAAAAA;
  int all_odd = !x_intersection_A;
  return all_odd;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  /* We have x + ~x = -1, so -x = ~x + 1
   */
  int minus_x = ~x+1;
  return minus_x;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  /* If the given x is within the range, -x + 0x2F will be negative,
   * and -x + 0x39 will be positive (including zero).
   * All other outputs are not within the range.
   */
  int minus_x = ~x+1;
  int minus_x_plus_0 = minus_x+0x2F;
  int minus_x_plus_9 = minus_x+0x39;
  int one_neg = (minus_x_plus_0>>31)&1;
  int one_pos = !(minus_x_plus_9>>31);
  int within_range = one_pos&one_neg;
  return within_range;
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  /* First, findout the !x. This outputs 0 or 1.
   * We want 0 or 1 in full-form, which means 0 to 0x00000000, 1 to 0xffffffff.
   * We are able to get the full form by computing the -condition = ~condition+1
   * Next, we could use the full form conditon and take the intersection of y and z.
   * If one condition is 0x00000000, the other would be 0xffffffff.
   * Thus, taking the & operation between the condition and y, z and take '|'
   * between them will give the target return value.
   */
  int not_condition = !x;
  int not_condition_full_form = ~not_condition+1;
  int return_value = ((~not_condition_full_form)&y)|(not_condition_full_form&z);
  return return_value;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  /* First, check the signs of two nubmers, if x is neg and y is pos, return 1.
   * If x is pos and y is neg, return 0.
   * Else, continue checking.
   * If x is tmin, then return 1.
   * If they are the same sign, y>=x -> y-x>=0 if x is not tmin.
   */
  int neg_x = (x>>31)&1;
  int pos_y = !(y>>31);
  int satisfy_one = neg_x|pos_y;
  int neg_pos = neg_x&pos_y;
  int x_is_tmin = !((1<<31)^x);
  int minus_x = ~x+1;
  int y_minus_x = y+minus_x;
  int y_minus_x_pos = !(y_minus_x>>31);
  int return_value = satisfy_one&(neg_pos|x_is_tmin|y_minus_x_pos);
  return return_value;
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  /* If x is 0, -x has the same sign as x.
   * But the same equation holds for tmin, thus, we further filter out the tmin case.
   */
  int minus_x = ~x+1;
  int sign_x = x>>31;
  int x_same_sign_minus_x = (sign_x^(minus_x>>31))+1;
  int same_sign_but_not_tmin = x_same_sign_minus_x&(sign_x+1);
  return same_sign_but_not_tmin;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  /* The max operator number is 90, and we have 32 bits in total.
   * Therefore, it is infeasible to perform a linear operation to check every bit.
   * In this function, I use the divide-and-conquer method to improve the efficiency.
   * First, for a fix bit two's complement number, the range of presentation is -2^(k-1) to 2^(k-1)-1.
   * Thus, we could transform the x into positive by ~x and tell the bits of the positive part.
   * We have to use the exactly the same bits to present x and ~x.
   * After we having the positive part, we could check how many bits are in need to represent this number.
   * Iteratively, we check whether the bits is larger than 16 bits, 8 bits, 4 bits, 2 bits, and 1 bit.
   * Noted that 0x00000000 needs 1 bit to represent, thus, the starting point of the total bit would be 1.
   */
  int sign_x_pos = ~((x>>31)+1)+1;
  int remaining_part = (sign_x_pos & x) | ((~sign_x_pos) & (~x));

  int total_bit = 1;

  int larger_than_part = remaining_part>>16;
  int is_larger_than = (~larger_than_part+1)>>31;
  int is_smaller_than = ~is_larger_than;
  // If number is larger than 16 bits, check the first 16 bits, otherwise, check the last 16 bits.
  remaining_part = (is_larger_than & larger_than_part) | (is_smaller_than & remaining_part & ((0xFF<<8)|0xFF));
  total_bit = total_bit + (16&is_larger_than);

  larger_than_part = remaining_part>>8;
  is_larger_than = (~larger_than_part+1)>>31;
  is_smaller_than = ~is_larger_than;
  remaining_part = (is_larger_than & larger_than_part) | (is_smaller_than & remaining_part & 0xFF);
  total_bit = total_bit + (8&is_larger_than);

  larger_than_part = remaining_part>>4;
  is_larger_than = (~larger_than_part+1)>>31;
  is_smaller_than = ~is_larger_than;
  remaining_part = (is_larger_than & larger_than_part) | (is_smaller_than & remaining_part & 0xF);
  total_bit = total_bit + (4&is_larger_than);

  larger_than_part = remaining_part>>2;
  is_larger_than = (~larger_than_part+1)>>31;
  is_smaller_than = ~is_larger_than;
  remaining_part = (is_larger_than & larger_than_part) | (is_smaller_than & remaining_part & 3);
  total_bit = total_bit + (2&is_larger_than);

  larger_than_part = remaining_part>>1;
  is_larger_than = (~larger_than_part+1)>>31;
  is_smaller_than = ~is_larger_than;
  remaining_part = (is_larger_than & larger_than_part) | (is_smaller_than & remaining_part & 1);
  total_bit = total_bit + (1&is_larger_than);

  larger_than_part = remaining_part;
  is_larger_than = (~larger_than_part+1)>>31;
  is_smaller_than = ~is_larger_than;
  remaining_part = (is_larger_than & larger_than_part) | (is_smaller_than & remaining_part & 1);
  total_bit = total_bit + (1&is_larger_than);

  return total_bit;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  /* First, we get the three component of the float. Sign, exponent and fraction.
   * If exponent is equal to 0, which means it is the denormalized value. 
   * Multiplied by 2 is equal to shift fraction by 1.
   * If exponent is within the range of 1 to 254 (inclusive), it is the normalized value.
   * Multiplied by 2 is equal to add exponent by 1.
   * If exponent is 0xff, which means the special value.
   * If it is infinity, multiply infinity by two is infinity.
   * If it is NaN, return the input. Thus, for exponent==0xff, just return the input.
   */
  int sign = !!(uf>>31);
  int exp = (uf&0x7f800000)>>23;
  int frac = (uf&0x7fffff);
  if(exp==0){
    unsigned return_value = (sign<<31)|(exp<<23)|(frac<<1);
    return return_value;
  }
  else if(exp<255){
    unsigned return_value = (sign<<31)|((exp+1)<<23)|(frac);
    return return_value;
  }
  else{
    return uf;
  }  
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  /* First, we get the three components of the float. Sign, exponent and fraction.
   * If the exponent is larger than or equal to 148, 148-127(bias)=31, is out of the range.
   * If the exponent is less than or equal to 126, 126-127=-1, is less than 1, return 0.
   * Else, we have to first find out the real significand M, and right-shift the it by 23 bit.
   * Next, left-shift it by the number of real exponent.
   * After getting the value, let's apply the sign bit to it and return back the value.
   */
  int sign = !!(uf>>31);
  int exp = (uf&0x7f800000)>>23;
  int frac = (uf&0x7fffff);
  if(exp>=148){
    return 0x80000000u;
  }
  else if (exp<=126){
    return 0;
  }
  else{
    int real_M = (0x800000|frac);
    int move = exp-127-23;
    int return_value;
    if (move>0)
      return_value = real_M<<move;
    else
      return_value = real_M>>(~move+1);
    if (sign)
      return_value = ~return_value+1;
    return return_value;
  }
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  /* If x is too large, larger than or equal to 128, is out of the range of float.
   * If x is too small, smaller than or equal to -150, is less than the smallest float.
   * If x is in the normalzed case, it will change the exponent part.
   * If x is in the denormalized case, it will change the fraction part.
   */
  if (x>=128){
    return 0x7f800000;
  }
  else if (x<=-150){
    return 0;
  }
  else{
    if (x>=-126){
      // Normalzied case
      int exp = x+127;
      return exp<<23;
    }
    else{
      // Denormalized case
      int shift = x+149;
      return 1<<shift;
    }
  }
}
