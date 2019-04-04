#ifndef _MENCI_HTML_PARSER_LEXER_H
#define _MENCI_HTML_PARSER_LEXER_H

#include <vector>
#include "Token.h"
#include "StringEx.h"

std::vector<Token> getTokens(const StringEx &htmlContents);

#endif // _MENCI_HTML_PARSER_LEXER_H
