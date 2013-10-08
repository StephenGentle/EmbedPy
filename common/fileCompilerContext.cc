#include <fstream>
#include <iostream>
#include "context.h"

namespace embedpy {

    bool FileCompilerContext::LoadFile(const std::string &input) {
        setFileName(input);
        
        // Open the input file for reading
        in.open(input, std::fstream::in);

        if (!in.is_open()) {
            std::cerr << "embedpy: error: could not open " << input << std::endl;
            return false;
        }

        return true;
    }
        

    int FileCompilerContext::Compile(const std::string &input, const std::string &output) {
        if (!LoadFile(input)) {
            return 2;
        }
       
        MainLoop();

        return 0;
    }

    int FileCompilerContext::TokeniseFile(const std::string &input) {
        if (!LoadFile(input)) {
            return 2;
        }

        this->Tokenise();

        return 0;
    }

}
