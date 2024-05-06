/// \file expressiontree.h
/// \brief Header for expression tree node CNode.

#pragma once

#include <string>
#include "uintx_t.h"

/// \brief Operator enumerated type.

enum class Operator{
  None, Add, Subtract, Multiply, Divide,
}; //Operator

/// \brief Structure for a node in an expression tree.
///
/// Each node can represent an operator or a number.
/// If it is an operator, m_bIsOperator is true, m_chOp is
/// a character representing an operator, and m_pLeftChild and
/// m_pRightChild point to the operands. If it is a number,
/// m_bIsOperator is false and m_nValue contains the value.

class CNode{
  private:
    /// \brief Node type.

    enum class NodeType{ 
      Number, Operator, Function, Unknown
    };

    NodeType m_eNodeType = NodeType::Unknown; ///< Node type.
    Operator m_eOperator = Operator::None; ///< For operator nodes.
    uintx_t m_nValue = 0; ///< For number nodes.
    std::string m_strIdentifier; ///< Identifier for function nodes.

    CNode* m_pLeftChild = nullptr; ///< Pointer to left child.
    CNode* m_pRightChild = nullptr; ///< Pointer to right child.

    const std::string OperatorToString(const Operator op) const; ///< Operator to string.

  public:
    CNode(Operator op, CNode* lchild, CNode* rchild=nullptr); ///< Constructor.
    CNode(uintx_t n); ///< Constructor.
    CNode(const std::string& identifier, CNode* lchild); ///< Constructor.
    
    ~CNode(); ///< Destructor.

    const uintx_t evaluate() const; ///< Evaluate expression sub-tree.
    void postorder(std::string& s) const; ///< Postorder traversal of sub-tree.
    void inorder(std::string& s) const; ///< Inorder traversal of sub-tree.
}; //CNode