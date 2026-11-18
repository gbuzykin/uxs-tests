#include "uxs_testsuite/suite.h"

#include <uxs-legacy/vector.h>

#include <vector>

using namespace uxs_testsuite;

namespace {

ADD_TEST_CASE("3-info <legacy>", "vector:T", []() -> int { return sizeof(uxs::vector<T>); });
ADD_TEST_CASE("3-info <legacy>", "vector:T:iterator", []() -> int { return sizeof(uxs::vector<T>::iterator); });
ADD_TEST_CASE("3-info <legacy>", "<STL> vector:T", []() -> int { return sizeof(std::vector<T>); });
ADD_TEST_CASE("3-info <legacy>", "<STL> vector:T:iterator", []() -> int { return sizeof(std::vector<T>::iterator); });

}  // namespace
