#include "rbtree_tests.h"

#include <uxs-legacy/set.h>
#include <uxs-legacy/vector.h>

using namespace uxs_test_suite;

namespace {

template<typename Src = uxs::vector<T>>
int test_range_empty() {
    test_allocator<void> al;

    {
        Src init;
        uxs::set<T, std::less<T>, test_allocator<T>> s(init.begin(), init.end(), al);
        CHECK_EMPTY(s);
        VERIFY(s.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

template<typename Src = uxs::vector<T>>
int test_range_not_empty() {
    test_allocator<void> al;

    {
        Src init = {1, 2, 2, 2, 3, 3, 4, 5};
        int tst[] = {1, 2, 3, 4, 5};
        uxs::set<T, std::less<T>, test_allocator<T>> s(init.begin(), init.end(), al);
        CHECK(s, 5, tst);
        VERIFY(s.get_allocator() == al);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, T>::value ? 13 : 5));
        VERIFY(T::not_empty_count == (std::is_same<typename Src::value_type, T>::value ? 13 : 5));
        VERIFY(al.get_alloc_detected() == 8);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "set", test_range_empty);
ADD_TEST_CASE("", "set", test_range_not_empty);
