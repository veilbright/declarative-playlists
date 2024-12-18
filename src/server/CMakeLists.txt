set(project_server_name "${PROJECT_NAME}-server")

add_executable(${project_server_name} main.cc)
set_target_properties(
    ${project_server_name}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# JSON
target_link_libraries(
    ${project_server_name}
    PRIVATE nlohmann_json::nlohmann_json
)

# OpenSSL
find_package(OpenSSL)
target_link_libraries(${project_server_name} PRIVATE OpenSSL::Crypto)
# target_link_libraries(${project_server_name} PRIVATE OpenSSL::rand)

# Crow (+asio)
target_include_directories(${project_server_name} PRIVATE ${ASIO_INCLUDE_DIR})
target_link_libraries(${project_server_name} PRIVATE Crow::Crow)

set(client_src_pattern "*.cc")
file(GLOB client_cpp_sources ${client_src_pattern})

target_sources(${project_server_name} PRIVATE ${client_cpp_sources})
target_include_directories(
    ${project_server_name}
    PUBLIC "${PROJECT_SOURCE_DIR}/src/server"
)
