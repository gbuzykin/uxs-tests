#include "vector_tests.h"

using namespace util_test_suite;

namespace {

int test_copy_empty_to_empty() {
    unfriendly_test_allocator<void> al, al2;

    {
        util::vector<T, unfriendly_test_allocator<T>> v(al), v_from(al2);
        v = v_from;
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(v.get_allocator() == al2);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
        VERIFY(al2.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

int test_copy_not_empty_to_empty() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<T> tst = {1, 2, 3, 4, 5};
        util::vector<T, unfriendly_test_allocator<T>> v(al), v_from(tst, al2);
        v = v_from;
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al2);
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 15);
        VERIFY(al.get_alloc_detected() == 0);
        VERIFY(al2.get_alloc_detected() == v_from.capacity() + v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

int test_copy_not_empty_to_not_empty() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<T> tst_prev = {1, 2, 3, 4, 5};
        std::initializer_list<T> tst = {11, 12, 13, 14, 15, 16, 17};
        util::vector<T, unfriendly_test_allocator<T>> v(tst_prev, al), v_from(tst, al2);
        al.reset_alloc_detected();
        v = v_from;
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al2);
        VERIFY(T::instance_count == 26);
        VERIFY(T::not_empty_count == 26);
        VERIFY(al.get_alloc_detected() == 0);
        VERIFY(al2.get_alloc_detected() == v_from.capacity() + v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

int test_copy_empty_to_not_empty() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<T> tst_prev = {1, 2, 3, 4, 5};
        util::vector<T, unfriendly_test_allocator<T>> v(tst_prev, al), v_from(al2);
        al.reset_alloc_detected();
        v = v_from;
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(v.get_allocator() == al2);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == 0);
        VERIFY(al2.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test_copy_empty_to_empty);
ADD_TEST_CASE("", "vector", test_copy_not_empty_to_empty);
ADD_TEST_CASE("", "vector", test_copy_not_empty_to_not_empty);
ADD_TEST_CASE("", "vector", test_copy_empty_to_not_empty);
