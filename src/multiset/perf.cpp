#include "../rbtree_perf.h"

#include "uxs/multiset.h"
#include "uxs/pool_allocator.h"

#include <set>

using namespace uxs_test_suite;

namespace {

const int perf_N = 1500;

int test_perf_insert_T_std_alloc() { return perf_insert<uxs::multiset<T>>(3 * perf_N); }
int test_perf_insert_T_global_pool() {
    return perf_insert<uxs::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(3 * perf_N);
}
int test_perf_insert_T_pool() {
    return perf_insert<uxs::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(3 * perf_N);
}
int test_perf_insert_int_std_alloc() { return perf_insert<uxs::multiset<int>>(3 * perf_N); }
int test_perf_insert_int_global_pool() {
    return perf_insert<uxs::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(3 * perf_N);
}
int test_perf_insert_int_pool() {
    return perf_insert<uxs::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(3 * perf_N);
}

int test_perf_insert_T_std_alloc_std() { return perf_insert<std::multiset<T>>(3 * perf_N); }
int test_perf_insert_T_global_pool_std() {
    return perf_insert<std::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(3 * perf_N);
}
int test_perf_insert_T_pool_std() {
    return perf_insert<std::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(3 * perf_N);
}
int test_perf_insert_int_std_alloc_std() { return perf_insert<std::multiset<int>>(3 * perf_N); }
int test_perf_insert_int_global_pool_std() {
    return perf_insert<std::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(3 * perf_N);
}
int test_perf_insert_int_pool_std() {
    return perf_insert<std::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(3 * perf_N);
}

int test_perf_forward_T_std_alloc() { return perf_forward<uxs::multiset<T>>(100 * perf_N); }
int test_perf_forward_T_global_pool() {
    return perf_forward<uxs::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(100 * perf_N);
}
int test_perf_forward_T_pool() {
    return perf_forward<uxs::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(100 * perf_N);
}
int test_perf_forward_int_std_alloc() { return perf_forward<uxs::multiset<int>>(100 * perf_N); }
int test_perf_forward_int_global_pool() {
    return perf_forward<uxs::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(100 * perf_N);
}
int test_perf_forward_int_pool() {
    return perf_forward<uxs::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(100 * perf_N);
}

int test_perf_forward_T_std_alloc_std() { return perf_forward<std::multiset<T>>(100 * perf_N); }
int test_perf_forward_T_global_pool_std() {
    return perf_forward<std::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(100 * perf_N);
}
int test_perf_forward_T_pool_std() {
    return perf_forward<std::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(100 * perf_N);
}
int test_perf_forward_int_std_alloc_std() { return perf_forward<std::multiset<int>>(100 * perf_N); }
int test_perf_forward_int_global_pool_std() {
    return perf_forward<std::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(100 * perf_N);
}
int test_perf_forward_int_pool_std() {
    return perf_forward<std::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(100 * perf_N);
}

int test_perf_backward_T_std_alloc() { return perf_backward<uxs::multiset<T>>(100 * perf_N); }
int test_perf_backward_T_global_pool() {
    return perf_backward<uxs::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(100 * perf_N);
}
int test_perf_backward_T_pool() {
    return perf_backward<uxs::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(100 * perf_N);
}
int test_perf_backward_int_std_alloc() { return perf_backward<uxs::multiset<int>>(100 * perf_N); }
int test_perf_backward_int_global_pool() {
    return perf_backward<uxs::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(100 * perf_N);
}
int test_perf_backward_int_pool() {
    return perf_backward<uxs::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(100 * perf_N);
}

int test_perf_backward_T_std_alloc_std() { return perf_backward<std::multiset<T>>(100 * perf_N); }
int test_perf_backward_T_global_pool_std() {
    return perf_backward<std::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(100 * perf_N);
}
int test_perf_backward_T_pool_std() {
    return perf_backward<std::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(100 * perf_N);
}
int test_perf_backward_int_std_alloc_std() { return perf_backward<std::multiset<int>>(100 * perf_N); }
int test_perf_backward_int_global_pool_std() {
    return perf_backward<std::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(100 * perf_N);
}
int test_perf_backward_int_pool_std() {
    return perf_backward<std::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(100 * perf_N);
}

int test_perf_find_T_std_alloc() { return perf_find<uxs::multiset<T>>(1000 * perf_N); }
int test_perf_find_T_global_pool() {
    return perf_find<uxs::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(1000 * perf_N);
}
int test_perf_find_T_pool() { return perf_find<uxs::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(1000 * perf_N); }
int test_perf_find_int_std_alloc() { return perf_find<uxs::multiset<int>>(1000 * perf_N); }
int test_perf_find_int_global_pool() {
    return perf_find<uxs::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(1000 * perf_N);
}
int test_perf_find_int_pool() {
    return perf_find<uxs::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(1000 * perf_N);
}

int test_perf_find_T_std_alloc_std() { return perf_find<std::multiset<T>>(1000 * perf_N); }
int test_perf_find_T_global_pool_std() {
    return perf_find<std::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(1000 * perf_N);
}
int test_perf_find_T_pool_std() {
    return perf_find<std::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(1000 * perf_N);
}
int test_perf_find_int_std_alloc_std() { return perf_find<std::multiset<int>>(1000 * perf_N); }
int test_perf_find_int_global_pool_std() {
    return perf_find<std::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(1000 * perf_N);
}
int test_perf_find_int_pool_std() {
    return perf_find<std::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(1000 * perf_N);
}

int test_perf_T_std_alloc() { return perf_integral<uxs::multiset<T>>(perf_N); }
int test_perf_T_global_pool() {
    return perf_integral<uxs::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N);
}
int test_perf_T_pool() { return perf_integral<uxs::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N); }
int test_perf_int_std_alloc() { return perf_integral<uxs::multiset<int>>(perf_N); }
int test_perf_int_global_pool() {
    return perf_integral<uxs::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N);
}
int test_perf_int_pool() { return perf_integral<uxs::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N); }

int test_perf_T_std_alloc_std() { return perf_integral<std::multiset<T>>(perf_N); }
int test_perf_T_global_pool_std() {
    return perf_integral<std::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N);
}
int test_perf_T_pool_std() { return perf_integral<std::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N); }
int test_perf_int_std_alloc_std() { return perf_integral<std::multiset<int>>(perf_N); }
int test_perf_int_global_pool_std() {
    return perf_integral<std::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N);
}
int test_perf_int_pool_std() {
    return perf_integral<std::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N);
}

}  // namespace

ADD_TEST_CASE("2-perf", "multiset:T: insert", test_perf_insert_T_std_alloc);
ADD_TEST_CASE("2-perf", "multiset:T:global_pool_allocator: insert", test_perf_insert_T_global_pool);
ADD_TEST_CASE("2-perf", "multiset:T:pool_allocator: insert", test_perf_insert_T_pool);
ADD_TEST_CASE("2-perf", "multiset:int: insert", test_perf_insert_int_std_alloc);
ADD_TEST_CASE("2-perf", "multiset:int:global_pool_allocator: insert", test_perf_insert_int_global_pool);
ADD_TEST_CASE("2-perf", "multiset:int:pool_allocator: insert", test_perf_insert_int_pool);
ADD_TEST_CASE("2-perf", "<std> multiset:T: insert", test_perf_insert_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> multiset:T:global_pool_allocator: insert", test_perf_insert_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> multiset:T:pool_allocator: insert", test_perf_insert_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> multiset:int: insert", test_perf_insert_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> multiset:int:global_pool_allocator: insert", test_perf_insert_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> multiset:int:pool_allocator: insert", test_perf_insert_int_pool_std);

ADD_TEST_CASE("2-perf", "multiset:T: forward", test_perf_forward_T_std_alloc);
ADD_TEST_CASE("2-perf", "multiset:T:global_pool_allocator: forward", test_perf_forward_T_global_pool);
ADD_TEST_CASE("2-perf", "multiset:T:pool_allocator: forward", test_perf_forward_T_pool);
ADD_TEST_CASE("2-perf", "multiset:int: forward", test_perf_forward_int_std_alloc);
ADD_TEST_CASE("2-perf", "multiset:int:global_pool_allocator: forward", test_perf_forward_int_global_pool);
ADD_TEST_CASE("2-perf", "multiset:int:pool_allocator: forward", test_perf_forward_int_pool);
ADD_TEST_CASE("2-perf", "<std> multiset:T: forward", test_perf_forward_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> multiset:T:global_pool_allocator: forward", test_perf_forward_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> multiset:T:pool_allocator: forward", test_perf_forward_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> multiset:int: forward", test_perf_forward_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> multiset:int:global_pool_allocator: forward", test_perf_forward_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> multiset:int:pool_allocator: forward", test_perf_forward_int_pool_std);

ADD_TEST_CASE("2-perf", "multiset:T: backward", test_perf_backward_T_std_alloc);
ADD_TEST_CASE("2-perf", "multiset:T:global_pool_allocator: backward", test_perf_backward_T_global_pool);
ADD_TEST_CASE("2-perf", "multiset:T:pool_allocator: backward", test_perf_backward_T_pool);
ADD_TEST_CASE("2-perf", "multiset:int: backward", test_perf_backward_int_std_alloc);
ADD_TEST_CASE("2-perf", "multiset:int:global_pool_allocator: backward", test_perf_backward_int_global_pool);
ADD_TEST_CASE("2-perf", "multiset:int:pool_allocator: backward", test_perf_backward_int_pool);
ADD_TEST_CASE("2-perf", "<std> multiset:T: backward", test_perf_backward_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> multiset:T:global_pool_allocator: backward", test_perf_backward_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> multiset:T:pool_allocator: backward", test_perf_backward_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> multiset:int: backward", test_perf_backward_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> multiset:int:global_pool_allocator: backward", test_perf_backward_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> multiset:int:pool_allocator: backward", test_perf_backward_int_pool_std);

ADD_TEST_CASE("2-perf", "multiset:T: find", test_perf_find_T_std_alloc);
ADD_TEST_CASE("2-perf", "multiset:T:global_pool_allocator: find", test_perf_find_T_global_pool);
ADD_TEST_CASE("2-perf", "multiset:T:pool_allocator: find", test_perf_find_T_pool);
ADD_TEST_CASE("2-perf", "multiset:int: find", test_perf_find_int_std_alloc);
ADD_TEST_CASE("2-perf", "multiset:int:global_pool_allocator: find", test_perf_find_int_global_pool);
ADD_TEST_CASE("2-perf", "multiset:int:pool_allocator: find", test_perf_find_int_pool);
ADD_TEST_CASE("2-perf", "<std> multiset:T: find", test_perf_find_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> multiset:T:global_pool_allocator: find", test_perf_find_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> multiset:T:pool_allocator: find", test_perf_find_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> multiset:int: find", test_perf_find_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> multiset:int:global_pool_allocator: find", test_perf_find_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> multiset:int:pool_allocator: find", test_perf_find_int_pool_std);

ADD_TEST_CASE("2-perf", "multiset:T", test_perf_T_std_alloc);
ADD_TEST_CASE("2-perf", "multiset:T:global_pool_allocator", test_perf_T_global_pool);
ADD_TEST_CASE("2-perf", "multiset:T:pool_allocator", test_perf_T_pool);
ADD_TEST_CASE("2-perf", "multiset:int", test_perf_int_std_alloc);
ADD_TEST_CASE("2-perf", "multiset:int:global_pool_allocator", test_perf_int_global_pool);
ADD_TEST_CASE("2-perf", "multiset:int:pool_allocator", test_perf_int_pool);
ADD_TEST_CASE("2-perf", "<std> multiset:T", test_perf_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> multiset:T:global_pool_allocator", test_perf_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> multiset:T:pool_allocator", test_perf_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> multiset:int", test_perf_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> multiset:int:global_pool_allocator", test_perf_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> multiset:int:pool_allocator", test_perf_int_pool_std);
