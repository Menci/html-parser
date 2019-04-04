#ifndef _MENCI_HTML_PARSER_HTMLDOCUMENT_H
#define _MENCI_HTML_PARSER_HTMLDOCUMENT_H

#include <istream>
#include <memory>
#include <type_traits>

#include "Lexer.h"
#include "DOM.h"
#include "DOMBuilder.h"
#include "DOMInspector.h"
#include "StringEx.h"

class HTMLDocument {
    DOM::RootNode domRoot;

public:    
    class Element {
        friend class HTMLDocument;

        const std::shared_ptr<DOM::ElementNode> node;

        explicit Element(const std::shared_ptr<DOM::ElementNode> node) : node(node) {}

        void checkNotNull() const {
            if (node == nullptr) {
                throw std::invalid_argument("Element is null");
            }
        }

    public:
        Element() = default;
        Element(const Element &) = default;
        Element(Element &&) = default;

        void inspect() const {
            checkNotNull();
            inspectNode(node.get());
        }

        StringEx getTextContent() const {
            checkNotNull();
            StringEx result;
            HTMLDocument::internelGetTextContent(node.get(), result);
            return result;
        }

        StringEx getAttribute(const StringEx &name) const {
            checkNotNull();
            return HTMLDocument::internelGetAttribute(node.get(), name);
        }

        std::vector<Element> getElementsByTagName(const StringEx &tagName) const {
            checkNotNull();
            std::vector<Element> result;
            HTMLDocument::internelGetElementsByTagName(node.get(), tagName, result);
            return result;
        }

        std::vector<Element> getElementsByClassName(const StringEx &className) const {
            checkNotNull();
            std::vector<Element> result;
            HTMLDocument::internelGetElementsByClassName(node.get(), className, result);
            return result;
        }

        operator bool() const {
            return node != nullptr;
        }
    };

private:
    static Element internelGetElementById(const DOM::NodeWithChildren *node, const StringEx &id);
    static void internelGetElementsByName(const DOM::NodeWithChildren *node, const StringEx &name, std::vector<Element> &result);
    static void internelGetElementsByTagName(const DOM::NodeWithChildren *node, const StringEx &tagName, std::vector<Element> &result);
    static void internelGetElementsByClassName(const DOM::NodeWithChildren *node, const StringEx &className, std::vector<Element> &result);
    static void internelGetTextContent(const DOM::NodeWithChildren *node, StringEx &result);
    static StringEx internelGetAttribute(const DOM::ElementNode *node, const StringEx &name);

public:
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<std::istream, std::remove_reference_t<T>>>>
    explicit HTMLDocument(T &&is) : HTMLDocument(StringEx(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>())) {}

    explicit HTMLDocument(const StringEx &html) {
        parse(html);
    }

    void parse(const StringEx &html) {
        domRoot = buildDOM(getTokens(html));
    }

    void inspect() const {
        inspectNode(&domRoot);
    }

    StringEx getTextContent() const {
        StringEx result;
        HTMLDocument::internelGetTextContent(&domRoot, result);
        return result;
    }

    Element getElementById(const StringEx &id) const {
        return HTMLDocument::internelGetElementById(&domRoot, id);
    }

    std::vector<Element> getElementsByName(const StringEx &name) const {
        std::vector<Element> result;
        HTMLDocument::internelGetElementsByName(&domRoot, name, result);
        return result;
    }

    std::vector<Element> getElementsByTagName(const StringEx &tagName) const {
        std::vector<Element> result;
        HTMLDocument::internelGetElementsByTagName(&domRoot, tagName, result);
        return result;
    }

    std::vector<Element> getElementsByClassName(const StringEx &className) const {
        std::vector<Element> result;
        HTMLDocument::internelGetElementsByClassName(&domRoot, className, result);
        return result;
    }

    StringEx getTitle() const {
        std::vector<Element> titleElements = this->getElementsByTagName("title");
        if (titleElements.empty()) return "";
        return titleElements[0].getTextContent().trim();
    }
};

#endif // _MENCI_HTML_PARSER_HTMLDOCUMENT_H
