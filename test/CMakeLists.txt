add_executable(test_hash test_hash.c)
target_link_libraries(test_hash mtest hash)

add_executable(test_item test_item.c)
target_link_libraries(test_item mtest item)

add_executable(test_list test_list.c)
target_link_libraries(test_list mtest item list)

discover_tests(test_hash test_item test_list)
