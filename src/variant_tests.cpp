#include "math.h"
#include "test_suite.h"
#include "util/variant.h"

namespace {

int test_variant_0() {
    {
        util::variant v;
        VERIFY(v.valid() == false && v.type() == util::variant_id::kInvalid);

        util::variant v2(v);
        VERIFY(v2.valid() == false && v2.type() == util::variant_id::kInvalid);

        util::variant v3(std::move(v));
        VERIFY(v3.valid() == false && v3.type() == util::variant_id::kInvalid);
        VERIFY(v.valid() == false && v.type() == util::variant_id::kInvalid);

        util::variant v4(util::variant_id::kString, v2);
        VERIFY(v4.valid() == true && v4.type() == util::variant_id::kString && v4 == "");
        v4 = "1234567890123456789012345678901234567890123456789012345678901234567890";
        VERIFY(v4.valid() == true && v4.type() == util::variant_id::kString &&
               v4 == "1234567890123456789012345678901234567890123456789012345678901234567890");

        util::variant v5(util::variant_id::kInvalid, v4);
        VERIFY(v5.valid() == false && v5.type() == util::variant_id::kInvalid);
    }

    {
        util::variant v(util::variant_id::kString);
        v = "1234567890123456789012345678901234567890123456789012345678901234567890";
        VERIFY(v.valid() == true && v.type() == util::variant_id::kString &&
               v == "1234567890123456789012345678901234567890123456789012345678901234567890");

        util::variant v2(v);
        VERIFY(v2.valid() == true && v2.type() == util::variant_id::kString &&
               v2 == "1234567890123456789012345678901234567890123456789012345678901234567890");

        util::variant v3(std::move(v));
        VERIFY(v3.valid() == true && v3.type() == util::variant_id::kString &&
               v3 == "1234567890123456789012345678901234567890123456789012345678901234567890");
    }

    {
        std::string s("1234567890123456789012345678901234567890123456789012345678901234567890");

        util::variant v(s);
        VERIFY(v.valid() == true && v.type() == util::variant_id::kString &&
               v == "1234567890123456789012345678901234567890123456789012345678901234567890");

        util::variant v2(std::move(s));
        VERIFY(v2.valid() == true && v2.type() == util::variant_id::kString &&
               v2 == "1234567890123456789012345678901234567890123456789012345678901234567890");

        util::variant v3(util::variant_id::kString, 3.123456789);
        VERIFY(v3.valid() == true && v3.type() == util::variant_id::kString && v3 == "3.12346");
    }

    util::variant v;
    std::string s1;
    std::string_view s2;
    bool r = v == v || v == s1 || s1 == v || v == s2 || s2 == v;
    (void)r;

    return 0;
}

int test_variant_1() {
    util::variant v(std::string{"1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1"});

    VERIFY(v.can_convert(util::variant_id::kMatrix4x4) == true);
    v.convert(util::variant_id::kMatrix4x4);
    VERIFY(v.value<vrc::math::mat4>() == vrc::math::mat4());

    VERIFY(v.can_convert(util::variant_id::kString) == true);
    v.convert(util::variant_id::kString);
    VERIFY(v.value<std::string>() == "1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1");

    util::variant v1(-234.57);

    VERIFY(v1.value<int>() == -234);
    VERIFY(v1.value<unsigned>() == std::numeric_limits<unsigned>::max() - 233u);
    VERIFY(v1.value<double>() == -234.57);
    VERIFY(v1.value<std::string>() == "-234.57");

    v1.convert(util::variant_id::kString);

    auto v2 = std::move(v1);
    VERIFY(v2.value<std::string>() == "-234.57");

    auto v3 = v2;
    VERIFY(v3.value<std::string>() == "-234.57");

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "variant", test_variant_0);
ADD_TEST_CASE("", "variant", test_variant_1);
