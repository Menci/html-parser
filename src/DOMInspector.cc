#include "DOMInspector.h"

#include <iostream>
#include "TerminalColor.h"

void inspectNode(const DOM::Node *node, size_t indentLevel) {
    auto indent = [indentLevel](int add = 0) -> StringEx {
        return StringEx(indentLevel * 4 + add, ' ');
    };

    if (auto rootNode = dynamic_cast<const DOM::RootNode *>(node)) {
        (void)rootNode; // Disable GCC -Wunused-variable.
        std::cout << indent()
                  << TerminalColor::Bold << TerminalColor::ForegroundYellow
                  << "DOM::RootNode"
                  << TerminalColor::Reset
                  << std::endl;
    } else if (auto textNode = dynamic_cast<const DOM::TextNode *>(node)) {
        std::cout << indent()
                  << TerminalColor::Bold << TerminalColor::ForegroundBlue
                  << "DOM::TextNode "
                  << TerminalColor::Reset << TerminalColor::ForegroundGreen
                  << textNode->content.inspect()
                  << TerminalColor::Reset
                  << std::endl;
    } else if (auto elementNode = dynamic_cast<const DOM::ElementNode *>(node)) {
        std::cout << indent()
                  << TerminalColor::Bold << TerminalColor::ForegroundCyan
                  << "DOM::ElementNode"
                  << TerminalColor::Reset
                  << '('
                  << TerminalColor::Bold
                  << elementNode->tagName
                  << TerminalColor::Reset
                  << ')'
                  << std::endl;
        for (const std::pair<StringEx, StringEx> &attribute : elementNode->attributes) {
            std::cout << indent(1)
                      << '['
                      << TerminalColor::ForegroundYellow
                      << "Attribute "
                      << TerminalColor::Reset << TerminalColor::Bold
                      << attribute.first.inspect()
                      << TerminalColor::Reset
                      << " = "
                      << TerminalColor::ForegroundMagenta
                      << attribute.second.inspect()
                      << TerminalColor::Reset
                      << ']'
                      << std::endl;
        }
    } else if (auto commentNode = dynamic_cast<const DOM::CommentNode *>(node)) {
        std::cout << indent()
                  << TerminalColor::Bold << TerminalColor::ForegroundMagenta
                  << "DOM::CommentNode "
                  << TerminalColor::Reset << TerminalColor::ForegroundGreen
                  << commentNode->content.inspect()
                  << TerminalColor::Reset
                  << std::endl;
    }

    if (auto nodeWithChildren = dynamic_cast<const DOM::NodeWithChildren *>(node)) {
        for (const std::shared_ptr<DOM::Node> &childNode : nodeWithChildren->children) {
            inspectNode(childNode.get(), indentLevel + 1);
        }
    }
}
