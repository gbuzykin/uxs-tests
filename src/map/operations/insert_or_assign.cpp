#include "rbtree_tests.h"

#include "uxs/map.h"

using namespace uxs_test_suite;

namespace {

int test_insert_or_assign() {
    uxs::map<std::string, std::string> m;
    m.insert_or_assign("a", "apple");
    m.insert_or_assign("b", "bannana");
    m.insert_or_assign("c", "cherry");
    m.insert_or_assign("c", "clementine");

    VERIFY(m["a"] == "apple");
    VERIFY(m["b"] == "bannana");
    VERIFY(m["c"] == "clementine");
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "map", test_insert_or_assign);
