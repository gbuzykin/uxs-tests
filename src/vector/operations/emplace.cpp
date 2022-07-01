#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_emplace_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {10};
        uxs::vector<Ty, test_allocator<Ty>> v(al);
        auto it = v.emplace(v.end(), 10);
        VERIFY(it == v.begin());
        CHECK(v, 1, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 1);
        VERIFY(T::not_empty_count == 1);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_emplace_no_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4, 5, 6};
        int tst2[] = {1, 2, 3, 7, 4, 5, 6};
        int tst3[] = {8, 1, 2, 3, 7, 4, 5, 6};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        v.reserve(10);
        al.reset_alloc_detected();
        // back
        auto it = v.emplace(v.end(), 6);
        VERIFY(it == v.end() - 1);
        CHECK(v, 6, tst1);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 11);
        VERIFY(al.get_alloc_detected() == 0);
        // mid
        it = v.emplace(v.begin() + 3, 7);
        VERIFY(it == v.begin() + 3);
        CHECK(v, 7, tst2);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_detected() == 0);
        // front
        it = v.emplace(v.begin(), 8);
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
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4, 5, 6};
        int tst2[] = {1, 2, 3, 7, 4, 5, 6};
        int tst3[] = {8, 1, 2, 3, 7, 4, 5, 6};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        // back
        v.shrink_to_fit();
        al.reset_alloc_detected();
        auto it = v.emplace(v.end(), 6);
        VERIFY(it == v.end() - 1);
        CHECK(v, 6, tst1);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 11);
        VERIFY(al.get_alloc_detected() == v.capacity());
        // mid
        v.shrink_to_fit();
        al.reset_alloc_detected();
        it = v.emplace(v.begin() + 3, 7);
        VERIFY(it == v.begin() + 3);
        CHECK(v, 7, tst2);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_detected() == v.capacity());
        // front
        v.shrink_to_fit();
        al.reset_alloc_detected();
        it = v.emplace(v.begin(), 8);
        VERIFY(it == v.begin());
        CHECK(v, 8, tst3);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 13);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_emplace_needs_realloc_nothrow_move() { return test_emplace_needs_realloc<T>(); }
int test_emplace_needs_realloc_throwing_move() { return test_emplace_needs_realloc<T_ThrowingMove>(); }

}  // namespace

ADD_TEST_CASE("", "vector", test_emplace_to_empty);
ADD_TEST_CASE("", "vector", test_emplace_no_realloc);
ADD_TEST_CASE("", "vector", test_emplace_needs_realloc_nothrow_move);
ADD_TEST_CASE("", "vector", test_emplace_needs_realloc_throwing_move);
