#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_move_empty_to_empty() {
    unfriendly_test_allocator<void> al, al2;

    {
        uxs::vector<Ty, unfriendly_test_allocator<Ty>> v(al), v_from(al2);
        v = std::move(v_from);
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(v.get_allocator() == al);
        VERIFY(v_from.capacity() == 0);
        VERIFY(v_from.get_allocator() == al2);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
        VERIFY(al2.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_not_empty_to_empty() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::vector<Ty, unfriendly_test_allocator<Ty>> v(al), v_from(init, al2);
        v = std::move(v_from);
        CHECK(v, init.size(), init.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(v_from.get_allocator() == al2);
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == v.capacity());
        VERIFY(al2.get_alloc_detected() == v_from.capacity());
        VERIFY(al2.get_alloc_count() == v_from.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_more_no_realloc() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15, 16, 17};
        uxs::vector<Ty, unfriendly_test_allocator<Ty>> v(init, al), v_from(init2, al2);
        v.reserve(10);
        al.reset_alloc_detected();
        v = std::move(v_from);
        CHECK(v, init2.size(), init2.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(v_from.get_allocator() == al2);
        VERIFY(T::instance_count == 26);
        VERIFY(T::not_empty_count == 19);
        VERIFY(al.get_alloc_detected() == 0);
        VERIFY(al2.get_alloc_detected() == v_from.capacity());
        VERIFY(al2.get_alloc_count() == v_from.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_move_more_needs_realloc() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15, 16, 17};
        uxs::vector<Ty, unfriendly_test_allocator<Ty>> v(init, al), v_from(init2, al2);
        v.shrink_to_fit();
        al.reset_alloc_detected();
        v = std::move(v_from);
        CHECK(v, init2.size(), init2.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(v_from.get_allocator() == al2);
        VERIFY(T::instance_count == 26);
        VERIFY(T::not_empty_count == 19);
        VERIFY(al.get_alloc_detected() == v.capacity());
        VERIFY(al2.get_alloc_detected() == v_from.capacity());
        VERIFY(al2.get_alloc_count() == v_from.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_less() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5, 6, 7};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15};
        uxs::vector<Ty, unfriendly_test_allocator<Ty>> v(init, al), v_from(init2, al2);
        v = std::move(v_from);
        CHECK(v, init2.size(), init2.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(v_from.get_allocator() == al2);
        VERIFY(T::instance_count == 22);
        VERIFY(T::not_empty_count == 17);
        VERIFY(al.get_alloc_detected() == v.capacity());
        VERIFY(al2.get_alloc_detected() == v_from.capacity());
        VERIFY(al2.get_alloc_count() == v_from.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_same_amount() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15};
        uxs::vector<Ty, unfriendly_test_allocator<Ty>> v(init, al), v_from(init2, al2);
        v = std::move(v_from);
        CHECK(v, init2.size(), init2.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(v_from.get_allocator() == al2);
        VERIFY(T::instance_count == 20);
        VERIFY(T::not_empty_count == 15);
        VERIFY(al.get_alloc_detected() == v.capacity());
        VERIFY(al2.get_alloc_detected() == v_from.capacity());
        VERIFY(al2.get_alloc_count() == v_from.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_empty_to_not_empty() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::vector<Ty, unfriendly_test_allocator<Ty>> v(init, al), v_from(al2);
        v = std::move(v_from);
        CHECK_EMPTY(v);
        VERIFY(v.get_allocator() == al);
        VERIFY(v_from.capacity() == 0);
        VERIFY(v_from.get_allocator() == al2);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == v.capacity());
        VERIFY(al2.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

int test_move_more_needs_realloc_nothrow_move() { return test_move_more_needs_realloc<T>(); }
int test_move_more_needs_realloc_throwing_move() { return test_move_more_needs_realloc<T_ThrowingMove>(); }

}  // namespace

ADD_TEST_CASE("", "vector", test_move_empty_to_empty);
ADD_TEST_CASE("", "vector", test_move_not_empty_to_empty);
ADD_TEST_CASE("", "vector", test_move_more_no_realloc);
ADD_TEST_CASE("", "vector", test_move_more_needs_realloc_nothrow_move);
ADD_TEST_CASE("", "vector", test_move_more_needs_realloc_throwing_move);
ADD_TEST_CASE("", "vector", test_move_less);
ADD_TEST_CASE("", "vector", test_move_same_amount);
ADD_TEST_CASE("", "vector", test_move_empty_to_not_empty);
