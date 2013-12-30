#include <fstream>
#include <iostream>
#include "context.h"

namespace embedpy {

    void InteractiveCompilerContext::MainLoop() {
        std::cout << "embedpy> ";
        GetNextToken();
        
        while (true) {
            switch (CurrentTok) {
                case Token::eof: 
                    return;
                
                default:
                    break;
            }

            GetNextToken();

        }

    }

}
