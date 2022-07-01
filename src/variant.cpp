#include "uxs/variant.h"

#include "math.h"
#include "test_suite.h"

namespace {

int test_variant_0() {
    {
        uxs::variant v;
        VERIFY(v.valid() == false && v.type() == uxs::variant_id::kInvalid);

        uxs::variant v2(v);
        VERIFY(v2.valid() == false && v2.type() == uxs::variant_id::kInvalid);

        uxs::variant v3(std::move(v));
        VERIFY(v3.valid() == false && v3.type() == uxs::variant_id::kInvalid);
        VERIFY(v.valid() == false && v.type() == uxs::variant_id::kInvalid);

        uxs::variant v4(uxs::variant_id::kString, v2);
        VERIFY(v4.valid() == true && v4.type() == uxs::variant_id::kString && v4 == "");
        v4 = "1234567890123456789012345678901234567890123456789012345678901234567890";
        VERIFY(v4.valid() == true && v4.type() == uxs::variant_id::kString &&
               v4 == "1234567890123456789012345678901234567890123456789012345678901234567890");

        uxs::variant v5(uxs::variant_id::kInvalid, v4);
        VERIFY(v5.valid() == false && v5.type() == uxs::variant_id::kInvalid);
    }

    {
        uxs::variant v(uxs::variant_id::kString);
        v = "1234567890123456789012345678901234567890123456789012345678901234567890";
        VERIFY(v.valid() == true && v.type() == uxs::variant_id::kString &&
               v == "1234567890123456789012345678901234567890123456789012345678901234567890");

        uxs::variant v2(v);
        VERIFY(v2.valid() == true && v2.type() == uxs::variant_id::kString &&
               v2 == "1234567890123456789012345678901234567890123456789012345678901234567890");

        uxs::variant v3(std::move(v));
        VERIFY(v3.valid() == true && v3.type() == uxs::variant_id::kString &&
               v3 == "1234567890123456789012345678901234567890123456789012345678901234567890");
    }

    {
        std::string s("1234567890123456789012345678901234567890123456789012345678901234567890");

        uxs::variant v(s);
        VERIFY(v.valid() == true && v.type() == uxs::variant_id::kString &&
               v == "1234567890123456789012345678901234567890123456789012345678901234567890");

        uxs::variant v2(std::move(s));
        VERIFY(v2.valid() == true && v2.type() == uxs::variant_id::kString &&
               v2 == "1234567890123456789012345678901234567890123456789012345678901234567890");

        uxs::variant v3(uxs::variant_id::kString, 3.123456789);
        VERIFY(v3.valid() == true && v3.type() == uxs::variant_id::kString && v3 == "3.123456789");
    }

    uxs::variant v;
    std::string s1;
    std::string_view s2;
    bool r = v == v || v == s1 || s1 == v || v == s2 || s2 == v;
    (void)r;

    return 0;
}

int test_variant_1() {
    uxs::variant v(std::string{"1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1"});

    VERIFY(v.can_convert(uxs::variant_id::kMatrix4x4) == true);
    v.convert(uxs::variant_id::kMatrix4x4);
    VERIFY(v.value<vrc::math::mat4>() == vrc::math::mat4());

    VERIFY(v.can_convert(uxs::variant_id::kString) == true);
    v.convert(uxs::variant_id::kString);
    VERIFY(v.value<std::string>() == "1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1");

    uxs::variant v1(-234.57);

    VERIFY(v1.value<int>() == -234);
    VERIFY(v1.value<double>() == -234.57);
    VERIFY(v1.value<std::string>() == "-234.57");

    v1.convert(uxs::variant_id::kString);

    auto v2 = std::move(v1);
    VERIFY(v2.value<std::string>() == "-234.57");

    auto v3 = v2;
    VERIFY(v3.value<std::string>() == "-234.57");

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "variant", test_variant_0);
ADD_TEST_CASE("", "variant", test_variant_1);
