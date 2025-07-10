#include "rbtree_tests.h"

#include <uxs-legacy/multiset.h>

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_initializer_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init;
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(init, al);
        CHECK_EMPTY(ms);
        VERIFY(ms.get_allocator() == al);
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
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(init, al);
        CHECK(ms, init.size(), init.begin());
        VERIFY(ms.get_allocator() == al);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multiset", test_initializer_empty);
ADD_TEST_CASE("", "multiset", test_initializer_not_empty);
