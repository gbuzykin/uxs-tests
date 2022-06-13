#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_erase_from_empty() {
    test_allocator<void> al;

    {
        uxs::list<Ty, test_allocator<Ty>> l(al);
        auto it = l.erase(l.begin(), l.end());
        VERIFY(it == l.end());
        CHECK_EMPTY(l);
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
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4};
        int tst2[] = {1, 3, 4};
        int tst3[] = {3, 4};
        uxs::list<Ty, test_allocator<Ty>> l(tst_prev, al);
        // back
        auto it = l.erase(std::prev(l.end()));
        VERIFY(it == l.end());
        CHECK(l, 4, tst1);
        VERIFY(T::instance_count == 9);
        VERIFY(al.get_alloc_count() == 4);
        // mid
        it = l.erase(std::next(l.begin()));
        VERIFY(it == std::next(l.begin()));
        CHECK(l, 3, tst2);
        VERIFY(T::instance_count == 8);
        VERIFY(al.get_alloc_count() == 3);
        // front
        it = l.erase(l.begin());
        VERIFY(it == l.begin());
        CHECK(l, 2, tst3);
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
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        int tst1[] = {1, 2, 3, 4, 5, 6, 7};
        int tst2[] = {1, 2, 3, 6, 7};
        int tst3[] = {3, 6, 7};
        uxs::list<Ty, test_allocator<Ty>> l(tst_prev, al);
        // back
        auto it = l.erase(std::prev(l.end(), 2), l.end());
        VERIFY(it == l.end());
        CHECK(l, 7, tst1);
        VERIFY(T::instance_count == 16);
        VERIFY(al.get_alloc_count() == 7);
        // mid
        it = l.erase(std::next(l.begin(), 3), std::next(l.begin(), 5));
        VERIFY(it == std::next(l.begin(), 3));
        CHECK(l, 5, tst2);
        VERIFY(T::instance_count == 14);
        VERIFY(al.get_alloc_count() == 5);
        // front
        it = l.erase(l.begin(), std::next(l.begin(), 2));
        VERIFY(it == l.begin());
        CHECK(l, 3, tst3);
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_count() == 3);
        // no erase
        it = l.erase(l.end(), l.end());
        VERIFY(it == l.end());
        CHECK(l, 3, tst3);
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_count() == 3);
        it = l.erase(std::next(l.begin()), std::next(l.begin()));
        VERIFY(it == std::next(l.begin()));
        CHECK(l, 3, tst3);
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_count() == 3);
        it = l.erase(l.begin(), l.begin());
        VERIFY(it == l.begin());
        CHECK(l, 3, tst3);
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_count() == 3);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_erase_from_empty);
ADD_TEST_CASE("", "list", test_erase_one);
ADD_TEST_CASE("", "list", test_erase_multi);
