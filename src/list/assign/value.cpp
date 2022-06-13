#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_value_empty_to_empty() {
    test_allocator<void> al;

    {
        uxs::list<Ty, test_allocator<Ty>> l(al);
        l.assign(0, 10);
        CHECK_EMPTY(l);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_value_not_empty_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {10, 10, 10, 10, 10};
        uxs::list<Ty, test_allocator<Ty>> l(al);
        l.assign(5, 10);
        CHECK(l, 5, tst);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_value_more() {
    test_allocator<void> al;

    {
        int tst[] = {20, 20, 20, 20, 20, 20, 20};
        uxs::list<Ty, test_allocator<Ty>> l(5, 10, al);
        l.assign(7, 20);
        CHECK(l, 7, tst);
        VERIFY(T::instance_count == 7);
        VERIFY(T::not_empty_count == 7);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 7);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_value_less() {
    test_allocator<void> al;

    {
        int tst[] = {20, 20, 20, 20, 20};
        uxs::list<Ty, test_allocator<Ty>> l(7, 10, al);
        l.assign(5, 20);
        CHECK(l, 5, tst);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_value_same_amount() {
    test_allocator<void> al;

    {
        int tst[] = {20, 20, 20, 20, 20};
        uxs::list<Ty, test_allocator<Ty>> l(5, 10, al);
        l.assign(5, 20);
        CHECK(l, 5, tst);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_value_empty_to_not_empty() {
    test_allocator<void> al;

    {
        uxs::list<Ty, test_allocator<Ty>> l(5, 10, al);
        l.assign(0, 20);
        CHECK_EMPTY(l);
        VERIFY(T::instance_count == 0);
        VERIFY(T::not_empty_count == 0);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_value_empty_to_empty);
ADD_TEST_CASE("", "list", test_value_not_empty_to_empty);
ADD_TEST_CASE("", "list", test_value_more);
ADD_TEST_CASE("", "list", test_value_less);
ADD_TEST_CASE("", "list", test_value_same_amount);
ADD_TEST_CASE("", "list", test_value_empty_to_not_empty);
