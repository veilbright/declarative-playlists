set(src_pattern "*.cc")

add_executable(test_crypto crypto_util_test.cc)

file(GLOB test_sources ${src_pattern})

# GoogleTest
target_link_libraries(test_crypto GTest::gtest_main)

# Project Libraries
target_link_libraries(test_crypto crypto)

include(GoogleTest)
gtest_discover_tests(test_crypto)
