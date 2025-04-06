add_subdirectory(crypto)
add_subdirectory(rule)
add_subdirectory(spotify)
add_subdirectory(util)

target_link_libraries(${PROJECT_NAME} PRIVATE crypto rule spotify util)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE yaml-cpp::yaml-cpp nlohmann_json::nlohmann_json
)

set(src_pattern "*.cc")
file(GLOB cpp_sources ${src_pattern})

target_sources(${PROJECT_NAME} PRIVATE ${cpp_sources})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
