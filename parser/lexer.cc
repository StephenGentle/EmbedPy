#include <string>
#include <cctype>
#include <cstdlib>
#include <iostream>

#include <context.h>

// Returns true if the character is allowed in an integer type
// 0 to 9, A-F, x (for leading 0x on hex)
inline bool isHexDigit(char c) {
    return isdigit(c) || (c >= 'A' && c <= 'F');
}

namespace embedpy {

Token CompilerContext::getToken() {
    static char lastChar = getChar();

    if (lastChar == '\n') {
        line++;
        column = 0;

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

        if (IdentifierStr == "def") return Token::FuncDef;
        if (IdentifierStr == "extern") return Token::Extern;
        if (IdentifierStr == "return") return Token::Return;

        return Token::Identifier;
    }

    // Get Hex or Octal number
    // TODO: leading plus or minus signs
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
            NumVal = 0;
            return Token::Integer;
        }
            
        NumVal = strtol(numStr.c_str(), nullptr, 0);

        return Token::Integer;
    }

    // Get normal decimal number
    if (isdigit(lastChar)) {
        std::string numStr;
        numStr += lastChar;

        while (isdigit(lastChar = getChar())) {
            numStr += lastChar;
        }

        NumVal = strtol(numStr.c_str(), nullptr, 0);

        return Token::Integer;
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

            case Token::String:
                std::cout << "String\t\t\t'" << IdentifierStr << "'";
                break;

            case Token::FuncDef:
                std::cout << "FuncDef\t\t\t'" << IdentifierStr << "'";
                break;

            case Token::Return:
                std::cout << "Return\t\t\t'" << IdentifierStr << "'";
                break;

            case Token::Indent:
                std::cout << "Indent\t\t\t'" << IdentifierStr << "'";
                break;

            case Token::Dedent:
                std::cout << "Dedent" << "\t\t\t" << "''";
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

            case Token::EqualTo:
                std::cout << "EqualTo" << "\t\t\t" << "'=='";
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

