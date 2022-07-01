#include "rbtree_tests.h"

#include "uxs/multimap.h"

using namespace uxs_test_suite;

namespace {

void test_types() {
    static_assert(uxs::is_input_iterator<uxs::multimap<T, std::string>::iterator>::value == true, "");
    static_assert(uxs::is_random_access_iterator<uxs::multimap<T, std::string>::iterator>::value == false, "");
}

}  // namespace
