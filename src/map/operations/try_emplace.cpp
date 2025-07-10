#include "rbtree_tests.h"

#include <uxs-legacy/map.h>

using namespace uxs_test_suite;

namespace {

int test_try_emplace() {
    uxs::map<std::string, std::string> m;

    m.try_emplace("a", "a");
    m.try_emplace("b", "abcd");
    m.try_emplace("c", 10, 'c');
    m.try_emplace("c", "Won't be inserted");

    VERIFY(m["a"] == "a");
    VERIFY(m["b"] == "abcd");
    VERIFY(m["c"] == std::string(10, 'c'));
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "map", test_try_emplace);
