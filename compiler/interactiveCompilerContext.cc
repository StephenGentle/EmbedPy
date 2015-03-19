#include <fstream>
#include <iostream>
#include "context.h"

namespace embedpy {

    void InteractiveCompilerContext::MainLoop() {

        DisplayPrompt();

        // Loop until we get an EOF
        while (true) {
            GetNextToken();

            switch (CurrentTok) {
                case Token::NewLine:
                case Token::eof:
                    break;

                default:
                    // Parse the input and execute
                    ParseStatement();
                    break;
            }

            // Check if the user exited
            if (CurrentTok == Token::eof) {
                std::cout << std::endl;
                return;
            }
        }
    }
}
