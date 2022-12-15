#include "vector_tests.h"

#include <vector>

using namespace uxs_test_suite;

namespace {

ADD_TEST_CASE("3-info", "vector:T", []() -> int { return sizeof(uxs::vector<T>); });
ADD_TEST_CASE("3-info", "vector:T:iterator", []() -> int { return sizeof(uxs::vector<T>::iterator); });
ADD_TEST_CASE("3-info", "<STL> vector:T", []() -> int { return sizeof(std::vector<T>); });
ADD_TEST_CASE("3-info", "<STL> vector:T:iterator", []() -> int { return sizeof(std::vector<T>::iterator); });

}  // namespace
