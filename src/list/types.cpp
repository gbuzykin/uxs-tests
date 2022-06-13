#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

int test_types() {
    static_assert(uxs::is_input_iterator<uxs::list<T>::iterator>::value == true, "");
    static_assert(uxs::is_random_access_iterator<uxs::list<T>::iterator>::value == false, "");
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_types);
