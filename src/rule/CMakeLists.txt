add_library(rule "")

# yaml-cpp
target_link_libraries(rule PRIVATE yaml-cpp::yaml-cpp)

set(src_pattern "*.cc")
file(GLOB cpp_sources ${src_pattern})

target_sources(rule PRIVATE ${cpp_sources})
target_include_directories(rule PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
