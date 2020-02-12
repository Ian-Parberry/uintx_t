/// \file Main.cpp
/// \brief Main() and a couple of useful functions.

#include <string>
#include <conio.h>

#include "uintx_t.h"
#include "uintx_math.h"

/// \brief Main.
///
/// \return 0 (what could possibly go wrong?)

int main(){
  uintx_t x = factorial(33);
  printf("%s\n", to_string(x).c_str());
  printf("%s\n", to_string16(x).c_str());
  printf("\n");

  uintx_t y = sqrt(x);
  printf("%s\n", to_string(y).c_str());
  printf("%s\n", to_string16(x).c_str());
  printf("\n");

  //uintx_t y2 = y*y;
  //printf("%s\n", to_string(y2).c_str());
  //printf("%s\n", to_string16(x).c_str());

  #if defined(_MSC_VER) //defined for Windoze 
    printf("Hit Almost Any Key to Exit...\n");
    _getch(); //hangs until the user hits a key
  #endif

  return 0; //what could possibly go wrong?
} //main
