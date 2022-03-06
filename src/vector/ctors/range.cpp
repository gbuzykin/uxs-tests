#include "util/list.h"
#include "util/vector.h"
#include "vector_tests.h"

using namespace util_test_suite;

namespace {

template<typename Src>
int test_range_empty() {
    test_allocator<void> al;

    {
        Src tst;
        util::vector<T, test_allocator<T>> v(tst.begin(), tst.end(), al);
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

template<typename Src>
int test_range_not_empty() {
    test_allocator<void> al;

    {
        Src tst = {1, 2, 3, 4, 5};
        util::vector<T, test_allocator<T>> v(tst.begin(), tst.end(), al);
        CHECK(v, 5, tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, T>::value ? 10 : 5));
        VERIFY(T::not_empty_count == (std::is_same<typename Src::value_type, T>::value ? 10 : 5));
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_range_empty_random_access_range() { return test_range_empty<util::vector<T>>(); }
int test_range_not_empty_random_access_range() { return test_range_not_empty<util::vector<T>>(); }
int test_range_empty_generic_input_range() { return test_range_empty<util::list<T>>(); }
int test_range_not_empty_generic_input_range() { return test_range_not_empty<util::list<T>>(); }
int test_range_empty_random_access_range_int() { return test_range_empty<util::vector<int>>(); }
int test_range_not_empty_random_access_range_int() { return test_range_not_empty<util::vector<int>>(); }
int test_range_empty_generic_input_range_int() { return test_range_empty<util::list<int>>(); }
int test_range_not_empty_generic_input_range_int() { return test_range_not_empty<util::list<int>>(); }

}  // namespace

ADD_TEST_CASE("", "vector", test_range_empty_random_access_range);
ADD_TEST_CASE("", "vector", test_range_not_empty_random_access_range);
ADD_TEST_CASE("", "vector", test_range_empty_generic_input_range);
ADD_TEST_CASE("", "vector", test_range_not_empty_generic_input_range);
ADD_TEST_CASE("", "vector", test_range_empty_random_access_range_int);
ADD_TEST_CASE("", "vector", test_range_not_empty_random_access_range_int);
ADD_TEST_CASE("", "vector", test_range_empty_generic_input_range_int);
ADD_TEST_CASE("", "vector", test_range_not_empty_generic_input_range_int);
