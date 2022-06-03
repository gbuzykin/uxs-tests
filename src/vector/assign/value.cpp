#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty>
int test_value_empty_to_empty() {
    test_allocator<void> al;

    {
        uxs::vector<Ty, test_allocator<Ty>> v(al);
        v.assign(0, 10);
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_value_not_empty_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {10, 10, 10, 10, 10};
        uxs::vector<Ty, test_allocator<Ty>> v(al);
        v.assign(5, 10);
        CHECK(v, 5, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_value_more_no_realloc() {
    test_allocator<void> al;

    {
        int tst[] = {20, 20, 20, 20, 20, 20, 20};
        uxs::vector<Ty, test_allocator<Ty>> v(5, 10, al);
        v.reserve(10);
        al.reset_alloc_detected();
        v.assign(7, 20);
        CHECK(v, 7, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 7);
        VERIFY(T::not_empty_count == 7);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_value_more_needs_realloc() {
    test_allocator<void> al;

    {
        int tst[] = {20, 20, 20, 20, 20, 20, 20};
        uxs::vector<Ty, test_allocator<Ty>> v(5, 10, al);
        v.shrink_to_fit();
        al.reset_alloc_detected();
        v.assign(7, 20);
        CHECK(v, 7, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 7);
        VERIFY(T::not_empty_count == 7);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_value_less() {
    test_allocator<void> al;

    {
        int tst[] = {20, 20, 20, 20, 20};
        uxs::vector<Ty, test_allocator<Ty>> v(7, 10, al);
        v.assign(5, 20);
        CHECK(v, 5, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_value_same_amount() {
    test_allocator<void> al;

    {
        int tst[] = {20, 20, 20, 20, 20};
        uxs::vector<Ty, test_allocator<Ty>> v(5, 10, al);
        v.assign(5, 20);
        CHECK(v, 5, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_value_empty_to_not_empty() {
    test_allocator<void> al;

    {
        uxs::vector<Ty, test_allocator<Ty>> v(5, 10, al);
        v.assign(0, 20);
        CHECK_EMPTY(v);
        VERIFY(T::instance_count == 0);
        VERIFY(T::not_empty_count == 0);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_value_empty_to_empty_assignable() { return test_value_empty_to_empty<T>(); }
int test_value_not_empty_to_empty_assignable() { return test_value_not_empty_to_empty<T>(); }
int test_value_more_no_realloc_assignable() { return test_value_more_no_realloc<T>(); }
int test_value_more_needs_realloc_assignable_nothrow_move() { return test_value_more_needs_realloc<T>(); }
int test_value_more_needs_realloc_assignable_throwing_move() { return test_value_more_needs_realloc<T_ThrowingMove>(); }
int test_value_less_assignable() { return test_value_less<T>(); }
int test_value_same_amount_assignable() { return test_value_same_amount<T>(); }
int test_value_empty_to_not_empty_assignable() { return test_value_empty_to_not_empty<T>(); }
int test_value_empty_to_empty_not_assignable() { return test_value_empty_to_empty<T_NotAssignable>(); }
int test_value_not_empty_to_empty_not_assignable() { return test_value_not_empty_to_empty<T_NotAssignable>(); }
int test_value_more_no_realloc_not_assignable() { return test_value_more_no_realloc<T_NotAssignable>(); }
int test_value_more_needs_realloc_not_assignable_nothrow_move() {
    return test_value_more_needs_realloc<T_NotAssignable>();
}
int test_value_more_needs_realloc_not_assignable_throwing_move() {
    return test_value_more_needs_realloc<T_ThrowingMove_NotAssignable>();
}
int test_value_less_not_assignable() { return test_value_less<T_NotAssignable>(); }
int test_value_same_amount_not_assignable() { return test_value_same_amount<T_NotAssignable>(); }
int test_value_empty_to_not_empty_not_assignable() { return test_value_empty_to_not_empty<T_NotAssignable>(); }

}  // namespace

ADD_TEST_CASE("", "vector", test_value_empty_to_empty_assignable);
ADD_TEST_CASE("", "vector", test_value_not_empty_to_empty_assignable);
ADD_TEST_CASE("", "vector", test_value_more_no_realloc_assignable);
ADD_TEST_CASE("", "vector", test_value_more_needs_realloc_assignable_nothrow_move);
ADD_TEST_CASE("", "vector", test_value_more_needs_realloc_assignable_throwing_move);
ADD_TEST_CASE("", "vector", test_value_less_assignable);
ADD_TEST_CASE("", "vector", test_value_same_amount_assignable);
ADD_TEST_CASE("", "vector", test_value_empty_to_not_empty_assignable);
ADD_TEST_CASE("", "vector", test_value_empty_to_empty_not_assignable);
ADD_TEST_CASE("", "vector", test_value_not_empty_to_empty_not_assignable);
ADD_TEST_CASE("", "vector", test_value_more_no_realloc_not_assignable);
ADD_TEST_CASE("", "vector", test_value_more_needs_realloc_not_assignable_nothrow_move);
ADD_TEST_CASE("", "vector", test_value_more_needs_realloc_not_assignable_throwing_move);
ADD_TEST_CASE("", "vector", test_value_less_not_assignable);
ADD_TEST_CASE("", "vector", test_value_same_amount_not_assignable);
ADD_TEST_CASE("", "vector", test_value_empty_to_not_empty_not_assignable);
