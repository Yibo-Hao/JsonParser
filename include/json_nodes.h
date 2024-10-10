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

namespace json {
    class JsonNode;

    using JsonObject = std::unordered_map<std::string, std::shared_ptr<JsonNode>>;

    using JsonArray = std::vector<std::shared_ptr<JsonNode>>;

    class JsonNode {
        enum class type {
            OBJECT, ARRAY, STRING, NUMBER, BOOLEAN, NULL_VALUE
        } type;

        std::variant<JsonObject, JsonArray, std::string, double, bool, nullptr_t> value{};
    };

}

#endif //JSONPARSER_JSON_NODES_H
