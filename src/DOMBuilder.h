#ifndef _MENCI_HTML_PARSER_DOMBUILDER_H
#define _MENCI_HTML_PARSER_DOMBUILDER_H

#include "Token.h"
#include "DOM.h"
#include "StringEx.h"

DOM::RootNode buildDOM(const std::vector<Token> &tokens);

#endif // _MENCI_HTML_PARSER_DOMBUILDER_H
