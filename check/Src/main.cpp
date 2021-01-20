/// \file main.cpp
///
/// \brief Main is as main does.

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "parser.h"
#include "expressiontree.h"
#include "uintx_t.h"

#if defined(_MSC_VER) //Microsoft Visual Studio 
  #define WIN32_LEAN_AND_MEAN
  #pragma warning(disable : 4996) //disable annoying stdio security warnings
#endif

/// \brief Output mode for extensible unsigned integers.

enum class OutputMode{
  Decimal, Hexadecimal, Commas
}; //OutputMode

bool g_bFinished = false; ///< Finished flag.
bool g_bVerbose = false; ///< Verbosity flag.
OutputMode g_eMode = OutputMode::Decimal; ///< Current output mode.

/// \brief Read a sline of text from stdin.
///
/// Prompt the user for an input and load it into a string.
/// \param s [out] The input string.

void readln(std::string& s){    
  printf("> "); //prompt user
  char c = getchar(); //preload first character

  while(c != '\n'){ //while not end of line
    s += c; //grab a character
    c = getchar(); //get next character
  } //while
} //readln

/// \brief Print help string.
///
/// Print a useful help string.

void PrintHelp(){ 
  printf("Enter a parenthesized arithmetic expression using unsigned integers\n");
  printf("of any length, the arithmetic operators +-*/ with normal operator\n");
  printf("precedence, and the following functions.\n");
  printf("  sqrtx: Floor of square root.\n");
  printf("  sqrx: Square.\n");
  printf("  factorialx: Factorial.\n");
  printf("  exp2x: Raise 2 to a power.\n");
  printf("  fibx: Fibonacci number.\n");
  printf("  log2x: Floor of log base 2.\n");

  printf("Instead of an arithmetic expression, you may also type the");
  printf(" following commands.\n");
  printf("  verbose: Verbose mode.\n");
  printf("  quiet: Quiet mode.\n");
  printf("  decimal: Print results in decimal.\n");
  printf("  hex:  Print results in hexadecimal.\n");
  printf("  commas: Print results in decimal with commas.\n");
  printf("  quit: Exit.\n");
} //PrintHelp

/// \brief Process a command.
///
/// Process a string to see whether it contains a command. If so, then
/// modify the global variables accordingly.
/// \param s Input string.
/// \return true if the string contains a command.

bool ProcessCommand(const std::string& s){
  if(s == "verbose"){
    printf("Verbose on\n");
    g_bVerbose = true;
    return true; //s is a command
  } //if

  if(s == "quiet"){
    printf("Verbose off\n");
    g_bVerbose = false;
    return true; //s is a command
  } //if

  if(s == "decimal"){
    printf("Printing in decimal mode\n");
    g_eMode = OutputMode::Decimal;
    return true; //s is a command
  } //if

  if(s == "hex"){
    printf("Printing in hexadecimal mode\n");
    g_eMode = OutputMode::Hexadecimal;
    return true; //s is a command
  } //if
  
  if(s == "commas"){
    printf("Printing in decimal mode with comma separators\n");
    g_eMode = OutputMode::Commas;
    return true; //s is a command
  } //if
  
  if(s == "quit" || s == "q"){
    g_bFinished = true;
    return true; //s is a command
  } //if
  
  if(s == "help" || s == "?"){ //Help! I need somebody.
    PrintHelp(); //print help string
    return true; //s is a command
  } //if

  return false; //s not recognized as a command
} //ProcessCommand

/// \brief Print extensible unsigned integer.
///
/// Print an extensible unsigned integer to stdout in one of the
/// output modes supported by uintx_t.
/// \param x An extensible unsigned integer.

void PrintUintx(const uintx_t& x){
  switch(g_eMode){
    case OutputMode::Decimal:
      printf("%s\n", to_string(x).c_str());
      break;
              
    case OutputMode::Hexadecimal:
      printf("%s\n", to_hexstring(x).c_str());
      break;
               
    case OutputMode::Commas:
      printf("%s\n", to_commastring(x).c_str());
      break;
  } //switch
} //PrintUintx

/// \brief Main.
/// \return 0 for success, 1 for failure.

int main(){
  printf("Extensible unsigned integer arithmetic expression evaluator.\n");
  printf("Type \"help\" or \"?\" for help.\n");
  CParser parser; //parser for arithmetic expressions

  while(!g_bFinished){ //process an arithmetic expression from the user
    std::string s; //input string
    readln(s); //read a line of text into s

    if(!ProcessCommand(s) && parser.parse(s)){ //input parsed correctly
      if(g_bVerbose){ //echo parsed string to stdout
        printf("Infix: %s\n", parser.GetInfixString().c_str());
        printf("Postfix: %s\n", parser.GetPostfixString().c_str());
      } //if

      PrintUintx(parser.evaluate()); //evaluate parsed string and print result
    } //if
  } //while

  return 0;
} //main