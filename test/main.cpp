//
// Created by root on 10/8/24.

#include <json.h>
#include <memory>

int main() {
    std::string text = R"(
{
    "name": "John",
    "age": 30,
    "cars": [
        {
            "model": "BMW 230",
            "mpg": 27.5
        },
        {
            "model": "Ford Edge",
            "mpg": 24.1
        }
    ]
})";

    json::Parser parser(text);
    std::shared_ptr<json::JsonNode> node = parser.parse();
    node->printf_no_space();
    return 0;
}