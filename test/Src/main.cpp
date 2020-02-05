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
  uintx_t x(256U);
  std::string s = std::string(factorial(x));
  printf("%s\n", s.c_str());

  #if defined(_MSC_VER) //defined for Windoze 
    printf("Hit Almost Any Key to Exit...\n");
    _getch(); //hangs until the user hits a key
  #endif

  return 0; //what could possibly go wrong?
} //main
