#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_initializer_empty_to_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init;
        uxs::vector<Ty, test_allocator<Ty>> v(al);
        v = init;
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_not_empty_to_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::vector<Ty, test_allocator<Ty>> v(al);
        v = init;
        CHECK(v, init.size(), init.begin());
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
int test_initializer_more_no_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15, 16, 17};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        v.reserve(10);
        al.reset_alloc_detected();
        v = init2;
        CHECK(v, init2.size(), init2.begin());
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
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15, 16, 17};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        v.shrink_to_fit();
        al.reset_alloc_detected();
        v = init2;
        CHECK(v, init2.size(), init2.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 19);
        VERIFY(T::not_empty_count == 19);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_less() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5, 6, 7};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        v = init2;
        CHECK(v, init2.size(), init2.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 17);
        VERIFY(T::not_empty_count == 17);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_same_amount() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        v = init2;
        CHECK(v, init2.size(), init2.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 15);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_empty_to_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2;
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        v = init2;
        CHECK_EMPTY(v);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_assign_func() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5, 6, 7};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        v.assign(init2);
        CHECK(v, init2.size(), init2.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 17);
        VERIFY(T::not_empty_count == 17);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_initializer_more_needs_realloc_nothrow_move() { return test_initializer_more_needs_realloc<T>(); }
int test_initializer_more_needs_realloc_throwing_move() {
    return test_initializer_more_needs_realloc<T_ThrowingMove>();
}

}  // namespace

ADD_TEST_CASE("", "vector", test_initializer_empty_to_empty);
ADD_TEST_CASE("", "vector", test_initializer_not_empty_to_empty);
ADD_TEST_CASE("", "vector", test_initializer_more_no_realloc);
ADD_TEST_CASE("", "vector", test_initializer_more_needs_realloc_nothrow_move);
ADD_TEST_CASE("", "vector", test_initializer_more_needs_realloc_throwing_move);
ADD_TEST_CASE("", "vector", test_initializer_less);
ADD_TEST_CASE("", "vector", test_initializer_same_amount);
ADD_TEST_CASE("", "vector", test_initializer_empty_to_not_empty);
