#include "rbtree_tests.h"

#include "uxs/map.h"

using namespace uxs_test_suite;

namespace {

int test_value_comp() {
    uxs::map<int, int> m;
    auto comp = m.value_comp();
    auto b = comp(std::make_pair(4, 2), std::make_pair(2, 3));
    (void)b;
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "map", test_value_comp);
