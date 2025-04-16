add_executable(test_crypto crypto_util_test.cc)
target_link_libraries(test_crypto GTest::gtest_main)
target_link_libraries(test_crypto crypto)
gtest_discover_tests(test_crypto)
