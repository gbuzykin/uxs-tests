#include "rbtree_tests.h"

#include "uxs/multiset.h"

using namespace uxs_test_suite;

namespace {

#if defined(__GNUC__)
#    pragma GCC diagnostic ignored "-Wunused-function"
#elif defined(__clang__)
#    pragma clang diagnostic ignored "-Wunused-function"
#endif

void test_types() {
    static_assert(uxs::is_input_iterator<uxs::multiset<T>::iterator>::value == true, "");
    static_assert(uxs::is_random_access_iterator<uxs::multiset<T>::iterator>::value == false, "");
}

}  // namespace
