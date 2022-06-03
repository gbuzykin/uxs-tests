#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

int test_clear_empty() {
    test_allocator<void> al;

    {
        uxs::vector<T, test_allocator<T>> v(al);
        v.clear();
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

int test_clear_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<T> tst = {1, 2, 3, 4, 5};
        uxs::vector<T, test_allocator<T>> v(tst, al);
        v.clear();
        CHECK_EMPTY(v);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test_clear_empty);
ADD_TEST_CASE("", "vector", test_clear_not_empty);
