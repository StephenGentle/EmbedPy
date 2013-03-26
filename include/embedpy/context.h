#ifndef EMBEDPY_CONTEXT_H
#define EMBEDPY_CONTEXT_H

#include <string>
#include <vector>

#include "token.h"
#include "ast.h"
#include "error.h"

namespace embedpy {
    
// Compuler Context
    class CompilerContext {
    public:
        CompilerContext() : line(1) {}
        virtual ~CompilerContext() {}

        Token getToken();

        Token GetNextToken();

        // Parse Functions
        ExprAST *ParseIntegerExpr();
        ExprAST *ParseParenExpr();
        ExprAST *ParseIdentifierExpr();
        ExprAST *ParseExpression();
        ExprAST *ParsePrimary();
        ExprAST *ParseBinOpRHS(int exprPrec, ExprAST *lhs);

        // Function/Prototype Parsing
        PrototypeAST *ParsePrototype();
        FunctionAST *ParseDefinition();
        PrototypeAST *ParseExtern();
        FunctionAST *ParseTopLevelExpr();

        // Error functions
        ExprAST *Error(const std::string &msg);
        PrototypeAST *ErrorP(const std::string &msg);
        FunctionAST *ErrorF(const std::string &msg);
       
        // Toplevel
        void HandleDefinition();
        void HandleExtern();
        void HandleTopLevelExpression();
        void MainLoop();

        // Helper functions
        int GetTokPrecedence();

        std::string getIdentifierStr() { return IdentifierStr; }
        int getNumVal() { return NumVal; }
        std::vector<CompileError> getErrors() { return errors; }

        void setFileName(std::string fname) { fileName = fname; } 

    protected:
        std::string IdentifierStr;
        int NumVal;

        Token CurrentTok;

        int line;
        std::string fileName;

        std::vector<CompileError> errors;
    };

    class InteractiveCompilerContext : public CompilerContext {
    public:
        InteractiveCompilerContext() {
            setFileName("stdin");
        }
    };

}

#endif
