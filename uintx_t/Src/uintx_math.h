/// \file uintx_math.h
/// \brief Declaration of some useful uintx_t math functions.

#if !defined(__uintx_math__)
#define __uintx_math__

#include "uintx_t.h"

const uintx_t powx(const uintx_t&, uint32_t); ///< Power.
const uintx_t sqrx(const uintx_t&); ///< Square.
const uintx_t factorialx(uint32_t); ///< Factorial.
const uintx_t exp2x(uint32_t); ///< Power of 2.
const uintx_t fibx(uint32_t); ///< Fibonacci number.

const uintx_t gcdx(const uintx_t&, const uintx_t&); ///< Greatest common divisor.
const uintx_t lcmx(const uintx_t&, const uintx_t&); ///< Least common multiple.

const uintx_t sqrtx(const uintx_t&); ///< Square root.

const uintx_t& maxx(const uintx_t& x, const uintx_t& y); ///< Maximum.
const uintx_t& minx(const uintx_t& x, const uintx_t& y); ///< Minimum.

#endif
