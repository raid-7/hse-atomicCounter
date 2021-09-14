cmake_minimum_required(VERSION 3.15)
project(counter)

add_executable(test_counter tl_random.cpp counter.cpp test.cpp)
target_link_libraries(test_counter Threads::Threads)
target_compile_options(test_counter PRIVATE "-Wall" "-Wextra")
