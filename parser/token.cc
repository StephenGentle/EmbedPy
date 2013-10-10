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
        { Token::EqualTo, "EqualTo" },
        { Token::Asterisk, "Asterisk" },
        { Token::Slash, "Slash" },
        { Token::Plus, "Plus" },
        { Token::Minus, "Minus" },
        { Token::PlusEquals, "PlusEquals" },
        { Token::MinusEquals, "MinusEquals" },
        { Token::SlashEquals, "SlashEquals" },
        { Token::AsteriskEquals, "AsteriskEquals" },
        { Token::OpenParen, "OpenParen" },
        { Token::CloseParen, "CloseParen" }
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

