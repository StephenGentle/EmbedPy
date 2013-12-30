#include <fstream>
#include <iostream>
#include "context.h"

namespace embedpy {

    void InteractiveCompilerContext::MainLoop() {

        // Display the prompt
        std::cout << "embedpy> ";
        
        // Loop until we get an EOF
        while (true) {
            GetNextToken();

            switch (CurrentTok) {
                case Token::NewLine:
                    // TODO: Execute
                    std::cout << "embedpy> ";
                    break;

                case Token::eof:
                    std::cout << std::endl;
                    return;
                
                default:
                    // TODO: Parse
                    break;
            }

        }

    }

}
