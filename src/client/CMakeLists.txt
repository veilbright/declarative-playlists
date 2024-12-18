set(project_client_name "${PROJECT_NAME}-client")

add_executable(${project_client_name} main.cc)
set_target_properties(
    ${project_client_name}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_link_libraries(${project_client_name} PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(
    ${project_client_name}
    PRIVATE nlohmann_json::nlohmann_json
)

set(client_source_pattern "*.cc")
file(GLOB client_cpp_sources ${client_source_pattern})

target_sources(${project_client_name} PRIVATE ${client_cpp_sources})
target_include_directories(
    ${project_client_name}
    PUBLIC "${PROJECT_SOURCE_DIR}/src/client"
)
