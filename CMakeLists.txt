cmake_minimum_required(VERSION 3.14)
project(DeclarativePlaylists)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# YAML Parsing
FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp
    GIT_TAG 0.8.0
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

# C++ Requests
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr
    GIT_TAG 1.11.2
)

# For Crow to find asio
set(ASIO_INCLUDE_DIR "${CMAKE_BINARY_DIR}/_deps/asio-src/asio/include")

FetchContent_MakeAvailable(yaml-cpp asio Crow cpr)

add_subdirectory(src)
