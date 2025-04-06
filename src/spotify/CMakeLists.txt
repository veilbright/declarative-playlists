add_library(spotify "")

# Crow needs asio
target_include_directories(spotify PRIVATE ${ASIO_INCLUDE_DIR})

target_link_libraries(
    spotify
    PRIVATE crypto util nlohmann_json::nlohmann_json Crow::Crow
)

target_link_libraries(spotify PUBLIC cpr::cpr)

set(src_pattern "*.cc")
file(GLOB cpp_sources ${src_pattern})

target_sources(spotify PRIVATE ${cpp_sources})
target_include_directories(spotify PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
