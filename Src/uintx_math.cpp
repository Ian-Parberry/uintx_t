/// \file uintx_math.cpp
/// \brief Implementation of some useful unsigned integer math functions.

template<class uint> uint power(uint y, uint z){  
  uint x = 1U;

  while(z > 0){
    if(z & 1)x *= y;
    z >>= 1; 
    y *= y;
  } //while

  return x;
} //power

template<class uint> uint factorial(uint x){
  uint result = 1U;
  uint temp = x;

  while(temp > 1){
    result *= temp; 
    temp -= 1; 
  } //while

  return result;
} //factorial

template<class uint> uint gcd(uint x, uint y){
  if(x < y)std::swap(x, y); //the first one must be no smaller than the second

  while(x != 0){
    const uint temp = x;
    x = y%x; 
    y = temp;
  } //while

  return y;
} //gcd

/// Find the the least significant bit that is set to one.
/// \return The smallest value of i such that (2^i & x) != 0.

template<class uint> uint findfirst1(uint x){
  return x & ~(x - 1);
} //findfirst1

/// Fast computation of Fibonacci numbers using successive doubling.
/// F(2k) = F(k)(2F(k+1)-F(k)) and F(2k+1) = F(k+1)^2 + F(k)^2.

template<class uint> uint fib(uint x){
	uint a = 0;
	uint b = 1;
  uint mask = t(1U) << findfirst1(x);

	while(mask > 0){
		uint d = a*(2*b - a);
		uint e = a*a + b*b;

		a = d; 
    b = e;

		if((mask & x) != 0){
			uint c = a + b;
			a = b; 
      b = c;
		} //if

    mask >>= 1;
	} //while

	return a;
} //fib 

template<class uint> uint sqrt(uint x){
  // Find the most significant bit (msb) of the result.

  uint j = findfirst1(x)/2 + 1U; 
  uint n = uint(1U) << j; //mask for msb of result
  uint n2 = n << j; //mask for msb of result^2

  // Find the least significant bits of the result.
  
  n >>= 1; //mask for next least significant bit of result
  n2 >>= 2;  //mask for next least significant bit of result^2

  uint m = n >> 1; 
  uint m2 = n2 >> 2; //m^2

  uint q = n | m;
  uint q2 = n2 + ((n << (j - 1)) | m2); //q^2
  
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
} //sqrt