#ifndef EMBEDPY_AST_H
#define EMBEDPY_AST_H

#include <vector>

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

    // AST Node for function call
    class CallExprAST : public ExprAST {
    public:
        CallExprAST(const std::string &funcName, const std::vector<ExprAST*> &args)
            : FuncName(funcName), Args(args) {}

    private:
        std::string FuncName;
        std::vector<ExprAST*> Args;
    };

    // AST node for prototype
    class PrototypeAST {
    public:
        PrototypeAST(const std::string &name, const std::vector<std::string> &args)
            : Name(name), Args(args) {}

        // Accessors
        const std::string &GetName() { return Name; }
        const std::vector<std::string> &GetArgs() { return Args; }

    private:
        std::string Name;
        std::vector<std::string> Args;
    };

    class FunctionAST {
    public:
        FunctionAST(PrototypeAST *proto, ExprAST *body)
            : Proto(proto), Body(body) {}

        // Accessors
        const std::string &GetName() { return Proto->GetName(); }
        const std::vector<std::string> &GetArgs() { return Proto->GetArgs(); }

    private:
        PrototypeAST *Proto;
        ExprAST *Body;
    };
}

#endif
