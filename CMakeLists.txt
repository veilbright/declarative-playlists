cmake_minimum_required(VERSION 4.0)
project(declists)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# YAML Parsing (newest versioned release uses CMake 3.4, which is too old)
FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp
    GIT_TAG 28f93bdec6387d42332220afa9558060c8016795
)

# JSON Parsing
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.3
)

# Crow and Dependencies
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio
    GIT_TAG asio-1-34-0
)
FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow
    GIT_TAG v1.2.1.2
)

# For Crow to find asio
set(ASIO_INCLUDE_DIR "${CMAKE_BINARY_DIR}/_deps/asio-src/asio/include")

# C++ Requests
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr
    GIT_TAG 1.11.2
)

# GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.16.0
)

FetchContent_MakeAvailable(
    asio
    Crow
    cpr
    json
    yaml-cpp
    googletest
)

add_executable(${PROJECT_NAME} src/main.cc)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
