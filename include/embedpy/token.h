#ifndef EMBEDPY_TOKEN_H
#define EMBEDPY_TOKEN_H

#include <map>
#include <string>

namespace embedpy {

    enum class Token {
        eof, Comment,

        // Commands
        Def, From, Class, Import, Extern, Return,

        // Primary
        Identifier, Invalid,
        
        // Separartors
        NewLine, Semicolon, Colon, Comma, Indent, Dedent,
        
        // Operators
        Equals, EqualTo, Asterisk, Slash, Plus, Minus,
        PlusEquals, MinusEquals, SlashEquals, AsteriskEquals,

        OpenParen, CloseParen,

        String, Integer, Double
    };

    extern std::string getOperator(Token t);
    extern std::string getTokenName(Token t);
}

#endif
