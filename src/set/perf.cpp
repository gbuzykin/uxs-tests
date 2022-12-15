#include "../rbtree_perf.h"

#include "uxs/pool_allocator.h"
#include "uxs/set.h"

#include <set>

using namespace uxs_test_suite;

namespace {

const int perf_N_secs = 5;

ADD_TEST_CASE("2-perf", "set:T: insert", []() { return perf_insert<uxs::set<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "set:T:global_pool_allocator: insert",
              ([]() { return perf_insert<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "set:T:pool_allocator: insert",
              ([]() { return perf_insert<uxs::set<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "set:int: insert", []() { return perf_insert<uxs::set<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "set:int:global_pool_allocator: insert", ([]() {
                  return perf_insert<uxs::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "set:int:pool_allocator: insert",
              ([]() { return perf_insert<uxs::set<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> set:T: insert", []() { return perf_insert<std::set<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "<STL> set:T:global_pool_allocator: insert",
              ([]() { return perf_insert<std::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> set:T:pool_allocator: insert",
              ([]() { return perf_insert<std::set<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> set:int: insert", []() { return perf_insert<std::set<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "<STL> set:int:global_pool_allocator: insert", ([]() {
                  return perf_insert<std::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<STL> set:int:pool_allocator: insert",
              ([]() { return perf_insert<std::set<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs); }));

ADD_TEST_CASE("2-perf", "set:T: forward", []() { return perf_forward<uxs::set<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "set:T:global_pool_allocator: forward",
              ([]() { return perf_forward<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "set:T:pool_allocator: forward",
              ([]() { return perf_forward<uxs::set<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "set:int: forward", []() { return perf_forward<uxs::set<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "set:int:global_pool_allocator: forward", ([]() {
                  return perf_forward<uxs::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "set:int:pool_allocator: forward",
              ([]() { return perf_forward<uxs::set<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> set:T: forward", []() { return perf_forward<std::set<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "<STL> set:T:global_pool_allocator: forward",
              ([]() { return perf_forward<std::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> set:T:pool_allocator: forward",
              ([]() { return perf_forward<std::set<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> set:int: forward", []() { return perf_forward<std::set<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "<STL> set:int:global_pool_allocator: forward", ([]() {
                  return perf_forward<std::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<STL> set:int:pool_allocator: forward",
              ([]() { return perf_forward<std::set<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs); }));

ADD_TEST_CASE("2-perf", "set:T: backward", []() { return perf_backward<uxs::set<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "set:T:global_pool_allocator: backward",
              ([]() { return perf_backward<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "set:T:pool_allocator: backward",
              ([]() { return perf_backward<uxs::set<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "set:int: backward", []() { return perf_backward<uxs::set<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "set:int:global_pool_allocator: backward", ([]() {
                  return perf_backward<uxs::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "set:int:pool_allocator: backward",
              ([]() { return perf_backward<uxs::set<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> set:T: backward", []() { return perf_backward<std::set<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "<STL> set:T:global_pool_allocator: backward",
              ([]() { return perf_backward<std::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> set:T:pool_allocator: backward",
              ([]() { return perf_backward<std::set<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> set:int: backward", []() { return perf_backward<std::set<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "<STL> set:int:global_pool_allocator: backward", ([]() {
                  return perf_backward<std::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<STL> set:int:pool_allocator: backward",
              ([]() { return perf_backward<std::set<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs); }));

ADD_TEST_CASE("2-perf", "set:T: find", []() { return perf_find<uxs::set<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "set:T:global_pool_allocator: find",
              ([]() { return perf_find<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "set:T:pool_allocator: find",
              ([]() { return perf_find<uxs::set<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "set:int: find", []() { return perf_find<uxs::set<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "set:int:global_pool_allocator: find", ([]() {
                  return perf_find<uxs::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "set:int:pool_allocator: find",
              ([]() { return perf_find<uxs::set<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> set:T: find", []() { return perf_find<std::set<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "<STL> set:T:global_pool_allocator: find",
              ([]() { return perf_find<std::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> set:T:pool_allocator: find",
              ([]() { return perf_find<std::set<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> set:int: find", []() { return perf_find<std::set<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "<STL> set:int:global_pool_allocator: find", ([]() {
                  return perf_find<std::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<STL> set:int:pool_allocator: find",
              ([]() { return perf_find<std::set<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs); }));

ADD_TEST_CASE("2-perf", "set:T", []() { return perf_integral<uxs::set<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "set:T:global_pool_allocator",
              ([]() { return perf_integral<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "set:T:pool_allocator",
              ([]() { return perf_integral<uxs::set<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "set:int", []() { return perf_integral<uxs::set<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "set:int:global_pool_allocator", ([]() {
                  return perf_integral<uxs::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "set:int:pool_allocator",
              ([]() { return perf_integral<uxs::set<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> set:T", []() { return perf_integral<std::set<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "<STL> set:T:global_pool_allocator",
              ([]() { return perf_integral<std::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> set:T:pool_allocator",
              ([]() { return perf_integral<std::set<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> set:int", []() { return perf_integral<std::set<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "<STL> set:int:global_pool_allocator", ([]() {
                  return perf_integral<std::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<STL> set:int:pool_allocator",
              ([]() { return perf_integral<std::set<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs); }));

}  // namespace
