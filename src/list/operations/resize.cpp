#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_resize_empty_to_empty() {
    test_allocator<void> al;

    {
        uxs::list<Ty, test_allocator<Ty>> l(al);
        l.resize(0);
        CHECK_EMPTY(l);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_resize_not_empty_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {0, 0, 0, 0, 0};
        uxs::list<Ty, test_allocator<Ty>> l(al);
        l.resize(5);
        CHECK(l, 5, tst);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 0);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_resize_more() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst[] = {1, 2, 3, 4, 5, 0, 0};
        uxs::list<Ty, test_allocator<Ty>> l(init, al);
        l.resize(7);
        CHECK(l, 7, tst);
        VERIFY(T::instance_count == 12);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 7);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_resize_less() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5, 6, 7};
        uxs::list<Ty, test_allocator<Ty>> l(init, al);
        l.resize(5);
        CHECK(l, 5, init.begin());
        VERIFY(T::instance_count == 12);
        VERIFY(T::not_empty_count == 12);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_resize_same_amount() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::list<Ty, test_allocator<Ty>> l(init, al);
        l.resize(5);
        CHECK(l, 5, init.begin());
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_resize_empty_to_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::list<Ty, test_allocator<Ty>> l(init, al);
        l.resize(0);
        CHECK_EMPTY(l);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_resize_value_empty_to_empty() {
    test_allocator<void> al;

    {
        uxs::list<Ty, test_allocator<Ty>> l(al);
        l.resize(0, 10);
        CHECK_EMPTY(l);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_resize_value_not_empty_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {10, 10, 10, 10, 10};
        uxs::list<Ty, test_allocator<Ty>> l(al);
        l.resize(5, 10);
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
int test_resize_value_more() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst[] = {1, 2, 3, 4, 5, 10, 10};
        uxs::list<Ty, test_allocator<Ty>> l(init, al);
        l.resize(7, 10);
        CHECK(l, 7, tst);
        VERIFY(T::instance_count == 12);
        VERIFY(T::not_empty_count == 12);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 7);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_resize_value_less() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5, 6, 7};
        uxs::list<Ty, test_allocator<Ty>> l(init, al);
        l.resize(5, 10);
        CHECK(l, 5, init.begin());
        VERIFY(T::instance_count == 12);
        VERIFY(T::not_empty_count == 12);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_resize_value_same_amount() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::list<Ty, test_allocator<Ty>> l(init, al);
        l.resize(5, 10);
        CHECK(l, 5, init.begin());
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_resize_value_empty_to_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::list<Ty, test_allocator<Ty>> l(init, al);
        l.resize(0, 10);
        CHECK_EMPTY(l);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_resize_empty_to_empty);
ADD_TEST_CASE("", "list", test_resize_not_empty_to_empty);
ADD_TEST_CASE("", "list", test_resize_more);
ADD_TEST_CASE("", "list", test_resize_less);
ADD_TEST_CASE("", "list", test_resize_same_amount);
ADD_TEST_CASE("", "list", test_resize_empty_to_not_empty);
ADD_TEST_CASE("", "list", test_resize_value_empty_to_empty);
ADD_TEST_CASE("", "list", test_resize_value_not_empty_to_empty);
ADD_TEST_CASE("", "list", test_resize_value_more);
ADD_TEST_CASE("", "list", test_resize_value_less);
ADD_TEST_CASE("", "list", test_resize_value_same_amount);
ADD_TEST_CASE("", "list", test_resize_value_empty_to_not_empty);
