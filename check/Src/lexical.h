/// \file lexical.h
///
/// \brief Header for lexical analyzer CLex.

#pragma once

#include <string>
#include <set>
#include <stack>

#include "uintx_t.h"

/// \brief Input symbol type.
///
/// Input symbol type for the lexical analyzer and parser.

enum class SymbolType{ 
  Null, Number, Identifier, LParen, RParen, Mul, Div, Plus, Minus
}; //SymbolType

/// \brief Lexical analyzer.
///
/// A simple lexical analyzer for arithmetic expressions.

class CLex{
  protected:
    const std::set<std::string> m_setFunctions = { 
      "sqrtx", "sqrx", "factorialx", "exp2x", "fibx", "log2x"
    }; ///< Function names.

    std::string m_strBuffer; ///< Expression string.
    size_t m_nStrLen = 0; ///< Length of expression string.
    size_t m_nCurChar = 0; ///< Index of current character from m_strBuffer.

    SymbolType m_eSymbol = SymbolType::Null; ///< Current symbol.
    uintx_t m_nNumber = 0; ///< Current number parsed, if there is one.
    std::stack<std::string> m_stdIdentifierStack; ///< Identifier stack.
    
    bool getsymbol(); ///< Get symbol into m_eSymbol.
    void getnumber(); ///< Get number into m_nNumber.  
    bool getidentifier(); ///< Get identifier into m_eSymbol.  
}; //CLex
