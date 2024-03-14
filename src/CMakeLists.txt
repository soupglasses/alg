add_library(hash hash.c)
target_include_directories(hash PUBLIC ../include)

add_library(item item.c)
target_include_directories(item PUBLIC ../include)

add_library(list list.c)
target_include_directories(list PUBLIC ../include)

install(TARGETS hash item list)
