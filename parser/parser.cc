#include <iostream>
#include <map>

#include <ast.h>
#include <context.h>

namespace embedpy {

    std::map<Token, int> BinOpPrecedence = { 
        //{ Token::LessThan, 10 },
        { Token::Plus, 20 },
        { Token::Minus, 20 },
        { Token::Asterisk, 30 },
        { Token::Slash, 30 }
    };

    Token CompilerContext::GetNextToken() {
        if (IsInteractive() && CurrentTok == Token::NewLine) {
            DisplayPrompt();
        }

        CurrentTok = getToken();

        return CurrentTok;
    }

    ExprAST *CompilerContext::Error(const std::string &msg) {
        CompileError err(msg, fileName, line);
        std::cerr << err.ToString() << std::endl;
        errors.push_back(err);

        return nullptr;
    }

    PrototypeAST *CompilerContext::ErrorP(const std::string &msg) {
        Error(msg);
        return nullptr;
    }

    FunctionAST *CompilerContext::ErrorF(const std::string &msg) {
        Error(msg);
        return nullptr;
    }

    // This is the entry point for an interactive statement
    void CompilerContext::ParseStatement() {
        switch (CurrentTok) {
            case Token::Def:    HandleDefinition(); break;
            case Token::Class:  HandleClass(); break;
            case Token::Extern: HandleExtern(); break;
            default:            HandleTopLevelExpression(); break;
        }
    }

    /// integerexpr ::= integer
    ExprAST *CompilerContext::ParseIntegerExpr() {
        ExprAST *result = new IntegerExprAST(IntVal);
        GetNextToken();
        return result;
    }
    
    /// parenexpr ::= '(' expression ')'
    ExprAST *CompilerContext::ParseParenExpr() {
        // Throw away the open paren
        GetNextToken();

        ExprAST *ret = ParseExpression();
        if (!ret) {
            return nullptr;
        }

        // After the expression we expect a close parenthesis
        if (CurrentTok != Token::CloseParen) {
            return Error("Expected closing parenthesis ')'");
        }

        GetNextToken();

        return ret;
    }

    /// identifierexpr
    ///   ::= identifier
    ///   ::= identifier '(' expression* ')'
    ExprAST *CompilerContext::ParseIdentifierExpr() {
        // Remember the function id before we move on
        std::string idName = IdentifierStr;
        GetNextToken();

        // TODO: Assignment?
        if (CurrentTok != Token::OpenParen) {
            return new VariableExprAST(idName);
        }

        // Function call - ignore the open paren
        GetNextToken();

        std::vector<ExprAST*> args;

        if (CurrentTok != Token::CloseParen) {
            while (true) {
                ExprAST *arg = ParseExpression();

                if (!arg) {
                    return nullptr;
                }

                args.push_back(arg);

                if (CurrentTok == Token::CloseParen) {
                    break;
                }

                // If we have more arguments, we need to find a comma first
                if (CurrentTok != Token::Comma) {
                    return Error("Expected ')' or ',' in argument list");
                }

                GetNextToken();
            }
        }

        // Throw away the close paren
        GetNextToken();
        
        return new CallExprAST(idName, args);
    }

    /// primary
    ///   ::= identifierexpr
    ///   ::= numberexpr
    ///   ::= parenexpr
    ExprAST *CompilerContext::ParsePrimary() {
        switch (CurrentTok) {
            case Token::Identifier:
                return ParseIdentifierExpr();

            case Token::Integer:
                return ParseIntegerExpr();

            case Token::OpenParen:
                return ParseParenExpr();
        
            default:
                break;
        }

        return Error("Unexpected token - expected expression");
    }

    int CompilerContext::GetTokPrecedence() {
        if (BinOpPrecedence.find(CurrentTok) == BinOpPrecedence.end()) {
            return -1;
        }

        return BinOpPrecedence[CurrentTok];
    }

    /// expression
    ///   ::= primary binoprhs
    ExprAST *CompilerContext::ParseExpression() {
        ExprAST *LHS = ParsePrimary();

        if (!LHS) {
            return nullptr;
        }

        return ParseBinOpRHS(0, LHS);
    }

    /// binoprhs
    ///   ::= ('+' primary)*
    ExprAST *CompilerContext::ParseBinOpRHS(int exprPrec, ExprAST *lhs) {
        while (true) {
            int precedence = GetTokPrecedence();

            if (precedence < exprPrec) {
                return lhs;
            }

            // This must be a binary expression
            Token op = CurrentTok;
            GetNextToken();

            ExprAST *rhs = ParsePrimary();
            if (!rhs) {
                return nullptr;
            }

            // If the operation binds less tightly with the RHS of the operator after RHS, let
            // the next operator take RHS as its LHS
            int nextPrecedence = GetTokPrecedence();
            if (precedence < nextPrecedence) {
                rhs = ParseBinOpRHS(precedence + 1, rhs);

                if (!rhs) {
                    return nullptr;
                }
            }

            // Merge LHS with RHS
            lhs = new BinaryExprAST(op, lhs, rhs);
        }
    }

    /// prototype
    ///   ::= id '(' id* ')'
    PrototypeAST *CompilerContext::ParsePrototype() {
        std::cout << "INFO: Parsing prototype" << std::endl;

        if (CurrentTok != Token::Identifier) {
            return ErrorP("Expected function name in prototype");
        }
        
        std::string funcName = IdentifierStr;
        GetNextToken();

        if (CurrentTok != Token::OpenParen) {
            return ErrorP("Expected '(' in prototype");
        }

        // Read the argument names
        std::vector<std::string> args;

        while (GetNextToken() == Token::Identifier) {
            args.push_back(IdentifierStr);

            GetNextToken();

            if (CurrentTok == Token::CloseParen) {
                break;
            }

            if (CurrentTok != Token::Comma) {
                return ErrorP("Expected ',' or ')' after identifier");
            }
        }

        // Check we've got a close parenthesis
        if (CurrentTok != Token::CloseParen) {
            return ErrorP("Expected ')'");
        }

        // Skip the close paren
        GetNextToken();

        return new PrototypeAST(funcName, args);
    }

    /// definition ::= 'def' prototype ':' expression
    FunctionAST *CompilerContext::ParseDefinition() {
        std::cout << "INFO: Handling function definition" << std::endl;
        // Skip the def keyword
        GetNextToken();

        PrototypeAST *proto = ParsePrototype();

        if (!proto) {
            return nullptr;
        }

        // Function definitions need a trailing colon
        if (CurrentTok != Token::Colon) {
            return ErrorF("Expected ':' in function definition");
        }

        // Set the context to be inside a function/loop/class
        insideConstruct = true;

        // Skip past the colon
        GetNextToken();

        // Skip past the NewLine if there is one
        if (CurrentTok == Token::NewLine) {
            // TODO: Handle the indent level
            GetNextToken();
        }

        // Now we expect an indented block
        if (CurrentTok != Token::Indent) {
            insideConstruct = false;
            return ErrorF("Expected an indented block");
        }

        // Skip past indent
        GetNextToken();

        if (ExprAST *e = ParseExpression()) {
            insideConstruct = false;
            return new FunctionAST(proto, e);
        }

        insideConstruct = false;
        return nullptr;
    }

    /// external ::= 'extern' prototype
    PrototypeAST *CompilerContext::ParseExtern() {
        GetNextToken();

        return ParsePrototype();
    }

    /// toplevelexpr ::= expression
    FunctionAST *CompilerContext::ParseTopLevelExpr() {
        if (ExprAST *expr = ParseExpression()) {
            // Create an anonymous function to run this in
            PrototypeAST *proto = new PrototypeAST("", std::vector<std::string>());

            return new FunctionAST(proto, expr);
        }

        return nullptr;
    }

    // =========================================
    // TEMP FUNCTIONS
    // =========================================
    void CompilerContext::HandleDefinition() {
        FunctionAST *func = ParseDefinition();

        if (func != nullptr) {
            std::cerr << "DEBUG: Parsed a function definition [" << func->GetName() << "]" << std::endl;

            // TODO: Add to AST
            delete func;
        } else {
            // Skip token for error recovery.
            if (!IsInteractive()) {
                GetNextToken();
            }
        }
    }

    void CompilerContext::HandleClass() {
        std::cerr << "FIXME: Class parsing Unimplemented" << std::endl;
        GetNextToken();
    }

    void CompilerContext::HandleExtern() {
        PrototypeAST *proto = ParseExtern();

        if (proto != nullptr) {
            std::cerr << "DEBUG: Parsed an extern [" << proto->GetName() << "]" << std::endl;

            // TODO: Add to AST
            delete proto;
        } else {
            // Skip token for error recovery.
            GetNextToken();
        }
    }

    void CompilerContext::HandleTopLevelExpression() {
        // Evaluate a top-level expression into an anonymous function.
        if (ParseTopLevelExpr()) {
            std::cerr << "DEBUG: Parsed a top-level expr" << std::endl;
        } else {
            // Skip token for error recovery.
            GetNextToken();
        }
    }

}

