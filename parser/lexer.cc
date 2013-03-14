#include <string>
#include <cctype>
#include <cstdlib>
#include <iostream>

#include <context.h>

// Returns true if the character is allowed in an integer type
// 0 to 9, A-F, x (for leading 0x on hex)
inline bool isIntChar(char c) {
    return isdigit(c) || c == 'x';
}

// Temp - just get char from stdin
inline char getChar() {
    return getchar();
}

namespace embedpy {

Token CompilerContext::getToken() {
    static char lastChar = ' ';

    // Skip whitespace
    while (isspace(lastChar)) {
        lastChar = getChar();
    }

    // Get identifier
    if (isalpha(lastChar)) {
        IdentifierStr = lastChar;

        while (isalnum(lastChar = getChar())) {
            IdentifierStr += lastChar;
        }

        if (IdentifierStr == "def") return Token::Def;
        if (IdentifierStr == "extern") return Token::Extern;
        if (IdentifierStr == "return") return Token::Return;

        return Token::Identifier;
    }

    // Get Number
    // Numbers can be integer, hex ('0x1A') or octal (04)
    if (isIntChar(lastChar)) {
        std::string numStr;
        numStr += lastChar;

        while (isIntChar(lastChar = getChar())) {
            numStr += lastChar;
        }

        NumVal = strtol(numStr.c_str(), nullptr, 0);

        return Token::Number;
    }
    
    // Comment to end of line
    if (lastChar == '#') {
        while (lastChar != EOF && lastChar != '\n' && lastChar != '\r') {
            lastChar = getChar();
        }

        if (lastChar != EOF && lastChar != '\0') {
            return getToken();
        }
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
        
        case '(':
            return Token::OpenParen;

        case ')':
            return Token::CloseParen;

        case '+':
            return Token::Plus;

        case '-':
            return Token::Minus;

        case '/':
            return Token::Slash;

        case '*':
            return Token::Asterisk;

    }

    // Invalid Character
    return Token::Invalid;
}
}

using namespace embedpy;

void mainLoop()
{
    CompilerContext c;

    while (true) {
        std::cout << std::endl << "> ";
        
        Token t = c.getToken();
        
        switch (t) {
            case Token::eof: 
                std::cout << "EOF" << std::endl;
                return;

            case Token::Def:
                std::cout << "FunctionDef";
                break;

            case Token::Extern:
                std::cout << "Extern";
                break;

            case Token::Return:
                std::cout << "Return";

            case Token::StmtDelim:
                std::cout << "Delim ";
                break;

            case Token::Identifier:
                std::cout << "Identifier[" << c.getIdentifierStr() << "] ";
                break;

            case Token::Number:
                std::cout << "Number[" << c.getNumVal() << "] ";
                break;

            case Token::Colon:
                std::cout << "Colon ";
                break;
            
            case Token::AssignOp:
                std::cout << "AssignOp ";
                break;

            case Token::EqualTo:
                std::cout << "EqualTo ";
                break;

            case Token::OpenParen:
                std::cout << "OpenParen ";
                break;

            case Token::CloseParen:
                std::cout << "CloseParen ";
                break;

            case Token::Plus:
                std::cout << "Plus ";
                break;

            case Token::Minus:
                std::cout << "Minus ";
                break;

            case Token::Slash:
                std::cout < "Slash";
                break;
            
            case Token::Asterisk:
                std::cout << "Asterisk";
                break;

            default:
                std::cout << "Invalid ";
                break;
        }
    }
}
