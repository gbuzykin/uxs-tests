#include "rbtree_tests.h"

#include <uxs-legacy/multiset.h>

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_erase_from_empty() {
    test_allocator<void> al;

    {
        uxs::multiset<T, std::less<T>, test_allocator<T>> ms(al);
        auto it = ms.erase(ms.begin(), ms.end());
        VERIFY(it == ms.end());
        CHECK_EMPTY(ms);
        VERIFY(T::instance_count == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_erase_one() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4};
        int tst2[] = {1, 3, 4};
        int tst3[] = {3, 4};
        uxs::multiset<T, std::less<T>, test_allocator<T>> ms(init, al);
        // back
        auto it = ms.erase(std::prev(ms.end()));
        VERIFY(it == ms.end());
        CHECK(ms, 4, tst1);
        VERIFY(T::instance_count == 9);
        VERIFY(al.get_alloc_count() == 4);
        // mid
        it = ms.erase(std::next(ms.begin()));
        VERIFY(it == std::next(ms.begin()));
        CHECK(ms, 3, tst2);
        VERIFY(T::instance_count == 8);
        VERIFY(al.get_alloc_count() == 3);
        // front
        it = ms.erase(ms.begin());
        VERIFY(it == ms.begin());
        CHECK(ms, 2, tst3);
        VERIFY(T::instance_count == 7);
        VERIFY(al.get_alloc_count() == 2);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_erase_multi() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        int tst1[] = {1, 2, 3, 4, 5, 6, 7};
        int tst2[] = {1, 2, 3, 6, 7};
        int tst3[] = {3, 6, 7};
        uxs::multiset<T, std::less<T>, test_allocator<T>> ms(init, al);
        // back
        auto it = ms.erase(std::prev(ms.end(), 2), ms.end());
        VERIFY(it == ms.end());
        CHECK(ms, 7, tst1);
        VERIFY(T::instance_count == 16);
        VERIFY(al.get_alloc_count() == 7);
        // mid
        it = ms.erase(std::next(ms.begin(), 3), std::next(ms.begin(), 5));
        VERIFY(it == std::next(ms.begin(), 3));
        CHECK(ms, 5, tst2);
        VERIFY(T::instance_count == 14);
        VERIFY(al.get_alloc_count() == 5);
        // front
        it = ms.erase(ms.begin(), std::next(ms.begin(), 2));
        VERIFY(it == ms.begin());
        CHECK(ms, 3, tst3);
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_count() == 3);
        // no erase
        it = ms.erase(ms.end(), ms.end());
        VERIFY(it == ms.end());
        CHECK(ms, 3, tst3);
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_count() == 3);
        it = ms.erase(std::next(ms.begin()), std::next(ms.begin()));
        VERIFY(it == std::next(ms.begin()));
        CHECK(ms, 3, tst3);
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_count() == 3);
        it = ms.erase(ms.begin(), ms.begin());
        VERIFY(it == ms.begin());
        CHECK(ms, 3, tst3);
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_count() == 3);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multiset", test_erase_from_empty);
ADD_TEST_CASE("", "multiset", test_erase_one);
ADD_TEST_CASE("", "multiset", test_erase_multi);
