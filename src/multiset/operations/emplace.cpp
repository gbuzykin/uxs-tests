#include "rbtree_tests.h"

#include <uxs-legacy/multiset.h>

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_emplace_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {10};
        uxs::multiset<T, std::less<T>, test_allocator<T>> ms(al);
        auto it = ms.emplace(10);
        VERIFY(it == ms.begin());
        CHECK(ms, 1, tst);
        VERIFY(T::instance_count == 1);
        VERIFY(T::not_empty_count == 1);
        VERIFY(al.get_alloc_detected() == 1);
        VERIFY(al.get_alloc_count() == 1);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_emplace() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {2, 3, 4, 6, 7};
        int tst1[] = {2, 3, 4, 6, 7, 8};
        int tst2[] = {2, 3, 4, 5, 6, 7, 8};
        int tst3[] = {1, 2, 3, 4, 5, 6, 7, 8};
        uxs::multiset<T, std::less<T>, test_allocator<T>> ms(init, al);
        // back
        auto it = ms.emplace(8);
        VERIFY(it == std::prev(ms.end()));
        CHECK(ms, 6, tst1);
        VERIFY(T::instance_count == 11);
        VERIFY(al.get_alloc_detected() == 6);
        VERIFY(al.get_alloc_count() == 6);
        // mid
        it = ms.emplace(5);
        VERIFY(it == std::next(ms.begin(), 3));
        CHECK(ms, 7, tst2);
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 7);
        // front
        it = ms.emplace(1);
        VERIFY(it == ms.begin());
        CHECK(ms, 8, tst3);
        VERIFY(T::instance_count == 13);
        VERIFY(al.get_alloc_detected() == 8);
        VERIFY(al.get_alloc_count() == 8);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multiset", test_emplace_to_empty);
ADD_TEST_CASE("", "multiset", test_emplace);
