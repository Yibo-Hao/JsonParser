//
// Created by root on 10/8/24.
//
#include <variant>
#include <iostream>
#include <string>
#include <utility>

#include <json_nodes.h>

using namespace json;

void JsonNode::printf(std::ostream &out, int indent) const {
    if (std::holds_alternative<JsonArray>(value)) {
        out << '[' << std::endl;
        JsonArray array = std::get<JsonArray>(value);
        for (auto &node : array) {
            out << std::string(indent, ' ');
            node->printf(out, indent * 2);
            if (node != array.back()) {
                out << ',';
            }
            out << std::endl;
        }
        out << ']' << std::endl;
    } else if (std::holds_alternative<JsonObject>(value)) {
        out << '{' << std::endl;
        JsonObject object = std::get<JsonObject>(value);
        int count = 0;
        for (std::pair<const std::string, std::shared_ptr<json::JsonNode>> &node : object) {
            count++;
            out << std::string(indent, ' ');
            out << '"' << node.first << "\": ";
            node.second->printf(out, indent * 2);
            if (count != object.size()) {
                out << ',';
            }
            out << std::endl;
        }
        out << '}' << std::endl;
    } else if (std::holds_alternative<std::string>(value)) {
        out << '"' << std::get<std::string>(value) << '"';
    } else if (std::holds_alternative<double>(value)) {
        out << std::get<double>(value);
    } else if (std::holds_alternative<bool>(value)) {
        out << std::boolalpha << std::get<bool>(value);
    } else if (std::holds_alternative<std::nullptr_t>(value)) {
        out << "null";
    }
    
}
