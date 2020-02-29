/// \file parser.h
///
/// \brief Header for the parser CParser.

#pragma once

#include <string>
#include <map>
#include <set>

#include "expressiontree.h"
#include "lexical.h"

#include "uintx_t.h"
#include "uintx_math.h"

/// \brief Arithmetic expression parser.
///
/// A recursive descent parser for arithmetic expressions.
/// Note that while arithmetic expressions are not LL(1), the thing that
/// stops them being LL(1) is easily handled by while-loops in functions term()
/// and expression().

class CParser: public CLex{
  private:
    /// \brief Error code.

    enum class ErrorCode{
      Syntax, UnexpectedSymbol, Malformed
    }; //ErrorCode

    std::map<ErrorCode, std::string> m_mapError = { 
      {ErrorCode::Syntax, "Syntax error"},
      {ErrorCode::UnexpectedSymbol, "Unexpected symbol"},
      {ErrorCode::Malformed, "Malformed expression"}
    }; ///< Error map.

    void error(const ErrorCode e); ///< Error processing.
    bool accept(const SymbolType s); ///< Skip over a particular kind of symbol.
    bool expect(const SymbolType s); ///< Verify that a symbol is next.

    bool factor(CNode*& tree); ///< Process factor.
    bool term(CNode*& tree);  ///< Process term.
    bool expression(CNode*& tree);  ///< Process expression.

    const bool IsExpressionOp(); ///< Is an expression operator.
    const bool IsTermOp(); ///< Is a term operator.

    //expression tree

    CNode* m_pExpressionTree = nullptr; ///< Pointer to expression tree root.

  public:  
    bool parse(const std::string& s); ///< Parse a string.
    const uintx_t evaluate(); ///< Evaluate arithmetic expression.  
    const std::string GetPostfixString(); ///< Get postfix string. 
    const std::string GetInfixString(); ///< Get infix string.
}; //CParser