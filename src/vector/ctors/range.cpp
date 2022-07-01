#include "vector_tests.h"

#include "uxs/list.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty, typename Src>
int test_range_empty() {
    test_allocator<void> al;

    {
        Src init;
        uxs::vector<Ty, test_allocator<Ty>> v(init.begin(), init.end(), al);
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

template<typename Ty, typename Src>
int test_range_not_empty() {
    test_allocator<void> al;

    {
        Src init = {1, 2, 3, 4, 5};
        uxs::vector<Ty, test_allocator<Ty>> v(init.begin(), init.end(), al);
        CHECK(v, init.size(), init.begin());
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

int test_range_empty_random_access_range() { return test_range_empty<T, uxs::vector<T>>(); }
int test_range_not_empty_random_access_range() { return test_range_not_empty<T, uxs::vector<T>>(); }
int test_range_empty_generic_input_range() { return test_range_empty<T, uxs::list<T>>(); }
int test_range_not_empty_generic_input_range() { return test_range_not_empty<T, uxs::list<T>>(); }

}  // namespace

ADD_TEST_CASE("", "vector", test_range_empty_random_access_range);
ADD_TEST_CASE("", "vector", test_range_not_empty_random_access_range);
ADD_TEST_CASE("", "vector", test_range_empty_generic_input_range);
ADD_TEST_CASE("", "vector", test_range_not_empty_generic_input_range);
