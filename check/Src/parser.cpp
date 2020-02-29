/// \file parser.cpp
///
/// \brief Code for the parser CParser.

#include "parser.h"

///////////////////////////////////////////////////////////////////////////////
//Helper functions.

/// Print an error message to stdout. Uses m_mapError to map an error code
/// to a printable string.
/// \param e Error code.
 
void CParser::error(const ErrorCode e){
  for(size_t i=0; i<m_nCurChar + 2; i++)putchar(' ');
  printf("^ %s\n", m_mapError[e].c_str());
} //error

/// Accept a symbol. If the current symbol is the expected symbol, then skip
/// to the next symbol and return true. Otherwise return false.
/// \param s The accepted symbol.
/// \return true if the current symbol is the accepted symbol.

bool CParser::accept(const SymbolType s){
  if(m_eSymbol != s)return false;
  if(getsymbol())return false;
  return true;
} //accept

/// Expect a symbol. Same as function accept(), but report an error if the
/// current symbol is not the expected symbol.
/// \param s Expected symbol.
/// \return true if the current symbol is the expected symbol.

bool CParser::expect(const SymbolType s){
  if(accept(s))return true;
  error(ErrorCode::UnexpectedSymbol);
  return false;
} //expect

/// Test whether the current symbol is an operation that can occur in an
/// \f$\langle\text{expression}\rangle\f$, that is, an additive operation.
/// \return true if the current symbol is an additive operation.

const bool CParser::IsExpressionOp(){
  return m_eSymbol == SymbolType::Plus || m_eSymbol == SymbolType::Minus;
} //IsExpressionOp

/// Test whether the current symbol is an operation that can occur in an
/// \f$\langle\text{term}\rangle\f$, that is, a multiplicative operation.
/// \return true if the current symbol is a multiplicative operation.

const bool CParser::IsTermOp(){
  return m_eSymbol == SymbolType::Mul || m_eSymbol == SymbolType::Div;
} //IsTermOp

///////////////////////////////////////////////////////////////////////////////
//Recursive descent functions.

/// Recursive descent function to build a sub-tree for a factor 
/// using the production \f[\langle\text{factor}\rangle \rightarrow
/// \langle\text{number}\rangle \mid ( \langle\text{expression}\rangle) \mid
/// \langle\text{identifier}\rangle( \langle\text{expression}\rangle)\f]
/// where \f$\langle\text{number}\rangle\f$ is a number and 
/// \f$\langle\text{identifier}\rangle\f$ is an identifier.
/// \param tree [out] Reference to a pointer to root of factor sub-tree.
/// \return true if an error occurred.

bool CParser::factor(CNode*& tree){
  bool bError = false;

  if(accept(SymbolType::Number))
    tree = new CNode(m_nNumber);

  else if(accept(SymbolType::LParen))
    bError = expression(tree) || !expect(SymbolType::RParen);

  else if(accept(SymbolType::Identifier) && expect(SymbolType::LParen)){
    CNode* node = nullptr;
    bError = expression(node) || !expect(SymbolType::RParen);
    if(!bError)tree = new CNode(m_strIdentifier, node);
  } //else if

  else{
    error(ErrorCode::Syntax);
    bError = true;
    getsymbol();
  } //else
  
  return bError;
} //factor

/// Recursive descent function to build a sub-tree for a term using the
/// production \f$\langle\text{term}\rangle \rightarrow 
/// \langle\text{factor}\rangle \left[ \langle \text{mulop}\rangle 
/// \langle\text{factor}\rangle)\right]^*\f$, where 
/// \f$\langle \text{mulop}\rangle\f$ is a multiplicative operator.
/// \param tree [out] Reference to a pointer to root of term sub-tree.
/// \return true if an error occurred.
 
bool CParser::term(CNode*& tree){
  CNode* lchild = nullptr;
  CNode* rchild = nullptr;
  bool bError = factor(lchild);
  tree = lchild;

  while(!bError && IsTermOp()){
    Operator op = Operator::None;

    switch(m_eSymbol){
      case SymbolType::Mul: op = Operator::Multiply; break;
      case SymbolType::Div: op = Operator::Divide; break;
    } //switch
    
    bError = getsymbol();

    if(!bError){
      bError = factor(rchild);
      lchild = tree = new CNode(op, lchild, rchild);
    } //if
  } //while
  
  return bError;
} //term

/// Recursive descent function to build a sub-tree for an expression
/// using the production \f$\langle\text{expression}\rangle \rightarrow 
/// \langle\text{term}\rangle \left[ \langle \text{addop}\rangle 
/// \langle\text{term}\rangle)\right]^*\f$, where 
/// \f$\langle \text{addop}\rangle\f$ is an additive operator.
/// \param tree [out] Reference to a pointer to root of expression sub-tree.
/// \return true if an error occurred.

bool CParser::expression(CNode*& tree){
  CNode* lchild = nullptr;
  CNode* rchild = nullptr;
  bool bError = false;

  if(m_eSymbol == SymbolType::Plus || m_eSymbol == SymbolType::Minus)
    bError = getsymbol();

  if(!bError){
    bError = term(lchild);
    tree = lchild;
  } //if

  while(!bError && IsExpressionOp()){
    SymbolType op = m_eSymbol;
    bError = getsymbol();

    if(!bError){
      bError = term(rchild);

      if(op == SymbolType::Plus)
        tree = new CNode(Operator::Add, lchild, rchild);

      else if(op == SymbolType::Minus)
        tree = new CNode(Operator::Subtract, lchild, rchild);

      lchild = tree;
    } //if
  } //while
  
  return bError;
} //expression

///////////////////////////////////////////////////////////////////////////////
//Public functions.

/// Parse a string containing an arithmetic expression into an expression tree.
/// \param s A string.
/// \return true if the string parsed correctly.

bool CParser::parse(const std::string& s){
  if(s == "")return true; //nothing to parse

  m_strBuffer = s; //grab a copy of the expression string
  m_nStrLen = m_strBuffer.length(); //record its length
  m_nCurChar = 0; //start of buffer string
  delete m_pExpressionTree; //clean up old tree
  getsymbol(); //get the first symbol to prime the pump

  //parse the arithmetic expression into an expression tree

  bool bOK = !expression(m_pExpressionTree);

  //check for stray symbols after the expression

  if(m_eSymbol != SymbolType::Null){ //current symbol should be null
    error(ErrorCode::Malformed);
    bOK = false;
  } //if

  return bOK;
} //parse

/// Evaluate a parsed arithmetic expression.
/// \return The result of evaluating a parsed arithmetic expression.

const uintx_t CParser::evaluate(){
  return m_pExpressionTree->evaluate();
} //evaluate

/// Get postfix expression string from a parsed arithmetic expression.
/// \return Postfix expression string.
 
const std::string CParser::GetPostfixString(){
  std::string s;

  if(m_pExpressionTree != nullptr)
    m_pExpressionTree->postorder(s);

  return s;
} //GetPostfixString

/// Get infix expression string from a parsed arithmetic expression.
/// \return Infix expression string.
 
const std::string CParser::GetInfixString(){
  std::string s;

  if(m_pExpressionTree != nullptr)
    m_pExpressionTree->inorder(s);

  return s;
} //GetInfixString
