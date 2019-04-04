#ifndef _MENCI_HTML_PARSER_ARGUMENTPARSER_H
#define _MENCI_HTML_PARSER_ARGUMENTPARSER_H

#include <cassert>
#include <map>
#include <memory>
#include <iostream>
#include <functional>
#include <optional>

#include "StringEx.h"

class ArgumentParser {
public:
    using ParserFunction = std::function<std::optional<StringEx> (const StringEx &)>;

    template <typename T>
    using ValidatorFunction = std::function<std::optional<StringEx> (const T &)>;

private:
    struct Argument {
        StringEx name, alias, valueName;
        StringEx description;
        bool optional, specfied;
        StringEx defaultValue;
        ParserFunction parserFunction;
    };

    size_t argc;
    char **argv;

    std::map<StringEx, std::shared_ptr<Argument>> mapNameArgument;
    std::map<StringEx, std::shared_ptr<Argument>> mapAliasArgument;
    std::vector<std::shared_ptr<Argument>> options;
    std::vector<Argument> positionalArguments;

    StringEx programDescription;

public:
    ArgumentParser(int argc, char **argv) : argc((size_t)argc), argv(argv) {}

    template <typename IntType = size_t>
    static ParserFunction integerParser(IntType &var,
                                        ValidatorFunction<IntType> validator = nullptr,
                                        std::function<IntType (const std::string &, size_t *, int)> convertFunction
                                            = (IntType (*)(const std::string &, size_t *, int))std::stoul) {
        return [=, &var](const StringEx &str) -> std::optional<StringEx> {
            try {
                IntType val = convertFunction(str, nullptr, 10);
                if (validator) {
                    std::optional<StringEx> errorMessage = validator(val);
                    if (errorMessage) return errorMessage;
                }

                var = val;
                return std::nullopt;
            } catch (const std::exception &e) {
                return "Not a valid integer: " + str;
            }
        };
    }

    template <typename StringType = StringEx>
    static ParserFunction stringParser(StringType &var,
                                       std::function<std::optional<StringEx> (const StringEx &)> validator = nullptr) {
        return [=, &var](const StringEx &str) -> std::optional<StringEx> {
            if (validator) {
                std::optional<StringEx> errorMessage = validator(str);
                if (errorMessage) return errorMessage;
            }

            var = str;
            return std::nullopt;
        };
    }

    ArgumentParser &setProgramDescription(const StringEx &programDescription);
    ArgumentParser &addOption(const StringEx &name,
                              const StringEx &alias,
                              const StringEx &valueName,
                              const StringEx &description,
                              ParserFunction parserFunction = nullptr,
                              bool optional = false,
                              const StringEx &defaultValue = "");
    ArgumentParser &addPositional(const StringEx &valueName,
                                  const StringEx &description,
                                  ParserFunction parserFunction = nullptr,
                                  bool optional = false,
                                  const StringEx &defaultValue = "");
    ArgumentParser &showHelp();
    ArgumentParser &parse();
};

#endif // _MENCI_HTML_PARSER_ARGUMENTPARSER_H
