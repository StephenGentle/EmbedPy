#ifndef EMBEDPY_TOKEN_H
#define EMBEDPY_TOKEN_H

namespace embedpy {

    enum class Token {
        eof,

        // Commands
        FuncDef, Extern, Return,

        // Primary
        Identifier, Integer, Invalid,
        
        // Separartors
        NewLine, StmtDelim, Colon, Comma,
        
        // Operators
        AssignOp, EqualTo, Asterisk, Slash, Plus, Minus,

        OpenParen, CloseParen
    };
    
}

#endif
