#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty>
int test_initializer_empty_to_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst;
        uxs::vector<Ty, test_allocator<Ty>> v(al);
        v = tst;
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
int test_initializer_not_empty_to_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst = {1, 2, 3, 4, 5};
        uxs::vector<Ty, test_allocator<Ty>> v(al);
        v = tst;
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_initializer_more_no_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> tst = {11, 12, 13, 14, 15, 16, 17};
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.reserve(10);
        al.reset_alloc_detected();
        v = tst;
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 19);
        VERIFY(T::not_empty_count == 19);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_initializer_more_needs_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> tst = {11, 12, 13, 14, 15, 16, 17};
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.shrink_to_fit();
        al.reset_alloc_detected();
        v = tst;
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 19);
        VERIFY(T::not_empty_count == 19);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_initializer_less() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5, 6, 7};
        std::initializer_list<Ty> tst = {11, 12, 13, 14, 15};
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v = tst;
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 17);
        VERIFY(T::not_empty_count == 17);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_initializer_same_amount() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> tst = {11, 12, 13, 14, 15};
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v = tst;
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 15);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_initializer_empty_to_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> tst;
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v = tst;
        CHECK_EMPTY(v);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_initializer_assign_func() {
    test_allocator<void> al;

    {
        std::initializer_list<T> tst_prev = {1, 2, 3, 4, 5, 6, 7};
        std::initializer_list<T> tst = {11, 12, 13, 14, 15};
        uxs::vector<T, test_allocator<T>> v(tst_prev, al);
        v.assign(tst);
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 17);
        VERIFY(T::not_empty_count == 17);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_initializer_empty_to_empty_assignable() { return test_initializer_empty_to_empty<T>(); }
int test_initializer_not_empty_to_empty_assignable() { return test_initializer_not_empty_to_empty<T>(); }
int test_initializer_more_no_realloc_assignable() { return test_initializer_more_no_realloc<T>(); }
int test_initializer_more_needs_realloc_assignable_nothrow_move() { return test_initializer_more_needs_realloc<T>(); }
int test_initializer_more_needs_realloc_assignable_throwing_move() {
    return test_initializer_more_needs_realloc<T_ThrowingMove>();
}
int test_initializer_less_assignable() { return test_initializer_less<T>(); }
int test_initializer_same_amount_assignable() { return test_initializer_same_amount<T>(); }
int test_initializer_empty_to_not_empty_assignable() { return test_initializer_empty_to_not_empty<T>(); }
int test_initializer_empty_to_empty_not_assignable() { return test_initializer_empty_to_empty<T_NotAssignable>(); }
int test_initializer_not_empty_to_empty_not_assignable() {
    return test_initializer_not_empty_to_empty<T_NotAssignable>();
}
int test_initializer_more_no_realloc_not_assignable() { return test_initializer_more_no_realloc<T_NotAssignable>(); }
int test_initializer_more_needs_realloc_not_assignable_nothrow_move() {
    return test_initializer_more_needs_realloc<T_NotAssignable>();
}
int test_initializer_more_needs_realloc_not_assignable_throwing_move() {
    return test_initializer_more_needs_realloc<T_ThrowingMove_NotAssignable>();
}
int test_initializer_less_not_assignable() { return test_initializer_less<T_NotAssignable>(); }
int test_initializer_same_amount_not_assignable() { return test_initializer_same_amount<T_NotAssignable>(); }
int test_initializer_empty_to_not_empty_not_assignable() {
    return test_initializer_empty_to_not_empty<T_NotAssignable>();
}

}  // namespace

ADD_TEST_CASE("", "vector", test_initializer_empty_to_empty_assignable);
ADD_TEST_CASE("", "vector", test_initializer_not_empty_to_empty_assignable);
ADD_TEST_CASE("", "vector", test_initializer_more_no_realloc_assignable);
ADD_TEST_CASE("", "vector", test_initializer_more_needs_realloc_assignable_nothrow_move);
ADD_TEST_CASE("", "vector", test_initializer_more_needs_realloc_assignable_throwing_move);
ADD_TEST_CASE("", "vector", test_initializer_less_assignable);
ADD_TEST_CASE("", "vector", test_initializer_same_amount_assignable);
ADD_TEST_CASE("", "vector", test_initializer_empty_to_not_empty_assignable);
ADD_TEST_CASE("", "vector", test_initializer_empty_to_empty_not_assignable);
ADD_TEST_CASE("", "vector", test_initializer_not_empty_to_empty_not_assignable);
ADD_TEST_CASE("", "vector", test_initializer_more_no_realloc_not_assignable);
ADD_TEST_CASE("", "vector", test_initializer_more_needs_realloc_not_assignable_nothrow_move);
ADD_TEST_CASE("", "vector", test_initializer_more_needs_realloc_not_assignable_throwing_move);
ADD_TEST_CASE("", "vector", test_initializer_less_not_assignable);
ADD_TEST_CASE("", "vector", test_initializer_same_amount_not_assignable);
ADD_TEST_CASE("", "vector", test_initializer_empty_to_not_empty_not_assignable);
ADD_TEST_CASE("", "vector", test_initializer_assign_func);
