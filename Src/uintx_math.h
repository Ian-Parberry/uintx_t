/// \file uintx_math.h
/// \brief Declaration of some useful unsigned integer math functions.

#if !defined(__uintx_math__)
#define __uintx_math__

#include "uintx_t.h"

uintx_t powerx(uintx_t, uintx_t); ///< Power.
uintx_t factorialx(uintx_t); ///< Factorial.
uintx_t gcdx(uintx_t, uintx_t); ///< Greatest common divisor.
uintx_t fibx(const uintx_t&); ///< Fibonacci number.
uintx_t sqrtx(const uintx_t&); ///< Square root.

#endif
