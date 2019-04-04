#ifndef _MENCI_HTML_PARSER_ARGUMENTS_H
#define _MENCI_HTML_PARSER_ARGUMENTS_H

#include "StringEx.h"

struct Arguments {
    StringEx inputFile;
};

Arguments parseArguments(int argc, char *argv[]);

#endif // _MENCI_HTML_PARSER_ARGUMENTS_H
