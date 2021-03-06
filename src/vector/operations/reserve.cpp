#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty>
int test_reserve_empty() {
    test_allocator<void> al;

    {
        uxs::vector<Ty, test_allocator<Ty>> v(al);
        v.reserve(0);
        VERIFY(v.capacity() == 0);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
        v.reserve(10);
        CHECK_EMPTY(v);
        VERIFY(v.capacity() >= 10);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_reserve_more() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        al.reset_alloc_detected();
        size_t cap = v.capacity() + 10;
        v.reserve(cap);
        CHECK(v, init.size(), init.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.capacity() >= cap);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == v.capacity());
        VERIFY(al.get_alloc_count() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_reserve_less() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        v.reserve(v.capacity() + 10);
        al.reset_alloc_detected();

        size_t cap = v.capacity();
        v.reserve(cap);
        CHECK(v, init.size(), init.begin());
        VERIFY(v.capacity() == cap);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 0);
        v.reserve(7);
        CHECK(v, init.size(), init.begin());
        VERIFY(v.capacity() == cap);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 0);
        v.reserve(3);
        CHECK(v, init.size(), init.begin());
        VERIFY(v.capacity() == cap);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 0);
        v.reserve(0);
        CHECK(v, init.size(), init.begin());
        VERIFY(v.capacity() == cap);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_reserve_empty_nothrow_move() { return test_reserve_empty<T>(); }
int test_reserve_more_nothrow_move() { return test_reserve_more<T>(); }
int test_reserve_less_nothrow_move() { return test_reserve_less<T>(); }
int test_reserve_empty_throwing_move() { return test_reserve_empty<T_ThrowingMove>(); }
int test_reserve_more_throwing_move() { return test_reserve_more<T_ThrowingMove>(); }
int test_reserve_less_throwing_move() { return test_reserve_less<T_ThrowingMove>(); }

}  // namespace

ADD_TEST_CASE("", "vector", test_reserve_empty_nothrow_move);
ADD_TEST_CASE("", "vector", test_reserve_more_nothrow_move);
ADD_TEST_CASE("", "vector", test_reserve_less_nothrow_move);
ADD_TEST_CASE("", "vector", test_reserve_empty_throwing_move);
ADD_TEST_CASE("", "vector", test_reserve_more_throwing_move);
ADD_TEST_CASE("", "vector", test_reserve_less_throwing_move);
