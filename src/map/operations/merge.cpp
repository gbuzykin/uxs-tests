#include "rbtree_tests.h"

#include "uxs/map.h"

using namespace uxs_test_suite;

namespace {

int test_merge() {
    test_allocator<void> al;
    uxs::map<int, std::string, std::less<int>, test_allocator<std::pair<const int, std::string>>> u(al);
    using init_type = std::initializer_list<std::decay_t<decltype(u)>::value_type>;
    uxs::map<int, std::string, std::less<int>, test_allocator<std::pair<const int, std::string>>> ma(
        init_type{{1, "apple"}, {5, "pear"}, {10, "banana"}}, al);
    uxs::map<int, std::string, std::less<int>, test_allocator<std::pair<const int, std::string>>> mb(
        init_type{{2, "zorro"}, {4, "batman"}, {5, "X"}, {8, "alpaca"}}, al);
    u.merge(ma);
    u.merge(mb);
    CHECK_EMPTY(ma);
    VERIFY(mb[5] == "X");

    VERIFY(u[1] == "apple");
    VERIFY(u[2] == "zorro");
    VERIFY(u[4] == "batman");
    VERIFY(u[5] == "pear");
    VERIFY(u[8] == "alpaca");
    VERIFY(u[10] == "banana");

    u.insert(std::make_pair(11, "dddd"));
    VERIFY(u[11] == "dddd");
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "map", test_merge);
