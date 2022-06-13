#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_initializer_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst;
        uxs::list<Ty, test_allocator<Ty>> l(tst, al);
        CHECK_EMPTY(l);
        VERIFY(l.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst = {1, 2, 3, 4, 5};
        uxs::list<Ty, test_allocator<Ty>> l(tst, al);
        CHECK(l, tst.size(), tst.begin());
        VERIFY(l.get_allocator() == al);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == l.size());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_initializer_empty);
ADD_TEST_CASE("", "list", test_initializer_not_empty);
