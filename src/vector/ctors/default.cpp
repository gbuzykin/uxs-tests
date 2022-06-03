#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

int test_default_empty() {
    test_allocator<void> al;

    {
        uxs::vector<T, test_allocator<T>> v(0, al);
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

int test_default_not_empty() {
    test_allocator<void> al;

    {
        T tst[5];
        uxs::vector<T, test_allocator<T>> v(5, al);
        CHECK(v, 5, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 0);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test_default_empty);
ADD_TEST_CASE("", "vector", test_default_not_empty);
