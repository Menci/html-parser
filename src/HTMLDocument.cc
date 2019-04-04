#include "HTMLDocument.h"

#include <exception>

HTMLDocument::Element HTMLDocument::internelGetElementById(const DOM::NodeWithChildren *node, const StringEx &id) {
    if (id.empty()) {
        throw std::logic_error("Empty string passed to getElementById()");
    }

    for (const std::shared_ptr<DOM::Node> &child : node->children) {
        if (auto childElement = std::dynamic_pointer_cast<DOM::ElementNode>(child)) {
            if (HTMLDocument::internelGetAttribute(childElement.get(), "id") == id) {
                return HTMLDocument::Element(childElement);
            }

            if (HTMLDocument::Element element = HTMLDocument::internelGetElementById(childElement.get(), id)) {
                return element;
            }
        }
    }

    return HTMLDocument::Element();
}

void HTMLDocument::internelGetElementsByName(const DOM::NodeWithChildren *node,
                                             const StringEx &name,
                                             std::vector<HTMLDocument::Element> &result) {
    if (name.empty()) {
        throw std::logic_error("Empty string passed to getElementsByName()");
    }

    for (const std::shared_ptr<DOM::Node> &child : node->children) {
        if (auto childElement = std::dynamic_pointer_cast<DOM::ElementNode>(child)) {
            if (HTMLDocument::internelGetAttribute(childElement.get(), "name") == name) {
                result.push_back(HTMLDocument::Element(childElement));
            }

            HTMLDocument::internelGetElementsByName(childElement.get(), name, result);
        }
    }
}

void HTMLDocument::internelGetElementsByTagName(const DOM::NodeWithChildren *node,
                                                const StringEx &tagName,
                                                std::vector<HTMLDocument::Element> &result) {
    if (tagName.empty()) {
        throw std::logic_error("Empty string passed to getElementsByTagName()");
    }

    for (const std::shared_ptr<DOM::Node> &child : node->children) {
        if (auto childElement = std::dynamic_pointer_cast<DOM::ElementNode>(child)) {
            if (childElement->tagName == tagName) {
                result.push_back(HTMLDocument::Element(childElement));
            }

            HTMLDocument::internelGetElementsByTagName(childElement.get(), tagName, result);
        }
    }
}

void HTMLDocument::internelGetElementsByClassName(const DOM::NodeWithChildren *node,
                                                  const StringEx &className,
                                                  std::vector<HTMLDocument::Element> &result) {
    if (className.empty()) {
        throw std::logic_error("Empty string passed to getElementsByName()");
    }

    for (const std::shared_ptr<DOM::Node> &child : node->children) {
        if (auto childElement = std::dynamic_pointer_cast<DOM::ElementNode>(child)) {
            if (childElement->classList.count(className) != 0) {
                result.push_back(HTMLDocument::Element(childElement));
            }

            HTMLDocument::internelGetElementsByClassName(childElement.get(), className, result);
        }
    }
}

void HTMLDocument::internelGetTextContent(const DOM::NodeWithChildren *node, StringEx &result) {
    for (const std::shared_ptr<DOM::Node> &child : node->children) {
        if (auto childElement = std::dynamic_pointer_cast<DOM::ElementNode>(child)) {
            HTMLDocument::internelGetTextContent(childElement.get(), result);
        } else if (auto childTextNode = std::dynamic_pointer_cast<DOM::TextNode>(child)) {
            result += childTextNode->content;
        }
    }
}

StringEx HTMLDocument::internelGetAttribute(const DOM::ElementNode *node, const StringEx &name) {
    std::map<StringEx, StringEx>::const_iterator it = node->attributes.find(name.toLower());
    if (it == node->attributes.end()) return "";
    return it->second;
}
