#include "db_value_tests.h"

namespace {

int test_string_value_null() {
    uxs::db::value v;
    VERIFY(v.type() == uxs::db::dtype::kNull);
    VERIFY(v.is_null());
    VERIFY(!v.is_bool());
    VERIFY(!v.is_int());
    VERIFY(!v.is_uint());
    VERIFY(!v.is_int64());
    VERIFY(!v.is_uint64());
    VERIFY(!v.is_integral());
    VERIFY(!v.is_float());
    VERIFY(!v.is_double());
    VERIFY(!v.is_numeric());
    VERIFY(!v.is_string());
    VERIFY(!v.is_array());
    VERIFY(!v.is_record());
    VERIFY(v.empty());
    VERIFY(v.size() == 0);
    MUST_THROW(v.as_bool());
    MUST_THROW(v.as_int());
    MUST_THROW(v.as_uint());
    MUST_THROW(v.as_int64());
    MUST_THROW(v.as_uint64());
    MUST_THROW(v.as_float());
    MUST_THROW(v.as_double());
    VERIFY(v.as_string() == "null");
    MUST_THROW(v.as_string_view());
    MUST_THROW(v.at(0));
    VERIFY(v["0"].is_null());
    VERIFY(v.is_record());
    return 0;
}

int test_string_value_bool() {
    {
        uxs::db::value v(true);
        VERIFY(v.type() == uxs::db::dtype::kBoolean);
        VERIFY(!v.is_null());
        VERIFY(v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(!v.is_int64());
        VERIFY(!v.is_uint64());
        VERIFY(!v.is_integral());
        VERIFY(!v.is_float());
        VERIFY(!v.is_double());
        VERIFY(!v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        VERIFY(v.as_int() == 1);
        VERIFY(v.as_uint() == 1);
        VERIFY(v.as_int64() == 1);
        VERIFY(v.as_uint64() == 1);
        VERIFY(v.as_float() == 1.f);
        VERIFY(v.as_double() == 1.);
        VERIFY(v.as_string() == "true");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(false);
        VERIFY(v.type() == uxs::db::dtype::kBoolean);
        VERIFY(!v.is_null());
        VERIFY(v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(!v.is_int64());
        VERIFY(!v.is_uint64());
        VERIFY(!v.is_integral());
        VERIFY(!v.is_float());
        VERIFY(!v.is_double());
        VERIFY(!v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == false);
        VERIFY(v.as_int() == 0);
        VERIFY(v.as_uint() == 0);
        VERIFY(v.as_int64() == 0);
        VERIFY(v.as_uint64() == 0);
        VERIFY(v.as_float() == 0.f);
        VERIFY(v.as_double() == 0.);
        VERIFY(v.as_string() == "false");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(true);
        VERIFY(&v[0] == &v);
        MUST_THROW(v.at(1));
        MUST_THROW(v["0"]);
    }
    return 0;
}

int test_string_value_int() {
    {
        uxs::db::value v(0);
        VERIFY(v.type() == uxs::db::dtype::kInteger);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(v.is_int());
        VERIFY(v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == false);
        VERIFY(v.as_int() == 0);
        VERIFY(v.as_uint() == 0);
        VERIFY(v.as_int64() == 0);
        VERIFY(v.as_uint64() == 0);
        VERIFY(v.as_float() == 0.f);
        VERIFY(v.as_double() == 0.);
        VERIFY(v.as_string() == "0");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(123);
        VERIFY(v.type() == uxs::db::dtype::kInteger);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(v.is_int());
        VERIFY(v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        VERIFY(v.as_int() == 123);
        VERIFY(v.as_uint() == 123);
        VERIFY(v.as_int64() == 123);
        VERIFY(v.as_uint64() == 123);
        VERIFY(v.as_float() == 123.f);
        VERIFY(v.as_double() == 123.);
        VERIFY(v.as_string() == "123");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(-123);
        VERIFY(v.type() == uxs::db::dtype::kInteger);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(!v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        VERIFY(v.as_int() == -123);
        MUST_THROW(v.as_uint());
        VERIFY(v.as_int64() == -123);
        MUST_THROW(v.as_uint64());
        VERIFY(v.as_float() == -123.f);
        VERIFY(v.as_double() == -123.);
        VERIFY(v.as_string() == "-123");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(123);
        VERIFY(&v[0] == &v);
        MUST_THROW(v.at(1));
        MUST_THROW(v["0"]);
    }
    return 0;
}

int test_string_value_uint() {
    {
        uxs::db::value v(0u);
        VERIFY(v.type() == uxs::db::dtype::kUInteger);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(v.is_int());
        VERIFY(v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == false);
        VERIFY(v.as_int() == 0);
        VERIFY(v.as_uint() == 0);
        VERIFY(v.as_int64() == 0);
        VERIFY(v.as_uint64() == 0);
        VERIFY(v.as_float() == 0.f);
        VERIFY(v.as_double() == 0.);
        VERIFY(v.as_string() == "0");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(123u);
        VERIFY(v.type() == uxs::db::dtype::kUInteger);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(v.is_int());
        VERIFY(v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        VERIFY(v.as_int() == 123);
        VERIFY(v.as_uint() == 123);
        VERIFY(v.as_int64() == 123);
        VERIFY(v.as_uint64() == 123);
        VERIFY(v.as_float() == 123.f);
        VERIFY(v.as_double() == 123.);
        VERIFY(v.as_string() == "123");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(3000000000u);
        VERIFY(v.type() == uxs::db::dtype::kUInteger);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        MUST_THROW(v.as_int());
        VERIFY(v.as_uint() == 3000000000u);
        VERIFY(v.as_int64() == 3000000000ll);
        VERIFY(v.as_uint64() == 3000000000ull);
        VERIFY(v.as_float() == 3.e+9f);
        VERIFY(v.as_double() == 3.e+9);
        VERIFY(v.as_string() == "3000000000");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(123u);
        VERIFY(&v[0] == &v);
        MUST_THROW(v.at(1));
        MUST_THROW(v["0"]);
    }
    return 0;
}

int test_string_value_int64() {
    {
        uxs::db::value v(int64_t(0));
        VERIFY(v.type() == uxs::db::dtype::kInteger64);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(v.is_int());
        VERIFY(v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == false);
        VERIFY(v.as_int() == 0);
        VERIFY(v.as_uint() == 0);
        VERIFY(v.as_int64() == 0);
        VERIFY(v.as_uint64() == 0);
        VERIFY(v.as_float() == 0.f);
        VERIFY(v.as_double() == 0.);
        VERIFY(v.as_string() == "0");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(int64_t(123));
        VERIFY(v.type() == uxs::db::dtype::kInteger64);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(v.is_int());
        VERIFY(v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        VERIFY(v.as_int() == 123);
        VERIFY(v.as_uint() == 123);
        VERIFY(v.as_int64() == 123);
        VERIFY(v.as_uint64() == 123);
        VERIFY(v.as_float() == 123.f);
        VERIFY(v.as_double() == 123.);
        VERIFY(v.as_string() == "123");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(int64_t(-123));
        VERIFY(v.type() == uxs::db::dtype::kInteger64);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(!v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        VERIFY(v.as_int() == -123);
        MUST_THROW(v.as_uint());
        VERIFY(v.as_int64() == -123);
        MUST_THROW(v.as_uint64());
        VERIFY(v.as_float() == -123.f);
        VERIFY(v.as_double() == -123.);
        VERIFY(v.as_string() == "-123");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(int64_t(3000000000ll));
        VERIFY(v.type() == uxs::db::dtype::kInteger64);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        MUST_THROW(v.as_int());
        VERIFY(v.as_uint() == 3000000000u);
        VERIFY(v.as_int64() == 3000000000ll);
        VERIFY(v.as_uint64() == 3000000000ull);
        VERIFY(v.as_float() == 3.e+9f);
        VERIFY(v.as_double() == 3.e+9);
        VERIFY(v.as_string() == "3000000000");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(int64_t(-3000000000ll));
        VERIFY(v.type() == uxs::db::dtype::kInteger64);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(!v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        MUST_THROW(v.as_int());
        MUST_THROW(v.as_uint());
        VERIFY(v.as_int64() == -3000000000ll);
        MUST_THROW(v.as_uint64());
        VERIFY(v.as_float() == -3.e+9f);
        VERIFY(v.as_double() == -3.e+9);
        VERIFY(v.as_string() == "-3000000000");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(int64_t(1000000000000000ll));
        VERIFY(v.type() == uxs::db::dtype::kInteger64);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        MUST_THROW(v.as_int());
        MUST_THROW(v.as_uint());
        VERIFY(v.as_int64() == 1000000000000000ll);
        VERIFY(v.as_uint64() == 1000000000000000ull);
        VERIFY(v.as_float() == 1.e+15f);
        VERIFY(v.as_double() == 1.e+15);
        VERIFY(v.as_string() == "1000000000000000");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(int64_t(-1000000000000000ll));
        VERIFY(v.type() == uxs::db::dtype::kInteger64);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(!v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        MUST_THROW(v.as_int());
        MUST_THROW(v.as_uint());
        VERIFY(v.as_int64() == -1000000000000000ll);
        MUST_THROW(v.as_uint64());
        VERIFY(v.as_float() == -1.e+15f);
        VERIFY(v.as_double() == -1.e+15);
        VERIFY(v.as_string() == "-1000000000000000");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(int64_t(123));
        VERIFY(&v[0] == &v);
        MUST_THROW(v.at(1));
        MUST_THROW(v["0"]);
    }
    return 0;
}

int test_string_value_uint64() {
    {
        uxs::db::value v(uint64_t(0u));
        VERIFY(v.type() == uxs::db::dtype::kUInteger64);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(v.is_int());
        VERIFY(v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == false);
        VERIFY(v.as_int() == 0);
        VERIFY(v.as_uint() == 0);
        VERIFY(v.as_int64() == 0);
        VERIFY(v.as_uint64() == 0);
        VERIFY(v.as_float() == 0.f);
        VERIFY(v.as_double() == 0.);
        VERIFY(v.as_string() == "0");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(uint64_t(123u));
        VERIFY(v.type() == uxs::db::dtype::kUInteger64);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(v.is_int());
        VERIFY(v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        VERIFY(v.as_int() == 123);
        VERIFY(v.as_uint() == 123);
        VERIFY(v.as_int64() == 123);
        VERIFY(v.as_uint64() == 123);
        VERIFY(v.as_float() == 123.f);
        VERIFY(v.as_double() == 123.);
        VERIFY(v.as_string() == "123");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(uint64_t(3000000000u));
        VERIFY(v.type() == uxs::db::dtype::kUInteger64);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        MUST_THROW(v.as_int());
        VERIFY(v.as_uint() == 3000000000u);
        VERIFY(v.as_int64() == 3000000000ll);
        VERIFY(v.as_uint64() == 3000000000ull);
        VERIFY(v.as_float() == 3.e+9f);
        VERIFY(v.as_double() == 3.e+9);
        VERIFY(v.as_string() == "3000000000");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(uint64_t(1000000000000000u));
        VERIFY(v.type() == uxs::db::dtype::kUInteger64);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        MUST_THROW(v.as_int());
        MUST_THROW(v.as_uint());
        VERIFY(v.as_int64() == 1000000000000000ll);
        VERIFY(v.as_uint64() == 1000000000000000ull);
        VERIFY(v.as_float() == 1.e+15f);
        VERIFY(v.as_double() == 1.e+15);
        VERIFY(v.as_string() == "1000000000000000");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(uint64_t(10000000000000000000ull));
        VERIFY(v.type() == uxs::db::dtype::kUInteger64);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(!v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        MUST_THROW(v.as_int());
        MUST_THROW(v.as_uint());
        MUST_THROW(v.as_int64());
        VERIFY(v.as_uint64() == 10000000000000000000ull);
        VERIFY(v.as_float() == 1.e+19f);
        VERIFY(v.as_double() == 1.e+19);
        VERIFY(v.as_string() == "10000000000000000000");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(int64_t(123u));
        VERIFY(&v[0] == &v);
        MUST_THROW(v.at(1));
        MUST_THROW(v["0"]);
    }
    return 0;
}

int test_string_value_double() {
    {
        uxs::db::value v(0.);
        VERIFY(v.type() == uxs::db::dtype::kDouble);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(v.is_int());
        VERIFY(v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == false);
        VERIFY(v.as_int() == 0);
        VERIFY(v.as_uint() == 0);
        VERIFY(v.as_int64() == 0);
        VERIFY(v.as_uint64() == 0);
        VERIFY(v.as_float() == 0.f);
        VERIFY(v.as_double() == 0.);
        VERIFY(v.as_string() == "0.0");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(123.);
        VERIFY(v.type() == uxs::db::dtype::kDouble);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(v.is_int());
        VERIFY(v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        VERIFY(v.as_int() == 123);
        VERIFY(v.as_uint() == 123);
        VERIFY(v.as_int64() == 123);
        VERIFY(v.as_uint64() == 123);
        VERIFY(v.as_float() == 123.f);
        VERIFY(v.as_double() == 123.);
        VERIFY(v.as_string() == "123.0");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(123.5);
        VERIFY(v.type() == uxs::db::dtype::kDouble);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(!v.is_int64());
        VERIFY(!v.is_uint64());
        VERIFY(!v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        VERIFY(v.as_int() == 123);
        VERIFY(v.as_uint() == 123);
        VERIFY(v.as_int64() == 123);
        VERIFY(v.as_uint64() == 123);
        VERIFY(v.as_float() == 123.5f);
        VERIFY(v.as_double() == 123.5);
        VERIFY(v.as_string() == "123.5");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(-123.);
        VERIFY(v.type() == uxs::db::dtype::kDouble);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(!v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        VERIFY(v.as_int() == -123);
        MUST_THROW(v.as_uint());
        VERIFY(v.as_int64() == -123);
        MUST_THROW(v.as_uint64());
        VERIFY(v.as_float() == -123.f);
        VERIFY(v.as_double() == -123.);
        VERIFY(v.as_string() == "-123.0");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(3.e+9);
        VERIFY(v.type() == uxs::db::dtype::kDouble);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        MUST_THROW(v.as_int());
        VERIFY(v.as_uint() == 3000000000u);
        VERIFY(v.as_int64() == 3000000000ll);
        VERIFY(v.as_uint64() == 3000000000ull);
        VERIFY(v.as_float() == 3.e+9f);
        VERIFY(v.as_double() == 3.e+9);
        VERIFY(v.as_string() == "3.0e+09");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(-3.e+9);
        VERIFY(v.type() == uxs::db::dtype::kDouble);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(!v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        MUST_THROW(v.as_int());
        MUST_THROW(v.as_uint());
        VERIFY(v.as_int64() == -3000000000ll);
        MUST_THROW(v.as_uint64());
        VERIFY(v.as_float() == -3.e+9f);
        VERIFY(v.as_double() == -3.e+9);
        VERIFY(v.as_string() == "-3.0e+09");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(1.e+15);
        VERIFY(v.type() == uxs::db::dtype::kDouble);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        MUST_THROW(v.as_int());
        MUST_THROW(v.as_uint());
        VERIFY(v.as_int64() == 1000000000000000ll);
        VERIFY(v.as_uint64() == 1000000000000000ull);
        VERIFY(v.as_float() == 1.e+15f);
        VERIFY(v.as_double() == 1.e+15);
        VERIFY(v.as_string() == "1.0e+15");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(-1.e+15);
        VERIFY(v.type() == uxs::db::dtype::kDouble);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(v.is_int64());
        VERIFY(!v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        MUST_THROW(v.as_int());
        MUST_THROW(v.as_uint());
        VERIFY(v.as_int64() == -1000000000000000ll);
        MUST_THROW(v.as_uint64());
        VERIFY(v.as_float() == -1.e+15f);
        VERIFY(v.as_double() == -1.e+15);
        VERIFY(v.as_string() == "-1.0e+15");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(1.e+19);
        VERIFY(v.type() == uxs::db::dtype::kDouble);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(!v.is_int64());
        VERIFY(v.is_uint64());
        VERIFY(v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        MUST_THROW(v.as_int());
        MUST_THROW(v.as_uint());
        MUST_THROW(v.as_int64());
        VERIFY(v.as_uint64() == 10000000000000000000ull);
        VERIFY(v.as_float() == 1.e+19f);
        VERIFY(v.as_double() == 1.e+19);
        VERIFY(v.as_string() == "1.0e+19");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(-1.e+19);
        VERIFY(v.type() == uxs::db::dtype::kDouble);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(!v.is_int64());
        VERIFY(!v.is_uint64());
        VERIFY(!v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        MUST_THROW(v.as_int());
        MUST_THROW(v.as_uint());
        MUST_THROW(v.as_int64());
        MUST_THROW(v.as_uint64());
        VERIFY(v.as_float() == -1.e+19f);
        VERIFY(v.as_double() == -1.e+19);
        VERIFY(v.as_string() == "-1.0e+19");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(1.e+25);
        VERIFY(v.type() == uxs::db::dtype::kDouble);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(!v.is_int64());
        VERIFY(!v.is_uint64());
        VERIFY(!v.is_integral());
        VERIFY(v.is_float());
        VERIFY(v.is_double());
        VERIFY(v.is_numeric());
        VERIFY(!v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        VERIFY(v.as_bool() == true);
        MUST_THROW(v.as_int());
        MUST_THROW(v.as_uint());
        MUST_THROW(v.as_int64());
        MUST_THROW(v.as_uint64());
        VERIFY(v.as_float() == 1.e+25f);
        VERIFY(v.as_double() == 1.e+25);
        VERIFY(v.as_string() == "1.0e+25");
        MUST_THROW(v.as_string_view());
    }
    {
        uxs::db::value v(123.);
        VERIFY(&v[0] == &v);
        MUST_THROW(v.at(1));
        MUST_THROW(v["0"]);
    }
    return 0;
}

int test_string_value_string() {
    {
        uxs::db::value v("hello");
        VERIFY(v.type() == uxs::db::dtype::kString);
        VERIFY(!v.is_null());
        VERIFY(!v.is_bool());
        VERIFY(!v.is_int());
        VERIFY(!v.is_uint());
        VERIFY(!v.is_int64());
        VERIFY(!v.is_uint64());
        VERIFY(!v.is_integral());
        VERIFY(!v.is_float());
        VERIFY(!v.is_double());
        VERIFY(!v.is_numeric());
        VERIFY(v.is_string());
        VERIFY(!v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 1);
        MUST_THROW(v.as_bool());
        MUST_THROW(v.as_int());
        MUST_THROW(v.as_uint());
        MUST_THROW(v.as_int64());
        MUST_THROW(v.as_uint64());
        MUST_THROW(v.as_float());
        MUST_THROW(v.as_double());
        VERIFY(v.as_string() == "hello");
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v("hello");
        VERIFY(&v[0] == &v);
        MUST_THROW(v.at(1));
        MUST_THROW(v["0"]);
    }
    {
        uxs::db::value v("");
        VERIFY(v.type() == uxs::db::dtype::kString);
        v = "";
        VERIFY(v.as_string_view() == "");
        v = "hello";
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v("");
        VERIFY(v.type() == uxs::db::dtype::kString);
        v = uxs::db::value("");
        VERIFY(v.as_string_view() == "");
        v = uxs::db::value("hello");
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v(123);
        VERIFY(v.type() == uxs::db::dtype::kInteger);
        v = "";
        VERIFY(v.as_string_view() == "");
        v = "hello";
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v(123);
        VERIFY(v.type() == uxs::db::dtype::kInteger);
        v = uxs::db::value("");
        VERIFY(v.as_string_view() == "");
        v = uxs::db::value("hello");
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v("");
        VERIFY(v.type() == uxs::db::dtype::kString);
        v = "hello";
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v("");
        VERIFY(v.type() == uxs::db::dtype::kString);
        v = uxs::db::value("hello");
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v(123);
        VERIFY(v.type() == uxs::db::dtype::kInteger);
        v = "hello";
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v(123);
        VERIFY(v.type() == uxs::db::dtype::kInteger);
        v = uxs::db::value("hello");
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v("hello");
        VERIFY(v.as_string_view() == "hello");
        v = "HELLO";
        VERIFY(v.as_string_view() == "HELLO");
        v = "HELLO-MORE";
        VERIFY(v.as_string_view() == "HELLO-MORE");
        v = "HE";
        VERIFY(v.as_string_view() == "HE");
        v = "";
        VERIFY(v.as_string_view() == "");
        v = "ME";
        VERIFY(v.as_string_view() == "ME");
        v = "HELLO-MORE-MORE-MORE-MORE-MORE-MORE-MORE";
        VERIFY(v.as_string_view() == "HELLO-MORE-MORE-MORE-MORE-MORE-MORE-MORE");
        v = 123;
        VERIFY(v.type() == uxs::db::dtype::kInteger);
    }
    {
        uxs::db::value v("hello");
        VERIFY(v.as_string_view() == "hello");
        v = uxs::db::value("HELLO");
        VERIFY(v.as_string_view() == "HELLO");
        v = uxs::db::value("HELLO-MORE");
        VERIFY(v.as_string_view() == "HELLO-MORE");
        v = uxs::db::value("HE");
        VERIFY(v.as_string_view() == "HE");
        v = uxs::db::value("");
        VERIFY(v.as_string_view() == "");
        v = uxs::db::value("ME");
        VERIFY(v.as_string_view() == "ME");
        v = uxs::db::value("HELLO-MORE-MORE-MORE-MORE-MORE-MORE-MORE");
        VERIFY(v.as_string_view() == "HELLO-MORE-MORE-MORE-MORE-MORE-MORE-MORE");
        v = uxs::db::value(123);
        VERIFY(v.type() == uxs::db::dtype::kInteger);
    }
    {
        uxs::db::value v("hello");
        VERIFY(v.as_string_view() == "hello");
        v.append_string("-hello");
        VERIFY(v.as_string_view() == "hello-hello");
        v.append_string("-hello");
        VERIFY(v.as_string_view() == "hello-hello-hello");
        v.append_string("-hello");
        VERIFY(v.as_string_view() == "hello-hello-hello-hello");
        v.append_string("-hello");
        VERIFY(v.as_string_view() == "hello-hello-hello-hello-hello");
        v.append_string("-hello");
        VERIFY(v.as_string_view() == "hello-hello-hello-hello-hello-hello");
    }
    {
        uxs::db::value v("");
        v.append_string("hello");
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v;
        v.append_string("hello");
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v = 1;
        MUST_THROW(v.append_string("hello"));
    }
    VERIFY(uxs::db::value("123").as_int() == 123);
    VERIFY(uxs::db::value("-123").as_int() == -123);
    VERIFY(uxs::db::value("123").as_uint() == 123);
    VERIFY(uxs::db::value("1000000000000000").as_int64() == 1000000000000000ll);
    VERIFY(uxs::db::value("-1000000000000000").as_int64() == -1000000000000000ll);
    VERIFY(uxs::db::value("10000000000000000000").as_uint64() == 10000000000000000000ull);
    VERIFY(uxs::db::value("12.334567").as_double() == 12.334567);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_string_value_null);
ADD_TEST_CASE("", "db::value", test_string_value_bool);
ADD_TEST_CASE("", "db::value", test_string_value_int);
ADD_TEST_CASE("", "db::value", test_string_value_uint);
ADD_TEST_CASE("", "db::value", test_string_value_int64);
ADD_TEST_CASE("", "db::value", test_string_value_uint64);
ADD_TEST_CASE("", "db::value", test_string_value_double);
ADD_TEST_CASE("", "db::value", test_string_value_string);
