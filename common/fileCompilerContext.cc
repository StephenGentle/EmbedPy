#include <fstream>
#include <iostream>
#include "context.h"

namespace embedpy {

    int FileCompilerContext::compile(const std::string &input, const std::string &output) {
        setFileName(input);
        
        // Open the input file for reading
        in.open(input, std::fstream::in);

        if (!in.is_open()) {
            std::cerr << "embedpy: error: could not open " << input << std::endl;
            return 1;
        }
        
        MainLoop();

        return 0;
    }

}
