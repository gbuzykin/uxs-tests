#include "rbtree_tests.h"

#include "uxs/multiset.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_empty_default_alloc() {
    uxs::multiset<Ty> ms;
    CHECK_EMPTY(ms);
    return 0;
}

template<typename Ty = T>
int test_empty_external_alloc() {
    test_allocator<void> al;

    {
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(al);
        CHECK_EMPTY(ms);
        VERIFY(ms.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multiset", test_empty_default_alloc);
ADD_TEST_CASE("", "multiset", test_empty_external_alloc);
