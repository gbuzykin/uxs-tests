#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

int test_move_empty_to_empty() {
    test_allocator<void> al;

    {
        uxs::vector<T, test_allocator<T>> v(al), v_from(al);
        v = std::move(v_from);
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(v.get_allocator() == al);
        CHECK_EMPTY(v_from);
        VERIFY(v_from.capacity() == 0);
        VERIFY(v_from.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_move_not_empty_to_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<T> tst = {1, 2, 3, 4, 5};
        uxs::vector<T, test_allocator<T>> v(al), v_from(tst, al);
        v = std::move(v_from);
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        CHECK_EMPTY(v_from);
        VERIFY(v_from.capacity() == 0);
        VERIFY(v_from.get_allocator() == al);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_move_not_empty_to_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<T> tst_prev = {1, 2, 3, 4, 5};
        std::initializer_list<T> tst = {11, 12, 13, 14, 15, 16, 17};
        uxs::vector<T, test_allocator<T>> v(tst_prev, al), v_from(tst, al);
        al.reset_alloc_detected();
        v = std::move(v_from);
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        CHECK_EMPTY(v_from);
        VERIFY(v_from.capacity() == 0);
        VERIFY(v_from.get_allocator() == al);
        VERIFY(T::instance_count == 19);
        VERIFY(T::not_empty_count == 19);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_move_empty_to_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<T> tst_prev = {1, 2, 3, 4, 5};
        uxs::vector<T, test_allocator<T>> v(tst_prev, al), v_from(al);
        al.reset_alloc_detected();
        v = std::move(v_from);
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(v.get_allocator() == al);
        CHECK_EMPTY(v_from);
        VERIFY(v_from.capacity() == 0);
        VERIFY(v_from.get_allocator() == al);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test_move_empty_to_empty);
ADD_TEST_CASE("", "vector", test_move_not_empty_to_empty);
ADD_TEST_CASE("", "vector", test_move_not_empty_to_not_empty);
ADD_TEST_CASE("", "vector", test_move_empty_to_not_empty);
