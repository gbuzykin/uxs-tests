#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_insert_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {10, 10, 10};
        uxs::list<Ty, test_allocator<Ty>> l(al);
        //  insert empty
        auto it = l.insert(l.end(), 0, 20);
        VERIFY(it == l.end());
        CHECK_EMPTY(l);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
        // not empty
        it = l.insert(l.end(), 3, 10);
        VERIFY(it == l.begin());
        CHECK(l, 3, tst);
        VERIFY(T::instance_count == 3);
        VERIFY(al.get_alloc_detected() == 3);
        VERIFY(al.get_alloc_count() == 3);
        //  insert empty back
        it = l.insert(l.end(), 0, 20);
        VERIFY(it == l.end());
        CHECK(l, 3, tst);
        VERIFY(T::instance_count == 3);
        VERIFY(al.get_alloc_detected() == 3);
        VERIFY(al.get_alloc_count() == 3);
        //  insert empty mid
        it = l.insert(std::next(l.begin(), 3), 0, 20);
        VERIFY(it == std::next(l.begin(), 3));
        CHECK(l, 3, tst);
        VERIFY(T::instance_count == 3);
        VERIFY(al.get_alloc_detected() == 3);
        VERIFY(al.get_alloc_count() == 3);
        //  insert empty front
        it = l.insert(l.begin(), 0, 20);
        VERIFY(it == l.begin());
        CHECK(l, 3, tst);
        VERIFY(T::instance_count == 3);
        VERIFY(al.get_alloc_detected() == 3);
        VERIFY(al.get_alloc_count() == 3);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_insert() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4, 5, 10, 10, 10};
        int tst2[] = {1, 2, 3, 20, 20, 20, 4, 5, 10, 10, 10};
        int tst3[] = {30, 30, 30, 1, 2, 3, 20, 20, 20, 4, 5, 10, 10, 10};
        uxs::list<Ty, test_allocator<Ty>> l(tst_prev, al);
        // back
        auto it = l.insert(l.end(), 3, 10);
        VERIFY(it == std::prev(l.end(), 3));
        CHECK(l, 8, tst1);
        VERIFY(T::instance_count == 13);
        VERIFY(al.get_alloc_detected() == 8);
        VERIFY(al.get_alloc_count() == 8);
        // mid
        it = l.insert(std::next(l.begin(), 3), 3, 20);
        VERIFY(it == std::next(l.begin(), 3));
        CHECK(l, 11, tst2);
        VERIFY(T::instance_count == 16);
        VERIFY(al.get_alloc_detected() == 11);
        VERIFY(al.get_alloc_count() == 11);
        // front
        it = l.insert(l.begin(), 3, 30);
        VERIFY(it == l.begin());
        CHECK(l, 14, tst3);
        VERIFY(T::instance_count == 19);
        VERIFY(al.get_alloc_detected() == 14);
        VERIFY(al.get_alloc_count() == 14);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_insert_to_empty);
ADD_TEST_CASE("", "list", test_insert);
