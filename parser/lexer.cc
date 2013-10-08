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

namespace embedpy {

Token CompilerContext::getToken() {
    static char lastChar = ' ';

    if (lastChar == '\n') {
        line++;
        column = 0;

        lastChar = getChar();
        return Token::NewLine;
    }

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

        if (IdentifierStr == "def") return Token::FuncDef;
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

        return Token::Integer;
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
       
        case ',':
            return Token::Comma;

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
                std::cout << "Integer\t\t\t'" << NumVal << "'";
                break;

            case Token::FuncDef:
                std::cout << "FuncDef\t\t\t'" << IdentifierStr << "'";
                break;

            case Token::Return:
                std::cout << "Return\t\t\t'" << IdentifierStr << "'";
                break;

            case Token::OpenParen:
                std::cout << "OpenParen" << "\t\t" << "'('";
                break;

            case Token::CloseParen:
                std::cout << "CloseParen" << "\t\t" << "')'";
                break;

            case Token::Colon:
                std::cout << "Colon" << "\t\t\t" << "':'"; 
                break;

            case Token::AssignOp:
                std::cout << "AssignOp" << "\t\t" << "'='";
                break;

            case Token::Plus:
                std::cout << "Plus" << "\t\t\t" << "'+'";
                break;

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

