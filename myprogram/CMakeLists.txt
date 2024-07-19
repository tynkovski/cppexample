cmake_minimum_required(VERSION 3.5)

project(cppexample)

find_library(MYLIB mylib PATHS /usr/local/lib/mylib)
add_executable(main src/main.cpp)
target_link_libraries(main PUBLIC ${MYLIB})
