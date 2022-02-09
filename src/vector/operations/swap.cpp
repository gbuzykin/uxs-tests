#include "vector_tests.h"

using namespace util_test_suite;

namespace {

int test_swap() {
    test_allocator<void> al1, al2;
    util::vector<T, test_allocator<T>> v1(al1), v2(al2);

    v1.swap(v2);
    CHECK_EMPTY(v1);
    VERIFY(v1.capacity() == 0);
    VERIFY(v1.get_allocator() == al2);
    CHECK_EMPTY(v2);
    VERIFY(v2.capacity() == 0);
    VERIFY(v2.get_allocator() == al1);

    std::initializer_list<T> tst1 = {1, 2, 3, 4, 5};
    v1.assign(tst1);
    v1.swap(v2);
    CHECK_EMPTY(v1);
    VERIFY(v1.capacity() == 0);
    VERIFY(v1.get_allocator() == al1);
    CHECK(v2, tst1.size(), tst1.begin());
    VERIFY(v2.capacity() >= v2.size());
    VERIFY(v2.get_allocator() == al2);

    v1.swap(v2);
    CHECK(v1, tst1.size(), tst1.begin());
    VERIFY(v1.capacity() >= v1.size());
    VERIFY(v1.get_allocator() == al2);
    CHECK_EMPTY(v2);
    VERIFY(v2.capacity() == 0);
    VERIFY(v2.get_allocator() == al1);

    std::initializer_list<T> tst2 = {10, 11, 12, 13, 14, 15, 16, 17};
    v2.assign(tst2);
    v1.swap(v2);
    CHECK(v1, tst2.size(), tst2.begin());
    VERIFY(v1.capacity() >= v1.size());
    VERIFY(v1.get_allocator() == al1);
    CHECK(v2, tst1.size(), tst1.begin());
    VERIFY(v2.capacity() >= v2.size());
    VERIFY(v2.get_allocator() == al2);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test_swap);
