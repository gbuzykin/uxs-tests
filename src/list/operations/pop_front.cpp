#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_pop_front() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst[] = {2, 3, 4, 5};
        uxs::list<Ty, test_allocator<Ty>> l(init, al);
        l.pop_front();
        CHECK(l, 4, tst);
        VERIFY(T::instance_count == 9);
        VERIFY(al.get_alloc_count() == 4);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_pop_front);
