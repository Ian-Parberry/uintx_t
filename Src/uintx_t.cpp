/// \file uintx_t.cpp
/// \brief Implementation of the extensible unsigned integer class.

#include "uintx_t.h"

#include <cmath>
#include <algorithm>

const uint32_t HalfBytesInWord = 2*sizeof(uint32_t); ///< Number of nibbles in a word.
const uint32_t BitsInHalfByte = 4; ///< Number of bits in a nibble.
const uint32_t BitsInWord = 8*sizeof(uint32_t); ///< Number of bits in a word

const uintx_t uintx_t::Zero(0U);

/////////////////////////////////////////////////////////////////////////////
//Constructors and destructors.

#pragma region structors

/// The void constructor creates a extensible unsigned integer equal to zero.

uintx_t::uintx_t(): uintx_t(0U){ 
} //void constructor

/// 32-bit unsigned integer constructor.
/// \param i Initial value.

uintx_t::uintx_t(uint32_t i):
  m_nSize(1) //one 32-bit word
{ 
  m_pData = new uint32_t[1]; 
  m_pData[0] = i;
} //uint32_t constructor

/// 64-bit unsigned integer constructor.
/// \param i Initial value.

uintx_t::uintx_t(uint64_t i):
  m_nSize(2) //two 32-bit words
{ 
  m_pData = new uint32_t[2]; 
  m_pData[0] = i & 0xFFFFFFFF;
  m_pData[1] = i >> 32;
} //uint64_t constructor

/// String constructor.
/// \param string String containing initial value in hexadecimal.

uintx_t::uintx_t(const std::string& s){ 
  const size_t n = s.size();
  m_nSize = (uint32_t)std::ceil((double)n/HalfBytesInWord);
  m_pData = new uint32_t[m_nSize]; //grab space

  for(uint32_t i=0; i<m_nSize; i++)
    m_pData[i] = 0; //clear m_pData

  loadstring(s); //load string
} //string constructor

/// Copy constructor.
/// \param x Extensible unsigned integer to be copied.
/// \return Reference to this extensible unsigned integer after copying.

uintx_t::uintx_t(const uintx_t& x){ 
  m_nSize = x.m_nSize;
  m_pData = new uint32_t[m_nSize]; //grab space

  for(uint32_t i=0; i<m_nSize; i++)
    m_pData[i] = x.m_pData[i]; //load m_pData
} //copy constructor

/// Destructor.

uintx_t::~uintx_t(){ 
  delete [] m_pData; 
} //destructor

#pragma endregion structors

/////////////////////////////////////////////////////////////////////////////
// General purpose functions.

#pragma region general

/// Change the number of words allocated and zero out the value stored.
/// \param s Number of words to allocate.

void uintx_t::reallocate(const uint32_t s){ 
  if(m_nSize != s){ //if change needed
    m_nSize = s; 
    delete [] m_pData; 
    m_pData = new uint32_t[s]; //get space

    for(uint32_t i=0; i<m_nSize; i++)
      m_pData[i] = 0; //zero it out
  } //if
} //reallocate

/// Increase the amount of space allocated while keeping the value stored.
/// \param size New size in words.

void uintx_t::grow(const uint32_t size){
  uint32_t* olddata = m_pData; //old m_pData
  uint32_t oldsize = m_nSize; //old m_nSize

  if(m_nSize <= size){ //if really an increase in m_nSize
    m_pData = new uint32_t[size]; //grab new space
    m_nSize = size; 
    
    uint32_t i; 

    for(i=0; i<oldsize; i++)
      m_pData[i] = olddata[i]; //copy over old digits

    for(; i<m_nSize; i++)
      m_pData[i] = 0; //zero out the rest

    delete [] olddata; //recycle old space
  } //if
} //grow

/// Minimize the amount of storage by removing the leading zero words
/// and reallocating a smaller amount of space if possible.

void uintx_t::normalize(){
  uint32_t* olddata = m_pData; //old m_pData
  uint32_t top;

  for(top=m_nSize - 1; top>0 && m_pData[top]==0; top--);

  if(top < m_nSize - 1){ //if really a change in m_nSize
    m_nSize = top + 1; 
    m_pData = new uint32_t[m_nSize]; //grab new space

    for(uint32_t i=0; i<=top; i++)
      m_pData[i] = olddata[i]; //copy over old digits

    delete [] olddata; //recycle old space
  } //if
} //normalize

/// Set the value stored to a hex value.
/// \param string A null-terminated string containing a hex value.

bool uintx_t::loadstring(const std::string& s){ 
  size_t i;
  uint32_t word = m_nSize - 1; //current word in long integer
  const size_t digitcount = s.size(); //number of digits in string

  uint32_t shift = (digitcount%HalfBytesInWord)*BitsInHalfByte; //shift within word
  if(shift <= 0)shift = BitsInWord; //wrap shift amount

  for(i=0; i<m_nSize; i++)
    m_pData[i]=0; //clear m_pData

  for(i=0; i<digitcount; i++){ //load each digit from string
    uint32_t digit = 0;

    //convert string[i] from char to digit

    if(s[i] >= '0' && s[i] <='9')
      digit = s[i] - '0';
    else if(s[i] >= 'A' && s[i] <= 'F')
      digit = 10 + s[i] - 'A';
    else if(s[i] >= 'a' && s[i] <= 'f')
      digit = 10 + s[i] - 'a';
    else return false; //safety

    //compute shift amount

    if(shift <= 0){
      shift = BitsInWord;
      word--;
    } //if

    shift = shift - BitsInHalfByte;

    //load digit into word of m_pData

    m_pData[word] = m_pData[word] | (digit<<shift);
  } //for

  return true;
} //loadstring

/// Compute the number of significant bits in the value stored.
/// \return the number of bits stored.

const uint32_t uintx_t::bitcount() const{
  if(m_nSize <= 0)return 0;

  uint32_t word = m_pData[m_nSize - 1]; //most significant word in x
  uint32_t count = 0; //counter

  while(word > 0){
    word >>= 1;
    count++;
  } //while

  return count + (m_nSize - 1)*BitsInWord;
} //bitcount

#pragma endregion general

/////////////////////////////////////////////////////////////////////////////
//Assignment operators.

#pragma region assignment

/// Assign a extensible unsigned integer.
/// \param x Extensible unsigned integer to be copied.
/// \return Reference to this extensible unsigned integer after copying.

uintx_t& uintx_t::operator=(const uintx_t& x){ 
  if(this != &x){ //protect against self assignment
    reallocate(x.m_nSize); //grab enough space
    for(uint32_t i=0; i<m_nSize; i++) //copy over data
      m_pData[i] = x.m_pData[i];
  } //if

  return *this;
} //operator=

/// Assign a 32-bit unsigned integer.
/// \param i Unsigned integer to be copied.
/// \return Reference to this extensible unsigned integer after copying.

uintx_t& uintx_t::operator=(const uint32_t i){ 
  reallocate(1); 
  *m_pData = i;
  return *this;
} //operator=

/// Assign a 64-bit unsigned integer.
/// \param i Unsigned integer to be copied.
/// \return Reference to this extensible unsigned integer after copying.

uintx_t& uintx_t::operator=(const uint64_t i){ 
  reallocate(2); 
  m_pData[0] = i & 0xFFFFFFFF;
  m_pData[1] = i >> 32;
  return *this;
} //operator=

/// Assign a hex value from a null-terminated string.
/// \param s A null-terminated string containing a hex value.
/// \return Reference to this extensible unsigned integer after copying.

uintx_t& uintx_t::operator=(const std::string& s){ 
  loadstring(s);
  return *this;
} //operator=

#pragma endregion assignment

/////////////////////////////////////////////////////////////////////////////
// Addition operators.

#pragma region addition

/// Add a pair of extensible unsigned integers.
/// \param x A extensible unsigned integer.
/// \param y A extensible unsigned integer.
/// \return x + y.

const uintx_t operator+(const uintx_t& x, const uintx_t& y){
  uintx_t result = x;
  return result += y;
} //operator+

/// Add a extensible unsigned integer.
/// \param y A extensible unsigned integer.
/// \return Reference to this extensible unsigned integer after y has been added.

uintx_t& uintx_t::operator+=(const uintx_t& y){ 
  uint32_t left, right, left_msb, right_msb; //operands and their msb's
  uint32_t sum, sum_msb; //sum and its msb
  uint32_t carry = 0; //single-bit carry

  const uint32_t mask_msb = 1 << (BitsInWord-1); //mask for most significant bit
  const uint32_t mask_lsb = ~mask_msb; //mask for all but most significant bit

  uint32_t i; //looping variable
  uint32_t oldsize = m_nSize;

  grow(m_nSize > y.m_nSize? m_nSize: y.m_nSize); //make enough space for result

  for(i=0; i<m_nSize; i++){ //for each word in the result
    //grab a word from each operand
    left = i < oldsize? m_pData[i]: 0;
    right = i < y.m_nSize? y.m_pData[i]: 0;

    //extract the most significant bit (msb) from each
    left_msb = (left & mask_msb) >> (BitsInWord - 1);
    right_msb = (right & mask_msb) >> (BitsInWord - 1);

    //zero out the msb from each
    left = left & mask_lsb;
    right = right & mask_lsb;

    //add them
    sum = left + right + carry;

    //compute carry
    sum_msb = (sum & mask_msb) >> (BitsInWord - 1);
    sum = sum & mask_lsb;
    carry = left_msb + right_msb + sum_msb; //carry is either 0, 1, 2, or 3 here

    //put leading bit of carry back into sum
    if((carry == 1) || (carry == 3))
      sum = sum | mask_msb;

    m_pData[i] = sum;

    //pass along leading bit of carry
    carry >>= 1;
  } //for

  if(carry >= 1){ //carry of 1 fell out, need more space for result
    grow(m_nSize + 1); //need one more place for carry
    m_pData[m_nSize - 1] = 1; //set most significant digit
  } //if

  return *this;
} //operator+=

#pragma endregion addition

/////////////////////////////////////////////////////////////////////////////
// Comparison operators.

#pragma region comparison

/// Greater than test for two extensible unsigned integers.
/// \param x A extensible unsigned integer.
/// \param y A extensible unsigned integer.
/// \return true If x is greater than y.

bool operator>(const uintx_t& x, const uintx_t& y){ 
  if(x.m_nSize > y.m_nSize)return true; //x>y
  if(x.m_nSize < y.m_nSize)return false; //x<y

  //check m_pData
  for(uint32_t i=x.m_nSize-1; i>=0; i--)
    if(x.m_pData[i] > y.m_pData[i])return true; //x>y
    else if(x.m_pData[i] < y.m_pData[i])return false; //x<y

  return false; //they're equal
} //operator>

/// Greater than test for a extensible unsigned integer and an integer.
/// \param x A extensible unsigned integer.
/// \param y An integer.
/// \return true If x is greater than y.

bool operator>(const uintx_t& x, uint32_t y){ 
  return x > uintx_t(y); //lazy way to do it
} //operator>

/// Greater than or equal to test for two extensible unsigned integers.
/// \param x A extensible unsigned integer.
/// \param y A extensible unsigned integer.
/// \return true If x is greater than or equal to y.

bool operator>=(const uintx_t& x, const uintx_t& y){ 
  if(x.m_nSize > y.m_nSize)return true; //x>y
  if(x.m_nSize < y.m_nSize)return false; //x<y

  //check m_pData
  for(uint32_t i=x.m_nSize-1; i>=0; i--)
    if(x.m_pData[i] >y .m_pData[i])return true; //x>y
    else if(x.m_pData[i] < y.m_pData[i])return false; //x<y

  return true; //they're equal
} //operator>=

/// Greater than or equal to test for a extensible unsigned integer and
/// an integer.
/// \param x A extensible unsigned integer.
/// \param y An integer.
/// \return true If x is greater than or equal to y.

bool operator>=(const uintx_t& x, uint32_t y){ 
  return x >= uintx_t(y); //lazy way to do it
} //operator>=

/// Less than test for two extensible unsigned integers.
/// \param x A extensible unsigned integer.
/// \param y A extensible unsigned integer.
/// \return true If x is less than y.

bool operator<(const uintx_t& x, const uintx_t& y){ 
  return y > x; //lazy again
} //operator<

/// Less than test for a extensible unsigned integer and an integer.
/// \param x A extensible unsigned integer.
/// \param y An integer.
/// \return true If x is less than y.

bool operator<(const uintx_t& x, uint32_t y){ 
  return x < uintx_t(y); //lazy way to do it
} //operator<

/// Less than or equal to test for two extensible unsigned integers.
/// \param x A extensible unsigned integer.
/// \param y A extensible unsigned integer.
/// \return true If x is less than or equal to y.

bool operator<=(const uintx_t& x, const uintx_t& y){ 
  return y >= x; //lazy again
} //operator<=

/// Less than or equal to test for a extensible unsigned integer and an integer.
/// \param x A extensible unsigned integer.
/// \param y An integer.
/// \return true If x is less than or equal to y.

bool operator<=(const uintx_t& x, uint32_t y){ 
  return x <= uintx_t(y); //lazy way to do it
} //operator<=

/// Equality test for two extensible unsigned integers.
/// \param x A extensible unsigned integer.
/// \param y A extensible unsigned integer.
/// \return true If x is equal to y.

bool operator==(const uintx_t& x, const uintx_t& y){ 
  if(x.m_nSize != y.m_nSize)
    return false;

  //check m_pData
  for(uint32_t i=x.m_nSize-1; i>=1; i--)
    if(x.m_pData[i] != y.m_pData[i])
      return false;
  
  if(x.m_pData[0] != y.m_pData[0])
    return false;

  return true; //they're equal
} //operator==

/// Equality test for a extensible unsigned integer and an unsigned integer.
/// \param x A extensible unsigned integer.
/// \param y An unsigned integer.
/// \return true If x is equal to y.

bool operator==(const uintx_t& x, uint32_t y){ 
  return (x.m_nSize == 1) && (x.m_pData[0] == y);
} //operator==

/// Equality test for an unsigned integer and a extensible unsigned integer.
/// \param x An unsigned integer.
/// \param y A extensible unsigned integer.
/// \return true If x is equal to y.

bool operator==(uint32_t x, const uintx_t& y){ 
  return y == x;
} //operator==

/// Unequality test for two extensible unsigned integers.
/// \param x A extensible unsigned integer.
/// \param y A extensible unsigned integer.
/// \return true If x is not equal to y.

bool operator!=(const uintx_t& x, const uintx_t& y){ 
  return !(x == y); 
} //operator!=

/// Unequality test for a extensible unsigned integer and an unsigned integer.
/// \param x A extensible unsigned integer.
/// \param y An unsigned integer.
/// \return true If x is not equal to y.

bool operator!=(const uintx_t& x, uint32_t y){ 
  return !(x == y); 
} //operator!=

/// Unequality test for an unsigned integer and a extensible unsigned integer.
/// \param x An unsigned integer.
/// \param y A extensible unsigned integer.
/// \return true If x is not equal to y.

bool operator!=(uint32_t x, const uintx_t& y){ 
  return !(x == y); 
} //operator!=

#pragma endregion comparison

/////////////////////////////////////////////////////////////////////////////
// Bit shift operators.

#pragma region shift

/// Left-shift this.
/// \param distance Number of bits to left-shift by.
/// \return Reference to this extensible unsigned integer after left-shifting.

uintx_t& uintx_t::operator<<=(uint32_t distance){ 
  //grow to required m_nSize
  if(*this > 0U){
    uint32_t oldsize = m_nSize; //save old m_nSize for later

    //compute new number of bits - divide by BitsPerWord and round up
    grow((this->bitcount() + distance + BitsInWord - 1)/BitsInWord);

    //shift by word
    uint32_t dest = m_nSize - 1; //copy destination
    uint32_t src = dest - distance/BitsInWord; //copy source

    while(src >= 0){ //until end of source
      if(src < oldsize)
        m_pData[dest] = m_pData[src]; //copy
      dest--; src--; //move along
    } //while

    while(dest >= 0)
      m_pData[dest--] = 0; //fill bottom with zeros

    //shift within words
    const uint32_t d = distance%BitsInWord; //shift distance within words

    if(d > 0)
      for(dest=m_nSize - 1; dest>=0; dest--){
        m_pData[dest] <<= d;
        if(dest > 0)
          m_pData[dest] = m_pData[dest] | (m_pData[dest - 1] >> (BitsInWord - d));
      } //for
  } //if

  return *this;
} //operator<<=

/// Left-shift a extensible unsigned integer.
/// \param x A extensible unsigned integer.
/// \param d Number of bits to left-shift by.
/// \return x left-shifted by d bits.

const uintx_t operator<<(const uintx_t& x, uint32_t d){ 
  uintx_t result = x;
  return result <<= d; 
} //operator<<

/// Right-shift this.
/// \param distance Number of bits to right-shift by.
/// \return Reference to this extensible unsigned integer after right-shifting.

uintx_t& uintx_t::operator>>=(const uint32_t distance){ 
  //find new size
  uint32_t newsize = (this->bitcount() - distance + BitsInWord - 1)/BitsInWord;

  if(newsize < 1)
    *this = 0U;

  else{
    //shift by word
    uint32_t dest = 0; //copy destination
    uint32_t src = dest + distance/BitsInWord; //copy source
    if(dest != src)
      while(src < m_nSize){ //until end of source
        m_pData[dest] = m_pData[src]; //copy
        m_pData[src] = 0; //zero out copied word
        dest++; src++; //move along
      } //whilke

    //shift within words
    const uint32_t d = distance%BitsInWord; //shift distance within words
    if(d > 0)
      for(dest=0; dest<newsize; dest++){
        m_pData[dest] >>= d;
        if(dest < m_nSize - 1)
          m_pData[dest] = m_pData[dest] | (m_pData[dest + 1] << (BitsInWord - d));
      } //for
  } //else

  for(uint32_t i=newsize; i<m_nSize; i++)
    m_pData[i] = 0; //zero out unused portion

  this->normalize(); //remove leading zero words

  return *this;
} //operator>>=

/// Right-shift a extensible unsigned integer.
/// \param x A extensible unsigned integer.
/// \param d Number of bits to right-shift by.
/// \return x right-shifted by d bits.

const uintx_t operator>>(const uintx_t& x, uint32_t d){ 
  uintx_t result = x;
  return result >>= d; 
} //operator>>

#pragma endregion shift

/////////////////////////////////////////////////////////////////////////////
// Bitwise operators.

#pragma region bitwise

/// Bitwise AND of two extensible unsigned integers.
/// \param x A extensible unsigned integer.
/// \param y A extensible unsigned integer
/// \return x ANDed with y.

const uintx_t operator&(const uintx_t& x, const uintx_t& y){
  const uint32_t n = std::min(x.m_nSize, y.m_nSize);
  uintx_t result(x);
  result.grow(n);

  for(uint32_t i=0; i<n; i++)
    result.m_pData[i] &= y.m_pData[i];

  return result;
} //operator&

/// Logical Bitwise of two extensible unsigned integers.
/// \param x A extensible unsigned integer.
/// \param y A extensible unsigned integer
/// \return x ORed with y.

const uintx_t operator|(const uintx_t& x, const uintx_t& y){
  const uint32_t n = std::min(x.m_nSize, y.m_nSize);
  uintx_t result(x);
  result.grow(n);

  for(uint32_t i=0; i<n; i++)
    result.m_pData[i] |= y.m_pData[i];

  return result;
} //operator|

/// Bitwise XOR of two extensible unsigned integers.
/// \param x A extensible unsigned integer.
/// \param y A extensible unsigned integer
/// \return x ORed with y.

const uintx_t operator^(const uintx_t& x, const uintx_t& y){
  const uint32_t n = std::min(x.m_nSize, y.m_nSize);
  uintx_t result(x);
  result.grow(n);

  for(uint32_t i=0; i<n; i++)
    result.m_pData[i] ^= y.m_pData[i];

  return result;
} //operator^

#pragma endregion bitwise

/////////////////////////////////////////////////////////////////////////////
// Multiplication operators.

#pragma region multiplication

/// Multiply two extensible unsigned integers.
/// \param y A extensible unsigned integer.
/// \param z A extensible unsigned integer
/// \return y multiplied by z.

const uintx_t operator*(const uintx_t& y, const uintx_t& z){ 
  uintx_t result(0U); //place for returned result
  uintx_t y0(y), z0(z); //local copies

  while(z > 0U){
    result += y0*z0.m_pData[0];
    y0 <<= BitsInWord;
    z0 >>= BitsInWord;
  } //while

  return result;
} //operator*

/// Multiply a extensible unsigned integer by an integer.
/// \param x A extensible unsigned integer.
/// \param y An integer.
/// \return x multiplied by y.

const uintx_t operator*(const uintx_t& x, uint32_t y){ 
  return x*(uintx_t)y;
} //operator*

/// Multiply an unsigned integer by a extensible unsigned integer.
/// \param x An unsigned integer.
/// \param y A extensible unsigned integer.
/// \return x multiplied by y.

const uintx_t operator*(uint32_t x, const uintx_t& y){ 
  return y*x;
} //operator*

/// Multiply by a extensible unsigned integer.
/// \param y A extensible unsigned integer.
/// \return Reference to this extensible unsigned integer after multiplication by y.

uintx_t& uintx_t::operator*=(const uintx_t& y){ 
  return *this = *this * y;
} //operator*=

#pragma endregion multiplication

/////////////////////////////////////////////////////////////////////////////
// Subtraction operators.

#pragma region subtraction

/// Subtract a extensible unsigned integer from a extensible unsigned integer.
/// \param x A extensible unsigned integer.
/// \param y A extensible unsigned integer
/// \return y subtracted from x, if non-negative.

const uintx_t operator-(const uintx_t& x, const uintx_t& y){ 
  uintx_t result(x);
  return result -= y;
} //operator-

/// Subtract a extensible unsigned integer.
/// \param y A extensible unsigned integer.
/// \return Reference to this extensible unsigned integer after y is subtracted.

uintx_t& uintx_t::operator-=(const uintx_t& y){ 
  if(y >= *this)
    *this = 0U; //subtracting something too big

  else if(y > 0U){
    uint32_t left, right; //operands
    bool borrow = false; //single-bit borrow
    uint32_t i; //looping variable

    for(i=0; i<m_nSize; i++){ //for each word in the result
      //grab a word from each operand
      left=m_pData[i];
      right = (i < y.m_nSize)? y.m_pData[i]: 0;

      //subtract them
      if(borrow)
        borrow = ++right == 0; //try to add borrow to right

      m_pData[i] = left - right; //subtraction of uint32_t borrows automatically

      if(left < right)
        borrow = true;
    } //for
  } //else

  normalize();

  return *this;
} //operator-=

#pragma endregion subtraction

/////////////////////////////////////////////////////////////////////////////
// Division and remainder operators.

#pragma region division

/// Divide a extensible unsigned integer by a extensible unsigned integer.
/// \param y A extensible unsigned integer.
/// \param z An unsigned integer.
/// \return x divided by y, rounded down.

const uintx_t operator/(const uintx_t& y, const uintx_t& z){
  uintx_t q(0U); //result

  if(y >= z){
    uintx_t r(y), w(z);

    w <<= y.bitcount() - z.bitcount();

    while(w <= y)
      w <<= 1;

    while(w > z){
      q <<= 1; 
      w >>= 1;

      if(w <= r){
        r -= w;
        q += 1U;
      } //if
    } //while
  } //if

  return q;
} //operator/

/// Divide a extensible unsigned integer by an unsigned integer.
/// \param y A extensible unsigned integer.
/// \param z An unsigned integer.
/// \return y divided by z, rounded down.

const uintx_t operator/(const uintx_t& y, uint32_t z){ 
  return y/uintx_t(z);
} //operator/

/// Divide by a extensible unsigned integer.
/// \param y A extensible unsigned integer.
/// \return Reference to this extensible unsigned integer after division.

uintx_t& uintx_t::operator/=(const uintx_t& y){ 
  return *this = *this/y;
} //operator/

/// Remainder after dividing a extensible unsigned integer by a extensible
/// unsigned integer.
/// \param y A extensible unsigned integer.
/// \param z A extensible unsigned integer.
/// \return The remainder after y is divided by z.

const uintx_t operator%(const uintx_t& y, const uintx_t& z){ 
  uintx_t result(y);
  uintx_t w(z);

  while(w <= y)
    w <<= 1;

  while(w > z){ 
    w >>= 1;

    if(w <= y)
      result -= w;
  } //while

  return result;
} //operator%

/// Remainder after dividing by a extensible unsigned integer.
/// \param y A extensible unsigned integer.
/// \return Reference to this extensible unsigned integer after remaindering.

uintx_t& uintx_t::operator%=(const uintx_t& y){ 
  return *this = *this%y;
} //operator%=

/// Remainder after dividing a extensible unsigned integer by an unsigned integer.
/// \param y A extensible unsigned integer.
/// \param z An unsigned integer.
/// \return The remainder after y is divided by z.

const uintx_t operator%(const uintx_t& y, uint32_t z){ 
  return y%uintx_t(z);
} //operator%

#pragma endregion division

/////////////////////////////////////////////////////////////////////////////
// Type casts.

#pragma region typecast

/// \return Least significant 32-bit unsigned integer.

uintx_t::operator uint32_t() const{
  return m_pData[0];
} //uint32_t

/// Construct a 64-bit unsigned integer from the two least-significant words.
/// \return Least significant 64-bit unsigned integer.

uintx_t::operator uint64_t() const{
  return (uint64_t(m_pData[1]) << 32) | uint64_t(m_pData[0]);
} //uint64_t

/// Convert to a hexadecimal string.
/// \return std::string containing extensible unsigned integer in hex.

uintx_t::operator std::string() const{  
  std::string s; //result

  //convert to a backwards hex string, ie. least-significant digit first

  for(uint32_t i=0; i<m_nSize; i++){ //for each word, least significant first
    uint32_t n = m_pData[i]; //current word
    const uint32_t size = 2*sizeof(uint32_t); //number of digits in a word

    for(auto i=0; i<size; i++){ //for each digit, least-significant first
      uint32_t digit = n & 0xF; //grab a digit
      char c = char(digit + (digit < 10? '0': 'A' - 10));
      s += c; //append to string
      n >>= 4; //next digit
    } //while
  } //for

  //remove leading zeros, which are at the end

  for(auto it=std::prev(s.end()); it!=s.begin() && *it=='0';){
    --it; //prepare for next iteration
    s.pop_back(); //remove leading zero
  } //while

  if(s == "")s = "0"; //safety

  //reverse the string to make it most significant digit first, and return

  std::reverse(s.begin(), s.end());
  return s;
} //string

#pragma endregion typecast
