/// \file uintx_math.cpp
/// \brief Implementation of some useful unsigned integer math functions.

#include <algorithm>
#include "uintx_t.h"

/// Raise an unsigned integer to the power of another.
/// \tparam uint An unsigned integer type.
/// \param y First operand.
/// \param z Second operand.
/// \return The first operand raised to the second.

template<class uint> uint power(uint y, uint z){  
  uint x = 1U;

  while(z > 0U){
    if(z & 1U)
      x *= y;

    z >>= 1U; 
    y *= y;
  } //while

  return x;
} //power

/// Factorial computation.
/// \tparam uint An unsigned integer type.
/// \param y First operand.
/// \return y!

template<class uint> uint factorial(uint x){
  uint result = 1U;
  uint temp = x;

  while(temp > 1U){
    result *= temp; 
    --temp; 
  } //while

  return result;
} //factorial

/// Greatest common divisor of two unsigned integers.
/// \tparam uint An unsigned integer type.
/// \param y First operand.
/// \param z Second operand.
/// \return Greatest common divisor of the operands.

template<class uint> uint gcd(uint x, uint y){
  if(x < y)
    std::swap(x, y); //the first one must be no smaller than the second

  while(x != 0U){
    const uint temp = x;
    x = y%x; 
    y = temp;
  } //while

  return y;
} //gcd

/// Find the the least significant bit that is set to one.
/// \tparam uint An unsigned integer type.
/// \return The smallest value of i such that (2^i & x) != 0.

template<class uint> uint findfirst1(const uint& x){
  return x & ~(x - 1U);
} //findfirst1

/// Fast computation of Fibonacci numbers using successive doubling, that is,
/// \f$F(2k) = F(k)(2F(k+1)-F(k)) and F(2k+1) = F(k+1)^2 + F(k)^2\f$.
/// \tparam uint An unsigned integer type.
/// \param x The operand.
/// \return The xth Fibonacci number.

template<class uint> uint fib(const uint& x){
	uint a = 0U;
	uint b = 1U;
  uint mask = findfirst1(x);

	while(mask > 0){
		uint d = a*(2U*b - a);
		uint e = a*a + b*b;

		a = d; 
    b = e;

		if((mask & x) != 0U){
			const uint c = a + b;
			a = b; 
      b = c;
		} //if

    mask >>= 1U;
	} //while

	return a;
} //fib 
//
//template<class uint> uint sqrt(uint x){
//  // Find the most significant bit (msb) of the result.
//
//  uint j = findfirst1(x)/2 + 1U; 
//
//  uint n = uint(1U) << j; //mask for msb of result
//  uint n2 = n << j; //mask for msb of result^2
//
//  // Find the least significant bits of the result.
//  
//  n >>= 1; //mask for next least significant bit of result
//  n2 >>= 2;  //mask for next least significant bit of result^2
//
//  uint m = n >> 1; 
//  uint m2 = n2 >> 2; //m^2
//
//  uint q = n | m;
//  uint q2 = n2 + ((n << (j - 1)) | m2); //q^2
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