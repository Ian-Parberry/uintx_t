/// \file uintx_math.cpp
/// \brief Implementation of some useful uintx_t math functions.
///
/// These math functions don't require any special access to the inner workings
/// of extensible unsigned integers. They just use uintx_t operators, which
/// means that none of them need be a friend to uintx_t.

#include <algorithm>
#include <cmath>
#include "uintx_t.h"

/// Raise an extensible unsigned integer to the power of another using
/// successive doubling.
/// \param y Base.
/// \param z Exponent.
/// \return The base raised to the power of the exponent.

const uintx_t powx(const uintx_t& y, uint32_t z){  
  if(y == uintx_t::NaN || z == uintx_t::NaN)
    return uintx_t::NaN; 

  uintx_t x(1), q(y);

  while(z > 0){
    if((z & 1) == 1)
      x *= q;

    z >>= 1; 
    q *= q;
  } //while

  return x;
} //powx

/// Square an extensible unsigned integer.
/// \param x Operand.
/// \return The operand squared.

const uintx_t sqrx(const uintx_t& x){
  return x*x;
} //sqrx

/// Compute the factorial of an extensible unsigned integer using the naive
/// algorithm. Yes, I know that there are faster algorithms.
/// \param n Operand.
/// \return Factorial of the operand.

const uintx_t factorialx(uint32_t n){
  uintx_t z(1); //result

  while(n > 1){
    z *= n; 
    --n; 
  } //while

  return z;
} //factorialx

/// Find the greatest common divisor of two extensible unsigned integers
/// using Euclid's Algorithm.  Yes, I know that there are faster algorithms.
/// \param x First operand.
/// \param y Second operand.
/// \return The greatest common divisor of the operands.

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

/// Find the least common multiple of two extensible unsigned integers.
/// \param x First operand.
/// \param y Second operand.
/// \return The least common multiple of the operands.

const uintx_t lcmx(const uintx_t& x, const uintx_t& y){
  return x*(y/gcdx(x, y));
} //lcmx

/// Fibonacci numbers computed using successive doubling, that is, using
/// the recurrences \f$F(0) = F(1) = 0\f$ and for all \f$k \geq 2\f$,
/// \f[F(2k) = F(k)(2F(k+1)-F(k))\f] \f[F(2k+1) = F(k+1)^2 + F(k)^2\f].
/// \param n The index of a Fibonacci number.
/// \return The Fibonacci number with that index, \f$F(n)\f$.

const uintx_t fibx(uint32_t n){
  uintx_t a(0); //the result
  uintx_t b(1);
  uint32_t mask = 1 << (uint32_t)(std::floor(std::log2(n))); //most significant bit

  while(mask > 0){
    uintx_t d = a*(2*b - a);
    uintx_t e = a*a + b*b;

    a = d; 
    b = e;

    if((mask & n) != 0){
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

  int32_t j = log2x(x)/2 + 1; 

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

/// Extensible unsigned integer power of 2.
/// \param n Exponent.
/// \return 2 raised to the power of the exponent.

const uintx_t exp2x(uint32_t n){
  return uintx_t(1) << n;
} //exp2x

/// Find the maximum of two extensible unsigned integers.
/// \param x First operand.
/// \param y Second operand.
/// \return The largest of the operands.

const uintx_t& maxx(const uintx_t& x, const uintx_t& y){
  return (x >= y)? x: y;
} //maxx

/// Find the minimum of two extensible unsigned integers.
/// \param x First operand.
/// \param y Second operand.
/// \return The smalles of the operands.

const uintx_t& minx(const uintx_t& x, const uintx_t& y){
  return (x <= y)? x: y;
} //minx