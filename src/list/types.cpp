#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

void test_types() {
    static_assert(uxs::is_input_iterator<uxs::list<T>::iterator>::value == true, "");
    static_assert(uxs::is_random_access_iterator<uxs::list<T>::iterator>::value == false, "");
}

}  // namespace
