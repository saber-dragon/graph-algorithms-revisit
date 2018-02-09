cmake_minimum_required(VERSION 3.9)
project(lemonOK)

set(CMAKE_CXX_STANDARD 11)

add_executable(lemonOK main.cpp)
target_link_libraries(lemonOK lemon)


set(Boost_INCLUDE_DIRS "E:\\MinGW-W64\\mingw64\\x86_64-w64-mingw32\\include\\boost-1_66")


include_directories(${Boost_INCLUDE_DIRS})
add_executable(boostOK boostOK.cpp)


add_executable(reader graph_reader_test.cpp)
target_link_libraries(reader lemon)