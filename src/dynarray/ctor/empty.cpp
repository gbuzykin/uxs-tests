#include "dynarray_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_empty_default_alloc() {
    uxs::inline_dynarray<Ty, 2> v;
    CHECK_EMPTY(v);
    VERIFY(v.capacity() == 2);
    return 0;
}

template<typename Ty = T>
int test_empty_external_alloc() {
    test_allocator<void> al;

    {
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(al);
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 2);
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "dynarray", test_empty_default_alloc);
ADD_TEST_CASE("", "dynarray", test_empty_external_alloc);
