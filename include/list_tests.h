#pragma once

#include "test_allocators.h"
#include "test_suite.h"
#include "test_types.h"

#include "uxs/list.h"

template<typename Ty, typename Alloc, typename InputIt>
bool check_list(const uxs::list<Ty, Alloc>& l, size_t sz, InputIt src) {
    if (l.size() != sz) { return false; }
    if (l.begin() != l.cbegin()) { return false; }
    if (l.end() != l.cend()) { return false; }
    if (std::distance(l.begin(), l.end()) != static_cast<ptrdiff_t>(sz)) { return false; }
    for (auto it = l.begin(); it != l.end(); ++it) {
        if (!(*it == *src++)) { return false; }
    }
    for (auto it = l.rbegin(); it != l.rend(); ++it) {
        if (!(*it == *(--src))) { return false; }
    }
    return true;
}

#define CHECK(...) \
    if (!check_list(__VA_ARGS__)) { \
        throw std::runtime_error(uxs_test_suite::report_error(__FILE__, __LINE__, "list mismatched")); \
    }

#define CHECK_EMPTY(...) \
    if (((__VA_ARGS__).size() != 0) || ((__VA_ARGS__).begin() != (__VA_ARGS__).end())) { \
        throw std::runtime_error(uxs_test_suite::report_error(__FILE__, __LINE__, "list is not empty")); \
    }
