#ifndef _MENCI_HTML_PARSER_TOKEN_H
#define _MENCI_HTML_PARSER_TOKEN_H

#include "StringEx.h"

struct Token {
    enum Type {
        TagLike, Text
    } type;

    StringEx content;

    Token(Type type, StringEx content) : type(type), content(content) {}
};

#endif // _MENCI_HTML_PARSER_TOKEN_H
