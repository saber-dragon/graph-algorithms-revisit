cmake_minimum_required(VERSION 3.8)
project(lemonOK)

set(CMAKE_CXX_STANDARD 11)


configure_file(example.lfg example.lfg COPYONLY)
add_executable(reader graph_reader_test.cpp)
target_link_libraries(reader lemon)

add_executable(reachability_of_dag
        grail.hpp grail_test.cpp)
target_link_libraries(reachability_of_dag lemon)