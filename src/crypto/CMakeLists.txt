add_library(crypto "")

# OpenSSL
find_package(OpenSSL)
target_link_libraries(crypto PRIVATE OpenSSL::Crypto)

set(src_pattern "*.cc")
file(GLOB cpp_sources ${src_pattern})

target_sources(crypto PRIVATE ${cpp_sources})
target_include_directories(crypto PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
