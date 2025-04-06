add_library(util "")

set(src_pattern "*.cc")
file(GLOB cpp_sources ${src_pattern})

target_sources(util PRIVATE ${cpp_sources})
target_include_directories(util PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
