#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

int test_sort() {
    test_allocator<void> al;

    {
        uxs::list<T, test_allocator<T>> l{{5, 9, 0, 1, 3}, al};
        int tst[] = {0, 1, 3, 5, 9};
        l.sort();
        CHECK(l, 5, tst);
    }

    {
        uxs::list<T, test_allocator<T>> l{{8, 7, 2, 6, 4}, al};
        int tst[] = {2, 4, 6, 7, 8};
        l.sort();
        CHECK(l, 5, tst);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_sort);
