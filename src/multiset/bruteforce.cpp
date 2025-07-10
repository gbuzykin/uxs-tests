#include "../rbtree_bruteforce.h"

#include <uxs-legacy/pool_allocator.h>

using namespace uxs_test_suite;

namespace {

#if defined(NDEBUG)
static const int brute_N = 50000;
#else   // defined(NDEBUG)
static const int brute_N = 500;
#endif  // defined(NDEBUG)

ADD_TEST_CASE("1-bruteforce", "multiset", ([]() {
                  rbtree_bruteforce_test<uxs::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(brute_N);
                  return 0;
              }));

}  // namespace
