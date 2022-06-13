#include "list_tests.h"

#include "uxs/pool_allocator.h"

#include <list>

using namespace uxs_test_suite;

namespace {

int test_info_sizeof_T_std_alloc() { return sizeof(uxs::list<T>); }
int test_info_sizeof_T_global_pool() { return sizeof(uxs::list<T, uxs::global_pool_allocator<T>>); }
int test_info_sizeof_T_pool() { return sizeof(uxs::list<T, uxs::pool_allocator<T>>); }
int test_info_sizeof_T_iterator() { return sizeof(uxs::list<T>::iterator); }
int test_info_sizeof_T_node() { return sizeof(uxs::detail::list_node_type<T>); }

int test_info_sizeof_T_std_alloc_std() { return sizeof(std::list<T>); }
int test_info_sizeof_T_global_pool_std() { return sizeof(std::list<T, uxs::global_pool_allocator<T>>); }
int test_info_sizeof_T_pool_std() { return sizeof(std::list<T, uxs::pool_allocator<T>>); }
int test_info_sizeof_T_iterator_std() { return sizeof(std::list<T>::iterator); }

}  // namespace

ADD_TEST_CASE("3-info", "size: list:T", test_info_sizeof_T_std_alloc);
ADD_TEST_CASE("3-info", "size: list:T:global_pool_allocator", test_info_sizeof_T_global_pool);
ADD_TEST_CASE("3-info", "size: list:T:pool_allocator", test_info_sizeof_T_pool);
ADD_TEST_CASE("3-info", "size: list:T:iterator", test_info_sizeof_T_iterator);
ADD_TEST_CASE("3-info", "size: list:T:node", test_info_sizeof_T_node);
ADD_TEST_CASE("3-info", "<std> size: list:T", test_info_sizeof_T_std_alloc_std);
ADD_TEST_CASE("3-info", "<std> size: list:T:global_pool_allocator", test_info_sizeof_T_global_pool_std);
ADD_TEST_CASE("3-info", "<std> size: list:T:pool_allocator", test_info_sizeof_T_pool_std);
ADD_TEST_CASE("3-info", "<std> size: list:T:iterator", test_info_sizeof_T_iterator_std);
