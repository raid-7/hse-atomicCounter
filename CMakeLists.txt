cmake_minimum_required(VERSION 3.15)
project(counter)

set(CMAKE_CXX_STANDARD 17)
enable_testing()

find_package(Threads REQUIRED)

add_executable(test_counter counter.cpp test.cpp)
target_link_libraries(test_counter Threads::Threads)
target_compile_options(test_counter PRIVATE "-Wall" "-Wextra")

add_test(test test_counter)
