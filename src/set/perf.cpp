#include "../rbtree_perf.h"

#include "uxs/pool_allocator.h"
#include "uxs/set.h"

#include <set>

using namespace uxs_test_suite;

namespace {

const int perf_N = 1500;

int test_perf_insert_T_std_alloc() { return perf_insert<uxs::set<T>>(3 * perf_N); }
int test_perf_insert_T_global_pool() {
    return perf_insert<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(3 * perf_N);
}
int test_perf_insert_T_pool() { return perf_insert<uxs::set<T, std::less<T>, uxs::pool_allocator<T>>>(3 * perf_N); }
int test_perf_insert_int_std_alloc() { return perf_insert<uxs::set<int>>(3 * perf_N); }
int test_perf_insert_int_global_pool() {
    return perf_insert<uxs::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(3 * perf_N);
}
int test_perf_insert_int_pool() {
    return perf_insert<uxs::set<int, std::less<int>, uxs::pool_allocator<int>>>(3 * perf_N);
}

int test_perf_insert_T_std_alloc_std() { return perf_insert<std::set<T>>(3 * perf_N); }
int test_perf_insert_T_global_pool_std() {
    return perf_insert<std::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(3 * perf_N);
}
int test_perf_insert_T_pool_std() { return perf_insert<std::set<T, std::less<T>, uxs::pool_allocator<T>>>(3 * perf_N); }
int test_perf_insert_int_std_alloc_std() { return perf_insert<std::set<int>>(3 * perf_N); }
int test_perf_insert_int_global_pool_std() {
    return perf_insert<std::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(3 * perf_N);
}
int test_perf_insert_int_pool_std() {
    return perf_insert<std::set<int, std::less<int>, uxs::pool_allocator<int>>>(3 * perf_N);
}

int test_perf_forward_T_std_alloc() { return perf_forward<uxs::set<T>>(100 * perf_N); }
int test_perf_forward_T_global_pool() {
    return perf_forward<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(100 * perf_N);
}
int test_perf_forward_T_pool() { return perf_forward<uxs::set<T, std::less<T>, uxs::pool_allocator<T>>>(100 * perf_N); }
int test_perf_forward_int_std_alloc() { return perf_forward<uxs::set<int>>(100 * perf_N); }
int test_perf_forward_int_global_pool() {
    return perf_forward<uxs::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(100 * perf_N);
}
int test_perf_forward_int_pool() {
    return perf_forward<uxs::set<int, std::less<int>, uxs::pool_allocator<int>>>(100 * perf_N);
}

int test_perf_forward_T_std_alloc_std() { return perf_forward<std::set<T>>(100 * perf_N); }
int test_perf_forward_T_global_pool_std() {
    return perf_forward<std::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(100 * perf_N);
}
int test_perf_forward_T_pool_std() {
    return perf_forward<std::set<T, std::less<T>, uxs::pool_allocator<T>>>(100 * perf_N);
}
int test_perf_forward_int_std_alloc_std() { return perf_forward<std::set<int>>(100 * perf_N); }
int test_perf_forward_int_global_pool_std() {
    return perf_forward<std::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(100 * perf_N);
}
int test_perf_forward_int_pool_std() {
    return perf_forward<std::set<int, std::less<int>, uxs::pool_allocator<int>>>(100 * perf_N);
}

int test_perf_backward_T_std_alloc() { return perf_backward<uxs::set<T>>(100 * perf_N); }
int test_perf_backward_T_global_pool() {
    return perf_backward<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(100 * perf_N);
}
int test_perf_backward_T_pool() {
    return perf_backward<uxs::set<T, std::less<T>, uxs::pool_allocator<T>>>(100 * perf_N);
}
int test_perf_backward_int_std_alloc() { return perf_backward<uxs::set<int>>(100 * perf_N); }
int test_perf_backward_int_global_pool() {
    return perf_backward<uxs::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(100 * perf_N);
}
int test_perf_backward_int_pool() {
    return perf_backward<uxs::set<int, std::less<int>, uxs::pool_allocator<int>>>(100 * perf_N);
}

int test_perf_backward_T_std_alloc_std() { return perf_backward<std::set<T>>(100 * perf_N); }
int test_perf_backward_T_global_pool_std() {
    return perf_backward<std::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(100 * perf_N);
}
int test_perf_backward_T_pool_std() {
    return perf_backward<std::set<T, std::less<T>, uxs::pool_allocator<T>>>(100 * perf_N);
}
int test_perf_backward_int_std_alloc_std() { return perf_backward<std::set<int>>(100 * perf_N); }
int test_perf_backward_int_global_pool_std() {
    return perf_backward<std::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(100 * perf_N);
}
int test_perf_backward_int_pool_std() {
    return perf_backward<std::set<int, std::less<int>, uxs::pool_allocator<int>>>(100 * perf_N);
}

int test_perf_find_T_std_alloc() { return perf_find<uxs::set<T>>(1000 * perf_N); }
int test_perf_find_T_global_pool() {
    return perf_find<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(1000 * perf_N);
}
int test_perf_find_T_pool() { return perf_find<uxs::set<T, std::less<T>, uxs::pool_allocator<T>>>(1000 * perf_N); }
int test_perf_find_int_std_alloc() { return perf_find<uxs::set<int>>(1000 * perf_N); }
int test_perf_find_int_global_pool() {
    return perf_find<uxs::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(1000 * perf_N);
}
int test_perf_find_int_pool() {
    return perf_find<uxs::set<int, std::less<int>, uxs::pool_allocator<int>>>(1000 * perf_N);
}

int test_perf_find_T_std_alloc_std() { return perf_find<std::set<T>>(1000 * perf_N); }
int test_perf_find_T_global_pool_std() {
    return perf_find<std::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(1000 * perf_N);
}
int test_perf_find_T_pool_std() { return perf_find<std::set<T, std::less<T>, uxs::pool_allocator<T>>>(1000 * perf_N); }
int test_perf_find_int_std_alloc_std() { return perf_find<std::set<int>>(1000 * perf_N); }
int test_perf_find_int_global_pool_std() {
    return perf_find<std::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(1000 * perf_N);
}
int test_perf_find_int_pool_std() {
    return perf_find<std::set<int, std::less<int>, uxs::pool_allocator<int>>>(1000 * perf_N);
}

int test_perf_T_std_alloc() { return perf_integral<uxs::set<T>>(perf_N); }
int test_perf_T_global_pool() {
    return perf_integral<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N);
}
int test_perf_T_pool() { return perf_integral<uxs::set<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N); }
int test_perf_int_std_alloc() { return perf_integral<uxs::set<int>>(perf_N); }
int test_perf_int_global_pool() {
    return perf_integral<uxs::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N);
}
int test_perf_int_pool() { return perf_integral<uxs::set<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N); }

int test_perf_T_std_alloc_std() { return perf_integral<std::set<T>>(perf_N); }
int test_perf_T_global_pool_std() {
    return perf_integral<std::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N);
}
int test_perf_T_pool_std() { return perf_integral<std::set<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N); }
int test_perf_int_std_alloc_std() { return perf_integral<std::set<int>>(perf_N); }
int test_perf_int_global_pool_std() {
    return perf_integral<std::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N);
}
int test_perf_int_pool_std() { return perf_integral<std::set<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N); }

}  // namespace

ADD_TEST_CASE("2-perf", "set:T: insert", test_perf_insert_T_std_alloc);
ADD_TEST_CASE("2-perf", "set:T:global_pool_allocator: insert", test_perf_insert_T_global_pool);
ADD_TEST_CASE("2-perf", "set:T:pool_allocator: insert", test_perf_insert_T_pool);
ADD_TEST_CASE("2-perf", "set:int: insert", test_perf_insert_int_std_alloc);
ADD_TEST_CASE("2-perf", "set:int:global_pool_allocator: insert", test_perf_insert_int_global_pool);
ADD_TEST_CASE("2-perf", "set:int:pool_allocator: insert", test_perf_insert_int_pool);
ADD_TEST_CASE("2-perf", "<std> set:T: insert", test_perf_insert_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> set:T:global_pool_allocator: insert", test_perf_insert_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> set:T:pool_allocator: insert", test_perf_insert_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> set:int: insert", test_perf_insert_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> set:int:global_pool_allocator: insert", test_perf_insert_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> set:int:pool_allocator: insert", test_perf_insert_int_pool_std);

ADD_TEST_CASE("2-perf", "set:T: forward", test_perf_forward_T_std_alloc);
ADD_TEST_CASE("2-perf", "set:T:global_pool_allocator: forward", test_perf_forward_T_global_pool);
ADD_TEST_CASE("2-perf", "set:T:pool_allocator: forward", test_perf_forward_T_pool);
ADD_TEST_CASE("2-perf", "set:int: forward", test_perf_forward_int_std_alloc);
ADD_TEST_CASE("2-perf", "set:int:global_pool_allocator: forward", test_perf_forward_int_global_pool);
ADD_TEST_CASE("2-perf", "set:int:pool_allocator: forward", test_perf_forward_int_pool);
ADD_TEST_CASE("2-perf", "<std> set:T: forward", test_perf_forward_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> set:T:global_pool_allocator: forward", test_perf_forward_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> set:T:pool_allocator: forward", test_perf_forward_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> set:int: forward", test_perf_forward_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> set:int:global_pool_allocator: forward", test_perf_forward_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> set:int:pool_allocator: forward", test_perf_forward_int_pool_std);

ADD_TEST_CASE("2-perf", "set:T: backward", test_perf_backward_T_std_alloc);
ADD_TEST_CASE("2-perf", "set:T:global_pool_allocator: backward", test_perf_backward_T_global_pool);
ADD_TEST_CASE("2-perf", "set:T:pool_allocator: backward", test_perf_backward_T_pool);
ADD_TEST_CASE("2-perf", "set:int: backward", test_perf_backward_int_std_alloc);
ADD_TEST_CASE("2-perf", "set:int:global_pool_allocator: backward", test_perf_backward_int_global_pool);
ADD_TEST_CASE("2-perf", "set:int:pool_allocator: backward", test_perf_backward_int_pool);
ADD_TEST_CASE("2-perf", "<std> set:T: backward", test_perf_backward_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> set:T:global_pool_allocator: backward", test_perf_backward_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> set:T:pool_allocator: backward", test_perf_backward_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> set:int: backward", test_perf_backward_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> set:int:global_pool_allocator: backward", test_perf_backward_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> set:int:pool_allocator: backward", test_perf_backward_int_pool_std);

ADD_TEST_CASE("2-perf", "set:T: find", test_perf_find_T_std_alloc);
ADD_TEST_CASE("2-perf", "set:T:global_pool_allocator: find", test_perf_find_T_global_pool);
ADD_TEST_CASE("2-perf", "set:T:pool_allocator: find", test_perf_find_T_pool);
ADD_TEST_CASE("2-perf", "set:int: find", test_perf_find_int_std_alloc);
ADD_TEST_CASE("2-perf", "set:int:global_pool_allocator: find", test_perf_find_int_global_pool);
ADD_TEST_CASE("2-perf", "set:int:pool_allocator: find", test_perf_find_int_pool);
ADD_TEST_CASE("2-perf", "<std> set:T: find", test_perf_find_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> set:T:global_pool_allocator: find", test_perf_find_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> set:T:pool_allocator: find", test_perf_find_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> set:int: find", test_perf_find_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> set:int:global_pool_allocator: find", test_perf_find_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> set:int:pool_allocator: find", test_perf_find_int_pool_std);

ADD_TEST_CASE("2-perf", "set:T", test_perf_T_std_alloc);
ADD_TEST_CASE("2-perf", "set:T:global_pool_allocator", test_perf_T_global_pool);
ADD_TEST_CASE("2-perf", "set:T:pool_allocator", test_perf_T_pool);
ADD_TEST_CASE("2-perf", "set:int", test_perf_int_std_alloc);
ADD_TEST_CASE("2-perf", "set:int:global_pool_allocator", test_perf_int_global_pool);
ADD_TEST_CASE("2-perf", "set:int:pool_allocator", test_perf_int_pool);
ADD_TEST_CASE("2-perf", "<std> set:T", test_perf_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> set:T:global_pool_allocator", test_perf_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> set:T:pool_allocator", test_perf_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> set:int", test_perf_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> set:int:global_pool_allocator", test_perf_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> set:int:pool_allocator", test_perf_int_pool_std);
