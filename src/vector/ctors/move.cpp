#include "vector_tests.h"

using namespace util_test_suite;

namespace {

int test_move_from_empty_move_alloc() {
    test_allocator<void> al;

    {
        util::vector<T, test_allocator<T>> v_from(al);
        util::vector<T, test_allocator<T>> v(std::move(v_from));
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(v.get_allocator() == al);
        CHECK_EMPTY(v_from);
        VERIFY(v_from.capacity() == 0);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

int test_move_from_empty_new_alloc() {
    test_allocator<void> al, al2;

    {
        util::vector<T, test_allocator<T>> v_from(al2);
        util::vector<T, test_allocator<T>> v(std::move(v_from), al);
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(v.get_allocator() == al);
        CHECK_EMPTY(v_from);
        VERIFY(v_from.capacity() == 0);
        VERIFY(v_from.get_allocator() == al2);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
        VERIFY(al2.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    VERIFY(al2.get_alloc_detected() == 0);
    return 0;
}

int test_move_from_empty_same_alloc() {
    test_allocator<void> al;

    {
        util::vector<T, test_allocator<T>> v_from(al);
        util::vector<T, test_allocator<T>> v(std::move(v_from), al);
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
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

int test_move_from_not_empty_move_alloc() {
    test_allocator<void> al;

    {
        std::initializer_list<T> tst = {1, 2, 3, 4, 5};
        util::vector<T, test_allocator<T>> v_from(tst, al);
        util::vector<T, test_allocator<T>> v(std::move(v_from));
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        CHECK_EMPTY(v_from);
        VERIFY(v_from.capacity() == 0);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_move_from_not_empty_new_alloc() {
    test_allocator<void> al, al2;

    {  // different allocators -> per-element movement
        std::initializer_list<T> tst = {1, 2, 3, 4, 5};
        util::vector<T, test_allocator<T>> v_from(tst, al2);
        util::vector<T, test_allocator<T>> v(std::move(v_from), al);
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(v_from.size() == 5);
        VERIFY(v_from.capacity() >= v_from.size());
        VERIFY(v_from.get_allocator() == al2);
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == v.capacity());
        VERIFY(al2.get_alloc_detected() == v_from.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

int test_move_from_not_empty_same_alloc() {
    test_allocator<void> al;

    {
        std::initializer_list<T> tst = {1, 2, 3, 4, 5};
        util::vector<T, test_allocator<T>> v_from(tst, al);
        util::vector<T, test_allocator<T>> v(std::move(v_from), al);
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

}  // namespace

ADD_TEST_CASE("", "vector", test_move_from_empty_move_alloc);
ADD_TEST_CASE("", "vector", test_move_from_empty_new_alloc);
ADD_TEST_CASE("", "vector", test_move_from_empty_same_alloc);
ADD_TEST_CASE("", "vector", test_move_from_not_empty_move_alloc);
ADD_TEST_CASE("", "vector", test_move_from_not_empty_new_alloc);
ADD_TEST_CASE("", "vector", test_move_from_not_empty_same_alloc);
