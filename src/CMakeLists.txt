add_library(list list.c)
target_include_directories(list PUBLIC ../include)

install(TARGETS list)
