#ifndef EMBEDPY_TOKEN_H
#define EMBEDPY_TOKEN_H

#include <map>
#include <string>

namespace embedpy {

    enum class Token {
        eof, Comment,

        // Commands
        FuncDef, Extern, Return,

        // Primary
        Identifier, Invalid,
        
        // Separartors
        NewLine, StmtDelim, Colon, Comma, Indent, Dedent,
        
        // Operators
        AssignOp, EqualTo, Asterisk, Slash, Plus, Minus,
        PlusEquals, MinusEquals,

        OpenParen, CloseParen,

        String, Integer, Double
    };

    extern std::string getOperator(Token t);
    extern std::string getTokenName(Token t);
}

#endif
