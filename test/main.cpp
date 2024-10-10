//
// Created by root on 10/8/24.

#include <json.h>
#include <memory>
#include <optional>


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

    json::Tokenizer tokenizer(text);
    std::optional<json::Token> token = tokenizer.getToken();
    while (token.has_value()) {
        std::cout << token.value() << std::endl;
        token = tokenizer.getToken();
    }
    return 0;
}