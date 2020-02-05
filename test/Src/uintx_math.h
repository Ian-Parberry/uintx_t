/// \file uintx_math.h
/// \brief Declaration of some useful unsigned integer math functions.

#if !defined(__uintx_math__)
#define __uintx_math__

uintx_t power(uintx_t y, uintx_t z); ///< Power.
uintx_t factorial(uintx_t x); ///< Factorial.
uintx_t gcd(uintx_t x, uintx_t y); ///< Greatest common divisor.
uintx_t fib(const uintx_t& x); ///< Fibonacci number.
uintx_t sqrt(const uintx_t& x); ///< Square root.

#endif
