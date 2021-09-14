cmake_minimum_required(VERSION 3.15)
project(counter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

enable_testing()

find_package(Threads REQUIRED)

if (CPP)
    add_subdirectory(cpp_impl)
else()
    add_executable(test_counter test.c)
    target_link_libraries(test_counter Threads::Threads)
    target_compile_options(test_counter PRIVATE "-Wall" "-Wextra")
endif()

add_test(test test_counter)
