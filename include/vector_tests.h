#pragma once

#include "test_allocators.h"
#include "test_suite.h"
#include "test_types.h"

#include "uxs/vector.h"

template<typename Ty, typename Alloc, typename InputIt>
bool vector_check(const uxs::vector<Ty, Alloc>& v, size_t sz, InputIt src) {
    if (v.size() != sz) { return false; }
    if (v.begin() != v.cbegin()) { return false; }
    if (v.end() != v.cend()) { return false; }
    if (std::distance(v.begin(), v.end()) != static_cast<ptrdiff_t>(sz)) { return false; }
    for (auto it = v.begin(); it != v.end(); ++it) {
        if (!(*it == *src++)) { return false; }
    }
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        if (!(*it == *(--src))) { return false; }
    }
    return true;
}

#define CHECK(...) \
    if (!vector_check(__VA_ARGS__)) { \
        throw std::runtime_error(uxs_test_suite::report_error(__FILE__, __LINE__, "vector mismatched")); \
    }

#define CHECK_EMPTY(x) \
    { \
        const auto& __v = x; \
        if (__v.size() != 0 || __v.begin() != __v.end()) { \
            throw std::runtime_error(uxs_test_suite::report_error(__FILE__, __LINE__, "vector is not empty")); \
        } \
    }
