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
/// A recursive descent parser for arithmetic expressions. Note that while
/// fully parenthesized infix arithmetic expressions are LL(1) and hence
/// capable of being parsed by a recursive descent parser, arithmetic expressions
/// with operator precedence are not. However, this is easily handled by
/// turning if-statements in functions term() and expression() into while-loops.
/// This parser constructs an expression tree from the tokens provided by the
/// lexical analyzer. This expression tree may then be evaluated or printed out.

class CParser: public CLex{
  private:
    /// \brief Error code.
    ///
    /// The parser can detect three kinds of error: a syntax error, an
    /// unexpected symbol in the input, and a malformed arithmetic expression.

    enum class ErrorCode{
      Syntax, UnexpectedSymbol, Malformed
    }; //ErrorCode

    /// \brief Error map.
    ///
    /// The error map maps an error code to a human-readable string that
    /// briefly explains the error.

    std::map<ErrorCode, std::string> m_mapError = { 
      {ErrorCode::Syntax, "Syntax error"},
      {ErrorCode::UnexpectedSymbol, "Unexpected symbol"},
      {ErrorCode::Malformed, "Malformed expression"}
    }; //Error map.

    void error(const ErrorCode e); ///< Error processing.
    bool accept(const SymbolType s); ///< Skip over a particular kind of symbol.
    bool expect(const SymbolType s); ///< Verify that a symbol is next.

    bool factor(CNode*& tree); ///< Process factor.
    bool term(CNode*& tree);  ///< Process term.
    bool expression(CNode*& tree);  ///< Process expression.

    const bool IsExpressionOp(); ///< Is an expression operator.
    const bool IsTermOp(); ///< Is a term operator.

    CNode* m_pExpressionTree = nullptr; ///< Pointer to expression tree root.

  public:  
    bool parse(const std::string& s); ///< Parse a string.
    const uintx_t evaluate(); ///< Evaluate arithmetic expression.  
    const std::string GetPostfixString(); ///< Get postfix string. 
    const std::string GetInfixString(); ///< Get infix string.
}; //CParser
