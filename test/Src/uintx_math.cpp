/// \file uintx_math.cpp
/// \brief Implementation of some useful unsigned integer math functions.

#include <algorithm>
#include "uintx_t.h"

/// Raise an unsigned integer to the power of another.
/// \param y First operand.
/// \param z Second operand.
/// \return The first operand raised to the second.

uintx_t power(uintx_t y, uintx_t z){  
  uintx_t x = 1;

  while(z > 0){
    if((z & 1) == 1)
      x *= y;

    z >>= 1; 
    y *= y;
  } //while

  return x;
} //power

/// Factorial computation.
/// \param y First operand.
/// \return y!

uintx_t factorial(uintx_t x){
  uintx_t result = 1;
  uintx_t temp = x;

  while(temp > 1){
    result *= temp; 
    --temp; 
  } //while

  return result;
} //factorial

/// Greatest common divisor of two unsigned integers.
/// \param y First operand.
/// \param z Second operand.
/// \return Greatest common divisor of the operands.

uintx_t gcd(uintx_t x, uintx_t y){
  if(x < y)
    std::swap(x, y); //the first one must be no smaller than the second

  while(x != 0){
    const uintx_t temp = x;
    x = y%x; 
    y = temp;
  } //while

  return y;
} //gcd

/// Make a mask that is all zeros except the least significant 1-bit.
/// \param x The operand.
/// \return \f$2^i\f$ for the smallest \f$i\f$ such that (\f$2^i\f$ & x) = 1.

uintx_t first1mask(const uintx_t& x){
  return x & ~(x - 1U);
} //first1mask

/// Find the index of the least significant 1-bit.
/// \param x The operand.
/// \return The smallest \f$i\f$ such that (\f$2^i\f$ & x) = 1.

int32_t first1index(const uintx_t& x){
  if(x == 0)return -1;

  int32_t count = 0;
  uintx_t mask(1);

  while((x & mask) == 0){
    mask <<= 1;
    count++;
  } //while

  return count;
} //first1index

/// Fast computation of Fibonacci numbers using successive doubling, that is,
/// \f$F(2k) = F(k)(2F(k+1)-F(k)) and F(2k+1) = F(k+1)^2 + F(k)^2\f$.
/// \param x The operand.
/// \return The xth Fibonacci number.

uintx_t fib(const uintx_t& x){
	uintx_t a = 0;
	uintx_t b = 1;
  uintx_t mask = first1mask(x);

	while(mask > 0){
		uintx_t d = a*(2*b - a);
		uintx_t e = a*a + b*b;

		a = d; 
    b = e;

		if((mask & x) != 0){
			const uintx_t c = a + b;
			a = b; 
      b = c;
		} //if

    mask >>= 1;
	} //while

	return a;
} //fib 

/// Square root.
/// \param x The operand.
/// \return The integer below the square root of the operand.

uintx_t sqrt(const uintx_t& x){
  //start by finding the most significant bit (msb) of the result.

  uint32_t j = first1index(x)/2 + 1; 

  uintx_t n = uintx_t(1) << j; //mask for msb of result
  uintx_t n2 = n << j; //mask for msb of result^2

  //now compute the rest.
  
  n >>= 1; //mask for next least significant bit of result
  n2 >>= 2;  //mask for next least significant bit of result^2

  uintx_t m = n >> 1U; 
  uintx_t m2 = n2 >> 2U; //m^2

  uintx_t q = n | m;
  uintx_t q2 = n2 + ((n << (j - 1)) | m2); //q^2
  
  while(j >= 0 && n2 != x){  
    if(q2 <= x){
      n = q; 
      n2 = q2;
    } //if

    m >>= 1; 
    m2 >>= 2; 
    q = n | m;

    if(--j > 0)
      q2 = n2 + ((n << (j - 1)) | m2); //q^2
  } //while
  
  return n;
} //sqrt