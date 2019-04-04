#ifndef _MENCI_HTML_PARSER_DOM_H
#define _MENCI_HTML_PARSER_DOM_H

#include <map>
#include <set>
#include <vector>
#include <memory>

#include "StringEx.h"

namespace DOM {

struct Node {
    struct NodeWithChildren *parentNode;

    Node(struct NodeWithChildren *parentNode) : parentNode(parentNode) {}

    // A class without virtual table can't be dymanic_cast-ed.
    virtual ~Node() {}
};

struct NodeWithChildren : Node {
    NodeWithChildren(NodeWithChildren *parentNode) : Node(parentNode) {}

    std::vector<std::shared_ptr<Node>> children;
};

struct RootNode : NodeWithChildren {
    RootNode() : NodeWithChildren(nullptr) {}
};

struct ElementNode : NodeWithChildren {
    ElementNode(NodeWithChildren *parentNode) : NodeWithChildren(parentNode) {}

    StringEx tagName;
    std::map<StringEx, StringEx> attributes;
    std::set<StringEx> classList;
};

struct TextNode : Node {
    StringEx content;

    TextNode(NodeWithChildren *parentNode, const StringEx &content) : Node(parentNode), content(content) {}
};

struct CommentNode : Node {
    StringEx content;

    CommentNode(NodeWithChildren *parentNode, const StringEx &content) : Node(parentNode), content(content) {}
};

}

#endif // _MENCI_HTML_PARSER_DOM_H
