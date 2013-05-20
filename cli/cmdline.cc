#include <algorithm>

// Basic command line option parser
char *getOptionValue(char **begin, int count, const std::string &option) {
    char **end = begin + count;
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        // Ensure this isn't another flag
        if (*itr[0] == '-') {
            return nullptr;
        }

        return *itr;
    }

    // No option found
    return nullptr;
}

bool flagExists(char** begin, int count, const std::string& option) {
    char **end = begin + count;
    return std::find(begin, end, option) != end;
}

