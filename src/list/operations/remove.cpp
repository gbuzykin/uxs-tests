#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

int test_remove() {
    test_allocator<void> al;

    {
        uxs::list<T, test_allocator<T>> list3{{0, 1, 2, 3, 4, 5, 6, 7, 3, 2}, al};
        list3.remove(2);
        std::initializer_list<T> tst4 = {0, 1, 3, 4, 5, 6, 7, 3};
        CHECK(list3, tst4.size(), tst4.begin());
        list3.remove_if([](decltype(*list3.begin()) el) { return el == 3; });
        std::initializer_list<T> tst5 = {0, 1, 4, 5, 6, 7};
        CHECK(list3, tst5.size(), tst5.begin());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_remove);
