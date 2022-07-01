#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_insert_empty() {
    test_allocator<void> al;

    {
        uxs::vector<Ty, test_allocator<Ty>> v(al);
        std::initializer_list<Ty> ins;
        std::initializer_list<Ty> ins2 = {1, 2, 3};
        // insert empty
        auto it = v.insert(v.end(), ins);
        VERIFY(it == v.end());
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(T::instance_count == 3);
        VERIFY(al.get_alloc_detected() == 0);
        // insert empty to not empty :
        it = v.insert(v.end(), ins2);
        VERIFY(it == v.begin());
        CHECK(v, ins2.size(), ins2.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 6);
        VERIFY(al.get_alloc_detected() == v.capacity());
        // back
        it = v.insert(v.end(), ins);
        VERIFY(it == v.end());
        CHECK(v, ins2.size(), ins2.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 6);
        VERIFY(al.get_alloc_detected() == v.capacity());
        // mid
        it = v.insert(v.begin() + 1, ins);
        VERIFY(it == v.begin() + 1);
        CHECK(v, ins2.size(), ins2.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 6);
        VERIFY(al.get_alloc_detected() == v.capacity());
        // front
        it = v.insert(v.begin(), ins);
        VERIFY(it == v.begin());
        CHECK(v, ins2.size(), ins2.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 6);
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
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4, 5, 6, 7, 8};
        int tst2[] = {1, 2, 3, 6, 7, 8, 4, 5, 6, 7, 8};
        int tst3[] = {6, 7, 8, 1, 2, 3, 6, 7, 8, 4, 5, 6, 7, 8};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        std::initializer_list<Ty> ins = {6, 7, 8};
        v.reserve(20);
        al.reset_alloc_detected();
        // back
        auto it = v.insert(v.end(), ins);
        VERIFY(it == v.end() - 3);
        CHECK(v, 8, tst1);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 16);
        VERIFY(al.get_alloc_detected() == 0);
        // mid
        it = v.insert(v.begin() + 3, ins);
        VERIFY(it == v.begin() + 3);
        CHECK(v, 11, tst2);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 19);
        VERIFY(al.get_alloc_detected() == 0);
        // front
        it = v.insert(v.begin(), ins);
        VERIFY(it == v.begin());
        CHECK(v, 14, tst3);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 22);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_insert_needs_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4, 5, 6, 7, 8};
        int tst2[] = {1, 2, 3, 6, 7, 8, 4, 5, 6, 7, 8};
        int tst3[] = {6, 7, 8, 1, 2, 3, 6, 7, 8, 4, 5, 6, 7, 8};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        std::initializer_list<Ty> ins = {6, 7, 8};
        // back
        v.shrink_to_fit();
        al.reset_alloc_detected();
        auto it = v.insert(v.end(), ins);
        VERIFY(it == v.end() - 3);
        CHECK(v, 8, tst1);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 16);
        VERIFY(al.get_alloc_detected() == v.capacity());
        // mid
        v.shrink_to_fit();
        al.reset_alloc_detected();
        it = v.insert(v.begin() + 3, ins);
        VERIFY(it == v.begin() + 3);
        CHECK(v, 11, tst2);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 19);
        VERIFY(al.get_alloc_detected() == v.capacity());
        // front
        v.shrink_to_fit();
        al.reset_alloc_detected();
        it = v.insert(v.begin(), ins);
        VERIFY(it == v.begin());
        CHECK(v, 14, tst3);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 22);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_insert_needs_realloc_nothrow_move() { return test_insert_needs_realloc<T>(); }
int test_insert_needs_realloc_throwing_move() { return test_insert_needs_realloc<T_ThrowingMove>(); }

}  // namespace

ADD_TEST_CASE("", "vector", test_insert_empty);
ADD_TEST_CASE("", "vector", test_insert_no_realloc);
ADD_TEST_CASE("", "vector", test_insert_needs_realloc_nothrow_move);
ADD_TEST_CASE("", "vector", test_insert_needs_realloc_throwing_move);
