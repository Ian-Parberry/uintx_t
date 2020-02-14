/// \file Main.cpp
/// \brief Main() and a couple of useful functions.

/// \file mail.cpp
/// \brief Test program for uintx_t.

#include "uintx_t.h"
#include "uintx_math.h"

/// \brief Main.
///
/// \return 0 (what could possibly go wrong?)

int main(){
  uintx_t x = powerx(0xFFFFFFFFF, 7);
  printf("x             = %s\n", to_string(x).c_str());
  //printf("sqrt(x) = %s\n", to_string(sqrtx(x)).c_str());
  printf("sqrt(x)^2     = %s\n", to_string(powerx(sqrtx(x), 2)).c_str());
  printf("(sqrt(x)+1)^2 = %s\n", to_string(powerx(sqrtx(x) + 1, 2)).c_str());
  printf("\n");

  printf("Fibonacci numbers: ");
  for(uint32_t i=0; i<100; i++)
    printf("%s, ", to_string(fibx(i)).c_str());
  printf("\n");

  return 0; //what could possibly go wrong?
} //main
