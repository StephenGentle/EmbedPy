#ifndef EMBEDPY_TOKEN_H
#define EMBEDPY_TOKEN_H

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
    
}

#endif
