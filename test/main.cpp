//
// Created by root on 10/8/24.

#include <json_nodes.h>
#include <memory>


int main() {
    json::JsonNode jsonNode{};
    std::shared_ptr<json::JsonNode> jsonNode1 = std::make_shared<json::JsonNode>();
    std::shared_ptr<json::JsonNode> jsonNode2 = std::make_shared<json::JsonNode>();

    // test print
    jsonNode.setString("Hello"); 
    jsonNode1->setNumber(3.14);
    jsonNode2->setString("World");

    jsonNode.setObject({{"key1", jsonNode1}, {"key2", jsonNode2}});
    jsonNode.printf(std::cout);

    return 0;
}