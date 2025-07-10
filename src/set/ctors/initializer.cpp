#include "rbtree_tests.h"

#include <uxs-legacy/set.h>

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_initializer_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init;
        uxs::set<Ty, std::less<Ty>, test_allocator<Ty>> s(init, al);
        CHECK_EMPTY(s);
        VERIFY(s.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 2, 2, 3, 4, 4, 5};
        int tst[] = {1, 2, 3, 4, 5};
        uxs::set<Ty, std::less<Ty>, test_allocator<Ty>> s(init, al);
        CHECK(s, 5, tst);
        VERIFY(s.get_allocator() == al);
        VERIFY(T::instance_count == 13);
        VERIFY(T::not_empty_count == 13);
        VERIFY(al.get_alloc_detected() == 8);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "set", test_initializer_empty);
ADD_TEST_CASE("", "set", test_initializer_not_empty);
