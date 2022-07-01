#include "rbtree_tests.h"

#include "uxs/pool_allocator.h"
#include "uxs/set.h"

#include <set>

using namespace uxs_test_suite;

namespace {

int test_info_sizeof_T_std_alloc() { return sizeof(uxs::set<T>); }
int test_info_sizeof_T_global_pool() { return sizeof(uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>); }
int test_info_sizeof_T_pool() { return sizeof(uxs::set<T, std::less<T>, uxs::pool_allocator<T>>); }
int test_info_sizeof_T_iterator() { return sizeof(uxs::set<T>::iterator); }
int test_info_sizeof_T_node() { return sizeof(uxs::detail::set_node_type<T>); }

int test_info_sizeof_T_std_alloc_std() { return sizeof(std::set<T>); }
int test_info_sizeof_T_global_pool_std() { return sizeof(std::set<T, std::less<T>, uxs::global_pool_allocator<T>>); }
int test_info_sizeof_T_pool_std() { return sizeof(std::set<T, std::less<T>, uxs::pool_allocator<T>>); }
int test_info_sizeof_T_iterator_std() { return sizeof(std::set<T>::iterator); }

}  // namespace

ADD_TEST_CASE("3-info", "size: set:T", test_info_sizeof_T_std_alloc);
ADD_TEST_CASE("3-info", "size: set:T:global_pool_allocator", test_info_sizeof_T_global_pool);
ADD_TEST_CASE("3-info", "size: set:T:pool_allocator", test_info_sizeof_T_pool);
ADD_TEST_CASE("3-info", "size: set:T:iterator", test_info_sizeof_T_iterator);
ADD_TEST_CASE("3-info", "size: set:T:node", test_info_sizeof_T_node);
ADD_TEST_CASE("3-info", "<std> size: set:T", test_info_sizeof_T_std_alloc_std);
ADD_TEST_CASE("3-info", "<std> size: set:T:global_pool_allocator", test_info_sizeof_T_global_pool_std);
ADD_TEST_CASE("3-info", "<std> size: set:T:pool_allocator", test_info_sizeof_T_pool_std);
ADD_TEST_CASE("3-info", "<std> size: set:T:iterator", test_info_sizeof_T_iterator_std);
