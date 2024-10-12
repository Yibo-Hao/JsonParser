# Json_parser
Json_parser in cpp

## Usage
```cpp
#include "json.h"

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

    json::Parser parser(text); 
    std::shared_ptr<json::JsonNode> node = parser.parse(); // parse the json text
    node->printf_no_space(); // print the json
    return 0;
}

```
