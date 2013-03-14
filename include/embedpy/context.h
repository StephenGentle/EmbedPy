#ifndef EMBEDPY_CONTEXT_H
#define EMBEDPY_CONTEXT_H

#include <string>

#include "token.h"
#include "ast.h"

namespace embedpy {
    
// Compuler Context
    class CompilerContext {
    public:
        Token getToken();

        void GetNextToken();

        // Parse Functions
        ExprAST *ParseIntegerExpr();

        ExprAST *Error(const std::string &msg);
        
        std::string getIdentifierStr() { return IdentifierStr; }
        int getNumVal() { return NumVal; }

    private:
        std::string IdentifierStr;
        int NumVal;

        Token CurrentTok;
    };

}

#endif
