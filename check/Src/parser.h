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
/// A recursive descent parser for unsigned arithmetic expressions. While
/// fully parenthesized infix arithmetic expressions are LL(1) and hence
/// capable of being parsed by a recursive descent parser, arithmetic expressions
/// with operator precedence are not. However, this is easily handled by
/// turning if-statements in functions term() and expression() into while-loops.
/// This parser constructs an expression tree from the tokens provided by the
/// lexical analyzer. This expression tree may then be evaluated or printed out.
/// The language recognized by the parser is given by the following context-free
/// grammar, where nonterminal symbols are enclosed in angle brackets
/// \f$\langle\rangle\f$, terminal symbols are single characters, square
/// brackets \f$[\,]\f$ are used for grouping, the vertical bar \f$|\f$ is
/// alternation, and the superscript \f$*\f$
/// indicates Kleene closure, that is, "zero or more copies of".
/// \f{eqnarray*}{
/// \langle\text{expression}\rangle &\rightarrow& 
/// \langle\text{term}\rangle \left[ \langle \text{addop}\rangle 
/// \langle\text{term}\rangle)\right]^*\\
/// \langle\text{term}\rangle &\rightarrow& 
/// \langle\text{factor}\rangle \left[ \langle \text{mulop}\rangle 
/// \langle\text{factor}\rangle)\right]^*\\
/// \langle\text{factor}\rangle &\rightarrow&
/// \langle\text{number}\rangle \mid \langle\text{lparen}\rangle 
/// \langle\text{expression}\rangle\langle\text{rparen}\rangle \mid
/// \langle\text{identifier}\rangle\langle\text{lparen}\rangle
/// \langle\text{expression}\rangle\langle\text{rparen}\rangle\\
/// \langle\text{identifier}\rangle &\rightarrow&
/// \langle\text{letter}\rangle\left[ \langle \text{letter}\rangle \mid
/// \langle\text{digit}\rangle)\right]^*\\
/// \langle\text{number}\rangle &\rightarrow&
/// \langle\text{digit}\rangle\langle\text{digit}\rangle^*\\
/// \langle\text{letter}\rangle &\rightarrow&
/// \text{a} | \text{b} | \text{c} | \ldots | \text{z}\\
/// \langle\text{digit}\rangle &\rightarrow&
/// \text{0}|\text{1}|\text{2}|\ldots |\text{9}\\
/// \langle\text{addop}\rangle &\rightarrow& + \mid -\\
/// \langle\text{mulop}\rangle &\rightarrow& * \mid /\\
/// \langle\text{lparen}\rangle &\rightarrow& (\\
/// \langle\text{lparen}\rangle &\rightarrow& )\\
/// \f}

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
    const uintx_t evaluate() const; ///< Evaluate arithmetic expression.  
    const std::string GetPostfixString() const; ///< Get postfix string. 
    const std::string GetInfixString() const; ///< Get infix string.
}; //CParser
