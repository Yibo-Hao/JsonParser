//
// Created by root on 10/8/24.
//

#include <optional>
#include <string>
#include <string_view>
#include <stdexcept>
#include <memory>

#include <json_nodes.h>
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

std::shared_ptr<JsonNode> Parser::parse() {
    while(std::optional<Token> opt = tokenizer.getToken()) {
        Token token = opt.value();
        if (token.type == Token::Type::OBJECT_OPEN) {
            return parseObject();
        } else if (token.type == Token::Type::ARRAY_OPEN) {
            return parseArray();
        } else if (token.type == Token::Type::STRING) {
            std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
            node->setString(std::get<std::string>(token.value));
            return node;
        } else if (token.type == Token::Type::NUMBER) {
            std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
            node->setNumber(std::get<double>(token.value));
            return node;
        } else if (token.type == Token::Type::BOOLEAN) {
            std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
            node->setBoolean(std::get<bool>(token.value));
            return node;
        } else if (token.type == Token::Type::NULL_VALUE) {
            std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
            node->setNull(std::get<std::nullptr_t>(token.value));
            return node;
        } else {
            throw std::invalid_argument("unexpected token");
        }
    }
}

std::shared_ptr<JsonNode> Parser::parseObject() {
    JsonObject object;

    while(std::optional<Token> opt = tokenizer.getToken()) {
        Token token = opt.value();
        std::string key;

        if (token.type == Token::Type::STRING) {
            key = std::get<std::string> (token.value);
        } else if (token.type == Token::Type::OBJECT_CLOSE) {
            std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
            node->setObject(object);
            return node;
        } else {
            throw std::invalid_argument("expected string as key");
        }

        std::optional<Token> colon = tokenizer.getToken();
        if (not colon.has_value() or colon.value().type != Token::Type::COLON) {
            throw std::invalid_argument("expected colon");
        }

        std::optional<Token> opt3 = tokenizer.getToken();
        if (not opt3.has_value()) {
            throw std::invalid_argument("unexpected end of input");
        }
        Token token3 = opt3.value();
        if (token3.type == Token::Type::OBJECT_OPEN) {
            object.emplace(key, parseObject());
        } else if (token3.type == Token::Type::ARRAY_OPEN) {
            object.emplace(key, parseArray());
        } else if (token3.type == Token::Type::STRING) {
            std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
            node->setString(std::get<std::string>(token3.value));
            object.emplace(key, node);
        } else if (token3.type == Token::Type::NUMBER) {
            std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
            node->setBoolean(std::get<double>(token3.value));
            object.emplace(key, node);
        } else if (token3.type == Token::Type::BOOLEAN) {
            std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
            node->setBoolean(std::get<bool>(token3.value));
            object.emplace(key, node);
        } else if (token3.type == Token::Type::NULL_VALUE) {
            std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
            node->setNull(std::get<std::nullptr_t>(token3.value));
            object.emplace(key, node);
        } else {
            throw std::invalid_argument("unexpected token");
        }

        std::optional<Token> opt4 = tokenizer.getToken();
        if (not opt4.has_value()) {
            throw std::invalid_argument("unexpected end of input");
        }

        Token token4 = opt4.value();
        if (token4.type == Token::Type::OBJECT_CLOSE) {
            std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
            node->setObject(object);
            return node;
        } else if (token4.type != Token::Type::COMMA) {
            throw std::invalid_argument("expected comma");
        }
    }

    return nullptr;
}

std::shared_ptr<JsonNode> Parser::parseArray() {
    JsonArray array;

    while(std::optional<Token> opt = tokenizer.getToken()) {
        Token token = opt.value();

        if (token.type == Token::Type::OBJECT_OPEN) {
            array.push_back(parseObject());
        } else if (token.type == Token::Type::ARRAY_OPEN) {
            array.push_back(parseArray());
        } else if (token.type == Token::Type::STRING) {
            std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
            node->setString(std::get<std::string>(token.value));
            array.push_back(node);
        } else if (token.type == Token::Type::NUMBER) {
            std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
            node->setNumber(std::get<double>(token.value));
            array.push_back(node);
        } else if (token.type == Token::Type::BOOLEAN) {
            std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
            node->setBoolean(std::get<bool>(token.value));
            array.push_back(node);
        } else if (token.type == Token::Type::NULL_VALUE) {
            std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
            node->setNull(std::get<std::nullptr_t>(token.value));
            array.push_back(node);
        } else if (token.type == Token::Type::COMMA) {
            continue;
        } else if (token.type == Token::Type::ARRAY_CLOSE) {
            std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
            node->setArray(array);
            return node;
        } else {
            throw std::invalid_argument("unexpected token");
        }
    }
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