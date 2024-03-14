add_executable(test_list test_list.c)
target_link_libraries(test_list mtest item list)

discover_tests(test_list)
