#ifndef EMBEDPY_CONTEXT_H
#define EMBEDPY_CONTEXT_H

#include <string>
#include <vector>
#include <cstdio>
#include <fstream>

#include "token.h"
#include "ast.h"
#include "error.h"

namespace embedpy {
    
// Compuler Context
    class CompilerContext {
    public:
        CompilerContext() : line(1), column(0), indentLevel(0), indentLength(0) {}
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
        int column;
        std::string fileName;

        int indentLevel;
        int indentLength;

        std::vector<CompileError> errors;
    };

    class InteractiveCompilerContext : public CompilerContext {
    public:
        InteractiveCompilerContext() {
            setFileName("stdin");
        }

        char getChar() {
            column++;
            return getchar();
        }
    };

    class FileCompilerContext : public CompilerContext {
    public:
        int Compile(const std::string &input, const std::string &output);
        int TokeniseFile(const std::string &input);

        char getChar() {
            column++;
            return in.get(); 
        }

    private:
        bool LoadFile(const std::string &input);

        std::fstream in;
        std::fstream out;
    };
}

#endif
