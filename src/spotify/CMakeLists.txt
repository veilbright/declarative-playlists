set(src_pattern "*.cc")
file(GLOB cpp_sources ${src_pattern})

# JSON
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# Crow (+asio)
target_include_directories(${PROJECT_NAME} PRIVATE ${ASIO_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE Crow::Crow)

# C++ Requests
target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr)

target_sources(${PROJECT_NAME} PRIVATE ${cpp_sources})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
