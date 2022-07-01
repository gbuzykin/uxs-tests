#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

int test_unique() {
    test_allocator<void> al;

    {
        uxs::list<T, test_allocator<T>> list4{{0, 1, 2, 2, 2, 5, 6, 7, 3, 3}, al};
        list4.unique();
        std::initializer_list<T> tst6 = {0, 1, 2, 5, 6, 7, 3};
        CHECK(list4, tst6.size(), tst6.begin());
        return 0;
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_unique);
