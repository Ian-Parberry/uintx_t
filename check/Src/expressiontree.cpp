/// \file expressiontree.cpp
/// \brief Code for expression tree node CNode.

#include "uintx_math.h"

#include "expressiontree.h"

/// Constructor for operator nodes.
/// \param op Operator.
/// \param lchild Pointer to left child.
/// \param rchild Pointer to right child.

CNode::CNode(Operator op, CNode* lchild, CNode* rchild):
  m_eNodeType(NodeType::Operator), m_eOperator(op), 
  m_pLeftChild(lchild), m_pRightChild(rchild){
} //constructor

/// Constructor for number nodes.
/// \param n Number.

CNode::CNode(uintx_t n):
  m_eNodeType(NodeType::Number), m_nValue(n){
} //constructor

/// Constructor for function nodes.
/// \param identifier Function identifier.
/// \param lchild Pointer to left child.

CNode::CNode(const std::string& identifier, CNode* lchild):
  m_eNodeType(NodeType::Function), m_strIdentifier(identifier),
  m_pLeftChild(lchild){
} //constructor

CNode::~CNode(){
  delete m_pLeftChild;
  delete m_pRightChild;
} //destructor

/// Evaluate the arithmetic expression rooted at this node.
/// \return The value of arithmetic expression rooted at this node.

const uintx_t CNode::evaluate() const{
  uintx_t operand0 = 0; //the first operand
  uintx_t operand1 = 0; //the second operand
  uintx_t result = 0; //the return result

  switch(m_eNodeType){
    case NodeType::Number: result = m_nValue; break;

    case NodeType::Operator:
      if(m_pLeftChild)operand0 = m_pLeftChild->evaluate(); //get 1st operand
      if(m_pRightChild)operand1 = m_pRightChild->evaluate(); //get 2nd operand

      switch(m_eOperator){ //perform the operation
        case Operator::Add:      result = operand0 + operand1; break;
        case Operator::Subtract: result = operand0 - operand1; break;
        case Operator::Multiply: result = operand0 * operand1; break;
        case Operator::Divide:   result = operand0 / operand1; break;
      } //switch
    break;

    case NodeType::Function: //evaluate function
      if(m_pLeftChild)operand0 = m_pLeftChild->evaluate(); //get operand

      if(m_strIdentifier == "sqrtx")
         result = sqrtx(operand0);

      else if(m_strIdentifier == "sqrx")
         result = operand0*operand0;

      else if(m_strIdentifier == "factorialx")
         result = factorialx(to_uint32(operand0));

      else if(m_strIdentifier == "exp2x")
         result = exp2x(to_uint32(operand0));

      else if(m_strIdentifier == "fibx")
         result = fibx(to_uint32(operand0));

      else if(m_strIdentifier == "log2x")
         result = log2x(operand0);

      else result = 0;
    break;
  } //switch

  return result;
} //evaluate

/// Get the printable string for an operator.
/// \param op Operator.
/// \return The string for the operator.

const std::string CNode::OperatorToString(const Operator op) const{
  switch(op){
    case Operator::None:     return std::string();
    case Operator::Add:      return std::string(" + ");
    case Operator::Subtract: return std::string(" - ");
    case Operator::Multiply: return std::string("*");
    case Operator::Divide:   return std::string("/");
    default: return std::string("?");
  } //switch
} //OperatorToString

/// Perform a postorder traversal starting at the current node and append the
/// result to a string.
/// \param s [in, out] String to append result of traversal to.

void CNode::postorder(std::string& s) const{
  switch(m_eNodeType){
    case NodeType::Number: //number
      s += to_string(m_nValue);
    break;

    case NodeType::Operator: //operator
      if(m_pLeftChild)m_pLeftChild->postorder(s); //left operand
      s += ' ';
      if(m_pRightChild)m_pRightChild->postorder(s); //right operand
      s += ' '+ OperatorToString(m_eOperator); //operator
    break;

    case NodeType::Function: //function call
      if(m_pLeftChild)m_pLeftChild->postorder(s); //operand
      s += ' ' + m_strIdentifier; //identifier
    break;
  } //switch
} //postorder

/// Perform an inorder traversal starting at the current node and append the
/// result to a string.
/// \param s [in, out] String to append result of traversal to.

void CNode::inorder(std::string& s) const{
  switch(m_eNodeType){
    case NodeType::Number: //number
      s += to_string(m_nValue);
    break;

    case NodeType::Operator: //operator
      s += "(";
      if(m_pLeftChild)m_pLeftChild->inorder(s); //left operand
      s += OperatorToString(m_eOperator); //operator
      if(m_pRightChild)m_pRightChild->inorder(s); //right operand
      s += ")";
    break;

    case NodeType::Function: //function call
      s += m_strIdentifier + "("; //identifier and open parenthesis
      if(m_pLeftChild)m_pLeftChild->inorder(s); //operand
      s += ")"; //close parenthesis
    break;
  } //switch
} //inorder
