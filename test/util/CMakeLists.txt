add_executable(test_url url_test.cc)
target_link_libraries(test_url GTest::gtest_main)
target_link_libraries(test_url util)
gtest_discover_tests(test_url)
