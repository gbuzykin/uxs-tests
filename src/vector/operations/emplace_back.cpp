#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_emplace_back_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {10};
        uxs::vector<Ty, test_allocator<Ty>> v(al);
        auto* p = &v.emplace_back(10);
        VERIFY(p == &v.back());
        CHECK(v, 1, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 1);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_emplace_back_no_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst[] = {1, 2, 3, 4, 5, 6};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        v.reserve(10);
        al.reset_alloc_detected();
        auto* p = &v.emplace_back(6);
        VERIFY(p == &v.back());
        CHECK(v, 6, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 11);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_emplace_back_needs_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst[] = {1, 2, 3, 4, 5, 6};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        v.shrink_to_fit();
        al.reset_alloc_detected();
        auto* p = &v.emplace_back(6);
        VERIFY(p == &v.back());
        CHECK(v, 6, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 11);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_emplace_back_needs_realloc_nothrow_move() { return test_emplace_back_needs_realloc<T>(); }
int test_emplace_back_needs_realloc_throwing_move() { return test_emplace_back_needs_realloc<T_ThrowingMove>(); }

}  // namespace

ADD_TEST_CASE("", "vector", test_emplace_back_to_empty);
ADD_TEST_CASE("", "vector", test_emplace_back_no_realloc);
ADD_TEST_CASE("", "vector", test_emplace_back_needs_realloc_nothrow_move);
ADD_TEST_CASE("", "vector", test_emplace_back_needs_realloc_throwing_move);
