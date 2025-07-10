#include "rbtree_tests.h"

#include <uxs-legacy/multiset.h>
#include <uxs-legacy/set.h>

using namespace uxs_test_suite;

namespace {

int test_extract() {
    test_allocator<void> al;
    uxs::multiset<T, std::less<T>, test_allocator<T>> s1(std::initializer_list<T>{1, 2, 3, 4, 5}, al), s2(al);

    auto nh = s1.extract(3);
    s2.insert(std::move(nh));

    int tst1[] = {1, 2, 4, 5};
    CHECK(s1, 4, tst1);

    int tst2[] = {3};
    CHECK(s2, 1, tst2);

    decltype(nh) nh2;
    decltype(nh) nh3{s1.extract(4)};

    uxs::set<T, std::less<T>, test_allocator<T>> s3(al);
    nh = s1.extract(5);
    auto result = s3.insert(std::move(nh));
    VERIFY(result.inserted);

    s1.emplace(5);
    nh = s1.extract(5);
    result = s3.insert(std::move(nh));
    VERIFY(!result.inserted);

    using value_type = std::decay_t<decltype(nh)>::value_type;
    value_type v = std::move(nh3.value());
    VERIFY(v == 4);

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multiset", test_extract);
