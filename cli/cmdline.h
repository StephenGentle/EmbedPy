#ifndef EMBEDPY_COMMANDLINE_H
#define EMBEDPY_COMMANDLINE_H

char *getOptionValue(char **begin, int count, const std::string &option);
bool flagExists(char** begin, int count, const std::string& option);

#endif
