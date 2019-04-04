#ifndef _MENCI_HTML_PARSER_STRINGEX_H
#define _MENCI_HTML_PARSER_STRINGEX_H

#include <cctype>
#include <string>
#include <vector>
#include <utility>

class StringEx {
    std::string data;

public:
    static const size_t npos = std::string::npos;

    template <typename ...Args>
    StringEx(Args &&...args) : data(std::forward<Args>(args)...) {}
    StringEx(char ch) : data(1, ch) {}

    operator std::string&() { return data; }
    operator const std::string&() const { return data; }
    
    char &operator[](size_t pos) { return data[pos]; }
    const char &operator[](size_t pos) const { return data[pos]; }

#define forwardCompareOperator(op)                                                   \
    friend bool operator op(const StringEx &lhs, const StringEx &rhs) {              \
        return lhs.data op rhs.data;                                                 \
    }                                                                                \
                                                                                     \
    template <typename T>                                                            \
    friend bool operator op(T lhs, const StringEx &rhs) {                            \
        return StringEx(lhs).data op rhs.data;                                       \
    }                                                                                \

    forwardCompareOperator(<)
    forwardCompareOperator(>)
    forwardCompareOperator(<=)
    forwardCompareOperator(>=)
    forwardCompareOperator(==)
    forwardCompareOperator(!=)
#undef forwardCompareOperator

    template <typename T>
    friend StringEx operator+(T lhs, const StringEx &rhs) {
        return StringEx(lhs).data + rhs.data;
    }

    friend StringEx operator+(const StringEx &lhs, const StringEx &rhs) {
        return lhs.data + rhs.data;
    }

    template <typename T> StringEx operator+=(T rhs) {
        data += (const std::string &)rhs;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const StringEx &str) {
        return os << str.data;
    }

    friend std::istream &operator<<(std::istream &is, StringEx &str) {
        return is >> str.data;
    }

#define forwardMemberFunction(newName, name, castReturnType, isConst) \
    template <typename ...Args>                                       \
    auto newName(Args &&...args) isConst { return castReturnType data.name(std::forward<Args>(args)...); }

    forwardMemberFunction(find,          find,             ,               const)
    forwardMemberFunction(reverseFind,   rfind,            ,               const)
    forwardMemberFunction(substr,        substr,           (StringEx),     const)
    forwardMemberFunction(length,        length,           ,               const)
    forwardMemberFunction(empty,         empty,            ,               const)
    forwardMemberFunction(erase,         erase,            (void),              )
    forwardMemberFunction(append,        append,           (void),              )
    forwardMemberFunction(reserve,       reserve,          (void),              )
    forwardMemberFunction(begin,         begin,            ,                    )
    forwardMemberFunction(begin,         begin,            ,               const)
    forwardMemberFunction(cbegin,        cbegin,           ,               const)
    forwardMemberFunction(end,           end,              ,                    )
    forwardMemberFunction(end,           end,              ,               const)
    forwardMemberFunction(cend,          cend,             ,               const)
    forwardMemberFunction(findLastNotOf, find_last_not_of, ,                    )
    forwardMemberFunction(findLastOf,    find_last_of,     ,                    )
    forwardMemberFunction(getData,       c_str,            ,               const)
    forwardMemberFunction(front,         front,            ,               const)
    forwardMemberFunction(back,          back,             ,               const)
#undef forwardMemberFunction

    std::vector<StringEx> split(const StringEx &separator) const {
        std::vector<StringEx> result;
        size_t posFrom = 0;
        for (size_t pos;
             (pos = this->find(separator, posFrom)) != npos;
             posFrom = pos + separator.length()) {
            result.push_back(this->substr(posFrom, pos - posFrom));
        }

        result.push_back(this->substr(posFrom));
        return result;
    }

    bool startsWith(const StringEx &str, bool ignore_case = false) const {
        return this->length() >= str.length() &&
               (ignore_case
                ? this->substr(0, str.length()).toLower() ==
                  str.toLower()
                : this->substr(0, str.length()) == str);
    }

    bool endsWith(const StringEx &str, bool ignore_case = false) const {
        return this->length() >= str.length() &&
               (ignore_case
                ? this->substr(this->length() - str.length(), str.length()).toLower() ==
                  str.toLower()
                : this->substr(this->length() - str.length(), str.length()) == str);
    }

    StringEx toLower() const {
        StringEx result = *this;
        for (size_t i = 0; i < result.length(); i++) result[i] = tolower(result[i]);
        return result;
    }
    
    StringEx toUpper() const {
        StringEx result = *this;
        for (size_t i = 0; i < result.length(); i++) result[i] = toupper(result[i]);
        return result;
    }

    template <typename InputIteartor>
    StringEx join(InputIteartor begin, InputIteartor end) const {
        InputIteartor it = begin;

        StringEx result = *it++;
        for (; it != end; it++) {
            result += *this + *it;
        }

        return result;
    }

    // Get a escaped string like Node.js's util.inspect(str).
    StringEx inspect() const {
        StringEx result;
        for (char ch : data) {
            switch (ch) {
            case '"': result.append("\\\""); break;
            case '\?': result.append("\\?"); break;
            case '\\': result.append("\\\\"); break;
            case '\a': result.append("\\a"); break;
            case '\b': result.append("\\b"); break;
            case '\f': result.append("\\f"); break;
            case '\n': result.append("\\n"); break;
            case '\r': result.append("\\r"); break;
            case '\t': result.append("\\t"); break;
            case '\v': result.append("\\v"); break;
            default: result.append(1, ch);
            }
        }
        return '"' + result + '"';
    }

    StringEx trimLeft() const {
        for (std::string::const_iterator it = data.cbegin(); it != data.cend(); it++) {
            if (!isspace(*it)) return StringEx(it, data.cend());
        }

        return "";
    }

    StringEx trimRight() const {
        for (std::string::const_reverse_iterator it = data.crbegin(); it != data.crend(); it++) {
            if (!isspace(*it)) return StringEx(data.cbegin(), ++it.base());
        }

        return "";
    }

    StringEx trim() const {
        return trimLeft().trimRight();
    }
};

#endif // _MENCI_HTML_PARSER_STRINGEX_H
