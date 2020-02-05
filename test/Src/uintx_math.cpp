/// \file uintx_math.cpp
/// \brief Implementation of some useful unsigned integer math functions.

#include <algorithm>
#include "uintx_t.h"

/// Raise an unsigned integer to the power of another.
/// \param y First operand.
/// \param z Second operand.
/// \return The first operand raised to the second.

uintx_t power(uintx_t y, uintx_t z){  
  uintx_t x = 1U;

  while(z > 0U){
    if((z & uintx_t(1U)) != uintx_t(1U))
      x *= y;

    z >>= 1U; 
    y *= y;
  } //while

  return x;
} //power

/// Factorial computation.
/// \param y First operand.
/// \return y!

uintx_t factorial(uintx_t x){
  uintx_t result = 1U;
  uintx_t temp = x;

  while(temp > 1U){
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

  while(x != 0U){
    const uintx_t temp = x;
    x = y%x; 
    y = temp;
  } //while

  return y;
} //gcd

/// Find the the least significant bit that is set to one.
/// \return The smallest value of i such that (2^i & x) != 0.

uintx_t findfirst1(const uintx_t& x){
  return x & ~(x - uintx_t(1U));
} //findfirst1

/// Fast computation of Fibonacci numbers using successive doubling, that is,
/// \f$F(2k) = F(k)(2F(k+1)-F(k)) and F(2k+1) = F(k+1)^2 + F(k)^2\f$.
/// \param x The operand.
/// \return The xth Fibonacci number.

uintx_t fib(const uintx_t& x){
	uintx_t a = 0U;
	uintx_t b = 1U;
  uintx_t mask = findfirst1(x);

	while(mask > uintx_t(0U)){
		uintx_t d = a*(2U*b - a);
		uintx_t e = a*a + b*b;

		a = d; 
    b = e;

		if((mask & x) != 0U){
			const uintx_t c = a + b;
			a = b; 
      b = c;
		} //if

    mask >>= 1U;
	} //while

	return a;
} //fib 

//uintx_t sqrt(uintx_t x){
//  // Find the most significant bit (msb) of the result.
//
//  uintx_t j = findfirst1(x)/2 + 1U; 
//
//  uintx_t n = uintx_t(1U) << j; //mask for msb of result
//  uintx_t n2 = n << j; //mask for msb of result^2
//
//  // Find the least significant bits of the result.
//  
//  n >>= 1; //mask for next least significant bit of result
//  n2 >>= 2;  //mask for next least significant bit of result^2
//
//  uintx_t m = n >> 1; 
//  uintx_t m2 = n2 >> 2; //m^2
//
//  uintx_t q = n | m;
//  uintx_t q2 = n2 + ((n << (j - 1)) | m2); //q^2
//  
//  while(j >= 0 && n2 != x){  
//    if(q2 <= x){
//      n = q; 
//      n2 = q2;
//    } //if
//
//    m >>= 1; 
//    m2 >>= 2; 
//    q = n | m;
//
//    if(--j > 0)
//      q2 = n2 + ((n << (j - 1)) | m2); //q^2
//  } //while
//  
//  return n;
//} //sqrt