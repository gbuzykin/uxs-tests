#include "list_tests.h"

#include "uxs/pool_allocator.h"

#include <list>

using namespace uxs_test_suite;

namespace {

ADD_TEST_CASE("3-info", "list:T", []() -> int { return sizeof(uxs::list<T>); });
ADD_TEST_CASE("3-info", "list:T:global_pool_allocator",
              []() -> int { return sizeof(uxs::list<T, uxs::global_pool_allocator<T>>); });
ADD_TEST_CASE("3-info", "list:T:pool_allocator", []() -> int { return sizeof(uxs::list<T, uxs::pool_allocator<T>>); });
ADD_TEST_CASE("3-info", "list:T:iterator", []() -> int { return sizeof(uxs::list<T>::iterator); });
ADD_TEST_CASE("3-info", "list:T:node", []() -> int { return sizeof(uxs::detail::list_node_type<T>); });
ADD_TEST_CASE("3-info", "<STL> list:T", []() -> int { return sizeof(std::list<T>); });
ADD_TEST_CASE("3-info", "<STL> list:T:global_pool_allocator",
              []() -> int { return sizeof(std::list<T, uxs::global_pool_allocator<T>>); });
ADD_TEST_CASE("3-info", "<STL> list:T:pool_allocator",
              []() -> int { return sizeof(std::list<T, uxs::pool_allocator<T>>); });
ADD_TEST_CASE("3-info", "<STL> list:T:iterator", []() -> int { return sizeof(std::list<T>::iterator); });

}  // namespace
