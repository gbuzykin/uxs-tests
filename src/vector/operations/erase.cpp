#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_erase_from_empty() {
    test_allocator<void> al;

    {
        uxs::vector<Ty, test_allocator<Ty>> v(al);
        VERIFY(v.erase(v.begin(), v.end()) == v.end());
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(T::instance_count == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_erase_one() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4};
        int tst2[] = {1, 3, 4};
        int tst3[] = {3, 4};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
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

template<typename Ty = T>
int test_erase_multi() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        int tst1[] = {1, 2, 3, 4, 5, 6, 7};
        int tst2[] = {1, 2, 3, 6, 7};
        int tst3[] = {3, 6, 7};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        al.reset_alloc_detected();
        // back
        auto it = v.erase(v.end() - 2, v.end());
        VERIFY(it == v.end());
        CHECK(v, 7, tst1);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 16);
        VERIFY(al.get_alloc_detected() == 0);
        // mid
        it = v.erase(v.begin() + 3, v.begin() + 5);
        VERIFY(it == v.begin() + 3);
        CHECK(v, 5, tst2);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 14);
        VERIFY(al.get_alloc_detected() == 0);
        // front
        it = v.erase(v.begin(), v.begin() + 2);
        VERIFY(it == v.begin());
        CHECK(v, 3, tst3);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_detected() == 0);
        // no erase
        it = v.erase(v.end(), v.end());
        VERIFY(it == v.end());
        CHECK(v, 3, tst3);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_detected() == 0);
        it = v.erase(v.begin() + 1, v.begin() + 1);
        VERIFY(it == v.begin() + 1);
        CHECK(v, 3, tst3);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_detected() == 0);
        it = v.erase(v.begin(), v.begin());
        VERIFY(it == v.begin());
        CHECK(v, 3, tst3);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test_erase_from_empty);
ADD_TEST_CASE("", "vector", test_erase_one);
ADD_TEST_CASE("", "vector", test_erase_multi);
