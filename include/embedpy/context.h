#ifndef EMBEDPY_CONTEXT_H
#define EMBEDPY_CONTEXT_H

#include <string>
#include <vector>
#include <cstdio>
#include <fstream>
#include <stack>

#include "token.h"
#include "ast.h"
#include "error.h"

namespace embedpy {
    
// Compuler Context
    class CompilerContext {
    public:
        CompilerContext() : line(1), column(0), indentLengths({0}), indentLevel(0) {}
        virtual ~CompilerContext() {}

        virtual char getChar() {
            return '\0';
        }

        Token getToken();

        Token GetNextToken();

        void Tokenise();

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
        virtual void MainLoop() = 0;

        // Helper functions
        int GetTokPrecedence();

        std::string getIdentifierStr() { return IdentifierStr; }
        int getIntVal() { return IntVal; }
        double getDoubleVal() { return DoubleVal; }
        std::vector<CompileError> getErrors() { return errors; }

        void setFileName(std::string fname) { fileName = fname; }

        void setInteractive(bool iactive) {
            interactive = iactive;
        }

    protected:
        std::string IdentifierStr;
        int IntVal;
        double DoubleVal;

        Token CurrentTok;

        int line;
        int column;
        std::string fileName;

        std::stack<int> indentLengths;
        int indentLevel;

        std::vector<CompileError> errors;

        bool interactive;
    };

    class InteractiveCompilerContext : public CompilerContext {
    public:
        InteractiveCompilerContext() {
            setFileName("stdin");
            setInteractive(true);
        }

        char getChar() {
            column++;
            return getchar();
        }

        virtual void MainLoop();
    };

    class FileCompilerContext : public CompilerContext {
    public:
        FileCompilerContext() {
            setInteractive(false);    
        }

        int Compile(const std::string &input, const std::string &output);
        int TokeniseFile(const std::string &input);

        char getChar() {
            column++;
            return in.get(); 
        }

        virtual void MainLoop();

    private:
        bool LoadFile(const std::string &input);

        std::fstream in;
        std::fstream out;
    };
}

#endif
