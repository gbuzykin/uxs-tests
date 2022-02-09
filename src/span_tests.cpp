#include "test_suite.h"
#include "util/span.h"
#include "util/vector.h"

namespace {

int test_span_1() {
    int* p = nullptr;
    const int* cp = nullptr;
    util::span<int> s(p, 10);
    util::span<const int> s2(p, 10);
    util::span<const int> s3(cp, 10);
    // util::span<int> s4(cp, 10);

    util::vector<int> v;
    util::span<int> s5(v);
    util::span<const int> s6(v);
    util::span<const int> s7(std::as_const(v));
    // util::span<int> s8(std::as_const(v));

    util::span<const int> s9(s5);
    // util::span<int> s10(s6);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "span", test_span_1);
