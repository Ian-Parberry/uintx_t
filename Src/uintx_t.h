/// \file uintx_t.h
/// \brief Declaration of the extensible unsigned integer class.

#if !defined(__uintx_t__)
#define __uintx_t__

#include <string>

/// \brief The extensible unsigned integer class.
///
/// Extensible unsigned integers store very large numbers as an array of
/// 32-bit words, least-significant first. Precision is limited by the amount
/// of memory that can be allocated, potentially up to
/// \f$2^{32} = 4,294,967,296\f$ 32-bit words, that is, 
/// \f$2^{37} = 137,438,953,472\f$ bits. All non-zero extensible unsigned
/// integers have a non-zero most-significant word. The amount of storage used
/// adjusts automatically. Any function or operation that results in a
/// negative number will return NaN (Not a Number), as will any function or 
/// operation that acts on NaN.

class uintx_t{ 
  private:
    uint32_t* m_pData = nullptr; ///< Array of 32-bit words.
    uint32_t m_nSize = 0; ///< Number of words in m_pData.
    bool m_bNaN = false; ///< Not a number.

    void loadstring(const std::string&); ///< Load hex string.
    void reallocate(const uint32_t); ///< Reallocate space.
    void grow(const uint32_t); ///< Grow space.
    void normalize(); ///< Remove leading zero words.

  public:
    uintx_t(); ///< Constructor.
    uintx_t(int32_t); ///< Constructor.
    uintx_t(int64_t); ///< Constructor.
    uintx_t(uint32_t); ///< Constructor.
    uintx_t(uint64_t); ///< Constructor.
    uintx_t(const std::string&); ///< Constructor.
    uintx_t(const char*); ///< Constructor.
    uintx_t(const uintx_t&); ///< Copy constructor.

    ~uintx_t(); ///< Destructor

    const uint32_t bitsize() const; ///< Number of bits.

    //assignment operator

    uintx_t& operator=(const uintx_t&); ///< Assignment.

    //increment and decrement operators

    uintx_t& operator++(); ///< Pre-increment.
    uintx_t operator++(int); ///< Post-increment.
    
    uintx_t& operator--(); ///< Pre-decrement.
    uintx_t operator--(int); ///< Post-decrement.

    //addition operators

    uintx_t& operator+=(const uintx_t&); ///< Add to.
    friend const uintx_t operator+(const uintx_t&, const uintx_t&); ///< Addition.

    //subtraction operators
    
    uintx_t& operator-=(const uintx_t&); ///< Subtract from.
    friend const uintx_t operator-(const uintx_t&, const uintx_t&); ///< Subtraction.

    //multiplication operators

    uintx_t& operator*=(const uintx_t&); ///< Multiply by.
    friend const uintx_t operator*(const uintx_t&, const uintx_t&); ///< Multiplication.

    //division operators

    uintx_t& operator/=(const uintx_t&); ///< Divide by.
    friend const uintx_t operator/(const uintx_t&, const uintx_t&); ///< Division.

    uintx_t& operator%=(const uintx_t&); ///< Remainder.
    friend const uintx_t operator%(const uintx_t&, const uintx_t&); ///< Remainder.

    //comparison operators

    friend bool operator>(const uintx_t&, const uintx_t&); ///< Greater than.
    friend bool operator>=(const uintx_t&, const uintx_t&); ///< Greater than or equal.
    friend bool operator<(const uintx_t&, const uintx_t&); ///< Less than.
    friend bool operator<=(const uintx_t&, const uintx_t&); ///< Less than or equal.
    friend bool operator==(const uintx_t&, const uintx_t&); ///< Equal to.
    friend bool operator!=(const uintx_t&, const uintx_t&); ///< Not equal to.

    //bit shift operators

    uintx_t& operator<<=(const int32_t); ///< Left shift by.
    friend const uintx_t operator<<(const uintx_t&, int32_t); ///< Left shift.

    uintx_t& operator>>=(int32_t); ///< Right shift by.
    friend const uintx_t operator>>(const uintx_t&, int32_t); ///< Right shift.

    //bitwise operators
    
    uintx_t& operator&=(const uintx_t&); ///< Bit-wise AND.
    friend const uintx_t operator&(const uintx_t&, const uintx_t&); ///< Bit-wise AND.
    
    uintx_t& operator|=(const uintx_t&); ///< Bit-wise OR.
    friend const uintx_t operator|(const uintx_t&, const uintx_t&); ///< Bit-wise OR.
    
    uintx_t& operator^=(const uintx_t&); ///< Bit-wise XOR.
    friend const uintx_t operator^(const uintx_t&, const uintx_t&); ///< Bit-wise XOR.

    friend const uintx_t operator~(const uintx_t&); ///< Bit-wise negation.

    //type conversions

    friend const std::string to_hexstring(uintx_t x); ///< To hex string.
    friend const std::string to_string(uintx_t x); ///<To decimal string.
    friend const std::string to_commastring(uintx_t x); ///<To comma separated string.

    friend const uint32_t to_uint32(uintx_t x); ///< To 32-bit unsigned int.
    friend const uint64_t to_uint64(uintx_t x); ///< To 64-bit unsigned int.
    friend const float to_float(uintx_t x); ///< To single precision floating point.
    friend const double to_double(uintx_t x); ///< To double precision floating point.

    //constants

    static const uintx_t NaN; ///< Not a number.
}; //uintx_t

#endif
