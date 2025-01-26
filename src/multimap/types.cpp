#include "rbtree_tests.h"

#include "uxs/multimap.h"

using namespace uxs_test_suite;

namespace {

#if defined(__GNUC__)
#    pragma GCC diagnostic ignored "-Wunused-function"
#elif defined(__clang__)
#    pragma clang diagnostic ignored "-Wunused-function"
#endif

void test_types() {
    static_assert(uxs::is_input_iterator<uxs::multimap<T, std::string>::iterator>::value == true, "");
    static_assert(uxs::is_random_access_iterator<uxs::multimap<T, std::string>::iterator>::value == false, "");
}

}  // namespace
