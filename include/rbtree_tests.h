#pragma once

#include "test_allocators.h"
#include "test_suite.h"
#include "test_types.h"

#include <uxs-legacy/rbtree_base.h>

#define CHECK(...) \
    if (!rbtree_check(__VA_ARGS__)) { \
        throw std::runtime_error(uxs_test_suite::report_error(__FILE__, __LINE__, "tree mismatched")); \
    }

#define CHECK_EMPTY(x) \
    { \
        const auto& __t = x; \
        if (__t.size() != 0 || __t.begin() != __t.end()) { \
            throw std::runtime_error(uxs_test_suite::report_error(__FILE__, __LINE__, "tree is not empty")); \
        } \
    }

namespace uxs_test_suite {

bool rbtree_check_balance(uxs::rbtree_node_t* node, int& black, int cur_black = 0,
                          uxs::rbtree_node_t::color_t color = uxs::rbtree_node_t::color_t::red);

template<typename NodeTy, typename Alloc, typename Comp, typename InputIt>
bool rbtree_check(const uxs::detail::rbtree_base<NodeTy, Alloc, Comp>& t, size_t sz, InputIt src) {
    int black = 0;
    if (t.size() != sz) { return false; }
    if (t.begin() != t.cbegin()) { return false; }
    if (t.end() != t.cend()) { return false; }
    if (!t.empty() && !rbtree_check_balance(t.end().node()->left, black)) { return false; }
    if (std::distance(t.begin(), t.end()) != static_cast<ptrdiff_t>(sz)) { return false; }
    for (auto it = t.begin(); it != t.end(); ++it) {
        if (!(*it == *src++)) { return false; }
    }
    for (auto it = t.rbegin(); it != t.rend(); ++it) {
        if (!(*it == *(--src))) { return false; }
    }
    return true;
}

}  // namespace uxs_test_suite
