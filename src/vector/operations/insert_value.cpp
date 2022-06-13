#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_insert_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {10, 10, 10};
        uxs::vector<Ty, test_allocator<Ty>> v(al);
        //  insert empty
        auto it = v.insert(v.end(), 0, 20);
        VERIFY(it == v.end());
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
        // not empty
        it = v.insert(v.end(), 3, 10);
        VERIFY(it == v.end() - 3);
        CHECK(v, 3, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 3);
        VERIFY(al.get_alloc_detected() == v.capacity());
        //  insert empty back
        it = v.insert(v.end(), 0, 20);
        VERIFY(it == v.end());
        CHECK(v, 3, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 3);
        VERIFY(al.get_alloc_detected() == v.capacity());
        //  insert empty mid
        it = v.insert(v.begin() + 3, 0, 20);
        VERIFY(it == v.begin() + 3);
        CHECK(v, 3, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 3);
        VERIFY(al.get_alloc_detected() == v.capacity());
        //  insert empty front
        it = v.insert(v.begin(), 0, 20);
        VERIFY(it == v.begin());
        CHECK(v, 3, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 3);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_insert_no_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4, 5, 10, 10, 10};
        int tst2[] = {1, 2, 3, 20, 20, 20, 4, 5, 10, 10, 10};
        int tst3[] = {30, 30, 30, 1, 2, 3, 20, 20, 20, 4, 5, 10, 10, 10};
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.reserve(20);
        al.reset_alloc_detected();
        // back
        auto it = v.insert(v.end(), 3, 10);
        VERIFY(it == v.end() - 3);
        CHECK(v, 8, tst1);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 13);
        VERIFY(al.get_alloc_detected() == 0);
        // mid
        it = v.insert(v.begin() + 3, 3, 20);
        VERIFY(it == v.begin() + 3);
        CHECK(v, 11, tst2);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 16);
        VERIFY(al.get_alloc_detected() == 0);
        // front
        it = v.insert(v.begin(), 3, 30);
        VERIFY(it == v.begin());
        CHECK(v, 14, tst3);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 19);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_insert_needs_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4, 5, 10, 10, 10};
        int tst2[] = {1, 2, 3, 20, 20, 20, 4, 5, 10, 10, 10};
        int tst3[] = {30, 30, 30, 1, 2, 3, 20, 20, 20, 4, 5, 10, 10, 10};
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        // back
        v.shrink_to_fit();
        al.reset_alloc_detected();
        auto it = v.insert(v.end(), 3, 10);
        VERIFY(it == v.end() - 3);
        CHECK(v, 8, tst1);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 13);
        VERIFY(al.get_alloc_detected() == v.capacity());
        // mid
        v.shrink_to_fit();
        al.reset_alloc_detected();
        it = v.insert(v.begin() + 3, 3, 20);
        VERIFY(it == v.begin() + 3);
        CHECK(v, 11, tst2);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 16);
        VERIFY(al.get_alloc_detected() == v.capacity());
        // front
        v.shrink_to_fit();
        al.reset_alloc_detected();
        it = v.insert(v.begin(), 3, 30);
        VERIFY(it == v.begin());
        CHECK(v, 14, tst3);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 19);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_insert_needs_realloc_nothrow_move() { return test_insert_needs_realloc<T>(); }
int test_insert_needs_realloc_throwing_move() { return test_insert_needs_realloc<T_ThrowingMove>(); }

}  // namespace

ADD_TEST_CASE("", "vector", test_insert_to_empty);
ADD_TEST_CASE("", "vector", test_insert_no_realloc);
ADD_TEST_CASE("", "vector", test_insert_needs_realloc_nothrow_move);
ADD_TEST_CASE("", "vector", test_insert_needs_realloc_throwing_move);
