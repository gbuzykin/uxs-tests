#include "../rbtree_perf.h"

#include <uxs-legacy/multiset.h>
#include <uxs-legacy/pool_allocator.h>

#include <set>

using namespace uxs_test_suite;

namespace {

const int perf_N_secs = 5;

ADD_TEST_CASE("2-perf <legacy>", "multiset:T: insert", []() { return perf_insert<uxs::multiset<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "multiset:T:global_pool_allocator: insert", ([]() {
                  return perf_insert<uxs::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "multiset:T:pool_allocator: insert",
              ([]() { return perf_insert<uxs::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf <legacy>", "multiset:int: insert", []() { return perf_insert<uxs::multiset<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "multiset:int:global_pool_allocator: insert", ([]() {
                  return perf_insert<uxs::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "multiset:int:pool_allocator: insert", ([]() {
                  return perf_insert<uxs::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:T: insert",
              []() { return perf_insert<std::multiset<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:T:global_pool_allocator: insert", ([]() {
                  return perf_insert<std::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:T:pool_allocator: insert",
              ([]() { return perf_insert<std::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:int: insert",
              []() { return perf_insert<std::multiset<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:int:global_pool_allocator: insert", ([]() {
                  return perf_insert<std::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:int:pool_allocator: insert", ([]() {
                  return perf_insert<std::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs);
              }));

ADD_TEST_CASE("2-perf <legacy>", "multiset:T: forward", []() { return perf_forward<uxs::multiset<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "multiset:T:global_pool_allocator: forward", ([]() {
                  return perf_forward<uxs::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "multiset:T:pool_allocator: forward",
              ([]() { return perf_forward<uxs::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf <legacy>", "multiset:int: forward",
              []() { return perf_forward<uxs::multiset<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "multiset:int:global_pool_allocator: forward", ([]() {
                  return perf_forward<uxs::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "multiset:int:pool_allocator: forward", ([]() {
                  return perf_forward<uxs::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:T: forward",
              []() { return perf_forward<std::multiset<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:T:global_pool_allocator: forward", ([]() {
                  return perf_forward<std::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:T:pool_allocator: forward",
              ([]() { return perf_forward<std::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:int: forward",
              []() { return perf_forward<std::multiset<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:int:global_pool_allocator: forward", ([]() {
                  return perf_forward<std::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:int:pool_allocator: forward", ([]() {
                  return perf_forward<std::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs);
              }));

ADD_TEST_CASE("2-perf <legacy>", "multiset:T: backward", []() { return perf_backward<uxs::multiset<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "multiset:T:global_pool_allocator: backward", ([]() {
                  return perf_backward<uxs::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "multiset:T:pool_allocator: backward",
              ([]() { return perf_backward<uxs::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf <legacy>", "multiset:int: backward",
              []() { return perf_backward<uxs::multiset<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "multiset:int:global_pool_allocator: backward", ([]() {
                  return perf_backward<uxs::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "multiset:int:pool_allocator: backward", ([]() {
                  return perf_backward<uxs::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:T: backward",
              []() { return perf_backward<std::multiset<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:T:global_pool_allocator: backward", ([]() {
                  return perf_backward<std::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:T:pool_allocator: backward",
              ([]() { return perf_backward<std::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:int: backward",
              []() { return perf_backward<std::multiset<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:int:global_pool_allocator: backward", ([]() {
                  return perf_backward<std::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:int:pool_allocator: backward", ([]() {
                  return perf_backward<std::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs);
              }));

ADD_TEST_CASE("2-perf <legacy>", "multiset:T: find", []() { return perf_find<uxs::multiset<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "multiset:T:global_pool_allocator: find",
              ([]() { return perf_find<uxs::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf <legacy>", "multiset:T:pool_allocator: find",
              ([]() { return perf_find<uxs::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf <legacy>", "multiset:int: find", []() { return perf_find<uxs::multiset<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "multiset:int:global_pool_allocator: find", ([]() {
                  return perf_find<uxs::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "multiset:int:pool_allocator: find",
              ([]() { return perf_find<uxs::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:T: find", []() { return perf_find<std::multiset<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:T:global_pool_allocator: find",
              ([]() { return perf_find<std::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:T:pool_allocator: find",
              ([]() { return perf_find<std::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:int: find",
              []() { return perf_find<std::multiset<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:int:global_pool_allocator: find", ([]() {
                  return perf_find<std::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:int:pool_allocator: find",
              ([]() { return perf_find<std::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs); }));

ADD_TEST_CASE("2-perf <legacy>", "multiset:T", []() { return perf_integral<uxs::multiset<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "multiset:T:global_pool_allocator", ([]() {
                  return perf_integral<uxs::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "multiset:T:pool_allocator",
              ([]() { return perf_integral<uxs::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf <legacy>", "multiset:int", []() { return perf_integral<uxs::multiset<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "multiset:int:global_pool_allocator", ([]() {
                  return perf_integral<uxs::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "multiset:int:pool_allocator", ([]() {
                  return perf_integral<uxs::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:T", []() { return perf_integral<std::multiset<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:T:global_pool_allocator", ([]() {
                  return perf_integral<std::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:T:pool_allocator",
              ([]() { return perf_integral<std::multiset<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:int", []() { return perf_integral<std::multiset<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:int:global_pool_allocator", ([]() {
                  return perf_integral<std::multiset<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N_secs);
              }));
ADD_TEST_CASE("2-perf <legacy>", "<STL> multiset:int:pool_allocator", ([]() {
                  return perf_integral<std::multiset<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N_secs);
              }));

}  // namespace
