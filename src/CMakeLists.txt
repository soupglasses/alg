add_library(item item.c)
target_include_directories(item PUBLIC ../include)

add_library(list list.c)
target_include_directories(list PUBLIC ../include)

install(TARGETS item list)
