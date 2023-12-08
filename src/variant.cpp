#include "uxs/variant.h"

#include "math.h"
#include "test_suite.h"
#include "test_types.h"

#define MUST_THROW(x) \
    try { \
        x; \
        VERIFY(false); \
    } catch (const uxs::variant_error&) {}

using namespace uxs_test_suite;

const uxs::variant_id test_variant_id = uxs::variant_id::custom0;
const uxs::variant_id test_dyn_variant_id = static_cast<uxs::variant_id>(
    static_cast<unsigned>(uxs::variant_id::custom0) + 1);

namespace uxs {
UXS_DECLARE_VARIANT_TYPE(T, test_variant_id);
UXS_DECLARE_VARIANT_TYPE(T_ThrowingMove, test_dyn_variant_id);
}  // namespace uxs

bool uxs::variant_type_impl<T>::convert_from(variant_id type, void* to, const void* from) {
    if (type != variant_id::string) { return false; }
    static_cast<T*>(to)->set(uxs::from_string<int>(*static_cast<const std::string*>(from)));
    return true;
}

bool uxs::variant_type_impl<T>::convert_to(variant_id type, void* to, const void* from) {
    if (type != variant_id::string) { return false; }
    *static_cast<std::string*>(to) = uxs::to_string(static_cast<int>(*static_cast<const T*>(from)));
    return true;
}

bool uxs::variant_type_impl<T_ThrowingMove>::convert_from(variant_id type, void* to, const void* from) {
    if (type != variant_id::string) { return false; }
    static_cast<T_ThrowingMove*>(to)->set(uxs::from_string<int>(*static_cast<const std::string*>(from)));
    return true;
}

bool uxs::variant_type_impl<T_ThrowingMove>::convert_to(variant_id type, void* to, const void* from) {
    if (type != variant_id::string) { return false; }
    *static_cast<std::string*>(to) = uxs::to_string(static_cast<int>(*static_cast<const T_ThrowingMove*>(from)));
    return true;
}

UXS_IMPLEMENT_VARIANT_TYPE(T, convert_from, convert_to);
UXS_IMPLEMENT_VARIANT_TYPE(T_ThrowingMove, convert_from, convert_to);

namespace {

int test_variant_0() {  // default constructor
    uxs::variant v;
    VERIFY(!v.has_value() && v.type() == uxs::variant_id::invalid);
    return 0;
}

int test_variant_1() {  // default value constructor

    {  // placement
        uxs::variant v(test_variant_id);
        VERIFY(v.has_value() && v.type() == test_variant_id && v.as<T>().empty());
    }

    {  // dynamic alloc
        uxs::variant v(test_dyn_variant_id);
        VERIFY(v.has_value() && v.type() == test_dyn_variant_id && v.as<T_ThrowingMove>().empty());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(T_ThrowingMove::instance_count == 0);
    return 0;
}

int test_variant_2() {  // copy constructor
    {
        uxs::variant v_from;
        uxs::variant v(v_from);
        VERIFY(!v.has_value() && v.type() == uxs::variant_id::invalid);
    }

    {  // placement
        uxs::variant v_from(uxs::in_place_type<T>(), 100);
        uxs::variant v(v_from);
        VERIFY(v.has_value() && v.type() == test_variant_id && v.as<T>() == 100);
        VERIFY(T::not_empty_count == 2);
    }

    {  // dynamic alloc (COW pointer)
        uxs::variant v_from(uxs::in_place_type<T_ThrowingMove>(), 100);
        uxs::variant v(v_from);
        VERIFY(v.has_value() && v.type() == test_dyn_variant_id && v.as<T_ThrowingMove>() == 100);
        VERIFY(v.as<const T_ThrowingMove&>() == 100);
        VERIFY(T::not_empty_count == 1);
        v.as<T_ThrowingMove&>().set(200);
        VERIFY(T::not_empty_count == 2);
        T_ThrowingMove v1 = v.as<T_ThrowingMove&&>();
        VERIFY(T::not_empty_count == 2);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(T_ThrowingMove::instance_count == 0);
    return 0;
}

int test_variant_3() {  // move constructor
    {
        uxs::variant v_from;
        uxs::variant v(std::move(v_from));
        VERIFY(!v.has_value() && v.type() == uxs::variant_id::invalid);
    }

    {  // placement
        uxs::variant v_from(uxs::in_place_type<T>(), 100);
        uxs::variant v(std::move(v_from));
        VERIFY(v.has_value() && v.type() == test_variant_id && v.as<T>() == 100);
        VERIFY(T::not_empty_count == 1);
    }

    {  // dynamic alloc
        uxs::variant v_from(uxs::in_place_type<T_ThrowingMove>(), 100);
        uxs::variant v(std::move(v_from));
        VERIFY(v.has_value() && v.type() == test_dyn_variant_id && v.as<T_ThrowingMove>() == 100);
        VERIFY(T::not_empty_count == 1);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(T_ThrowingMove::instance_count == 0);
    return 0;
}

int test_variant_4() {  // copy with conversion
    {
        uxs::variant v_from;
        uxs::variant v(test_variant_id, v_from);
        VERIFY(v.has_value() && v.type() == test_variant_id && v.as<T>().empty());
    }

    {  // placement
        uxs::variant v_from(std::string("100")), v_from2(uxs::in_place_type<T>(), 200);
        uxs::variant v(test_variant_id, v_from);
        VERIFY(v.has_value() && v.type() == test_variant_id && v.as<T>() == 100);
        VERIFY(T::not_empty_count == 2);

        uxs::variant v2(test_variant_id, v_from2);
        VERIFY(v2.has_value() && v2.type() == test_variant_id && v2.as<T>() == 200);
        VERIFY(T::not_empty_count == 3);
    }

    {  // dynamic alloc
        uxs::variant v_from(std::string("100")), v_from2(uxs::in_place_type<T_ThrowingMove>(), 200);
        uxs::variant v(test_dyn_variant_id, v_from);
        VERIFY(v.has_value() && v.type() == test_dyn_variant_id && v.as<T_ThrowingMove>() == 100);
        VERIFY(T::not_empty_count == 2);

        uxs::variant v2(test_dyn_variant_id, v_from2);
        VERIFY(v2.has_value() && v2.type() == test_dyn_variant_id && v2.as<T_ThrowingMove>() == 200);
        VERIFY(T::not_empty_count == 2);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(T_ThrowingMove::instance_count == 0);
    return 0;
}

int test_variant_5() {  // copy assignment
    {
        uxs::variant v_from;
        uxs::variant v;
        v = v_from;  // empty <- empty
        VERIFY(!v.has_value() && v.type() == uxs::variant_id::invalid);
    }

    {  // placement
        uxs::variant v_from(uxs::in_place_type<T>(), 100), v_from2(uxs::in_place_type<T>(), 200);
        uxs::variant v;
        v = v_from;  // empty <- not empty
        VERIFY(v.has_value() && v.type() == test_variant_id && v.as<T>() == 100);
        VERIFY(T::not_empty_count == 3);

        uxs::variant v2(std::string("long enough for memory allocation"));
        v2 = v;  // not empty <- not empty (reconstruction)
        VERIFY(v2.has_value() && v2.type() == test_variant_id && v2.as<T>() == 100);
        VERIFY(T::not_empty_count == 4);

        v2 = v_from2;  // not empty <- not empty (assignment)
        VERIFY(v2.has_value() && v2.type() == test_variant_id && v2.as<T>() == 200);
        VERIFY(T::not_empty_count == 4);

        uxs::variant v_empty;
        v2 = v_empty;  // not empty <- empty
        VERIFY(T::not_empty_count == 3);
    }

    {  // dynamic alloc (COW pointer)
        uxs::variant v_from(uxs::in_place_type<T_ThrowingMove>(), 100),
            v_from2(uxs::in_place_type<T_ThrowingMove>(), 200);
        uxs::variant v;
        v = v_from;  // empty <- not empty
        VERIFY(v.has_value() && v.type() == test_dyn_variant_id && v.as<T_ThrowingMove>() == 100);
        VERIFY(T::not_empty_count == 2);

        uxs::variant v2(std::string("long enough for memory allocation"));
        v2 = v;  // not empty <- not empty (reconstruction)
        VERIFY(v2.has_value() && v2.type() == test_dyn_variant_id && v2.as<T_ThrowingMove>() == 100);
        VERIFY(T::not_empty_count == 2);

        v2 = v_from2;  // not empty <- not empty (assignment)
        VERIFY(v2.has_value() && v2.type() == test_dyn_variant_id && v2.as<T_ThrowingMove>() == 200);
        VERIFY(T::not_empty_count == 2);

        uxs::variant v_empty;
        v2 = v_empty;  // not empty <- empty
        VERIFY(T::not_empty_count == 2);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(T_ThrowingMove::instance_count == 0);
    return 0;
}

int test_variant_6() {  // move assignment
    {
        uxs::variant v_from;
        uxs::variant v;
        v = std::move(v_from);  // empty <- empty
        VERIFY(!v.has_value() && v.type() == uxs::variant_id::invalid);
    }

    {  // placement
        uxs::variant v_from(uxs::in_place_type<T>(), 100), v_from2(uxs::in_place_type<T>(), 200);
        uxs::variant v;
        v = std::move(v_from);  // empty <- not empty
        VERIFY(v.has_value() && v.type() == test_variant_id && v.as<T>() == 100);
        VERIFY(T::not_empty_count == 2);

        uxs::variant v2(std::string("long enough for memory allocation"));
        v2 = std::move(v);  // not empty <- not empty (reconstruction)
        VERIFY(v2.has_value() && v2.type() == test_variant_id && v2.as<T>() == 100);
        VERIFY(T::not_empty_count == 2);

        v2 = std::move(v_from2);  // not empty <- not empty (assignment)
        VERIFY(v2.has_value() && v2.type() == test_variant_id && v2.as<T>() == 200);
        VERIFY(T::not_empty_count == 1);

        uxs::variant v_empty;
        v2 = std::move(v_empty);  // not empty <- empty
        VERIFY(T::not_empty_count == 0);
    }

    {  // dynamic alloc (COW pointer)
        uxs::variant v_from(uxs::in_place_type<T_ThrowingMove>(), 100),
            v_from2(uxs::in_place_type<T_ThrowingMove>(), 200);
        uxs::variant v;
        v = std::move(v_from);  // empty <- not empty
        VERIFY(v.has_value() && v.type() == test_dyn_variant_id && v.as<T_ThrowingMove>() == 100);
        VERIFY(T::not_empty_count == 2);

        uxs::variant v2(std::string("long enough for memory allocation"));
        v2 = std::move(v);  // not empty <- not empty (reconstruction)
        VERIFY(v2.has_value() && v2.type() == test_dyn_variant_id && v2.as<T_ThrowingMove>() == 100);
        VERIFY(T::not_empty_count == 2);

        v2 = std::move(v_from2);  // not empty <- not empty (assignment)
        VERIFY(v2.has_value() && v2.type() == test_dyn_variant_id && v2.as<T_ThrowingMove>() == 200);
        VERIFY(T::not_empty_count == 1);

        uxs::variant v_empty;
        v2 = std::move(v_empty);  // not empty <- empty
        VERIFY(T::not_empty_count == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(T_ThrowingMove::instance_count == 0);
    return 0;
}

int test_variant_7() {  // value assignment
    {                   // placement
        uxs::variant v;
        v = T(100);  // empty <- not empty
        VERIFY(v.has_value() && v.type() == test_variant_id && v.as<T>() == 100);
        VERIFY(T::not_empty_count == 1);

        uxs::variant v2(std::string("long enough for memory allocation"));
        v2 = T(100);  // not empty <- not empty (reconstruction)
        VERIFY(v2.has_value() && v2.type() == test_variant_id && v2.as<T>() == 100);
        VERIFY(T::not_empty_count == 2);

        v2 = T(200);  // not empty <- not empty (assignment)
        VERIFY(v2.has_value() && v2.type() == test_variant_id && v2.as<T>() == 200);
        VERIFY(T::not_empty_count == 2);

        v2.reset();  // reset
        VERIFY(T::not_empty_count == 1);
    }

    {  // dynamic alloc (COW pointer)
        uxs::variant v;
        v = T_ThrowingMove(100);  // empty <- not empty
        VERIFY(v.has_value() && v.type() == test_dyn_variant_id && v.as<T_ThrowingMove>() == 100);
        VERIFY(T::not_empty_count == 1);

        uxs::variant v2(std::string("long enough for memory allocation"));
        v2 = T_ThrowingMove(100);  // not empty <- not empty (reconstruction)
        VERIFY(v2.has_value() && v2.type() == test_dyn_variant_id && v2.as<T_ThrowingMove>() == 100);
        VERIFY(T::not_empty_count == 2);

        v2 = T_ThrowingMove(200);  // not empty <- not empty (assignment)
        VERIFY(v2.has_value() && v2.type() == test_dyn_variant_id && v2.as<T_ThrowingMove>() == 200);
        VERIFY(T::not_empty_count == 2);

        v2.reset();  // reset
        VERIFY(T::not_empty_count == 1);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(T_ThrowingMove::instance_count == 0);
    return 0;
}

int test_variant_8() {  // value emplacement
    {                   // placement
        uxs::variant v;
        v.emplace<T>(100);  // empty <- not empty
        VERIFY(v.has_value() && v.type() == test_variant_id && v.as<T>() == 100);
        VERIFY(T::not_empty_count == 1);

        uxs::variant v2(std::string("long enough for memory allocation"));
        v2.emplace<T>(100);  // not empty <- not empty (reconstruction)
        VERIFY(v2.has_value() && v2.type() == test_variant_id && v2.as<T>() == 100);
        VERIFY(T::not_empty_count == 2);

        v2.emplace<T>(200);  // not empty <- not empty (assignment)
        VERIFY(v2.has_value() && v2.type() == test_variant_id && v2.as<T>() == 200);
        VERIFY(T::not_empty_count == 2);

        v2.reset();  // reset
        VERIFY(T::not_empty_count == 1);
    }

    {  // dynamic alloc (COW pointer)
        uxs::variant v;
        v.emplace<T_ThrowingMove>(100);  // empty <- not empty
        VERIFY(v.has_value() && v.type() == test_dyn_variant_id && v.as<T_ThrowingMove>() == 100);
        VERIFY(T::not_empty_count == 1);

        uxs::variant v2(std::string("long enough for memory allocation"));
        v2.emplace<T_ThrowingMove>(100);  // not empty <- not empty (reconstruction)
        VERIFY(v2.has_value() && v2.type() == test_dyn_variant_id && v2.as<T_ThrowingMove>() == 100);
        VERIFY(T::not_empty_count == 2);

        v2.emplace<T_ThrowingMove>(200);  // not empty <- not empty (assignment)
        VERIFY(v2.has_value() && v2.type() == test_dyn_variant_id && v2.as<T_ThrowingMove>() == 200);
        VERIFY(T::not_empty_count == 2);

        v2.reset();  // reset
        VERIFY(T::not_empty_count == 1);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(T_ThrowingMove::instance_count == 0);
    return 0;
}

int test_variant_9() {  // value comparison
    {
        uxs::variant v, v2;
        VERIFY(v != "hello" && "hello" != v);
        VERIFY(v != 100 && 100 != v);
        VERIFY(v != 1. && 1. != v);
        VERIFY(v == v2);
    }
    {
        uxs::variant v, v2;
        v.emplace<T>(100);
        VERIFY(v == T(100) && T(100) == v);
        VERIFY(v == "100" && "100" == v);
        VERIFY(v != 100 && 100 != v);
        VERIFY(v != v2 && v2 != v);
    }
    {
        uxs::variant v("100"), v2;
        VERIFY(v == T(100) && T(100) == v);
        VERIFY(v == "100" && "100" == v);
        VERIFY(v == 100 && 100 == v);
        VERIFY(v != v2 && v2 != v);
    }
    {
        uxs::variant v, v2;
        v.emplace<T_ThrowingMove>(100);
        VERIFY(v == T_ThrowingMove(100) && T_ThrowingMove(100) == v);
        VERIFY(v == "100" && "100" == v);
        VERIFY(v != 100 && 100 != v);
        VERIFY(v != v2 && v2 != v);
    }
    {
        uxs::variant v(T(100)), v2("100"), v3(100);
        VERIFY(v == v2 && v2 == v);
        VERIFY(v != v3 && v3 != v);
    }
    {
        uxs::variant v(T_ThrowingMove(100)), v2("100"), v3(100);
        VERIFY(v == v2 && v2 == v);
        VERIFY(v != v3 && v3 != v);
        VERIFY(v2 == v3 && v3 == v2);
    }
    return 0;
}

int test_variant_10() {
    {
        uxs::variant v;
        VERIFY(!v.has_value() && v.type() == uxs::variant_id::invalid);

        uxs::variant v2(v);
        VERIFY(!v2.has_value() && v2.type() == uxs::variant_id::invalid);

        uxs::variant v3(std::move(v));
        VERIFY(!v3.has_value() && v3.type() == uxs::variant_id::invalid);
        VERIFY(!v.has_value() && v.type() == uxs::variant_id::invalid);

        uxs::variant v4(uxs::variant_id::string, v2);
        VERIFY(v4.has_value() && v4.type() == uxs::variant_id::string && v4 == "");
        v4 = "1234567890123456789012345678901234567890123456789012345678901234567890";
        VERIFY(v4.has_value() && v4.type() == uxs::variant_id::string &&
               v4 == "1234567890123456789012345678901234567890123456789012345678901234567890");

        uxs::variant v5(uxs::variant_id::invalid, v4);
        VERIFY(!v5.has_value() && v5.type() == uxs::variant_id::invalid);
    }

    {
        uxs::variant v(uxs::variant_id::string);
        v = "1234567890123456789012345678901234567890123456789012345678901234567890";
        VERIFY(v.has_value() && v.type() == uxs::variant_id::string &&
               v == "1234567890123456789012345678901234567890123456789012345678901234567890");

        uxs::variant v2(v);
        VERIFY(v2.has_value() && v2.type() == uxs::variant_id::string &&
               v2 == "1234567890123456789012345678901234567890123456789012345678901234567890");

        uxs::variant v3(std::move(v));
        VERIFY(v3.has_value() && v3.type() == uxs::variant_id::string &&
               v3 == "1234567890123456789012345678901234567890123456789012345678901234567890");
    }

    {
        std::string s("1234567890123456789012345678901234567890123456789012345678901234567890");

        uxs::variant v(s);
        VERIFY(v.has_value() && v.type() == uxs::variant_id::string &&
               v == "1234567890123456789012345678901234567890123456789012345678901234567890");

        uxs::variant v2(std::move(s));
        VERIFY(v2.has_value() && v2.type() == uxs::variant_id::string &&
               v2 == "1234567890123456789012345678901234567890123456789012345678901234567890");

        uxs::variant v3(uxs::variant_id::string, 3.123456789);
        VERIFY(v3.has_value() && v3.type() == uxs::variant_id::string && v3 == "3.123456789");
    }

    uxs::variant v;
    std::string s1;
    std::string_view s2;
    bool r = v == v || v == s1 || s1 == v || v == s2 || s2 == v;
    (void)r;

    return 0;
}

int test_variant_11() {
    uxs::variant v(std::string{"1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1"});

    VERIFY(v.as<std::string>() == "1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1");
    VERIFY(v.as<vrc::math::mat4>() == vrc::math::mat4());
    VERIFY(v.convert(uxs::variant_id::matrix4x4));
    VERIFY(v.as<std::string>() == "1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1");
    VERIFY(v.as<vrc::math::mat4>() == vrc::math::mat4());

    VERIFY(v.convert(uxs::variant_id::string));
    VERIFY(v.as<std::string>() == "1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1");
    VERIFY(v.as<vrc::math::mat4>() == vrc::math::mat4());

    uxs::variant v1(-234.57);

    VERIFY(v1.as<int>() == -234);
    VERIFY(v1.as<double>() == -234.57);
    VERIFY(v1.as<std::string>() == "-234.57");

    v1.convert(uxs::variant_id::string);

    auto v2 = std::move(v1);
    VERIFY(v2.as<std::string>() == "-234.57");

    auto v3 = v2;
    VERIFY(v3.as<std::string>() == "-234.57");

    return 0;
}

int test_variant_12() {
    uxs::variant v(uxs::in_place_type<T>(), 100);
    VERIFY(v.value<std::string>() == "100" && v.value<int>() == 0);
    VERIFY(v.value_or<std::string>("200") == "100" && v.value_or<int>(200) == 200);
    return 0;
}

int test_string_value_invalid() {
    uxs::variant v;
    VERIFY(v.type() == uxs::variant_id::invalid);
    MUST_THROW(v.as<bool>());
    MUST_THROW(v.as<int32_t>());
    MUST_THROW(v.as<uint32_t>());
    MUST_THROW(v.as<int64_t>());
    MUST_THROW(v.as<uint64_t>());
    MUST_THROW(v.as<float>());
    MUST_THROW(v.as<double>());
    MUST_THROW(v.as<std::string>());
    return 0;
}

int test_string_value_bool() {
    {
        uxs::variant v(true);
        VERIFY(v.type() == uxs::variant_id::boolean);
        VERIFY(v.as<bool>() == true);
        VERIFY(v.as<int32_t>() == 1);
        VERIFY(v.as<uint32_t>() == 1);
        VERIFY(v.as<int64_t>() == 1);
        VERIFY(v.as<uint64_t>() == 1);
        VERIFY(v.as<float>() == 1.f);
        VERIFY(v.as<double>() == 1.);
        VERIFY(v.as<std::string>() == "true");
    }
    {
        uxs::variant v(false);
        VERIFY(v.type() == uxs::variant_id::boolean);
        VERIFY(v.as<bool>() == false);
        VERIFY(v.as<int32_t>() == 0);
        VERIFY(v.as<uint32_t>() == 0);
        VERIFY(v.as<int64_t>() == 0);
        VERIFY(v.as<uint64_t>() == 0);
        VERIFY(v.as<float>() == 0.f);
        VERIFY(v.as<double>() == 0.);
        VERIFY(v.as<std::string>() == "false");
    }
    return 0;
}

int test_string_value_int() {
    {
        uxs::variant v(0);
        VERIFY(v.type() == uxs::variant_id::integer);
        VERIFY(v.as<bool>() == false);
        VERIFY(v.as<int32_t>() == 0);
        VERIFY(v.as<uint32_t>() == 0);
        VERIFY(v.as<int64_t>() == 0);
        VERIFY(v.as<uint64_t>() == 0);
        VERIFY(v.as<float>() == 0.f);
        VERIFY(v.as<double>() == 0.);
        VERIFY(v.as<std::string>() == "0");
    }
    {
        uxs::variant v(123);
        VERIFY(v.type() == uxs::variant_id::integer);
        VERIFY(v.as<bool>() == true);
        VERIFY(v.as<int32_t>() == 123);
        VERIFY(v.as<uint32_t>() == 123);
        VERIFY(v.as<int64_t>() == 123);
        VERIFY(v.as<uint64_t>() == 123);
        VERIFY(v.as<float>() == 123.f);
        VERIFY(v.as<double>() == 123.);
        VERIFY(v.as<std::string>() == "123");
    }
    {
        uxs::variant v(-123);
        VERIFY(v.type() == uxs::variant_id::integer);
        VERIFY(v.as<bool>() == true);
        VERIFY(v.as<int32_t>() == -123);
        MUST_THROW(v.as<uint32_t>());
        VERIFY(v.as<int64_t>() == -123);
        MUST_THROW(v.as<uint64_t>());
        VERIFY(v.as<float>() == -123.f);
        VERIFY(v.as<double>() == -123.);
        VERIFY(v.as<std::string>() == "-123");
    }
    return 0;
}

int test_string_value_uint() {
    {
        uxs::variant v(0u);
        VERIFY(v.type() == uxs::variant_id::unsigned_integer);
        VERIFY(v.as<bool>() == false);
        VERIFY(v.as<int32_t>() == 0);
        VERIFY(v.as<uint32_t>() == 0);
        VERIFY(v.as<int64_t>() == 0);
        VERIFY(v.as<uint64_t>() == 0);
        VERIFY(v.as<float>() == 0.f);
        VERIFY(v.as<double>() == 0.);
        VERIFY(v.as<std::string>() == "0");
    }
    {
        uxs::variant v(123u);
        VERIFY(v.type() == uxs::variant_id::unsigned_integer);
        VERIFY(v.as<bool>() == true);
        VERIFY(v.as<int32_t>() == 123);
        VERIFY(v.as<uint32_t>() == 123);
        VERIFY(v.as<int64_t>() == 123);
        VERIFY(v.as<uint64_t>() == 123);
        VERIFY(v.as<float>() == 123.f);
        VERIFY(v.as<double>() == 123.);
        VERIFY(v.as<std::string>() == "123");
    }
    {
        uxs::variant v(3000000000u);
        VERIFY(v.type() == uxs::variant_id::unsigned_integer);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        VERIFY(v.as<uint32_t>() == 3000000000u);
        VERIFY(v.as<int64_t>() == 3000000000ll);
        VERIFY(v.as<uint64_t>() == 3000000000ull);
        VERIFY(v.as<float>() == 3.e+9f);
        VERIFY(v.as<double>() == 3.e+9);
        VERIFY(v.as<std::string>() == "3000000000");
    }
    return 0;
}

int test_string_value_int64() {
    {
        uxs::variant v(int64_t(0));
        VERIFY(v.type() == uxs::variant_id::long_integer);
        VERIFY(v.as<bool>() == false);
        VERIFY(v.as<int32_t>() == 0);
        VERIFY(v.as<uint32_t>() == 0);
        VERIFY(v.as<int64_t>() == 0);
        VERIFY(v.as<uint64_t>() == 0);
        VERIFY(v.as<float>() == 0.f);
        VERIFY(v.as<double>() == 0.);
        VERIFY(v.as<std::string>() == "0");
    }
    {
        uxs::variant v(int64_t(123));
        VERIFY(v.type() == uxs::variant_id::long_integer);
        VERIFY(v.as<bool>() == true);
        VERIFY(v.as<int32_t>() == 123);
        VERIFY(v.as<uint32_t>() == 123);
        VERIFY(v.as<int64_t>() == 123);
        VERIFY(v.as<uint64_t>() == 123);
        VERIFY(v.as<float>() == 123.f);
        VERIFY(v.as<double>() == 123.);
        VERIFY(v.as<std::string>() == "123");
    }
    {
        uxs::variant v(int64_t(-123));
        VERIFY(v.type() == uxs::variant_id::long_integer);
        VERIFY(v.as<bool>() == true);
        VERIFY(v.as<int32_t>() == -123);
        MUST_THROW(v.as<uint32_t>());
        VERIFY(v.as<int64_t>() == -123);
        MUST_THROW(v.as<uint64_t>());
        VERIFY(v.as<float>() == -123.f);
        VERIFY(v.as<double>() == -123.);
        VERIFY(v.as<std::string>() == "-123");
    }
    {
        uxs::variant v(int64_t(3000000000ll));
        VERIFY(v.type() == uxs::variant_id::long_integer);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        VERIFY(v.as<uint32_t>() == 3000000000u);
        VERIFY(v.as<int64_t>() == 3000000000ll);
        VERIFY(v.as<uint64_t>() == 3000000000ull);
        VERIFY(v.as<float>() == 3.e+9f);
        VERIFY(v.as<double>() == 3.e+9);
        VERIFY(v.as<std::string>() == "3000000000");
    }
    {
        uxs::variant v(int64_t(-3000000000ll));
        VERIFY(v.type() == uxs::variant_id::long_integer);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        VERIFY(v.as<int64_t>() == -3000000000ll);
        MUST_THROW(v.as<uint64_t>());
        VERIFY(v.as<float>() == -3.e+9f);
        VERIFY(v.as<double>() == -3.e+9);
        VERIFY(v.as<std::string>() == "-3000000000");
    }
    {
        uxs::variant v(int64_t(1000000000000000ll));
        VERIFY(v.type() == uxs::variant_id::long_integer);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        VERIFY(v.as<int64_t>() == 1000000000000000ll);
        VERIFY(v.as<uint64_t>() == 1000000000000000ull);
        VERIFY(v.as<float>() == 1.e+15f);
        VERIFY(v.as<double>() == 1.e+15);
        VERIFY(v.as<std::string>() == "1000000000000000");
    }
    {
        uxs::variant v(int64_t(-1000000000000000ll));
        VERIFY(v.type() == uxs::variant_id::long_integer);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        VERIFY(v.as<int64_t>() == -1000000000000000ll);
        MUST_THROW(v.as<uint64_t>());
        VERIFY(v.as<float>() == -1.e+15f);
        VERIFY(v.as<double>() == -1.e+15);
        VERIFY(v.as<std::string>() == "-1000000000000000");
    }
    return 0;
}

int test_string_value_uint64() {
    {
        uxs::variant v(uint64_t(0u));
        VERIFY(v.type() == uxs::variant_id::unsigned_long_integer);
        VERIFY(v.as<bool>() == false);
        VERIFY(v.as<int32_t>() == 0);
        VERIFY(v.as<uint32_t>() == 0);
        VERIFY(v.as<int64_t>() == 0);
        VERIFY(v.as<uint64_t>() == 0);
        VERIFY(v.as<float>() == 0.f);
        VERIFY(v.as<double>() == 0.);
        VERIFY(v.as<std::string>() == "0");
    }
    {
        uxs::variant v(uint64_t(123u));
        VERIFY(v.type() == uxs::variant_id::unsigned_long_integer);
        VERIFY(v.as<bool>() == true);
        VERIFY(v.as<int32_t>() == 123);
        VERIFY(v.as<uint32_t>() == 123);
        VERIFY(v.as<int64_t>() == 123);
        VERIFY(v.as<uint64_t>() == 123);
        VERIFY(v.as<float>() == 123.f);
        VERIFY(v.as<double>() == 123.);
        VERIFY(v.as<std::string>() == "123");
    }
    {
        uxs::variant v(uint64_t(3000000000u));
        VERIFY(v.type() == uxs::variant_id::unsigned_long_integer);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        VERIFY(v.as<uint32_t>() == 3000000000u);
        VERIFY(v.as<int64_t>() == 3000000000ll);
        VERIFY(v.as<uint64_t>() == 3000000000ull);
        VERIFY(v.as<float>() == 3.e+9f);
        VERIFY(v.as<double>() == 3.e+9);
        VERIFY(v.as<std::string>() == "3000000000");
    }
    {
        uxs::variant v(uint64_t(1000000000000000u));
        VERIFY(v.type() == uxs::variant_id::unsigned_long_integer);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        VERIFY(v.as<int64_t>() == 1000000000000000ll);
        VERIFY(v.as<uint64_t>() == 1000000000000000ull);
        VERIFY(v.as<float>() == 1.e+15f);
        VERIFY(v.as<double>() == 1.e+15);
        VERIFY(v.as<std::string>() == "1000000000000000");
    }
    {
        uxs::variant v(uint64_t(10000000000000000000ull));
        VERIFY(v.type() == uxs::variant_id::unsigned_long_integer);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        MUST_THROW(v.as<int64_t>());
        VERIFY(v.as<uint64_t>() == 10000000000000000000ull);
        VERIFY(v.as<float>() == 1.e+19f);
        VERIFY(v.as<double>() == 1.e+19);
        VERIFY(v.as<std::string>() == "10000000000000000000");
    }
    return 0;
}

int test_string_value_float() {
    {
        uxs::variant v(0.f);
        VERIFY(v.type() == uxs::variant_id::single_precision);
        VERIFY(v.as<bool>() == false);
        VERIFY(v.as<int32_t>() == 0);
        VERIFY(v.as<uint32_t>() == 0);
        VERIFY(v.as<int64_t>() == 0);
        VERIFY(v.as<uint64_t>() == 0);
        VERIFY(v.as<float>() == 0.f);
        VERIFY(v.as<double>() == 0.);
        VERIFY(v.as<std::string>() == "0.0");
    }
    {
        uxs::variant v(123.f);
        VERIFY(v.type() == uxs::variant_id::single_precision);
        VERIFY(v.as<bool>() == true);
        VERIFY(v.as<int32_t>() == 123);
        VERIFY(v.as<uint32_t>() == 123);
        VERIFY(v.as<int64_t>() == 123);
        VERIFY(v.as<uint64_t>() == 123);
        VERIFY(v.as<float>() == 123.f);
        VERIFY(v.as<double>() == 123.);
        VERIFY(v.as<std::string>() == "123.0");
    }
    {
        uxs::variant v(123.5f);
        VERIFY(v.type() == uxs::variant_id::single_precision);
        VERIFY(v.as<bool>() == true);
        VERIFY(v.as<int32_t>() == 123);
        VERIFY(v.as<uint32_t>() == 123);
        VERIFY(v.as<int64_t>() == 123);
        VERIFY(v.as<uint64_t>() == 123);
        VERIFY(v.as<float>() == 123.5f);
        VERIFY(v.as<double>() == 123.5);
        VERIFY(v.as<std::string>() == "123.5");
    }
    {
        uxs::variant v(-123.f);
        VERIFY(v.type() == uxs::variant_id::single_precision);
        VERIFY(v.as<bool>() == true);
        VERIFY(v.as<int32_t>() == -123);
        MUST_THROW(v.as<uint32_t>());
        VERIFY(v.as<int64_t>() == -123);
        MUST_THROW(v.as<uint64_t>());
        VERIFY(v.as<float>() == -123.f);
        VERIFY(v.as<double>() == -123.);
        VERIFY(v.as<std::string>() == "-123.0");
    }
    {
        uxs::variant v(3.e+9f);
        VERIFY(v.type() == uxs::variant_id::single_precision);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        VERIFY(v.as<uint32_t>() == 3000000000u);
        VERIFY(v.as<int64_t>() == 3000000000ll);
        VERIFY(v.as<uint64_t>() == 3000000000ull);
        VERIFY(v.as<float>() == 3.e+9f);
        VERIFY(v.as<double>() == 3.e+9);
        VERIFY(v.as<std::string>() == "3.0e+09");
    }
    {
        uxs::variant v(-3.e+9f);
        VERIFY(v.type() == uxs::variant_id::single_precision);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        VERIFY(v.as<int64_t>() == -3000000000ll);
        MUST_THROW(v.as<uint64_t>());
        VERIFY(v.as<float>() == -3.e+9f);
        VERIFY(v.as<double>() == -3.e+9);
        VERIFY(v.as<std::string>() == "-3.0e+09");
    }
    {
        uxs::variant v(1.e+15f);
        VERIFY(v.type() == uxs::variant_id::single_precision);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        VERIFY(v.as<int64_t>() == 1000000000000000ll);
        VERIFY(v.as<uint64_t>() == 1000000000000000ull);
        VERIFY(v.as<float>() == 1.e+15f);
        VERIFY(v.as<double>() == 1.e+15);
        VERIFY(v.as<std::string>() == "1.0e+15");
    }
    {
        uxs::variant v(-1.e+15f);
        VERIFY(v.type() == uxs::variant_id::single_precision);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        VERIFY(v.as<int64_t>() == -1000000000000000ll);
        MUST_THROW(v.as<uint64_t>());
        VERIFY(v.as<float>() == -1.e+15f);
        VERIFY(v.as<double>() == -1.e+15);
        VERIFY(v.as<std::string>() == "-1.0e+15");
    }
    {
        uxs::variant v(1.e+19f);
        VERIFY(v.type() == uxs::variant_id::single_precision);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        MUST_THROW(v.as<int64_t>());
        VERIFY(v.as<uint64_t>() == 10000000000000000000ull);
        VERIFY(v.as<float>() == 1.e+19f);
        VERIFY(v.as<double>() == 1.e+19);
        VERIFY(v.as<std::string>() == "1.0e+19");
    }
    {
        uxs::variant v(-1.e+19f);
        VERIFY(v.type() == uxs::variant_id::single_precision);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        MUST_THROW(v.as<int64_t>());
        MUST_THROW(v.as<uint64_t>());
        VERIFY(v.as<float>() == -1.e+19f);
        VERIFY(v.as<double>() == -1.e+19);
        VERIFY(v.as<std::string>() == "-1.0e+19");
    }
    {
        uxs::variant v(1.e+25f);
        VERIFY(v.type() == uxs::variant_id::single_precision);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        MUST_THROW(v.as<int64_t>());
        MUST_THROW(v.as<uint64_t>());
        VERIFY(v.as<float>() == 1.e+25f);
        VERIFY(v.as<double>() == 1.e+25);
        VERIFY(v.as<std::string>() == "1.0e+25");
    }
    return 0;
}

int test_string_value_double() {
    {
        uxs::variant v(0.);
        VERIFY(v.type() == uxs::variant_id::double_precision);
        VERIFY(v.as<bool>() == false);
        VERIFY(v.as<int32_t>() == 0);
        VERIFY(v.as<uint32_t>() == 0);
        VERIFY(v.as<int64_t>() == 0);
        VERIFY(v.as<uint64_t>() == 0);
        VERIFY(v.as<float>() == 0.f);
        VERIFY(v.as<double>() == 0.);
        VERIFY(v.as<std::string>() == "0.0");
    }
    {
        uxs::variant v(123.);
        VERIFY(v.type() == uxs::variant_id::double_precision);
        VERIFY(v.as<bool>() == true);
        VERIFY(v.as<int32_t>() == 123);
        VERIFY(v.as<uint32_t>() == 123);
        VERIFY(v.as<int64_t>() == 123);
        VERIFY(v.as<uint64_t>() == 123);
        VERIFY(v.as<float>() == 123.f);
        VERIFY(v.as<double>() == 123.);
        VERIFY(v.as<std::string>() == "123.0");
    }
    {
        uxs::variant v(123.5);
        VERIFY(v.type() == uxs::variant_id::double_precision);
        VERIFY(v.as<bool>() == true);
        VERIFY(v.as<int32_t>() == 123);
        VERIFY(v.as<uint32_t>() == 123);
        VERIFY(v.as<int64_t>() == 123);
        VERIFY(v.as<uint64_t>() == 123);
        VERIFY(v.as<float>() == 123.5f);
        VERIFY(v.as<double>() == 123.5);
        VERIFY(v.as<std::string>() == "123.5");
    }
    {
        uxs::variant v(-123.);
        VERIFY(v.type() == uxs::variant_id::double_precision);
        VERIFY(v.as<bool>() == true);
        VERIFY(v.as<int32_t>() == -123);
        MUST_THROW(v.as<uint32_t>());
        VERIFY(v.as<int64_t>() == -123);
        MUST_THROW(v.as<uint64_t>());
        VERIFY(v.as<float>() == -123.f);
        VERIFY(v.as<double>() == -123.);
        VERIFY(v.as<std::string>() == "-123.0");
    }
    {
        uxs::variant v(3.e+9);
        VERIFY(v.type() == uxs::variant_id::double_precision);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        VERIFY(v.as<uint32_t>() == 3000000000u);
        VERIFY(v.as<int64_t>() == 3000000000ll);
        VERIFY(v.as<uint64_t>() == 3000000000ull);
        VERIFY(v.as<float>() == 3.e+9f);
        VERIFY(v.as<double>() == 3.e+9);
        VERIFY(v.as<std::string>() == "3.0e+09");
    }
    {
        uxs::variant v(-3.e+9);
        VERIFY(v.type() == uxs::variant_id::double_precision);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        VERIFY(v.as<int64_t>() == -3000000000ll);
        MUST_THROW(v.as<uint64_t>());
        VERIFY(v.as<float>() == -3.e+9f);
        VERIFY(v.as<double>() == -3.e+9);
        VERIFY(v.as<std::string>() == "-3.0e+09");
    }
    {
        uxs::variant v(1.e+15);
        VERIFY(v.type() == uxs::variant_id::double_precision);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        VERIFY(v.as<int64_t>() == 1000000000000000ll);
        VERIFY(v.as<uint64_t>() == 1000000000000000ull);
        VERIFY(v.as<float>() == 1.e+15f);
        VERIFY(v.as<double>() == 1.e+15);
        VERIFY(v.as<std::string>() == "1.0e+15");
    }
    {
        uxs::variant v(-1.e+15);
        VERIFY(v.type() == uxs::variant_id::double_precision);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        VERIFY(v.as<int64_t>() == -1000000000000000ll);
        MUST_THROW(v.as<uint64_t>());
        VERIFY(v.as<float>() == -1.e+15f);
        VERIFY(v.as<double>() == -1.e+15);
        VERIFY(v.as<std::string>() == "-1.0e+15");
    }
    {
        uxs::variant v(1.e+19);
        VERIFY(v.type() == uxs::variant_id::double_precision);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        MUST_THROW(v.as<int64_t>());
        VERIFY(v.as<uint64_t>() == 10000000000000000000ull);
        VERIFY(v.as<float>() == 1.e+19f);
        VERIFY(v.as<double>() == 1.e+19);
        VERIFY(v.as<std::string>() == "1.0e+19");
    }
    {
        uxs::variant v(-1.e+19);
        VERIFY(v.type() == uxs::variant_id::double_precision);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        MUST_THROW(v.as<int64_t>());
        MUST_THROW(v.as<uint64_t>());
        VERIFY(v.as<float>() == -1.e+19f);
        VERIFY(v.as<double>() == -1.e+19);
        VERIFY(v.as<std::string>() == "-1.0e+19");
    }
    {
        uxs::variant v(1.e+25);
        VERIFY(v.type() == uxs::variant_id::double_precision);
        VERIFY(v.as<bool>() == true);
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        MUST_THROW(v.as<int64_t>());
        MUST_THROW(v.as<uint64_t>());
        VERIFY(v.as<float>() == 1.e+25f);
        VERIFY(v.as<double>() == 1.e+25);
        VERIFY(v.as<std::string>() == "1.0e+25");
    }
    return 0;
}

int test_string_value_string() {
    {
        uxs::variant v("hello");
        VERIFY(v.type() == uxs::variant_id::string);
        MUST_THROW(v.as<bool>());
        MUST_THROW(v.as<int32_t>());
        MUST_THROW(v.as<uint32_t>());
        MUST_THROW(v.as<int64_t>());
        MUST_THROW(v.as<uint64_t>());
        MUST_THROW(v.as<float>());
        MUST_THROW(v.as<double>());
        VERIFY(v.as<std::string>() == "hello");
    }
    VERIFY(uxs::variant("123").as<int32_t>() == 123);
    VERIFY(uxs::variant("-123").as<int32_t>() == -123);
    VERIFY(uxs::variant("123").as<uint32_t>() == 123);
    VERIFY(uxs::variant("1000000000000000").as<int64_t>() == 1000000000000000ll);
    VERIFY(uxs::variant("-1000000000000000").as<int64_t>() == -1000000000000000ll);
    VERIFY(uxs::variant("10000000000000000000").as<uint64_t>() == 10000000000000000000ull);
    VERIFY(uxs::variant("12.334567").as<double>() == 12.334567);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "variant", test_variant_0);
ADD_TEST_CASE("", "variant", test_variant_1);
ADD_TEST_CASE("", "variant", test_variant_2);
ADD_TEST_CASE("", "variant", test_variant_3);
ADD_TEST_CASE("", "variant", test_variant_4);
ADD_TEST_CASE("", "variant", test_variant_5);
ADD_TEST_CASE("", "variant", test_variant_6);
ADD_TEST_CASE("", "variant", test_variant_7);
ADD_TEST_CASE("", "variant", test_variant_8);
ADD_TEST_CASE("", "variant", test_variant_9);
ADD_TEST_CASE("", "variant", test_variant_10);
ADD_TEST_CASE("", "variant", test_variant_11);
ADD_TEST_CASE("", "variant", test_variant_12);
ADD_TEST_CASE("", "variant", test_string_value_invalid);
ADD_TEST_CASE("", "variant", test_string_value_bool);
ADD_TEST_CASE("", "variant", test_string_value_int);
ADD_TEST_CASE("", "variant", test_string_value_uint);
ADD_TEST_CASE("", "variant", test_string_value_int64);
ADD_TEST_CASE("", "variant", test_string_value_uint64);
ADD_TEST_CASE("", "variant", test_string_value_double);
ADD_TEST_CASE("", "variant", test_string_value_string);
