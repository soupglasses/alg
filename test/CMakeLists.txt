add_executable(test_list test_list.c)
target_link_libraries(test_list mtest list)

discover_tests(test_list)
