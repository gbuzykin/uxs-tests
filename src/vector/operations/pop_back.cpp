#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_pop_back() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst[] = {1, 2, 3, 4};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        al.reset_alloc_detected();
        v.pop_back();
        CHECK(v, 4, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 9);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test_pop_back);
