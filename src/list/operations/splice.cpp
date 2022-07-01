#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

int test_splice() {
    test_allocator<void> al;

    {
        uxs::list<T, test_allocator<T>> l1{{1, 2, 3, 4, 5}, al};
        uxs::list<T, test_allocator<T>> l2{{10, 20, 30, 40, 50}, al};

        auto it = l1.begin();
        std::advance(it, 2);

        int tst1[] = {1, 2, 10, 20, 30, 40, 50, 3, 4, 5};
        l1.splice(it, l2);
        CHECK(l1, 10, tst1);
        VERIFY(l2.size() == 0);
        VERIFY(l2.begin() == l2.end());

        int tst2[] = {1, 2, 10, 20, 30, 40, 50};
        int tst3[] = {3, 4, 5};
        l2.splice(l2.begin(), l1, it, l1.end());
        CHECK(l1, 7, tst2);
        CHECK(l2, 3, tst3);

        int tst4[] = {1, 2, 5, 10, 20, 30, 40, 50};
        int tst5[] = {3, 4};
        l1.splice(std::next(l1.begin(), 2), l2, std::next(l2.begin(), 2));
        CHECK(l1, 8, tst4);
        CHECK(l2, 2, tst5);

        int tst6[] = {1, 2, 20, 5, 10, 30, 40, 50};
        l1.splice(std::next(l1.begin(), 2), l1, std::next(l1.begin(), 4));
        CHECK(l1, 8, tst6);

        int tst7[] = {1, 2, 5, 10, 30, 40, 50, 20};
        l1.splice(std::next(l1.begin(), 2), l1, std::next(l1.begin(), 3), l1.end());
        CHECK(l1, 8, tst7);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_splice);
