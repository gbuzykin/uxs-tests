#include "rbtree_tests.h"

#include "uxs/pool_allocator.h"
#include "uxs/set.h"

#include <set>

using namespace uxs_test_suite;

namespace {

ADD_TEST_CASE("3-info", "set:T", []() -> int { return sizeof(uxs::set<T>); });
ADD_TEST_CASE("3-info", "set:T:global_pool_allocator",
              ([]() -> int { return sizeof(uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>); }));
ADD_TEST_CASE("3-info", "set:T:pool_allocator",
              ([]() -> int { return sizeof(uxs::set<T, std::less<T>, uxs::pool_allocator<T>>); }));
ADD_TEST_CASE("3-info", "set:T:iterator", []() -> int { return sizeof(uxs::set<T>::iterator); });
ADD_TEST_CASE("3-info", "set:T:node", []() -> int { return sizeof(uxs::detail::set_node_type<T>); });
ADD_TEST_CASE("3-info", "<STL> set:T", []() -> int { return sizeof(std::set<T>); });
ADD_TEST_CASE("3-info", "<STL> set:T:global_pool_allocator",
              ([]() -> int { return sizeof(std::set<T, std::less<T>, uxs::global_pool_allocator<T>>); }));
ADD_TEST_CASE("3-info", "<STL> set:T:pool_allocator",
              ([]() -> int { return sizeof(std::set<T, std::less<T>, uxs::pool_allocator<T>>); }));
ADD_TEST_CASE("3-info", "<STL> set:T:iterator", []() -> int { return sizeof(std::set<T>::iterator); });

}  // namespace
