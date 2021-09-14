#pragma once

#include <cstdlib>
#include <iostream>


#define ensure(type, ...) detail::assert_##type(__FILE__, __LINE__, __VA_ARGS__)

namespace detail {

void fail(const char* file, size_t line) {
    std::cerr << "Assertion failed: file \"" << file << "\", line " << line << std::endl;
    std::abort();
}

template <class T>
void assert_true(const char* file, size_t line, T&& v1) {
    if (!v1) {
        fail(file, line);
    }
}


template <class P, class Q>
void assert_eq(const char* file, size_t line, const P& v1, const Q& v2) {
    assert_true(file, line, v1 == v2);
}

template <class P, class Q>
void assert_lt(const char* file, size_t line, const P& v1, const Q& v2) {
    assert_true(file, line, v1 < v2);
}

template <class P, class Q>
void assert_lte(const char* file, size_t line, const P& v1, const Q& v2) {
    assert_true(file, line, v1 <= v2);
}

template <class P, class Q>
void assert_gt(const char* file, size_t line, const P& v1, const Q& v2) {
    assert_true(file, line, v1 > v2);
}

template <class P, class Q>
void assert_gte(const char* file, size_t line, const P& v1, const Q& v2) {
    assert_true(file, line, v1 >= v2);
}

}
