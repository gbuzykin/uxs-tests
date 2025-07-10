#include "test_suite.h"

#include <uxs/span.h>

#include <uxs-legacy/vector.h>

namespace {

int test_span_1() {
    int* p = nullptr;
    const int* cp = nullptr;
    est::span<int> s(p, 10);
    est::span<const int> s2(p, 10);
    est::span<const int> s3(cp, 10);
    // est::span<int> s4(cp, 10);

    uxs::vector<int> v;
    est::span<int> s5(v);
    est::span<const int> s6(v);
    est::span<const int> s7(std::as_const(v));
    // est::span<int> s8(std::as_const(v));

    est::span<const int> s9(s5);
    // est::span<int> s10(s6);

    est::span<int> s11(s5);
    s11 = s5;
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "span", test_span_1);
