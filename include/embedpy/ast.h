#ifndef EMBEDPY_AST_H
#define EMBEDPY_AST_H

#include "token.h"

namespace embedpy {

    // Expression AST base node
    class ExprAST {
    public:
        virtual ~ExprAST() {}
    };

    // AST Node for an integer
    class IntegerExprAST : public ExprAST {
    public:
        IntegerExprAST(int val) : Value(val) {} 

    private:
        int Value;
    };

    // AST Node for referencing a variable
    class VariableExprAST : public ExprAST {
    public:
        VariableExprAST(const std::string &name) : Name(name) {}

    private:
        std::string Name;
    };

    // AST Node for a binary expression
    class BinaryExprAST : public ExprAST {
    public:
        BinaryExprAST(Token op, ExprAST *lhs, ExprAST *rhs) : Operator(op), LHS(lhs), RHS(rhs) {}

    private:
        Token Operator;
        ExprAST *LHS, *RHS;
    };



}

#endif
