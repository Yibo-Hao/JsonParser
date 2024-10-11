cmake_minimum_required(VERSION 3.28)
project(json_parser)

set(CMAKE_CXX_STANDARD 17)

add_library(json
    src/json_parser.cpp
    src/json_nodes.cpp)

target_include_directories(json
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include)

add_executable(test
    ${PROJECT_SOURCE_DIR}/test/main.cpp)
target_link_libraries(test
    PUBLIC
    json)