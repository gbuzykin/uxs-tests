#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_copy_from_empty_propagate_alloc() {
    test_allocator<void> al;

    {
        uxs::vector<Ty, test_allocator<Ty>> v_from(al);
        uxs::vector<Ty, test_allocator<Ty>> v(v_from);
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_from_empty_new_alloc() {
    test_allocator<void> al, al2;

    {
        uxs::vector<Ty, test_allocator<Ty>> v_from(al2);
        uxs::vector<Ty, test_allocator<Ty>> v(v_from, al);
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
        VERIFY(al2.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    VERIFY(al2.get_alloc_detected() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_from_not_empty_propagate_alloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst = {1, 2, 3, 4, 5};
        uxs::vector<Ty, test_allocator<Ty>> v_from(tst, al);
        uxs::vector<Ty, test_allocator<Ty>> v(v_from);
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 15);
        VERIFY(al.get_alloc_detected() == v_from.capacity() + v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_from_not_empty_new_alloc() {
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> tst = {1, 2, 3, 4, 5};
        uxs::vector<Ty, test_allocator<Ty>> v_from(tst, al2);
        uxs::vector<Ty, test_allocator<Ty>> v(v_from, al);
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 15);
        VERIFY(al.get_alloc_detected() == v.capacity());
        VERIFY(al2.get_alloc_detected() == v_from.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test_copy_from_empty_propagate_alloc);
ADD_TEST_CASE("", "vector", test_copy_from_empty_new_alloc);
ADD_TEST_CASE("", "vector", test_copy_from_not_empty_propagate_alloc);
ADD_TEST_CASE("", "vector", test_copy_from_not_empty_new_alloc);
