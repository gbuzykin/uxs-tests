#include "list_tests.h"

#include "uxs/vector.h"

using namespace uxs_test_suite;

namespace {

template<typename Src = uxs::vector<T>>
int test_range_empty() {
    test_allocator<void> al;

    {
        Src init;
        uxs::list<T, test_allocator<T>> l(init.begin(), init.end(), al);
        CHECK_EMPTY(l);
        VERIFY(l.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

template<typename Src = uxs::vector<T>>
int test_range_not_empty() {
    test_allocator<void> al;

    {
        Src init = {1, 2, 3, 4, 5};
        uxs::list<T, test_allocator<T>> l(init.begin(), init.end(), al);
        CHECK(l, init.size(), init.begin());
        VERIFY(l.get_allocator() == al);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, T>::value ? 10 : 5));
        VERIFY(T::not_empty_count == (std::is_same<typename Src::value_type, T>::value ? 10 : 5));
        VERIFY(al.get_alloc_detected() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_range_empty);
ADD_TEST_CASE("", "list", test_range_not_empty);
