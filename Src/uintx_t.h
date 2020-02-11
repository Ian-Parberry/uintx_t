/// \file uintx_t.h
/// \brief Declaration of the extensible unsigned integer class.

#if !defined(__uintx_t__)
#define __uintx_t__

#include <string>

/// \brief The extensible unsigned integer class.
///
/// An extensible unsigned integer can have arbitrary length.

class uintx_t{ 
  private:
    uint32_t* m_pData = nullptr; ///< Array of 32-bit words, least significant first.
    uint32_t m_nSize = 0; ///< Number of words in m_pData.
    bool m_bNaN = false; ///< Not a number.

    bool loadstring(const std::string&); ///< Load hex string.
    void reallocate(const uint32_t s); ///< Reallocate space for s words.
    void grow(const uint32_t s); ///< Grow space for s words.
    void normalize(); ///< Remove leading zero words.
    const uint32_t bitcount() const; ///< Number of bits.

  public:
    uintx_t(); ///< Constructor.
    uintx_t(int32_t); ///< Constructor.
    uintx_t(int64_t); ///< Constructor.
    uintx_t(uint32_t); ///< Constructor.
    uintx_t(uint64_t); ///< Constructor.
    uintx_t(const std::string&); ///< Constructor.
    uintx_t(const uintx_t&); ///< Copy constructor.

    ~uintx_t(); ///< Destructor

    //assignment operators

    uintx_t& operator=(const uintx_t&); ///< Assignment.
    uintx_t& operator=(const int32_t); ///< Assignment.
    uintx_t& operator=(const int64_t); ///< Assignment.
    uintx_t& operator=(const std::string& s); ///< Assignment.

    //increment and decrement operators

    uintx_t& operator++(); ///< Pre-increment.
    uintx_t operator++(int); ///< Post-increment.
    
    uintx_t& operator--(); ///< Pre-decrement.
    uintx_t operator--(int); ///< Post-decrement.

    //addition operators

    uintx_t& operator+=(const uintx_t&); ///< Add to.
    friend const uintx_t operator+(const uintx_t&, const uintx_t&); ///< Addition.

    //multiplication operators

    uintx_t& operator*=(const uintx_t&); ///< Multiply by.
    friend const uintx_t operator*(const uintx_t&, const uintx_t&); ///< Multiplication.
    friend const uintx_t operator*(int32_t, const uintx_t&); ///< Multiplication.
    friend const uintx_t operator*(const uintx_t&, int32_t); ///< Multiplication.
    friend const uintx_t operator*(const uintx_t&, uint32_t); ///< Multiplication.
    friend const uintx_t operator*(int32_t, const uintx_t&); ///< Multiplication.

    //comparison operators

    friend bool operator>(const uintx_t&, const uintx_t&); ///< Greater than.
    friend bool operator>(const uintx_t&, uint32_t); ///< Greater than.
    friend bool operator>(const uintx_t&, int32_t); ///< Greater than.

    friend bool operator>=(const uintx_t&, const uintx_t&); ///< Greater than or equal.
    friend bool operator>=(const uintx_t&, uint32_t); ///< Greater than or equal.
    friend bool operator>=(const uintx_t&, int32_t); ///< Greater than or equal.

    friend bool operator<(const uintx_t&, const uintx_t&); ///< Less than.
    friend bool operator<(const uintx_t&, uint32_t); ///< Less than.
    friend bool operator<(const uintx_t&, int32_t); ///< Less than.

    friend bool operator<=(const uintx_t&, const uintx_t&); ///< Less than or equal.
    friend bool operator<=(const uintx_t&, uint32_t); ///< Less than or equal.
    friend bool operator<=(const uintx_t&, int32_t); ///< Less than or equal.

    friend bool operator==(const uintx_t&, const uintx_t&); ///< Equal to.
    friend bool operator==(const uintx_t&, uint32_t); ///< Equal to.
    friend bool operator==(const uintx_t&, int32_t); ///< Equal to.

    friend bool operator!=(const uintx_t&, const uintx_t&); ///< Not equal to.
    friend bool operator!=(const uintx_t&, uint32_t); ///< Not equal to.
    friend bool operator!=(const uintx_t&, int32_t); ///< Not equal to.

    //bit shift operators

    uintx_t& operator<<=(const uint32_t); ///< Left shift by.
    friend const uintx_t operator<<(const uintx_t&, uint32_t); ///< Left shift.
    friend const uintx_t operator<<(const uintx_t&, int32_t); ///< Left shift.

    uintx_t& operator>>=(uint32_t); ///< Right shift by.
    friend const uintx_t operator>>(const uintx_t&, uint32_t); ///< Right shift.
    friend const uintx_t operator>>(const uintx_t&, int32_t); ///< Right shift.

    //bitwise operators

    friend const uintx_t operator&(const uintx_t&, const uintx_t&); ///< Bit-wise AND.
    friend const uintx_t operator|(const uintx_t&, const uintx_t&); ///< Bit-wise OR.
    friend const uintx_t operator^(const uintx_t&, const uintx_t&); ///< Bit-wise XOR.
    friend const uintx_t operator~(const uintx_t&); ///< Bit-wise negation.

    //subtraction operators
    
    uintx_t& operator-=(const uintx_t&); ///< Subtract from.
    uintx_t& operator-=(uint32_t); ///< Subtract from.
    friend const uintx_t operator-(const uintx_t&, const uintx_t&); ///< Subtraction.
    friend const uintx_t operator-(uint32_t, const uintx_t&); ///< Subtraction.
    friend const uintx_t operator-(int32_t, const uintx_t&); ///< Subtraction.
    friend const uintx_t operator-(const uintx_t&, uint32_t); ///< Subtraction.
    friend const uintx_t operator-(const uintx_t&, int32_t); ///< Subtraction.

    //division operators

    uintx_t& operator/=(const uintx_t&); ///< Divide by.
    friend const uintx_t operator/(const uintx_t&, const uintx_t&); ///< Division.
    friend const uintx_t operator/(const uintx_t&, uint32_t); ///< Division.

    uintx_t& operator%=(const uintx_t&); ///< Remainder.
    friend const uintx_t operator%(const uintx_t&, const uintx_t&); ///< Remainder.
    friend const uintx_t operator%(const uintx_t&, uint32_t); ///< Remainder.

    //type conversions and casts

    uint32_t uint32() const; ///< Convert to uint32_t.
    uint64_t uint64() const; ///< Convert to uint64_t.
    operator std::string() const; ///< Cast to std::string.

    //constants

    static const uintx_t Zero; ///< Zero.
    static const uintx_t NaN; ///< Not a number.
}; //uintx_t

#endif
