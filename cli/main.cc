#include <iostream>

#include <embedpy/context.h>

extern void mainLoop();

int main()
{
    std::cout << "EmbedPy Compiler" << std::endl;

    embedpy::CompilerContext c;

    c.MainLoop();

    return 0;
}
