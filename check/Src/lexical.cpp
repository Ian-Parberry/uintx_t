/// \file lexical.cpp
///
/// \brief Code for lexical analyzer CLex.

#include "lexical.h"

//////////////////////////////////////////////////////////////////////////////
//Helper functions

/// Test whether a character is a numeral.
/// \param c A character.
/// \return true if the character is numeric.

const bool isNumeric(const char c){
  return c >= '0' && c <= '9';
} //isNumeric

/// Test whether a character is alphabetic (lower case only).
/// \param c A character.
/// \return true if the character is alphabetic.

const bool isAlpha(const char c){
  return c >= 'a' && c <= 'z';
} //isNumeric

/// Test whether a character is alphanumeric (lower case only).
/// \param c A character.
/// \return true if the character is alphanumeric.

const bool isAlphaNumeric(const char c){
  return isNumeric(c) || isAlpha(c);
} //isNumeric

/// Test whether a character is white space, that is, either a space or a tab.
/// \param c A character.
/// \return true if the character is white space

const bool isWhitespace(const char c){
  return c == ' ' || c =='\t';
} //isWhitespace

//////////////////////////////////////////////////////////////////////////////
//Lexical analyzer functions

/// Get next symbol (at index `m_nCurChar`) from `m_strBuffer` into `m_eSymbol`.
/// \return true if there is an error.

bool CLex::getsymbol(){
  bool error = false; //error flag

  //skip white space
  while(m_nCurChar < m_nStrLen && isWhitespace(m_strBuffer[m_nCurChar]))
    m_nCurChar++;

  if(m_nCurChar >= m_nStrLen) //overflow
    m_eSymbol = SymbolType::Null;

  else{ //normal case, no overflow
    char c = m_strBuffer[m_nCurChar]; //shorthand for current character

    if(isNumeric(c)){ //first character is a digit
      m_eSymbol = SymbolType::Number; //it's going to be a number
      getnumber(); //get the rest of it
    } //if

    else if(isAlpha(c)){ //first character is alphabetic
      error = getidentifier(); //get the rest of it
      m_eSymbol = error? SymbolType::Null: SymbolType::Identifier; 
    } //if

    else{ //single character symbols
      switch(c){
        case '(': m_eSymbol = SymbolType::LParen; break;     
        case ')': m_eSymbol = SymbolType::RParen; break;
        case '*': m_eSymbol = SymbolType::Mul; break;
        case '/': m_eSymbol = SymbolType::Div; break;    
        case '+': m_eSymbol = SymbolType::Plus; break; 
        case '-': m_eSymbol = SymbolType::Minus; break;
        default:  m_eSymbol = SymbolType::Null;
      } //switch

      m_nCurChar++; //ready for next character
    } //else
  } //else

  return error;
} //getsymbol

/// Get a number. Assumes that `m_strBuffer[m_nCurChar]` is a digit. 
/// Reads the unsigned number starting there into `m_nNumber`.

void CLex::getnumber(){
  m_nNumber = 0;

  do{ //must have at least one digit
    m_nNumber = m_nNumber*10 + m_strBuffer[m_nCurChar++] - '0';
  }while(m_nCurChar < m_nStrLen && isNumeric(m_strBuffer[m_nCurChar]));
} //getnumber

/// Get an identifier from `m_strBuffer` into `m_strIdentifier`.
/// Identifiers are alphanumeric and must start with a letter.
/// Assumes that `m_strBuffer[m_nCurChar]` is a letter.
/// \return true if there is an error.

bool CLex::getidentifier(){
  bool bError = false; //error flag
  std::string strIdentifier; //current identifier

  do{ //must have at least one letter
    strIdentifier += m_strBuffer[m_nCurChar++];
  }while(m_nCurChar < m_nStrLen && isAlphaNumeric(m_strBuffer[m_nCurChar]));

  if(m_setFunctions.find(strIdentifier) == m_setFunctions.end()){ //error
    bError = true;
    
    for(size_t i=0; i<m_nCurChar; i++)putchar(' ');
      printf("  ^ %s\n", "Unknown identifier"); //2 extra spaces for the prompt
  } //if

  else m_strIdentifier = strIdentifier; //valid identifier

  return bError;
} //getidentifier