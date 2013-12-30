#include "token.h"

namespace embedpy {

    std::map<Token, std::string> ep_operators = { 
        { Token::AssignOp, "=" },
        { Token::EqualTo, "==" },
        { Token::Asterisk, "*" },
        { Token::Slash, "/" },
        { Token::Plus, "+" },
        { Token::Minus, "-" },
        { Token::PlusEquals, "+=" },
        { Token::MinusEquals, "-=" },
        { Token::SlashEquals, "/=" },
        { Token::AsteriskEquals, "*=" },
        { Token::OpenParen, "(" },
        { Token::CloseParen, ")" },
        { Token::Colon, ":" },
        { Token::StmtDelim, ";" },
        { Token::Comma, "," }
    };

    std::map<Token, std::string> ep_tok_names = {
        { Token::AssignOp, "Equals" },
        { Token::Asterisk, "Asterisk" },
        { Token::AsteriskEquals, "AsteriskEquals" },
        { Token::Class, "Class" },
        { Token::CloseParen, "CloseParen" },
        { Token::Colon, "Colon" },
        { Token::Def, "Def" },
        { Token::From, "From" },
        { Token::EqualTo, "EqualTo" },
        { Token::Indent, "Indent" },
        { Token::Import, "Import" },
        { Token::Minus, "Minus" },
        { Token::MinusEquals, "MinusEquals" },
        { Token::OpenParen, "OpenParen" },
        { Token::Plus, "Plus" },
        { Token::PlusEquals, "PlusEquals" },
        { Token::Return, "Return" },
        { Token::Slash, "Slash" },
        { Token::SlashEquals, "SlashEquals" }
    };

    std::string getOperator(Token t) {
        auto item = ep_operators.find(t);

        if (item == ep_operators.end()) {
            return std::string();
        }

        return item->second;
    }

    std::string getTokenName(Token t) {
        auto item = ep_tok_names.find(t);

        if (item == ep_tok_names.end()) {
            return std::string("[Token]");
        }

        return item->second;
    }
    
}

