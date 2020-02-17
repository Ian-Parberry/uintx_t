/// \file uintx_t.cpp
/// \brief Implementation of the extensible unsigned integer class.

#include "uintx_t.h"

#include <cmath>
#include <algorithm>

const uint32_t NYBBLES = 2*sizeof(uint32_t); ///< Number of nybbles in a word.
const uint32_t BITS = 4*NYBBLES; ///< Number of bits in a word.

const uintx_t uintx_t::NaN(-1); ///< Extensible unsigned integer not-a-number.

/////////////////////////////////////////////////////////////////////////////
//Constructors and destructors.

#pragma region structors

/// The void constructor creates a extensible unsigned integer equal to zero.

uintx_t::uintx_t(): uintx_t(0){ 
} //void constructor

/// 32-bit unsigned integer constructor.
/// \param i Initial value.

uintx_t::uintx_t(uint32_t i){ 
  m_pData = new uint32_t[1]; 
  m_nSize = 1;
  m_pData[0] = i;
} //uint32_t constructor

/// 32-bit integer constructor.
/// \param i Initial value.

uintx_t::uintx_t(int32_t i){ 
  m_pData = new uint32_t[1]; 
  m_nSize = 1;

  if(i >= 0)
    m_pData[0] = i;

  else{
    m_pData[0] = 0;
    m_bNaN = true;
  } //else
} //uint32_t constructor

/// 64-bit unsigned integer constructor.
/// \param i Initial value.

uintx_t::uintx_t(uint64_t i){ 
  if(i > 0xFFFFFFFF){
    m_pData = new uint32_t[2]; 
    m_nSize = 2;
    m_pData[0] = uint32_t(i) & 0xFFFFFFFF;
    m_pData[1] = uint32_t(i >> 32);
  } //if

  else{
    m_pData = new uint32_t[1]; 
    m_nSize = 1;
    m_pData[0] = uint32_t(i);
  } //else
} //uint64_t constructor
  
/// 64-bit integer constructor.
/// \param i Initial value.

uintx_t::uintx_t(int64_t i){ 
  if(i >= 0)
    *this = uintx_t(uint64_t(i));

  else{ //negative number supplied
    m_pData = new uint32_t[1]; 
    m_nSize = 1;
    m_pData[0] = 0;
    m_bNaN = true;
  } //else
} //int64_t constructor

/// String constructor. See loadstring() for string requirements.
/// \param s String containing initial value in hexadecimal.

uintx_t::uintx_t(const std::string& s){ 
  loadstring(s); //load string
} //string constructor

/// Null-terminated string constructor. See loadstring() for string requirements.
/// \param s Null-terminated string containing initial value in hexadecimal.

uintx_t::uintx_t(const char* s){ 
  loadstring(std::string(s)); //load string
} //null-terminated string constructor

/// Copy constructor.
/// \param x Extensible unsigned integer to be copied.

uintx_t::uintx_t(const uintx_t& x){ 
  m_nSize = x.m_nSize;
  m_pData = new uint32_t[m_nSize]; //grab space
  m_bNaN = x.m_bNaN;

  for(uint32_t i=0; i<m_nSize; i++)
    m_pData[i] = x.m_pData[i]; //load m_pData
} //copy constructor

/// Destructor.

uintx_t::~uintx_t(){ 
  delete [] m_pData; 
  m_pData = nullptr; //safety
} //destructor

#pragma endregion structors

/////////////////////////////////////////////////////////////////////////////
// General purpose functions.

#pragma region general

/// Change the number of words allocated and zero out the value stored.
/// \param size Number of words to allocate.

void uintx_t::reallocate(const uint32_t size){ 
  if(m_nSize != size){ //if change needed
    m_nSize = size; 
    delete [] m_pData; //get rid of old space
    m_pData = new uint32_t[size]; //get new space

    for(uint32_t i=0; i<m_nSize; i++)
      m_pData[i] = 0; //zero it out
  } //if
} //reallocate

/// Increase the amount of space allocated while keeping the value stored.
/// \param size New size in words.

void uintx_t::grow(const uint32_t size){
  if(m_nSize < size){ //if really an increase in m_nSize
    uint32_t* olddata = m_pData; //old m_pData
    uint32_t oldsize = m_nSize; //old m_nSize

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
  int32_t top = m_nSize - 1;

  while(top > 0 && m_pData[top] == 0)
    top--;

  top = std::max(0, top);

  if((uint32_t)top < m_nSize - 1){ //change is needed
    m_nSize = top + 1; //new size
    m_pData = new uint32_t[m_nSize]; //grab new space

    for(int32_t i=0; i<=top; i++) //copy over old digits
      m_pData[i] = olddata[i];

    delete [] olddata; //recycle old space
  } //if
} //normalize

/// Set to a hex value contained in an std::string. Both lower-case and
/// upper case letters are allowed. 0x at the start of the string is optional. 
/// Gets set to NaN if there's an unexpected character in the string.
/// \param s An std::string containing a hex value.

void uintx_t::loadstring(const std::string& s){ 
  m_bNaN = false;  
  
  const size_t n = s.size();
  const uint32_t nSize = (uint32_t)std::ceil((double)n/NYBBLES);

  if(nSize != m_nSize){
    delete [] m_pData;
    m_pData = new uint32_t[m_nSize]; //grab space
  } //if

  for(uint32_t i=0; i<m_nSize; i++)
    m_pData[i] = 0; //clear m_pData

  uint32_t word = m_nSize - 1; //current word in long integer
  const size_t digitcount = s.size(); //number of digits in string

  uint32_t shift = (digitcount%NYBBLES)*4; //shift within word
  if(shift <= 0)shift = BITS; //wrap shift amount

  const size_t i0 = (n >= 2 && s[0] == '0' && s[1] == 'x')? 2: 0; //skip 0x if present
  m_bNaN = false; //optimistically we believe that this is a number

  for(size_t i=i0; i<digitcount && !m_bNaN; i++){ //load each digit from string
    uint32_t digit = 0; //current digit

    //get current digit from hex character

    if(s[i] >= '0' && s[i] <='9') //0-9
      digit = s[i] - '0';

    else if(s[i] >= 'A' && s[i] <= 'F') //A-F
      digit = 10 + s[i] - 'A';

    else if(s[i] >= 'a' && s[i] <= 'f') //a-f
      digit = 10 + s[i] - 'a';

    else m_bNaN = true; //non-hex character encountered

    //put digit into m_pData

    if(!m_bNaN){
      if(shift <= 0){
        shift = BITS;
        word--;
      } //if

      shift = shift - 4;
      m_pData[word] = m_pData[word] | (digit<<shift);
    } //if
  } //for
} //loadstring

/// Compute the number of significant bits in the value stored.
/// \return The number of bits stored.

const uint32_t uintx_t::log2() const{
  if(m_bNaN || m_nSize <= 0)return 1;

  uint32_t word = m_pData[m_nSize - 1]; //most significant word in x
  uint32_t count = 0; //counter

  while(word > 0){
    word >>= 1;
    count++;
  } //while

  return count + (m_nSize - 1)*BITS;
} //log2

#pragma endregion general

/////////////////////////////////////////////////////////////////////////////
//Assignment operators.

#pragma region assignment

/// Assignment operator.
/// \param x Operand.
/// \return Reference after assignment.

uintx_t& uintx_t::operator=(const uintx_t& x){ 
  if(this != &x){ //protect against self assignment
    m_bNaN = x.m_bNaN;
    reallocate(x.m_nSize); //grab enough space
    for(uint32_t i=0; i<m_nSize; i++) //copy over data
      m_pData[i] = x.m_pData[i];
  } //if

  return *this;
} //operator=

#pragma endregion assignment

/////////////////////////////////////////////////////////////////////////////
// Addition operators.

#pragma region addition

/// Add a pair of extensible unsigned integers.
/// \param x First operand.
/// \param y Second operand.
/// \return The sum of the two operands.

const uintx_t operator+(const uintx_t& x, const uintx_t& y){
  uintx_t result = x;
  return result += y;
} //operator+

/// Add a extensible unsigned integer.
/// \param y Operand.
/// \return Reference after addition.

uintx_t& uintx_t::operator+=(const uintx_t& y){ 
  if(y.m_bNaN)
    m_bNaN = true;

  if(!m_bNaN){
    uint32_t left, right, left_msb, right_msb; //operands and their msb's
    uint32_t sum, sum_msb; //sum and its msb
    uint32_t carry = 0; //single-bit carry

    const uint32_t mask_msb = 1 << (BITS-1); //mask for most significant bit
    const uint32_t mask_lsb = ~mask_msb; //mask for all but most significant bit

    uint32_t i; //looping variable
    uint32_t oldsize = m_nSize;

    grow(m_nSize > y.m_nSize? m_nSize: y.m_nSize); //make enough space for result

    for(i=0; i<m_nSize; i++){ //for each word in the result
      //grab a word from each operand
      left = i < oldsize? m_pData[i]: 0;
      right = i < y.m_nSize? y.m_pData[i]: 0;

      //extract the most significant bit (msb) from each
      left_msb = (left & mask_msb) >> (BITS - 1);
      right_msb = (right & mask_msb) >> (BITS - 1);

      //zero out the msb from each
      left = left & mask_lsb;
      right = right & mask_lsb;

      //add them
      sum = left + right + carry;

      //compute carry
      sum_msb = (sum & mask_msb) >> (BITS - 1);
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
  } //if

  return *this;
} //operator+=

#pragma endregion addition

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
  if(y.m_bNaN)
    m_bNaN = true;

  if(!m_bNaN){
    if(y >= *this)
      m_bNaN = true; //subtracting something too big

    else if(y > 0U){
      bool borrow = false; //single-bit borrow

      for(uint32_t i=0; i<m_nSize; i++){ //for each word in the result
        //grab a word from each operand
        uint32_t left = m_pData[i];
        uint32_t right = (i < y.m_nSize)? y.m_pData[i]: 0;

        //subtract them
        if(borrow)
          borrow = ++right == 0; //try to add borrow to right

        m_pData[i] = left - right; //subtraction of uint32_t borrows automatically

        if(left < right)
          borrow = true;
      } //for
    } //else

    normalize();
  } //if

  return *this;
} //operator-=

#pragma endregion subtraction

/////////////////////////////////////////////////////////////////////////////
// Increment and decrement operators.

#pragma region increment

/// Prefix increment operator.
/// \return Reference after increment.

uintx_t& uintx_t::operator++(){
  (*this) += 1;
  return *this;
} //operator++

/// Post increment operator.
/// \return Value before increment.

uintx_t uintx_t::operator++(int){
  uintx_t temp = *this;
  ++*this;
  return temp;
} //operator++

/// Prefix decrement operator.
/// \return Reference after decrement.
    
uintx_t& uintx_t::operator--(){
  (*this) -= 1;
  return *this;
} //operator--

/// Post decrement operator.
/// \return Value before decrement.

uintx_t uintx_t::operator--(int){
  uintx_t temp = *this;
  --(*this);
  return temp;
} //operator--

#pragma endregion increment

/////////////////////////////////////////////////////////////////////////////
// Comparison operators.

#pragma region comparison

/// Greater than test. Assumes that both operands are normalized.
/// \param x First operand.
/// \param y Second operand.
/// \return true If the first operand is greater than the second.

bool operator>(const uintx_t& x, const uintx_t& y){ 
  if(x.m_bNaN)return false;
  if(y.m_bNaN)return true;

  if(x.m_nSize > y.m_nSize)return true; 
  if(x.m_nSize < y.m_nSize)return false; 

  //check m_pData
  for(int32_t i=x.m_nSize-1; i>=0; i--)
    if(x.m_pData[i] > y.m_pData[i])return true; 
    else if(x.m_pData[i] < y.m_pData[i])return false; 

  return false; //they're equal
} //operator>

/// Greater than or equal to test.
/// \param x First operand.
/// \param y Second operand.
/// \return true If the first operand is greater than or equal to the second.

bool operator>=(const uintx_t& x, const uintx_t& y){ 
  if(y.m_bNaN)return true;
  if(x.m_bNaN)return false;
  if(x.m_nSize > y.m_nSize)return true; 
  if(x.m_nSize < y.m_nSize)return false; 

  //check m_pData
  for(int32_t i=x.m_nSize-1; i>=0; i--)
    if(x.m_pData[i] >y .m_pData[i])return true; //x>y
    else if(x.m_pData[i] < y.m_pData[i])return false; //x<y

  return true; //they're equal
} //operator>=

/// Less than test.
/// \param x First operand.
/// \param y Second operand.
/// \return true If the first operand is less than the second.

bool operator<(const uintx_t& x, const uintx_t& y){ 
  return y > x;
} //operator<

/// Less than or equal to test.
/// \param x First operand.
/// \param y Second operand.
/// \return true If the first operand is less than or equal to the second.

bool operator<=(const uintx_t& x, const uintx_t& y){ 
  return y >= x;
} //operator<=

/// Equality test.
/// \param x First operand.
/// \param y Second operand.
/// \return true If the first operand is equal to the second.

bool operator==(const uintx_t& x, const uintx_t& y){ 
  if(x.m_nSize != y.m_nSize)
    return false;

  if(x.m_bNaN && y.m_bNaN)
    return true;

  if(x.m_bNaN || y.m_bNaN)
    return false;

  //check m_pData
  for(int32_t i=x.m_nSize-1; i>=1; i--)
    if(x.m_pData[i] != y.m_pData[i])
      return false;
  
  if(x.m_pData[0] != y.m_pData[0])
    return false;

  return true; //they're equal
} //operator==

/// Inequality test.
/// \param x First operand.
/// \param y Second operand.
/// \return true If the first operand is equal to the second.

bool operator!=(const uintx_t& x, const uintx_t& y){ 
  return !(x == y); 
} //operator!=

#pragma endregion comparison

/////////////////////////////////////////////////////////////////////////////
// Bit shift operators.

#pragma region shift

/// Left-shift operator.
/// \param n Shift distance in bits.
/// \return Reference after left-shifting.

uintx_t& uintx_t::operator<<=(int32_t n){ 
  if(n < 0)
    return *this >>= -n;

  if(*this > 0 && !m_bNaN){
    int32_t oldsize = m_nSize; //save old m_nSize for later

    //compute new number of bits - divide by BitsPerWord and round up
    grow((log2() + n + BITS - 1)/BITS);

    //shift by word
    int32_t dest = m_nSize - 1; //copy destination
    int32_t src = dest - n/BITS; //copy source

    while(src >= 0){ //until end of source
      if(src < oldsize)
        m_pData[dest] = m_pData[src]; //copy
        dest--; src--; //move along
    } //while

    while(dest >= 0)
      m_pData[dest--] = 0; //fill bottom with zeros

    //shift within words
    const uint32_t d = n%BITS; //shift distance within words

    if(d > 0)
      for(dest=m_nSize - 1; dest>=0; --dest){
        m_pData[dest] <<= d;
        if(dest > 0)
          m_pData[dest] = m_pData[dest] | (m_pData[dest - 1] >> (BITS - d));
      } //for
  } //if

  return *this;
} //operator<<=

/// Left-shift operator.
/// \param x First operand.
/// \param d Second operand.
/// \return The first operand left-shifted by the second operand.

const uintx_t operator<<(const uintx_t& x, int32_t d){ 
  return uintx_t(x) <<= d;
} //operator<<

/// Right-shift operator.
/// \param n Number of bits to right-shift by.
/// \return Reference after right-shifting.

uintx_t& uintx_t::operator>>=(const int32_t n){ 
  if(n < 0)
    return *this >>= -n;

  if(!m_bNaN){
    //uint32_t newsize = (this->bitsize() - n + BITS - 1)/BITS;
    uint32_t newsize = m_nSize - n/BITS;

    if(newsize <= 0)
      *this = 0;

    else{
      //first shift by word
      uint32_t dest = 0; //copy destination
      uint32_t src = dest + n/BITS; //copy source

      if(dest != src)
        while(src < m_nSize){ //until end of source
          m_pData[dest] = m_pData[src]; //copy
          m_pData[src] = 0; //zero out copied word
          dest++; src++; //move along
        } //while

      //then shift within words
      const uint32_t d = n%BITS; //shift distance within words

      if(d > 0)
        for(dest=0; dest<newsize; dest++){
          m_pData[dest] >>= d;
          if(dest < m_nSize - 1)
            m_pData[dest] = m_pData[dest] | (m_pData[dest + 1] << (BITS - d));
        } //for
    } //else

    for(uint32_t i=newsize; i<m_nSize; i++)
      m_pData[i] = 0; //zero out unused portion

    normalize(); //remove leading zero words
  } //if

  return *this;
} //operator>>=

/// Right-shift operator.
/// \param x First operand.
/// \param d Second operand.
/// \return The first operand left-shifted by the second operand.

const uintx_t operator>>(const uintx_t& x, int32_t d){ 
  return uintx_t(x) >>= d;
} //operator>>

#pragma endregion shift

/////////////////////////////////////////////////////////////////////////////
// Logical operators.

#pragma region bitwise

/// Bitwise conjunction operator.
/// \param y Operand.
/// \return Reference after ANDing with operand.

uintx_t& uintx_t::operator&=(const uintx_t& y){
  if(y.m_bNaN)
    m_bNaN = true;

  if(!m_bNaN){
    //const uint32_t n = std::min(m_nSize, y.m_nSize);
    //grow(n);

    for(uint32_t i=0; i<m_nSize; i++)
      m_pData[i] &= y.m_pData[i];
  } //if

  normalize();

  return *this;
} //operator&=

/// Bitwise conjunction operator.
/// \param x First operand.
/// \param y Second operand.
/// \return The first operand ANDed with the second.

const uintx_t operator&(const uintx_t& x, const uintx_t& y){
  return uintx_t(x) &= y;
} //operator&

/// Bitwise disjunction operator.
/// \param y Operand.
/// \return Reference after ORing with operand.

uintx_t& uintx_t::operator|=(const uintx_t& y){
  if(y.m_bNaN)
    m_bNaN = true;

  if(!m_bNaN){
    const uint32_t n = std::min(m_nSize, y.m_nSize);
    grow(n);

    for(uint32_t i=0; i<n; i++)
      m_pData[i] |= y.m_pData[i];
  } //if

  return *this;
} //operator|=

/// Bitwise disjunction operator.
/// \param x First operand.
/// \param y Second operand.
/// \return The first operand ORed with the second.

const uintx_t operator|(const uintx_t& x, const uintx_t& y){
  return uintx_t(x) |= y;
} //operator|

/// Bitwise exclusive-or operator.
/// \param y Operand.
/// \return Reference after XORing with operand.

uintx_t& uintx_t::operator^=(const uintx_t& y){
  if(y.m_bNaN)
    m_bNaN = true;

  if(!m_bNaN){
    const uint32_t n = std::min(m_nSize, y.m_nSize);
    grow(n);

    for(uint32_t i=0; i<n; i++)
      m_pData[i] ^= y.m_pData[i];
  } //if

  normalize();

  return *this;
} //operator^=

/// Bitwise exclusive-or operator.
/// \param x First operand.
/// \param y Second operand.
/// \return The first operand XORed with the second.

const uintx_t operator^(const uintx_t& x, const uintx_t& y){
  return uintx_t(x) ^= y;
} //operator^

/// Bitwise negation.
/// \param x Operand.
/// \return The operand with all bits flipped.

const uintx_t operator~(const uintx_t& x){
  uintx_t result(x);

  if(!result.m_bNaN)
    for(uint32_t i=0; i<x.m_nSize; i++)
      result.m_pData[i] = ~result.m_pData[i];

  result.normalize();

  return result;
} //operator~

#pragma endregion bitwise

/////////////////////////////////////////////////////////////////////////////
// Multiplication operators.

#pragma region multiplication

/// Multiplication operator.
/// \param y First operand.
/// \param z Second operand.
/// \return The first operand multiplied by the second.

const uintx_t operator*(const uintx_t& y, const uintx_t& z){ 
  if(y.m_bNaN || z.m_bNaN)
    return uintx_t::NaN;

  uintx_t result(0); //return result

  if(y.m_nSize == 1 && z.m_nSize == 1)
    result = to_uint64(y)*to_uint64(z);

  else if(z.m_nSize == 1)
    result = z*y;

  else if(y.m_nSize == 1){  //special case, y is a single word and z is not
    uintx_t word, carry(0);
    const uint64_t y64 = to_uint64(y);

    word.grow(z.m_nSize);
    carry.grow(z.m_nSize + 1);

    for(uint32_t i=0; i<z.m_nSize; i++){
      const uint64_t product = (uint64_t)z.m_pData[i]*y64;
      carry.m_pData[i + 1] = uint32_t(product >> 32);
      word.m_pData[i] = uint32_t(product & 0xFFFFFFFF);
    } //for

    word.normalize();
    carry.normalize();

    result = word + carry;
  } //if

  else{ //general case, both operands have 2 or more words
    uintx_t y0(y), z0(z); //local copies

    while(z0 > 0){
      result += uintx_t(y0*z0.m_pData[0]);

      y0 <<= BITS;
      z0 >>= BITS;
    } //while
  } //else

  return result;
} //operator*

/// Multiplication operator.
/// \param y Operand.
/// \return Reference after multiplication by the second.

uintx_t& uintx_t::operator*=(const uintx_t& y){ 
  return *this = (*this)*y;
} //operator*=

#pragma endregion multiplication

/////////////////////////////////////////////////////////////////////////////
// Division and remainder operators.

#pragma region division

/// Division operator, rounding down.
/// \param y First operand.
/// \param z Second operand.
/// \return The first operand divided by the second.

const uintx_t operator/(const uintx_t& y, const uintx_t& z){
  if(y.m_bNaN || z.m_bNaN || z > y)
    return uintx_t::NaN;

  uintx_t q(0); //result

  if(y >= z){
    uintx_t r(y), w(z);

    w <<= y.log2() - z.log2();

    while(w <= y)
      w <<= 1;

    while(w > z){
      q <<= 1; 
      w >>= 1;

      if(w <= r){
        r -= w;
        ++q;
      } //if
    } //while
  } //if

  return q;
} //operator/

/// Division operator, rounding down.
/// \param y Operand.
/// \return Reference after division by the second.

uintx_t& uintx_t::operator/=(const uintx_t& y){ 
  return *this = *this/y;
} //operator/

/// Remainder operator.
/// \param y First operand.
/// \param z Second operand.
/// \return Remainder after the first operand if divided by the second.

const uintx_t operator%(const uintx_t& y, const uintx_t& z){ 
  if(y.m_bNaN || z.m_bNaN)
    return uintx_t::NaN;

  uintx_t result(y);
  uintx_t w(z);

  while(w <= y)
    w <<= 1;

  while(w > z){ 
    w >>= 1;

    if(w <= result)
      result -= w;
  } //while

  return result;
} //operator%

/// Remainder operator.
/// \param y Operand.
/// \return Reference after division by the operand.

uintx_t& uintx_t::operator%=(const uintx_t& y){ 
  return *this = *this%y;
} //operator%=

#pragma endregion division

/////////////////////////////////////////////////////////////////////////////
// Type conversions.

#pragma region conversions

/// Convert to a 32-bit unsigned integer from the least significant word.
/// \param x Operand.
/// \return Least significant 32 bits of the operand.

const uint32_t to_uint32(uintx_t x){
  if(x.m_bNaN)return 0;
  else if(x.m_nSize <= 0)return 0;
  else return x.m_pData[0];
} //to_uint32

/// Convert to a 64-bit unsigned integer from the two least-significant words.
/// \param x Operand.
/// \return Least significant 64 bits of the operand.

const uint64_t to_uint64(uintx_t x){
  if(x.m_bNaN)return 0;
  else if(x.m_nSize <= 1)return to_uint32(x);
  return (uint64_t(x.m_pData[1]) << 32) | uint64_t(x.m_pData[0]);
} //to_uint64

/// Convert to a single-precision floating point number. Note that floats
/// can only store numbers up to approximately \f$3.4 \times 10^{38)\f$,
/// so anything larger than that will be inf.
/// \param x Operand.
/// \return A float that is approximately equal to the operand.

const float to_float(uintx_t x){
  const float m = float(1LL << 32); //multiplier
  float result = 0; //return result

  for(int32_t i=x.m_nSize-1; i>=0; i--)
    result = result*m + float(x.m_pData[i]);

  return result;
} //to_float

/// Convert to a double-precision floating point number. Note that doubles
/// can only store numbers up to approximately \f$1.8 \times 10^{308)\f$,
/// so anything larger than that will be inf.
/// \param x Operand.
/// \return A double that is approximately equal to the operand.

const double to_double(uintx_t x){
  const double m = double(1LL << 32); //multiplier
  double result = 0; //return result

  for(int32_t i=x.m_nSize-1; i>=0; i--)
    result = result*m + double(x.m_pData[i]);

  return result;
} //to_double

/// Convert to a hexadecimal string with "0x" at the front.
/// \param x Operand.
/// \return std::string containing the operand in hexadecimal notation.

const std::string to_hexstring(uintx_t x){  
  if(x.m_bNaN)return std::string("NaN");
  std::string s; //result

  //convert to a backwards hex string, ie. least-significant digit first

  for(uint32_t i=0; i<x.m_nSize; i++){ //for each word, least significant first
    uint32_t n = x.m_pData[i]; //current word
    const uint32_t size = 2*sizeof(uint32_t); //number of digits in a word

    for(auto i=0; i<size; i++){ //for each digit, least-significant first
      const uint32_t digit = n & 0xF; //grab a digit
      const char c = char(digit + (digit < 10? '0': 'A' - 10));
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
  s += "x0"; //hex indicator, backwards

  //reverse the string to make it most significant digit first, and return

  std::reverse(s.begin(), s.end());
  return s;
} //to_hexstring

/// Convert to a decimal string.
/// \param x Operand.
/// \return std::string containing the operand in decimal notation.

const std::string to_string(uintx_t x){  
  if(x.m_bNaN)return std::string("NaN");
  std::string s; //result

  if(x == 0)
    s = "0";

  else{
    while(x > 0){
      const uint32_t digit = to_uint32(x%10);
      const char c = char(digit + '0');
      s += c;
      x /= 10;
    } //while
  } //else
  
  std::reverse(s.begin(), s.end());
  return s;
} //to_string

/// Convert to a comma-separated decimal string.
/// \param x Operand.
/// \return std::string containing the operand in decimal notation with commas.

const std::string to_commastring(uintx_t x){  
  if(x.m_bNaN)return std::string("NaN");

  std::string s = to_string(x); //result
  const int32_t n = (int32_t)s.length() - 3; //position of first comma

  for(int32_t i=n; i>0; i-=3) //every third character
    s.insert(i, ","); //insert a comma

  return s;
} //to_commastring

#pragma endregion conversions
