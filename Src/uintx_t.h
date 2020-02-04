/// \file uintx_t.h
/// \brief Declaration of the extensible unsigned integer class.

#ifndef __uintx_t__
#define __uintx_t__

#include <string>

/// \brief The extensible unsigned integer class.
///
/// An extensible unsigned integer can have arbitrary length.

class uintx_t{ 
  private:
    uint32_t* m_pData; ///< Array of 32-bit words, least significant first.
    uint32_t m_nSize; ///< Number of words in m_pData.

    bool loadstring(const std::string&); ///< Load hex string.
    void reallocate(const uint32_t s); ///< Reallocate space for s words.
    void grow(const uint32_t s); ///< Grow space for s words.
    void normalize(); ///< Remove leading zero words.
    const uint32_t bitcount() const; ///< Number of bits.

  public:
    uintx_t(); ///< Constructor.
    uintx_t(uint32_t); ///< Constructor.
    uintx_t(uint64_t); ///< Constructor.
    uintx_t(const std::string&); ///< Constructor.
    uintx_t(const uintx_t&); ///< Copy constructor.

    ~uintx_t(); ///< Destructor

    //assignment operators

    uintx_t& operator=(const uintx_t&); ///< Assignment.
    uintx_t& operator=(const uint32_t); ///< Assignment.
    uintx_t& operator=(const uint64_t); ///< Assignment.
    uintx_t& operator=(const std::string& s); ///< Assignment.

    //addition operators

    uintx_t& operator+=(const uintx_t&); ///< Add to.
    friend const uintx_t operator+(const uintx_t&, const uintx_t&); ///< Addition.

    //multiplication operators

    uintx_t& operator*=(const uintx_t&); ///< Multiply by.
    friend const uintx_t operator*(const uintx_t&, const uintx_t&); ///< Multiplication.
    
    friend const uintx_t operator*(uint32_t, const uintx_t&); ///< Multiplication.
    friend const uintx_t operator*(const uintx_t&, uint32_t); ///< Multiplication.
    friend const uintx_t operator*(uint32_t, const uintx_t&); ///< Multiplication.

    //comparison operators

    friend bool operator>(const uintx_t&, const uintx_t&); ///< Greater than.
    friend bool operator>(const uintx_t&, uint32_t); ///< Greater than.

    friend bool operator>=(const uintx_t&, const uintx_t&); ///< Greater than or equal.
    friend bool operator>=(const uintx_t&, uint32_t); ///< Greater than or equal.

    friend bool operator<(const uintx_t&, const uintx_t&); ///< Less than.
    friend bool operator<(const uintx_t&, uint32_t); ///< Less than.

    friend bool operator<=(const uintx_t&, const uintx_t&); ///< Less than or equal.
    friend bool operator<=(const uintx_t&, uint32_t); ///< Less than or equal.

    friend bool operator==(const uintx_t&, const uintx_t&); ///< Equal to.
    friend bool operator==(const uintx_t&, uint32_t); ///< Equal to.
    friend bool operator==(uint32_t, const uintx_t&); ///< Equal to.

    friend bool operator!=(const uintx_t&, const uintx_t&); ///< Not equal to.
    friend bool operator!=(const uintx_t&, uint32_t); ///< Not equal to.
    friend bool operator!=(uint32_t, const uintx_t&); ///< Not equal to.

    //bit shift operators

    uintx_t& operator<<=(const uint32_t); ///< Left shift by.
    friend const uintx_t operator<<(const uintx_t&, uint32_t); ///< Left shift.

    uintx_t& operator>>=(uint32_t); ///< Right shift by.
    friend const uintx_t operator>>(const uintx_t&, uint32_t); ///< Right shift.

    //bitwise operators

    friend const uintx_t operator&(const uintx_t&, const uintx_t&); ///< Bit-wise AND.
    friend const uintx_t operator|(const uintx_t&, const uintx_t&); ///< Bit-wise OR.
    friend const uintx_t operator^(const uintx_t&, const uintx_t&); ///< Bit-wise XOR.

    //subtraction operators

    uintx_t& operator-=(const uintx_t&); ///< Subtract from.
    friend const uintx_t operator-(const uintx_t&, const uintx_t&); ///< Subtraction.

    //division operators

    uintx_t& operator/=(const uintx_t&); ///< Divide by.
    friend const uintx_t operator/(const uintx_t&, const uintx_t&); ///< Division.
    friend const uintx_t operator/(const uintx_t&, uint32_t); ///< Division.

    uintx_t& operator%=(const uintx_t&); ///< Remainder.
    friend const uintx_t operator%(const uintx_t&, const uintx_t&); ///< Remainder.
    friend const uintx_t operator%(const uintx_t&, uint32_t); ///< Remainder.

    //type casts

    operator uint32_t() const; ///< Cast to uint32_t.
    operator uint64_t() const; ///< Cast to uint64_t.
    operator std::string() const; ///< Cast to std::string.

    //constants

    static const uintx_t Zero; ///< Zero.
}; //uintx_t

#endif
