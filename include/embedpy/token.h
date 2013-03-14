#ifndef EMBEDPY_TOKEN_H
#define EMBEDPY_TOKEN_H

namespace embedpy {

    enum class Token {
        eof,

        // Commands
        Def, Extern, Return,

        // Primary
        Identifier, Number, Invalid, StmtDelim, Colon, 
        
        // Operators
        AssignOp, EqualTo, Asterisk, Slash, Plus, Minus,

        OpenParen, CloseParen
    };
    
}

#endif
