#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

int test_merge() {
    test_allocator<void> al;

    {
        uxs::list<T, test_allocator<T>> l1{{0, 1, 3, 5, 9}, al};
        uxs::list<T, test_allocator<T>> l2{{2, 4, 6, 7, 8}, al};
        int tst[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        l1.merge(l2);
        CHECK(l1, 10, tst);
        CHECK_EMPTY(l2);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_merge);
