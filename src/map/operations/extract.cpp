#include "rbtree_tests.h"

#include <uxs-legacy/map.h>

using namespace uxs_test_suite;

namespace {

int test_extract() {
    test_allocator<void> al;

    uxs::map<T, std::string, std::less<T>, test_allocator<std::pair<const T, std::string>>> m(al);
    m.emplace(1, "aaa");
    auto nh4 = m.extract(1);
    using key_type = std::decay_t<decltype(nh4)>::key_type;
    using mapped_type = std::decay_t<decltype(nh4)>::mapped_type;
    key_type k = std::move(nh4.key());
    mapped_type obj = std::move(nh4.mapped());
    VERIFY(k == 1);
    VERIFY(obj == "aaa");
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "map", test_extract);
