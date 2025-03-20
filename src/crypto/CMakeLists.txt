# OpenSSL
find_package(OpenSSL)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::Crypto)

set(src_pattern "*.cc")
file(GLOB cpp_sources ${src_pattern})

target_sources(${PROJECT_NAME} PRIVATE ${cpp_sources})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
