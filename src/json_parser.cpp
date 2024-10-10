//
// Created by root on 10/8/24.
//

#include <optional>
#include <string>
#include <string_view>
#include <stdexcept>

#include <json_parser.h>

using namespace json;

std::optional<Token> Tokenizer::getToken() {
    while (isspace(text[pos]) and pos < text.size()) {
        pos++;
    }

    if (pos == text.size()) {
        return std::nullopt;
    } else if (text[pos] == '[') {
        pos++;
        return Token{Token::Type::ARRAY_OPEN};
    } else if (text[pos] == ']') {
        pos++;
        return Token{Token::Type::ARRAY_CLOSE};
    } else if (text[pos] == '{') {
        pos++;
        return Token{Token::Type::OBJECT_OPEN};
    } else if (text[pos] == '}') {
        pos++;
        return Token{Token::Type::OBJECT_CLOSE};
    } else if (text[pos] == ',') {
        pos++;
        return Token{Token::Type::COMMA};
    } else if (text[pos] == ':') {
        pos++;
        return Token{Token::Type::COLON};
    } else if (text[pos] == 't') {
        size_t start = pos;
        if (not (text.substr(start, 4) == "true")) {
            throw std::invalid_argument("expected true");
        }
        pos += 4;
        return Token{Token::Type::BOOLEAN, true};
    } else if (text[pos] == 'f') {
        size_t start = pos;
        if (not (text.substr(start, 5) == "false")) {
            throw std::invalid_argument("expected false");
        }
        pos += 5;
        return Token{Token::Type::BOOLEAN, false};
    } else if (text[pos] == 'n') {
        size_t start = pos;
        if (not (text.substr(start, 4) == "null")) {
            throw std::invalid_argument("expected null");
        }
        pos += 4;
        return Token{Token::Type::NULL_VALUE, nullptr};
    } else if (text[pos] == '"') {
        size_t start = pos + 1;
        size_t end_pos = text.find('"', start);
        if (end_pos == std::string::npos) {
            throw std::invalid_argument("expected closing quote");
        }
        pos += end_pos - start + 2;
        return Token{Token::Type::STRING, std::string(text.substr(start, end_pos - start))};
    } else if (isdigit(text[pos]) or text[pos] == '-' or text[pos] == '+') {
        size_t start = pos;
        while (isdigit(text[pos]) or text[pos] == '.') {
            pos++;
        }
        return Token{Token::Type::NUMBER, std::stod(std::string(text.substr(start, pos - start)))};
    }

    throw std::invalid_argument("unexpected character");
}

std::ostream &operator<<(std::ostream &out, const json::Token &token) {
    if (token.type == json::Token::Type::OBJECT_OPEN) {
        return out << "TOKEN: {";
    } else if (token.type == json::Token::Type::OBJECT_CLOSE) {
        return out << "TOKEN }";
    } else if (token.type == json::Token::Type::ARRAY_OPEN) {
        return out << "TOKEN [";
    } else if (token.type == json::Token::Type::ARRAY_CLOSE) {
        return out << "TOKEN ]";
    } else if (token.type == json::Token::Type::STRING) {
        return out << "TOKEN " << std::get<std::string>(token.value);
    } else if (token.type == json::Token::Type::NUMBER) {
        return out << "TOKEN " << std::get<double>(token.value);
    } else if (token.type == json::Token::Type::BOOLEAN) {
        return out << "TOKEN " << std::get<bool>(token.value);
    } else if (token.type == json::Token::Type::NULL_VALUE) {
        return out << "TOKEN null";
    } else if (token.type == json::Token::Type::COMMA) {
        return out << "TOKEN ,";
    } else if (token.type == json::Token::Type::COLON) {
        return out << "TOKEN :";
    }

    return out;
}