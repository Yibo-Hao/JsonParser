//
// Created by root on 10/8/24.
//

#ifndef JSON_PARSER_JSON_PARSER_H
#define JSON_PARSER_JSON_PARSER_H

#include <variant>
#include <string>
#include <string_view>
#include <optional>
#include <iostream>

namespace json {
    struct Token {
        enum class Type {
            OBJECT_OPEN, OBJECT_CLOSE, ARRAY_OPEN, ARRAY_CLOSE,
            STRING, NUMBER, BOOLEAN, NULL_VALUE, COMMA, COLON
        } type;

        std::variant<std::string, nullptr_t, double, bool> value = nullptr;
    };

    class Tokenizer {
    public:
        Tokenizer(std::string_view text) : text(text), pos(0) {}

        std::optional<Token> getToken();
    private:
        std::string_view text;

        size_t pos = 0;
    };
};

std::ostream &operator<<(std::ostream &out, const json::Token &token);

#endif //JSON_PARSER_JSON_PARSER_H
