#include "dynarray_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_erase_one() {
    test_allocator<void> al;

    {
        std::array<Ty, 5> init = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4};
        int tst2[] = {1, 3, 4};
        int tst3[] = {3, 4};
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(init.size(), al);
        for (size_t n = 0; n < v.size(); ++n) { v[n] = init[n]; }
        al.reset_alloc_detected();
        // back
        auto it = v.erase(v.end() - 1);
        VERIFY(it == v.end());
        CHECK(v, 4, tst1);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 9);
        VERIFY(al.get_alloc_detected() == 0);
        // mid
        it = v.erase(v.begin() + 1);
        VERIFY(it == v.begin() + 1);
        CHECK(v, 3, tst2);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 8);
        VERIFY(al.get_alloc_detected() == 0);
        // front
        it = v.erase(v.begin());
        VERIFY(it == v.begin());
        CHECK(v, 2, tst3);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 7);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "dynarray", test_erase_one);
