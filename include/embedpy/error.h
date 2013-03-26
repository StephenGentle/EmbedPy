#ifndef EMBEDPY_ERROR_H
#define EMBEDPY_ERROR_H

#include <sstream>

namespace embedpy {

    class CompileError {
    public:
        CompileError(const std::string &ErrorText, const std::string &FileName, int Line) 
            : errorText(ErrorText), fileName(FileName), line(Line) {}

        std::string ToString() {
            std::stringstream out;
            out << " File \"" << fileName << "\", line " << line << std::endl;
            out << errorText;

            return out.str();
        }

    private:
        std::string errorText;
        std::string fileName;
        int line;
    };
}

#endif
