add_executable(${PROJECT_NAME} main.cc)
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_subdirectory(crypto)
# Crow (+asio)
target_include_directories(${PROJECT_NAME} PRIVATE ${ASIO_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE Crow::Crow)

# C++ Requests
target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr)

set(src_pattern "*.cc")
file(GLOB cpp_sources ${src_pattern})

target_sources(${PROJECT_NAME} PRIVATE ${cpp_sources})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
