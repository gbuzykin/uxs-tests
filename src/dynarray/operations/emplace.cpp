#include "dynarray_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_emplace_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {10};
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(al);
        auto it = v.emplace(v.end(), 10);
        VERIFY(it == v.begin());
        CHECK(v, 1, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 1);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_emplace_no_realloc() {
    test_allocator<void> al;

    {
        std::array<Ty, 5> init = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4, 5, 6};
        int tst2[] = {1, 2, 3, 7, 4, 5, 6};
        int tst3[] = {8, 1, 2, 3, 7, 4, 5, 6};
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(init.size(), al);
        for (size_t n = 0; n < v.size(); ++n) { v[n] = init[n]; }
        v.reserve(10);
        al.reset_alloc_detected();
        // back
        auto it = v.insert(v.end(), 6);
        VERIFY(it == v.end() - 1);
        CHECK(v, 6, tst1);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 11);
        VERIFY(al.get_alloc_detected() == 0);
        // mid
        it = v.insert(v.begin() + 3, 7);
        VERIFY(it == v.begin() + 3);
        CHECK(v, 7, tst2);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_detected() == 0);
        // front
        it = v.insert(v.begin(), 8);
        VERIFY(it == v.begin());
        CHECK(v, 8, tst3);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 13);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_emplace_needs_realloc() {
    test_allocator<void> al;

    {
        // back
        std::array<Ty, 5> init = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4, 5, 6};
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(init.size(), al);
        for (size_t n = 0; n < v.size(); ++n) { v[n] = init[n]; }
        al.reset_alloc_detected();
        auto it = v.insert(v.end(), 6);
        VERIFY(it == v.end() - 1);
        CHECK(v, 6, tst1);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 11);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    {
        // mid
        std::array<Ty, 7> init = {1, 2, 3, 4, 5, 6, 7};
        int tst2[] = {1, 2, 3, 8, 4, 5, 6, 7};
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(init.size(), al);
        for (size_t n = 0; n < v.size(); ++n) { v[n] = init[n]; }
        al.reset_alloc_detected();
        auto it = v.insert(v.begin() + 3, 8);
        VERIFY(it == v.begin() + 3);
        CHECK(v, 8, tst2);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 15);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    {
        // front
        std::array<Ty, 7> init = {1, 2, 3, 7, 4, 5, 6};
        int tst3[] = {8, 1, 2, 3, 7, 4, 5, 6};
        uxs::inline_dynarray<Ty, 2, test_allocator<Ty>> v(init.size(), al);
        for (size_t n = 0; n < v.size(); ++n) { v[n] = init[n]; }
        al.reset_alloc_detected();
        auto it = v.insert(v.begin(), 8);
        VERIFY(it == v.begin());
        CHECK(v, 8, tst3);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 15);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_emplace_needs_realloc_nothrow_move() { return test_emplace_needs_realloc<T>(); }
int test_emplace_needs_realloc_throwing_move() { return test_emplace_needs_realloc<T_ThrowingMove>(); }

}  // namespace

ADD_TEST_CASE("", "dynarray", test_emplace_to_empty);
ADD_TEST_CASE("", "dynarray", test_emplace_no_realloc);
ADD_TEST_CASE("", "dynarray", test_emplace_needs_realloc_nothrow_move);
ADD_TEST_CASE("", "dynarray", test_emplace_needs_realloc_throwing_move);
