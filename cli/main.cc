#include <iostream>
#include "cmdline.h"
#include <embedpy/context.h>

int main(int argc, char **argv)
{
    if (argc == 1) {
        // Instaiante an interactive session
        embedpy::InteractiveCompilerContext c;

        c.MainLoop();

        return 0;
    }

    // Get version flag
    if (flagExists(argv, argc, "-v") || flagExists(argv, argc, "--version")) {
        std::cout << "EmbedPy Compiler 0.1. Copyright Nanodesign Pty Ltd." << std::endl;
        return 0;
    }

    // Help flag
    if (flagExists(argv, argc, "-h") || flagExists(argv, argc, "--help")) {
        std::cout << "embedpy [flags] -o output source" << std::endl;
        return 0;
    }

    int cmdCount = 1;
    const char *output;
    const char *input = argv[argc - 1];

    if ((output = getOptionValue(argv, argc, "-o"))) {
        cmdCount += 2;
    } else {
        // default output file like GCC
        output = "a.out";
    }

    // Check if we have enough array values left and the last is not a flag
    if (cmdCount == argc || (cmdCount < argc && argv[argc - 1][0] == '-')) {
        std::cerr << "embedpy: error: no input file" << std::endl;
        return 1;
    }

    // Create a file-reading compiler context, compile
    embedpy::FileCompilerContext c;
    return c.compile(input, output);
}
