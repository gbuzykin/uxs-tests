#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_empty_default_alloc() {
    uxs::list<Ty> l;
    CHECK_EMPTY(l);
    return 0;
}

template<typename Ty = T>
int test_empty_external_alloc() {
    test_allocator<void> al;

    {
        uxs::list<Ty, test_allocator<Ty>> l(al);
        CHECK_EMPTY(l);
        VERIFY(l.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_empty_default_alloc);
ADD_TEST_CASE("", "list", test_empty_external_alloc);
