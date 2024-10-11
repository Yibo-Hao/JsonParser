//
// Created by root on 10/8/24.
//

#ifndef JSONPARSER_JSON_NODES_H
#define JSONPARSER_JSON_NODES_H

#include <unordered_map>
#include <variant>
#include <string>
#include <memory>
#include <vector>
#include <iostream>

namespace json {
    class JsonNode;

    enum class type;

    using JsonObject = std::unordered_map<std::string, std::shared_ptr<JsonNode>>;

    using JsonArray = std::vector<std::shared_ptr<JsonNode>>;

    class JsonNode {
    public:
        JsonObject getObject() const {
            return std::get<JsonObject>(value);
        }

        void setObject(JsonObject object) {
            this->type = type::OBJECT;
            value = std::move(object);
        }

        JsonArray getArray() const {
            return std::get<JsonArray>(value);
        }

        void setArray(JsonArray array) {
            this->type = type::ARRAY;
            value = std::move(array);
        }

        std::string getString() const {
            return std::get<std::string>(value);
        }

        void setString(std::string string) {
            this->type = type::STRING;
            value = std::move(string);
        }

        double getNumber() const {
            return std::get<double>(value);
        }

        void setNumber(double number) {
            this->type = type::NUMBER;
            value = number;
        }

        bool getBoolean() const {
            return std::get<bool>(value);
        }

        void setBoolean(bool boolean) {
            this->type = type::BOOLEAN;
            value = boolean;
        }

        std::nullptr_t getNull() const {
            return std::get<std::nullptr_t>(value);
        }

        void setNull(std::nullptr_t null) {
            this->type = type::NULL_VALUE;
            value = null;
        }

        void printf(std::ostream &out = std::cout, int indent = 0) const;
    private:
        enum class type {
            OBJECT, ARRAY, STRING, NUMBER, BOOLEAN, NULL_VALUE
        } type;

        std::variant<JsonObject, JsonArray, std::string, double, bool, nullptr_t> value{};
    };

}

#endif //JSONPARSER_JSON_NODES_H
