#include "dynarray_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_clear_empty() {
    test_allocator<void> al;

    {
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(al);
        v.clear();
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 2);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

template<typename Ty = T>
int test_clear_not_empty() {
    test_allocator<void> al;

    {
        std::array<Ty, 5> init = {1, 2, 3, 4, 5};
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(init.size(), al);
        for (size_t n = 0; n < v.size(); ++n) { v[n] = init[n]; }
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

ADD_TEST_CASE("", "dynarray", test_clear_empty);
ADD_TEST_CASE("", "dynarray", test_clear_not_empty);
