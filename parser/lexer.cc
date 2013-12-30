#include <string>
#include <cctype>
#include <cstdlib>
#include <iostream>

#include "context.h"

// Returns true if the character is allowed in an integer type
// 0 to 9, A-F, x (for leading 0x on hex)
inline bool isHexDigit(char c) {
    return isdigit(c) || (c >= 'A' && c <= 'F');
}

namespace embedpy {

Token CompilerContext::getToken() {
    static char lastChar = getChar();

    // lastChar will be reset to 0 if we need to re-read. Usually in
    // interactive mode
    if (lastChar == 0) {
        lastChar = getChar();
    }

    if (lastChar == '\n') {
        line++;
        column = 0;

        if (this->interactive) {
            lastChar = 0;
            return Token::NewLine;
        }

        lastChar = getChar();
        return Token::NewLine;
    }

    // Skip whitespace
    if (isspace(lastChar)) {
        IdentifierStr = lastChar;
        
        // Check if this is the start of the line
        if (column == 1) {
            while (isspace(lastChar = getChar())) {
                IdentifierStr += lastChar;
            }

            int indent = IdentifierStr.length();

            if (indent > indentLengths.top()) {
                indentLengths.push(indent);
                indentLevel++;
                return Token::Indent;
            }
        }

        // Ignore whitespace if we're not at the start of a line
        while (isspace(lastChar)) {
            lastChar = getChar();
        }
    } else {
        // Check for dedent back to 0
        if (column == 1 && indentLevel != 0) {
            IdentifierStr = "";
            indentLevel--;
            indentLengths.pop();
            return Token::Dedent;
        }
    }

    if (column < indentLengths.top()) {
        indentLevel--;
        indentLengths.pop();
        return Token::Dedent;
    }

    // Get identifier
    if (isalpha(lastChar) || lastChar == '_') {
        IdentifierStr = lastChar;

        while (isalnum(lastChar = getChar()) || lastChar == '_') {
            IdentifierStr += lastChar;
        }

        if (IdentifierStr == "def") return Token::Def;
        if (IdentifierStr == "class") return Token::Class;
        if (IdentifierStr == "import") return Token::Import;
        if (IdentifierStr == "from") return Token::From;
        if (IdentifierStr == "extern") return Token::Extern;
        if (IdentifierStr == "return") return Token::Return;

        return Token::Identifier;
    }

    // Get Hex or Octal number
    if (lastChar == '0') {
        std::string numStr = std::string("0");

        lastChar = getChar();
        if (lastChar == 'x') {
            numStr += lastChar;

            while (isHexDigit(lastChar = getChar())) {
                numStr += lastChar;
            }

        } else if(isdigit(lastChar) && lastChar - '0' < 8) {
            // Octal
            numStr += lastChar;
    
            while(isdigit(lastChar = getChar()) && lastChar - '0' < 8) {
                numStr += lastChar;
            }

        } else {
            IntVal = 0;
            return Token::Integer;
        }
            
        IntVal = strtol(numStr.c_str(), nullptr, 0);

        return Token::Integer;
    }

    // Get normal base 10 number - integer or double
    if (isdigit(lastChar)) {
        bool dbl = false;
        std::string numStr;
        numStr += lastChar;

        while (isdigit(lastChar = getChar()) || lastChar == '.') {
            numStr += lastChar;

            // Check if have a floating point value
            if (lastChar == '.') {
                dbl = true;
            }
        }

        if (dbl) {
            DoubleVal = strtod(numStr.c_str(), nullptr);
            return Token::Double;
        } else {
            IntVal = strtol(numStr.c_str(), nullptr, 0);
            return Token::Integer;
        }
    }

    // Parse strings
    if (lastChar == '"' || lastChar == '\'') {
        char openChar = lastChar;
        IdentifierStr = std::string("");

        while ((lastChar = getChar()) != openChar) {
            IdentifierStr += lastChar;
        }

        lastChar = getChar();
        return Token::String;
    }
    
    // Comment to end of line
    if (lastChar == '#') {
        std::string commentStr("#");

        while ((lastChar = getChar()) != EOF && lastChar != '\n' && lastChar != '\r') {
            commentStr += lastChar;
        }

        IdentifierStr = commentStr;
        return Token::Comment;
    }
    
    // Check for end of file
    if (lastChar == EOF || lastChar == '\0') {
        return Token::eof;
    }

    // Find assignment or comparision operators
    if (lastChar == '=') {
        if((lastChar = getChar()) == '=') {
            lastChar = getChar();
            return Token::EqualTo;
        }

        return Token::AssignOp;
    }

    // Get the next char for next time
    char thisChar = lastChar;
    lastChar = getChar();

    switch (thisChar) {
        case ';':
            return Token::StmtDelim;

        case ':':
            return Token::Colon;
       
        case ',':
            return Token::Comma;

        case '(':
            return Token::OpenParen;

        case ')':
            return Token::CloseParen;

        case '+':
            if (lastChar == '=') {
                lastChar = getChar();
                return Token::PlusEquals;
            }

            return Token::Plus;

        case '-':
            if (lastChar == '=') {
                lastChar = getChar();
                return Token::MinusEquals;
            }

            return Token::Minus;

        case '/':
            if (lastChar == '=') {
                lastChar = getChar();
                return Token::SlashEquals;
            }

            return Token::Slash;

        case '*':
            if (lastChar == '=') {
                lastChar = getChar();
                return Token::AsteriskEquals;
            }

            return Token::Asterisk;

    }

    // Invalid Character
    return Token::Invalid;
}

void CompilerContext::Tokenise() {
    Token t;
    int endcol;

    while (true) {
        std::cout << line << "," << column;
        
        t = this->getToken();

        if (t != Token::eof) {
            endcol = column - 1;
        } else {
            endcol = column;
        }

        std::cout << "-" << line << "," << endcol << ":\t\t";

        switch (t) {
            case Token::NewLine:
                std::cout << "NewLine" << "\t\t\t" << "'\\n'";
                break;

            case Token::Identifier:
                std::cout << "Identifier\t\t'" << IdentifierStr << "'";
                break;

            case Token::Integer:
                std::cout << "Integer\t\t\t'" << IntVal << "'";
                break;

            case Token::Double:
                std::cout << "Double\t\t\t'" << DoubleVal << "'";
                break;

            case Token::String:
            case Token::Def:
            case Token::Class:
            case Token::Import:
            case Token::From:
            case Token::Return:
            case Token::Comment:
            case Token::Indent:
                std::cout << getTokenName(t) << "\t\t\t'" << IdentifierStr << "'";
                break;

            case Token::Dedent:
                std::cout << "Dedent" << "\t\t\t" << "''";
                break;

            case Token::OpenParen:
            case Token::CloseParen:
            case Token::Colon:
            case Token::Comma:
            case Token::AssignOp:
            case Token::EqualTo:
            case Token::Plus:
            case Token::PlusEquals:
            case Token::Minus:
            case Token::MinusEquals: 
            case Token::Slash:
            case Token::SlashEquals:
            case Token::Asterisk: 
            case Token::AsteriskEquals: {
                std::string tokName = getTokenName(t);
                std::string tabs("\t\t");

                if (tokName.length() < 8) {
                    tabs += "\t";
                }

                std::cout << tokName << tabs << "'" << getOperator(t) << "'";
                break;
            }

            case Token::eof:
                std::cout << "EOF" << "\t\t\t" << "''";
                break;

            default:
                std::cout << "Implement Me!";
        }

        std::cout << std::endl;
        
        if (t == Token::eof) {
            return;
        }

    }
}

}

