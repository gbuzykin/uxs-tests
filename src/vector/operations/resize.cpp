#include "vector_tests.h"

using namespace util_test_suite;

namespace {

template<typename Ty>
int test_resize_empty_to_empty() {
    test_allocator<void> al;

    {
        util::vector<Ty, test_allocator<Ty>> v(al);
        v.resize(0);
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(Ty::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_resize_not_empty_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {0, 0, 0, 0, 0};
        util::vector<Ty, test_allocator<Ty>> v(al);
        v.resize(5);
        CHECK(v, 5, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(Ty::instance_count == 5);
        VERIFY(Ty::not_empty_count == 0);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_resize_more_no_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        int tst[] = {1, 2, 3, 4, 5, 0, 0};
        util::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.reserve(10);
        al.reset_alloc_detected();
        v.resize(7);
        CHECK(v, 7, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(Ty::instance_count == 12);
        VERIFY(Ty::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_resize_more_needs_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        int tst[] = {1, 2, 3, 4, 5, 0, 0};
        util::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.shrink_to_fit();
        al.reset_alloc_detected();
        v.resize(7);
        CHECK(v, 7, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(Ty::instance_count == 12);
        VERIFY(Ty::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_resize_less() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5, 6, 7};
        util::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.resize(5);
        CHECK(v, 5, tst_prev.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(Ty::instance_count == 12);
        VERIFY(Ty::not_empty_count == 12);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_resize_same_amount() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        util::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.resize(5);
        CHECK(v, 5, tst_prev.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(Ty::instance_count == 10);
        VERIFY(Ty::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_resize_empty_to_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        util::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.resize(0);
        CHECK_EMPTY(v);
        VERIFY(Ty::instance_count == 5);
        VERIFY(Ty::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_resize_value_empty_to_empty() {
    test_allocator<void> al;

    {
        util::vector<Ty, test_allocator<Ty>> v(al);
        v.resize(0, 10);
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(Ty::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_resize_value_not_empty_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {10, 10, 10, 10, 10};
        util::vector<Ty, test_allocator<Ty>> v(al);
        v.resize(5, 10);
        CHECK(v, 5, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(Ty::instance_count == 5);
        VERIFY(Ty::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_resize_value_more_no_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        int tst[] = {1, 2, 3, 4, 5, 10, 10};
        util::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.reserve(10);
        al.reset_alloc_detected();
        v.resize(7, 10);
        CHECK(v, 7, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(Ty::instance_count == 12);
        VERIFY(Ty::not_empty_count == 12);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_resize_value_more_needs_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        int tst[] = {1, 2, 3, 4, 5, 10, 10};
        util::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.shrink_to_fit();
        al.reset_alloc_detected();
        v.resize(7, 10);
        CHECK(v, 7, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(Ty::instance_count == 12);
        VERIFY(Ty::not_empty_count == 12);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_resize_value_less() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5, 6, 7};
        util::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.resize(5, 10);
        CHECK(v, 5, tst_prev.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(Ty::instance_count == 12);
        VERIFY(Ty::not_empty_count == 12);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_resize_value_same_amount() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        util::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.resize(5, 10);
        CHECK(v, 5, tst_prev.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(Ty::instance_count == 10);
        VERIFY(Ty::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_resize_value_empty_to_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        util::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.resize(0, 10);
        CHECK_EMPTY(v);
        VERIFY(Ty::instance_count == 5);
        VERIFY(Ty::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_resize_empty_to_empty_assignable() { return test_resize_empty_to_empty<T>(); }
int test_resize_not_empty_to_empty_assignable() { return test_resize_not_empty_to_empty<T>(); }
int test_resize_more_no_realloc_assignable() { return test_resize_more_no_realloc<T>(); }
int test_resize_more_needs_realloc_assignable_nothrow_move() { return test_resize_more_needs_realloc<T>(); }
int test_resize_more_needs_realloc_assignable_throwing_move() {
    return test_resize_more_needs_realloc<T_ThrowingMove>();
}
int test_resize_more_needs_realloc_assignable_nothrow_move_default() {
    return test_resize_more_needs_realloc<T_NothrowDefaultCopyMove>();
}
int test_resize_less_assignable() { return test_resize_less<T>(); }
int test_resize_same_amount_assignable() { return test_resize_same_amount<T>(); }
int test_resize_empty_to_not_empty_assignable() { return test_resize_empty_to_not_empty<T>(); }

int test_resize_value_empty_to_empty_assignable() { return test_resize_value_empty_to_empty<T>(); }
int test_resize_value_not_empty_to_empty_assignable() { return test_resize_value_not_empty_to_empty<T>(); }
int test_resize_value_more_no_realloc_assignable() { return test_resize_value_more_no_realloc<T>(); }
int test_resize_value_more_needs_realloc_assignable_nothrow_move() { return test_resize_value_more_needs_realloc<T>(); }
int test_resize_value_more_needs_realloc_assignable_throwing_move() {
    return test_resize_value_more_needs_realloc<T_ThrowingMove>();
}
int test_resize_value_more_needs_realloc_assignable_nothrow_move_default() {
    return test_resize_value_more_needs_realloc<T_NothrowDefaultCopyMove>();
}
int test_resize_value_less_assignable() { return test_resize_value_less<T>(); }
int test_resize_value_same_amount_assignable() { return test_resize_value_same_amount<T>(); }
int test_resize_value_empty_to_not_empty_assignable() { return test_resize_value_empty_to_not_empty<T>(); }

}  // namespace

ADD_TEST_CASE("", "vector", test_resize_empty_to_empty_assignable);
ADD_TEST_CASE("", "vector", test_resize_not_empty_to_empty_assignable);
ADD_TEST_CASE("", "vector", test_resize_more_no_realloc_assignable);
ADD_TEST_CASE("", "vector", test_resize_more_needs_realloc_assignable_nothrow_move);
ADD_TEST_CASE("", "vector", test_resize_more_needs_realloc_assignable_throwing_move);
ADD_TEST_CASE("", "vector", test_resize_more_needs_realloc_assignable_nothrow_move_default);
ADD_TEST_CASE("", "vector", test_resize_less_assignable);
ADD_TEST_CASE("", "vector", test_resize_same_amount_assignable);
ADD_TEST_CASE("", "vector", test_resize_empty_to_not_empty_assignable);
ADD_TEST_CASE("", "vector", test_resize_value_empty_to_empty_assignable);
ADD_TEST_CASE("", "vector", test_resize_value_not_empty_to_empty_assignable);
ADD_TEST_CASE("", "vector", test_resize_value_more_no_realloc_assignable);
ADD_TEST_CASE("", "vector", test_resize_value_more_needs_realloc_assignable_nothrow_move);
ADD_TEST_CASE("", "vector", test_resize_value_more_needs_realloc_assignable_throwing_move);
ADD_TEST_CASE("", "vector", test_resize_value_more_needs_realloc_assignable_nothrow_move_default);
ADD_TEST_CASE("", "vector", test_resize_value_less_assignable);
ADD_TEST_CASE("", "vector", test_resize_value_same_amount_assignable);
ADD_TEST_CASE("", "vector", test_resize_value_empty_to_not_empty_assignable);