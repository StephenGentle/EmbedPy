#include <iostream>

#include <ast.h>
#include <context.h>

namespace embedpy {

    void CompilerContext::GetNextToken() {
        CurrentTok = getToken();
    }

    ExprAST *CompilerContext::Error(const std::string &msg) {
        std::cerr << "Error: " << msg << std::endl;
        return nullptr;
    }

    // ProtoError... FunctionErr...
    
    ExprAST *CompilerContext::ParseIntegerExpr() {
        ExprAST *result = new IntegerExprAST(NumVal);
        GetNextToken();
        return result;
    }

}
