#include "DOMBuilder.h"

#include <regex>

#include "HTMLEntities.h"

// These elements' open tags don't need close tags and they don't have a subtree.
// From https://developer.mozilla.org/en-US/docs/Glossary/Empty_element.
static const char *const emptyElements[] = {
    "area",
    "base",
    "br",
    "col",
    "colgroup",
    "command",
    "embed",
    "hr",
    "img",
    "input",
    "keygen",
    "link",
    "meta",
    "param",
    "source",
    "track",
    "wbr"
};

// These elements don't contain any child elements. Everything inside them is
// just a TextNode.
// From https://www.w3.org/TR/html5/syntax.html#writing-html-documents-elements.
static const char *const rawTextElements[] = {
    "script",
    "style",
    "textarea",
    "title"
};

// These raw text elements contain escaped string -- e.g. &nbsp; will be replaced
// with a space ' ' in these elements but not in other raw text elements.
// From https://www.w3.org/TR/html5/syntax.html#writing-html-documents-elements.
static const char *const escapableRawTextElements[] = {
    "textarea",
    "title"
};

static void parseTagOpen(const StringEx &token, StringEx &tagName, std::map<StringEx, StringEx> &attributes) {
    // Extract tag name and all attributes.
    // ^<(\S+)\s*([\S\s]*)>$
    static const std::regex reTag(
        "^<(\\S+)\\s*([\\S\\s]*)>$"
    );

    std::smatch match;
    if (!std::regex_search(token.begin(), token.end(), match, reTag)) {
        throw std::logic_error("Failed to apply regex on a tag open token");
    }

    tagName = match[1].str();
    StringEx attributesString = match[2].str();

    // HTML tag's name is case-insensitive.
    tagName = tagName.toLower();
    
    // Extract all attributes.
    // ([^>\s][^>\s=]*)(?:\s*=\s*(?:(?:"([\S\s]*?)")|(?:'([\S\s]*?)')|([^\s>]+)))?\s*
    static const std::regex reAttribute(
        "([^>\\s][^>\\s=]*)(?:\\s*=\\s*(?:(?:\"([\\S\\s]*?)\")|(?:'([\\S\\s]*?)')|([^\\s>]+)))?\\s*"
    );

    std::string::const_iterator searchStart = attributesString.cbegin();
    while (std::regex_search(searchStart, attributesString.cend(), match, reAttribute)) {
        StringEx key = match[1].str(),
                 value;
        
        // A self-closing tag's close sign '/' is parsed to an attribute name
        if (key != "/") {
            // 3 types: <"str">, <'str'> and <str>, in capture group 2, 3 and 4.
            for (int i = 2; i <= 4; i++) {
                if (match[i].length() > 0) {
                    value = match[i].str();
                    break;
                }
            }

            // HTML tag's attribute's name is case-insensitive.
            key = key.toLower();

            attributes[key] = value;
        }

        searchStart = match.suffix().first;
    }
}

static StringEx parseTagClose(const StringEx &token) {
    // Extract tag name.
    static const std::regex reTagName("</(.+?)[\\s>]");

    std::smatch match;
    if (!std::regex_search(token.begin(), token.end(), match, reTagName)) {
        throw std::logic_error("Failed to apply regex on a tag close token");
    }

    StringEx tagName = match[1].str();
    return tagName.toLower();
}

DOM::RootNode buildDOM(const std::vector<Token> &tokens) {
    DOM::RootNode root;

    DOM::NodeWithChildren *currentNode = &root;
    for (const Token &token : tokens) {
        // Check if we're inside a raw text element.
        if (auto *currentElement = dynamic_cast<DOM::ElementNode *>(currentNode)) {
            const StringEx &tagName = currentElement->tagName;
            if (std::find(std::begin(rawTextElements),
                          std::end(rawTextElements),
                          tagName)
             != std::end(rawTextElements)) {
                // Inside a raw text element, everything except a matching close tag is text.
                if (token.content.startsWith("</") && parseTagClose(token.content) == tagName) {
                    // OK, close it.
                    currentNode = currentNode->parentNode;
                } else {
                    // Append the token to the TextNode.
                    if (currentElement->children.empty()) {
                        currentElement->children.push_back(std::make_shared<DOM::TextNode>(
                            currentNode,
                            ""
                        ));
                    }

                    StringEx &textContent = std::dynamic_pointer_cast<DOM::TextNode>(
                        currentElement->children[0]
                    )->content;

                    StringEx newText = token.content;
                    if (std::find(std::begin(escapableRawTextElements),
                                  std::end(escapableRawTextElements),
                                  tagName)
                     != std::end(escapableRawTextElements)) {
                        newText = decodeHTMLEntites(newText);
                    }

                    textContent += newText;
                }

                continue;
            }
        }

        switch (token.type) {
        case Token::Type::TagLike:
            static const std::string commentStart = "<!--",
                                     commentEnd = "-->";
            if (token.content.startsWith(commentStart) && token.content.endsWith(commentEnd)) {
                // Comment
                currentNode->children.push_back(std::make_shared<DOM::CommentNode>(currentNode, token.content.substr(
                    commentStart.length(),
                    token.content.length() - commentStart.length() - commentEnd.length()
                )));
            } else {
                // Tag open or tag close.

                if (!token.content.startsWith("</")) {
                    // Tag open.
                    std::shared_ptr<DOM::ElementNode> tag = std::make_shared<DOM::ElementNode>(currentNode);
                    parseTagOpen(token.content, tag->tagName, tag->attributes);

                    currentNode->children.push_back(tag);

                    if (std::find(std::begin(emptyElements),
                                  std::end(emptyElements),
                                  tag->tagName)
                     == std::end(emptyElements)) {
                        // It's NOT a empty element. Open a new subtree.
                        currentNode = tag.get();
                    }
                } else {
                    // Tag close.
                    StringEx tagName = parseTagClose(token.content);

                    if (std::find(std::begin(emptyElements),
                                  std::end(emptyElements),
                                  tagName)
                     != std::end(emptyElements)) {
                        // Someone silly is trying to close a empty element. Just ignore it.
                    } else {
                        // OK. Let's close it.
                        
                        DOM::NodeWithChildren *originalCurrentNode = currentNode;
                        DOM::ElementNode *currentElement;
                        while ((currentElement = dynamic_cast<DOM::ElementNode *>(currentNode)) != nullptr // The node IS a element
                            && currentElement->tagName != tagName) {                                       // and tag name is not matched.
                            // Close current node and continue to check tag name if tag name is NOT matched.
                            currentNode = currentNode->parentNode;
                        }

                        if (currentElement == nullptr) {
                            // Someone silly is trying to close a non-exist tag. Just ignore it and go back.
                            currentNode = originalCurrentNode;
                        } else {
                            // Close current element's node.
                            currentNode = currentNode->parentNode;
                        }
                    }
                }
            }

            break;
        case Token::Type::Text:
            // <!DOCTYPE> is lexed to some text -- since we don't support it, just ignore it.
            if (token.content.trimLeft().startsWith("<!DOCTYPE", true)) {
                break;
            }

            currentNode->children.push_back(std::make_shared<DOM::TextNode>(
                currentNode,
                decodeHTMLEntites(token.content)
            ));
            break;
        default:
            break;
        }
    }

    return root;
}
