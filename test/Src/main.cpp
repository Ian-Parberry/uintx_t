/// \file main.cpp
/// \brief A test program for uintx_t.

#include "uintx_t.h"
#include "uintx_math.h"

uint32_t g_nTestCount = 0; ///< Number of tests performed.
uint32_t g_nPassCount = 0; ///< Number of tests passed.

//////////////////////////////////////////////////////////////////////////////
// Helper functions

/// \brief Report a test result.
///
/// Print the result of a test (passed or failed) to stdout and increment
/// the test counter and pass counter (in the event of a pass).
///
/// \param result True if the test was passed.

void ReportTestResult(bool result){
  g_nTestCount++;

  if(result){
    printf("Test passed\n\n");
    g_nPassCount++;
  } //if

  else printf("**** Test failed ****\n\n");
} //ReportTestResult

//////////////////////////////////////////////////////////////////////////////
// Test functions

/// \brief Test division and remainder operations.
///
/// Test the extensible unsigned integer division and remainder operations and
/// print the result to stdout. The test performed is 
/// \f$x \bmod y < y\f$ and \f$y\lfloor x/y\rfloor + (x\bmod y) = x\f$.
///
/// \param x Numerator.
/// \param y Denominator.

void TestDivMod(const uintx_t& x, const uintx_t& y){
  printf("Division and remainder test: x%%y < y and (x/y)*y + x%%y == x\n");
  bool result = true;

  printf("x             = %s\n", to_commastring(x).c_str());
  printf("y             = %s\n", to_commastring(y).c_str());

  const uintx_t frac = x/y;
  printf("x/y           = %s\n", to_commastring(frac).c_str());

  const uintx_t rem = x%y;
  printf("x%%y           = %s\n", to_commastring(rem).c_str());
  if(rem >= y)result = false;

  const uintx_t x2 = frac*y + rem;
  printf("(x/y)*y + x%%y = %s\n", to_commastring(x2).c_str());
  if(x2 != x)result = false;

  ReportTestResult(result);
} //TestDivMod

/// \brief Test Fibonacci numbers.
///
/// Test the extensible unsigned integer Fibonacci number function and print
/// the result to stdout. The test performed is \f$F(i) = F(i-1) + F(i-2)\f$,
/// which is a reasonable test because Fibonacci numbers aren't computed this
/// way by fibx(). Only the first few and the last Fibonacci number will be
/// printed to save clutter on the output.
///
/// \param n The number of Fibonacci numbers to be generated.

void TestFibonacci(uint32_t n){
  printf("Fibonacci number test: F(i) = F(i-1) + F(i-2) for 2 <= i <= %u\n", n);
  bool result = true;

  const uint32_t printlimit = 15; //print this many Fibonacci numbers
  uintx_t prev(0), current(0), next(0); //keep last two Fibonacci numbers
  uint32_t i = 0; //iterator 

  //print but don't test the first two Fibonacci numbers
  
  while(i <= 1){
    next = fibx(i++);
    printf("%s ", to_string(next).c_str());
    prev = current; current = next;
  } //while

  //print and test from there up to the print limit
  
  while(i < printlimit){ 
    next = fibx(i++);
    if(next != current + prev)result = false;
    printf("%s ", to_string(next).c_str());
    prev = current; current = next; 
  } //while

  //test at the print limit but print a marker instead

  next = fibx(i++);
  if(next != current + prev)result = false;
  printf("... etc.\n");
  prev = current; current = next;

  //test the rest but don't print anything

  while(i <= n){ 
    next = fibx(i++);
    if(next != current + prev)result = false;
    prev = current; current = next; 
  } //for

  printf("The last Fibonacci number is %s\n", to_commastring(next).c_str());
  ReportTestResult(result);
} //TestFibonacci

/// \brief Test the gcd function.
///
/// Test the extensible unsigned integer greatest common denominator function
/// and print the result to stdout. The test performed is 
/// \f$x \bmod \text{gcd}(x, y) = y \bmod \text{gcd}(x, y) = 0\f$ and 
/// \f$\text{gcd}(x/\text{gcd}(x, y), y/\text{gcd}(x, y)) = 1\f$.
///
/// \param x First extensible unsigned integer.
/// \param y Second extensible unsigned integer.

void TestGcd(const uintx_t& x, const uintx_t& y){
  printf("Gcd test: x%%gcd(x, y) == y%%gcd(x, y) == 0");
  printf(" and gcd(x/gcd(x, y), y/gcd(x, y)) == 1\n");
  bool result = true;

  printf("x = %s\n", to_commastring(x).c_str());
  printf("y = %s\n", to_commastring(y).c_str());

  const uintx_t z = gcdx(x, y);
  printf("gcd(x, y) = %s\n", to_commastring(z).c_str());

  result = x%z == 0 && y%z == 0 && gcdx(x/z, y/z) == 1;
  
  ReportTestResult(result);
} //TestGcd

/// \brief Test powers.
///
/// Test the extensible unsigned integer power function and print the
/// result to stdout. The test performed is 
/// \f[x^k = \underbrace{x\times x \times \ldots \times x}_{k\text{ times}}
/// \text{ for } 2 \leq k \leq 7.\f]
///
/// \param x An extensible unsigned integer.

void TestPower(const uintx_t& x){
  printf("Power test: pow(x, k) = x*x*...*x (k times) for 2 <= k <= 7\n");
  bool result = true;

  printf("x             = %s\n", to_hexstring(x).c_str());

  for(uint32_t k=2; k<=7 && result; k++){
    const uintx_t y = powx(x, k); //what does powx say?
    printf("power(x, %u)   = %s\n", k, to_hexstring(y).c_str());
    uintx_t z(1); //accumulator for multiplications

    for(uint32_t i=0; i<k; i++) //multiply out
      z *= x;

    //print power computed both ways

    printf("x");

    for(uint32_t j=1; j<k; j++)
      printf("*x");

    for(uint32_t j=2*k-1; j<13; j++)
      printf(" ");

    printf(" = %s\n", to_hexstring(z).c_str());

    //is this correct?
    
    result = y == z;
  } //for

  ReportTestResult(result);
} //TestPower

/// \brief Test print.
///
/// Print an extensible unsigned integer to stdout in the following formats:
/// hexadecimal, decimal, decimal with commas, double precision floating point,
/// and single precision floating point.
///
/// \param x An extensible unsigned integer.

void TestPrint(const uintx_t& x){
  printf("Print format test:\n");

  printf("x = %s\n", to_hexstring(x).c_str());
  printf("  = %s\n", to_string(x).c_str());
  printf("  = %s\n", to_commastring(x).c_str());
  printf("  = %.10e (double)\n", to_double(x));
  printf("  = %.10e (float)\n", to_float(x));
  printf("\n");
} //TestPrint

/// \brief Test square roots.
///
/// Test the extensible unsigned integer square root function and print the
/// result to stdout. The test performed is 
/// \f$(\sqrt{x})^2 \leq x \leq (\sqrt{x} + 1)^2\f$.
///
/// \param x An extensible unsigned integer.

void TestSqrt(const uintx_t& x){
  printf("Square root test: sqrt(x)^2 <= x <= (sqrt(x) + 1)^2\n");
  bool result = true;

  printf("x             = %s\n", to_commastring(x).c_str());
  printf("sqrt(x)       = %s\n", to_commastring(sqrtx(x)).c_str());

  uintx_t temp = sqrx(sqrtx(x));
  printf("sqrt(x)^2     = %s\n", to_commastring(temp).c_str());
  if(temp > x)result = false;

  temp = sqrx(sqrtx(x) + 1);
  printf("(sqrt(x)+1)^2 = %s\n", to_commastring(temp).c_str());
  if(temp < x)result = false;

  ReportTestResult(result);
} //TestSqrt

//////////////////////////////////////////////////////////////////////////////
// Main

/// \brief Main.
///
/// Test extensible unsigned integer printing, square roots, division and
/// remainder, Fibonacci number generation, and greatest common divisor
/// on hard-coded parameters. The results are printed to stdout along with
/// the results of the calculations so the user can check them by eye.
///
/// \return 0 (what could possibly go wrong?)

int main(){ 
  TestPrint("0x17322A2CFD320A23266116C4C2C95B3FEEA3E57FA3D9DFE8B8591E1D72120F26C6FADB");

  TestDivMod("0x3DD8FD1EB259B9D5AD5B3487472222EC4F163435177E42A4DC40",
    "0x1CB6EBD33066A30F74");
  TestFibonacci(250);
  TestGcd("0x766C1042499184CAAB4A80602433449A", "0x21C3677C82B40000");
  TestPower("0x8B99E6BDDEC48");
  TestSqrt(powx(0xFFFFFFFFF, 5)*fibx(100));

  printf("%u tests passed out of %u\n", g_nPassCount, g_nTestCount);

  return 0; //what could possibly go wrong?
} //main
