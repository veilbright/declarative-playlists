cmake_minimum_required(VERSION 3.14)
project(DeclarativePlaylists)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp
    GIT_TAG 0.8.0
)
FetchContent_MakeAvailable(yaml-cpp)

set(src-pattern "src/*.cc")
file(GLOB dec-cpp-sources ${src-pattern})

add_executable(${PROJECT_NAME} src/main.cc)
target_link_libraries(${PROJECT_NAME} PUBLIC yaml-cpp::yaml-cpp)
target_sources(${PROJECT_NAME} PRIVATE ${dec-cpp-sources})
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/src")
