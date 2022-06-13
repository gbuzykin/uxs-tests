#include "vector_tests.h"

#include <vector>

using namespace uxs_test_suite;

namespace {

int test_info_sizeof_T() { return sizeof(uxs::vector<T>); }
int test_info_sizeof_T_iterator() { return sizeof(uxs::vector<T>::iterator); }

int test_info_sizeof_T_std() { return sizeof(std::vector<T>); }
int test_info_sizeof_T_iterator_std() { return sizeof(std::vector<T>::iterator); }

}  // namespace

ADD_TEST_CASE("3-info", "size: vector:T", test_info_sizeof_T);
ADD_TEST_CASE("3-info", "size: vector:T:iterator", test_info_sizeof_T_iterator);
ADD_TEST_CASE("3-info", "<std> size: vector:T", test_info_sizeof_T_std);
ADD_TEST_CASE("3-info", "<std> size: vector:T:iterator", test_info_sizeof_T_iterator_std);
