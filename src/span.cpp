#include "uxs/span.h"

#include "test_suite.h"

#include "uxs/vector.h"

namespace {

int test_span_1() {
    int* p = nullptr;
    const int* cp = nullptr;
    uxs::span<int> s(p, 10);
    uxs::span<const int> s2(p, 10);
    uxs::span<const int> s3(cp, 10);
    // uxs::span<int> s4(cp, 10);

    uxs::vector<int> v;
    uxs::span<int> s5(v);
    uxs::span<const int> s6(v);
    uxs::span<const int> s7(std::as_const(v));
    // uxs::span<int> s8(std::as_const(v));

    uxs::span<const int> s9(s5);
    // uxs::span<int> s10(s6);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "span", test_span_1);
