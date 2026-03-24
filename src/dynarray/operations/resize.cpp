#include "dynarray_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_resize_empty_to_empty() {
    test_allocator<void> al;

    {
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(al);
        v.resize(0);
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 2);
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
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(al);
        v.resize(5);
        CHECK(v, 5, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 0);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_resize_more_no_realloc() {
    test_allocator<void> al;

    {
        std::array<Ty, 5> init = {1, 2, 3, 4, 5};
        int tst[] = {1, 2, 3, 4, 5, 0, 0};
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(init.size(), al);
        for (size_t n = 0; n < v.size(); ++n) { v[n] = init[n]; }
        v.reserve(10);
        al.reset_alloc_detected();
        v.resize(7);
        CHECK(v, 7, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 12);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_resize_more_needs_realloc() {
    test_allocator<void> al;

    {
        std::array<Ty, 5> init = {1, 2, 3, 4, 5};
        int tst[] = {1, 2, 3, 4, 5, 0, 0};
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(init.size(), al);
        for (size_t n = 0; n < v.size(); ++n) { v[n] = init[n]; }
        al.reset_alloc_detected();
        v.resize(7);
        CHECK(v, 7, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 12);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_resize_less() {
    test_allocator<void> al;

    {
        std::array<Ty, 7> init = {1, 2, 3, 4, 5, 6, 7};
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(init.size(), al);
        for (size_t n = 0; n < v.size(); ++n) { v[n] = init[n]; }
        v.resize(5);
        CHECK(v, 5, init.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 12);
        VERIFY(T::not_empty_count == 12);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_resize_same_amount() {
    test_allocator<void> al;

    {
        std::array<Ty, 5> init = {1, 2, 3, 4, 5};
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(init.size(), al);
        for (size_t n = 0; n < v.size(); ++n) { v[n] = init[n]; }
        v.resize(5);
        CHECK(v, 5, init.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_resize_empty_to_not_empty() {
    test_allocator<void> al;

    {
        std::array<Ty, 5> init = {1, 2, 3, 4, 5};
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(init.size(), al);
        for (size_t n = 0; n < v.size(); ++n) { v[n] = init[n]; }
        v.resize(0);
        CHECK_EMPTY(v);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_resize_more_needs_realloc_nothrow_move() { return test_resize_more_needs_realloc<T>(); }
int test_resize_more_needs_realloc_throwing_move() { return test_resize_more_needs_realloc<T_ThrowingMove>(); }

}  // namespace

ADD_TEST_CASE("", "dynarray", test_resize_empty_to_empty);
ADD_TEST_CASE("", "dynarray", test_resize_not_empty_to_empty);
ADD_TEST_CASE("", "dynarray", test_resize_more_no_realloc);
ADD_TEST_CASE("", "dynarray", test_resize_more_needs_realloc_nothrow_move);
ADD_TEST_CASE("", "dynarray", test_resize_more_needs_realloc_throwing_move);
ADD_TEST_CASE("", "dynarray", test_resize_less);
ADD_TEST_CASE("", "dynarray", test_resize_same_amount);
ADD_TEST_CASE("", "dynarray", test_resize_empty_to_not_empty);
