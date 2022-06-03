#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

int test_initializer_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<T> tst;
        uxs::vector<T, test_allocator<T>> v(tst, al);
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

int test_initializer_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<T> tst = {1, 2, 3, 4, 5};
        uxs::vector<T, test_allocator<T>> v(tst, al);
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test_initializer_empty);
ADD_TEST_CASE("", "vector", test_initializer_not_empty);
