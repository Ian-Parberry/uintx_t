/// \file uintx_math.cpp
/// \brief Implementation of some useful unsigned integer math functions.
///
/// These math functions don't require any special access to the inner workings
/// of extensible unsigned integers. They just use uintx_t operators, which
/// means that none of them need be a friend to uintx_t.

#include <algorithm>
#include "uintx_t.h"

/// Raise an extensible unsigned integer to the power of another using
/// successive doubling.
/// \param y First operand.
/// \param z Second operand.
/// \return The first operand raised to the power of the second.

const uintx_t powerx(const uintx_t& y, const uintx_t& z){  
  uintx_t x(1), q(y), r(z);

  while(r > 0){
    if((r & 1) == 1)
      x *= q;

    r >>= 1; 
    q *= q;
  } //while

  return x;
} //powerx

/// Square an extensible unsigned integer.
/// \param x Operand.
/// \return The operand squared.

const uintx_t sqrx(const uintx_t& x){
  return x*x;
} //sqrx

/// Compute the factorial of an extensible unsigned integer using the naive
/// algorithm. Yes, I know that there are faster algorithms.
/// \param x Operand.
/// \return Factorial of the operand.

const uintx_t factorialx(const uintx_t& x){
  uintx_t y(x), z(1); 

  while(y > 1){
    z *= y; 
    --y; 
  } //while

  return z;
} //factorialx

/// Find treatest common divisor of two an extensible unsigned integers
/// using Euclid's Algorithm.  Yes, I know that there are faster algorithms.
/// \param x First operand.
/// \param y Second operand.
/// \return Greatest common divisor of the operands.

const uintx_t gcdx(const uintx_t& x, const uintx_t& y){
  if(x == uintx_t::NaN || y == uintx_t::NaN) //not a number
    return uintx_t::NaN;

  uintx_t w(x), z(y); //copies of parameters

  //Euclid's algorithm

  while(w != 0){
    const uintx_t temp = w;
    w = z%w; 
    z = temp;
  } //while

  return z;
} //gcdx

/// Fibonacci numbers computed using successive doubling, that is, using
/// the recurrences \f$F(0) = F(1) = 0\f$ and for all \f$k \geq 1\f$,
/// \f[F(2k) = F(k)(2F(k+1)-F(k))\f] \f[F(2k+1) = F(k+1)^2 + F(k)^2\f].
/// \param x The index of a Fibonacci number.
/// \return The Fibonacci number with that index.

const uintx_t fibx(const uintx_t& x){
	uintx_t a = 0;
	uintx_t b = 1;
  uintx_t mask = uintx_t(1) << (log2x(x) - 1); //most significant bit

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
} //fibx

/// Find the floor of the square root of an extensible unsigned integer.
/// \param x The operand.
/// \return Floor of the square root of the operand.

const uintx_t sqrtx(const uintx_t& x){
  if(x == uintx_t::NaN)return x;

  //start by finding the most significant bit (msb) of the result.

  int32_t j = (log2x(x) - 1)/2 + 1; 

  uintx_t n = uintx_t(1) << j; //mask for msb of result
  uintx_t n2 = n << j; //mask for msb of result^2

  //now compute the rest.
  
  n >>= 1; //mask for next least significant bit of result
  n2 >>= 2;  //mask for next least significant bit of result^2

  uintx_t m = n >> 1; 
  uintx_t m2 = n2 >> 2; //m^2

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
} //sqrtx