#include "uxs/db/json.h"
#include "uxs/io/filebuf.h"
#include "uxs/io/istringbuf.h"
#include "uxs/io/ostringbuf.h"

#include "test_suite.h"

#define MUST_THROW(x) \
    try { \
        x; \
        VERIFY(false); \
    } catch (const uxs::db::exception&) {}

extern std::string g_testdata_path;

namespace {

int test_string_value_null() {
    uxs::db::value v;
    VERIFY(v.type() == uxs::db::value::dtype::kNull);
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
    MUST_THROW(v[0]);
    VERIFY(v["0"].is_null());
    VERIFY(v.is_record());
    return 0;
}

int test_string_value_bool() {
    {
        uxs::db::value v = true;
        VERIFY(v.type() == uxs::db::value::dtype::kBoolean);
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
        uxs::db::value v = false;
        VERIFY(v.type() == uxs::db::value::dtype::kBoolean);
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
        uxs::db::value v = true;
        VERIFY(&v[0] == &v);
        MUST_THROW(v[1]);
        MUST_THROW(v["0"]);
    }
    return 0;
}

int test_string_value_int() {
    {
        uxs::db::value v = 0;
        VERIFY(v.type() == uxs::db::value::dtype::kInteger);
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
        uxs::db::value v = 123;
        VERIFY(v.type() == uxs::db::value::dtype::kInteger);
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
        uxs::db::value v = -123;
        VERIFY(v.type() == uxs::db::value::dtype::kInteger);
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
        uxs::db::value v = 123;
        VERIFY(&v[0] == &v);
        MUST_THROW(v[1]);
        MUST_THROW(v["0"]);
    }
    return 0;
}

int test_string_value_uint() {
    {
        uxs::db::value v = 0u;
        VERIFY(v.type() == uxs::db::value::dtype::kUInteger);
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
        uxs::db::value v = 123u;
        VERIFY(v.type() == uxs::db::value::dtype::kUInteger);
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
        uxs::db::value v = 3000000000u;
        VERIFY(v.type() == uxs::db::value::dtype::kUInteger);
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
        uxs::db::value v = 123u;
        VERIFY(&v[0] == &v);
        MUST_THROW(v[1]);
        MUST_THROW(v["0"]);
    }
    return 0;
}

int test_string_value_int64() {
    {
        uxs::db::value v = int64_t(0);
        VERIFY(v.type() == uxs::db::value::dtype::kInteger64);
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
        uxs::db::value v = int64_t(123);
        VERIFY(v.type() == uxs::db::value::dtype::kInteger64);
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
        uxs::db::value v = int64_t(-123);
        VERIFY(v.type() == uxs::db::value::dtype::kInteger64);
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
        uxs::db::value v = int64_t(3000000000ll);
        VERIFY(v.type() == uxs::db::value::dtype::kInteger64);
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
        uxs::db::value v = int64_t(-3000000000ll);
        VERIFY(v.type() == uxs::db::value::dtype::kInteger64);
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
        uxs::db::value v = int64_t(1000000000000000ll);
        VERIFY(v.type() == uxs::db::value::dtype::kInteger64);
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
        uxs::db::value v = int64_t(-1000000000000000ll);
        VERIFY(v.type() == uxs::db::value::dtype::kInteger64);
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
        uxs::db::value v = int64_t(123);
        VERIFY(&v[0] == &v);
        MUST_THROW(v[1]);
        MUST_THROW(v["0"]);
    }
    return 0;
}

int test_string_value_uint64() {
    {
        uxs::db::value v = uint64_t(0u);
        VERIFY(v.type() == uxs::db::value::dtype::kUInteger64);
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
        uxs::db::value v = uint64_t(123u);
        VERIFY(v.type() == uxs::db::value::dtype::kUInteger64);
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
        uxs::db::value v = uint64_t(3000000000u);
        VERIFY(v.type() == uxs::db::value::dtype::kUInteger64);
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
        uxs::db::value v = uint64_t(1000000000000000u);
        VERIFY(v.type() == uxs::db::value::dtype::kUInteger64);
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
        uxs::db::value v = uint64_t(10000000000000000000ull);
        VERIFY(v.type() == uxs::db::value::dtype::kUInteger64);
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
        uxs::db::value v = int64_t(123u);
        VERIFY(&v[0] == &v);
        MUST_THROW(v[1]);
        MUST_THROW(v["0"]);
    }
    return 0;
}

int test_string_value_double() {
    {
        uxs::db::value v = 0.;
        VERIFY(v.type() == uxs::db::value::dtype::kDouble);
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
        uxs::db::value v = 123.;
        VERIFY(v.type() == uxs::db::value::dtype::kDouble);
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
        uxs::db::value v = 123.5;
        VERIFY(v.type() == uxs::db::value::dtype::kDouble);
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
        uxs::db::value v = -123.;
        VERIFY(v.type() == uxs::db::value::dtype::kDouble);
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
        uxs::db::value v = 3.e+9;
        VERIFY(v.type() == uxs::db::value::dtype::kDouble);
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
        uxs::db::value v = -3.e+9;
        VERIFY(v.type() == uxs::db::value::dtype::kDouble);
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
        uxs::db::value v = 1.e+15;
        VERIFY(v.type() == uxs::db::value::dtype::kDouble);
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
        uxs::db::value v = -1.e+15;
        VERIFY(v.type() == uxs::db::value::dtype::kDouble);
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
        uxs::db::value v = 1.e+19;
        VERIFY(v.type() == uxs::db::value::dtype::kDouble);
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
        uxs::db::value v = -1.e+19;
        VERIFY(v.type() == uxs::db::value::dtype::kDouble);
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
        uxs::db::value v = 1.e+25;
        VERIFY(v.type() == uxs::db::value::dtype::kDouble);
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
        uxs::db::value v = 123.;
        VERIFY(&v[0] == &v);
        MUST_THROW(v[1]);
        MUST_THROW(v["0"]);
    }
    return 0;
}

int test_string_value_string() {
    {
        uxs::db::value v = "hello";
        VERIFY(v.type() == uxs::db::value::dtype::kString);
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
        uxs::db::value v("");
        VERIFY(v.type() == uxs::db::value::dtype::kString);
        v = "";
        VERIFY(v.as_string_view() == "");
        v = "hello";
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v("");
        VERIFY(v.type() == uxs::db::value::dtype::kString);
        v = uxs::db::value("");
        VERIFY(v.as_string_view() == "");
        v = uxs::db::value("hello");
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v = 123;
        VERIFY(v.type() == uxs::db::value::dtype::kInteger);
        v = "";
        VERIFY(v.as_string_view() == "");
        v = "hello";
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v = 123;
        VERIFY(v.type() == uxs::db::value::dtype::kInteger);
        v = uxs::db::value("");
        VERIFY(v.as_string_view() == "");
        v = uxs::db::value("hello");
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v("");
        VERIFY(v.type() == uxs::db::value::dtype::kString);
        v = "hello";
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v("");
        VERIFY(v.type() == uxs::db::value::dtype::kString);
        v = uxs::db::value("hello");
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v = 123;
        VERIFY(v.type() == uxs::db::value::dtype::kInteger);
        v = "hello";
        VERIFY(v.as_string_view() == "hello");
    }
    {
        uxs::db::value v = 123;
        VERIFY(v.type() == uxs::db::value::dtype::kInteger);
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
        v = "HELLO-MORE-MORE";
        VERIFY(v.as_string_view() == "HELLO-MORE-MORE");
        v = 123;
        VERIFY(v.type() == uxs::db::value::dtype::kInteger);
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
        v = uxs::db::value("HELLO-MORE-MORE");
        VERIFY(v.as_string_view() == "HELLO-MORE-MORE");
        v = uxs::db::value(123);
        VERIFY(v.type() == uxs::db::value::dtype::kInteger);
    }
    {
        uxs::db::value v("hello");
        VERIFY(v.as_string_view() == "hello");
        v.push_back('-');
        v.append("hello");
        VERIFY(v.as_string_view() == "hello-hello");
        v.push_back('-');
        v.append("hello");
        VERIFY(v.as_string_view() == "hello-hello-hello");
        v.push_back('-');
        v.append("hello");
        VERIFY(v.as_string_view() == "hello-hello-hello-hello");
    }
    {
        uxs::db::value v("");
        v.append("hello");
        VERIFY(v.as_string_view() == "hello");
        v.push_back('-');
        v.append("hello");
        VERIFY(v.as_string_view() == "hello-hello");
    }
    {
        uxs::db::value v("");
        v.push_back('-');
        v.append("hello");
        VERIFY(v.as_string_view() == "-hello");
        v.push_back('-');
        v.append("hello");
        VERIFY(v.as_string_view() == "-hello-hello");
    }
    VERIFY(uxs::db::value("123").as_int() == 123);
    VERIFY(uxs::db::value("-123").as_int() == -123);
    VERIFY(uxs::db::value("123").as_uint() == 123);
    VERIFY(uxs::db::value("1000000000000000").as_int64() == 1000000000000000ll);
    VERIFY(uxs::db::value("-1000000000000000").as_int64() == -1000000000000000ll);
    VERIFY(uxs::db::value("10000000000000000000").as_uint64() == 10000000000000000000ull);
    VERIFY(uxs::db::value("12.334567").as_double() == 12.334567);
    {
        uxs::db::value v = "hello";
        VERIFY(&v[0] == &v);
        MUST_THROW(v[1]);
        MUST_THROW(v["0"]);
    }
    return 0;
}

bool compare_array(const uxs::db::value& v, std::string_view s) {
    if (!v.is_array()) { return false; }
    if (v.size() != s.size()) { return false; }
    size_t n = 0;
    for (const uxs::db::value& el : v.as_array()) {
        if (!el.is_string()) { return false; }
        if (el.as_string_view() != std::string_view(&s[n++], 1)) { return false; }
    }
    return true;
}

uxs::db::value make_array(std::string_view s) {
    uxs::db::value v;
    if (!s.empty()) {
        for (char ch : s) { v.push_back(std::string_view(&ch, 1)); }
    } else {
        v.convert(uxs::db::value::dtype::kArray);
    }
    return v;
}

int test_string_value_array() {
    {
        uxs::db::value v = make_array("hello");
        VERIFY(v.type() == uxs::db::value::dtype::kArray);
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
        VERIFY(!v.is_string());
        VERIFY(v.is_array());
        VERIFY(!v.is_record());
        VERIFY(!v.empty());
        VERIFY(v.size() == 5);
        MUST_THROW(v.as_bool());
        MUST_THROW(v.as_int());
        MUST_THROW(v.as_uint());
        MUST_THROW(v.as_int64());
        MUST_THROW(v.as_uint64());
        MUST_THROW(v.as_float());
        MUST_THROW(v.as_double());
        MUST_THROW(v.as_string());
        MUST_THROW(v.as_string_view());
        VERIFY(compare_array(v, "hello"));
    }
    {
        uxs::db::value v = make_array("");
        VERIFY(v.type() == uxs::db::value::dtype::kArray);
        v = make_array("");
        VERIFY(compare_array(v, ""));
        v = make_array("hello");
        VERIFY(compare_array(v, "hello"));
    }
    {
        uxs::db::value v = "123";
        VERIFY(v.type() == uxs::db::value::dtype::kString);
        v = make_array("");
        VERIFY(compare_array(v, ""));
        v = make_array("hello");
        VERIFY(compare_array(v, "hello"));
    }
    {
        uxs::db::value v = make_array("");
        VERIFY(v.type() == uxs::db::value::dtype::kArray);
        v = make_array("hello");
        VERIFY(compare_array(v, "hello"));
    }
    {
        uxs::db::value v = "123";
        VERIFY(v.type() == uxs::db::value::dtype::kString);
        VERIFY(v.as_string() == "123");
        v = make_array("hello");
        VERIFY(compare_array(v, "hello"));
    }
    {
        uxs::db::value v = make_array("hello");
        VERIFY(compare_array(v, "hello"));
        v = make_array("HELLO");
        VERIFY(compare_array(v, "HELLO"));
        v = make_array("HELLO-MORE");
        VERIFY(compare_array(v, "HELLO-MORE"));
        v = make_array("HE");
        VERIFY(compare_array(v, "HE"));
        v = make_array("");
        VERIFY(compare_array(v, ""));
        v = make_array("ME");
        VERIFY(compare_array(v, "ME"));
        v = make_array("HELLO-MORE-MORE");
        VERIFY(compare_array(v, "HELLO-MORE-MORE"));
        v = "123";
        VERIFY(v.type() == uxs::db::value::dtype::kString);
        VERIFY(v.as_string() == "123");
        v = 123;
        VERIFY(v.type() == uxs::db::value::dtype::kInteger);
        VERIFY(v.as_string() == "123");
    }
    {
        uxs::db::value v = "hello";
        VERIFY(&v[0] == &v);
        MUST_THROW(v[1]);
        MUST_THROW(v["0"]);
    }
    return 0;
}

int test_string_json_1() {
    std::string str{
        "{\n"
        "    \"array_of_strings\": [\"1\", \"2\", \"3\"],\n"
        "    \"null\": null,\n"
        "    \"object\": {\n"
        "        \"array_of_i32\": [1, 2, 3],\n"
        "        \"bool_val\": true,\n"
        "        \"d_val\": 12.5646,\n"
        "        \"i32_val\": 123456,\n"
        "        \"mixed_array\": [1, {\n"
        "            \"F\": false,\n"
        "            \"T\": true\n"
        "        }, \"3\"],\n"
        "        \"str_val\": \"hello\"\n"
        "    }\n"
        "}"};

    uxs::istringbuf input(str);
    uxs::db::value root;
    VERIFY(root = uxs::db::json::reader(input).read());
    VERIFY(root["array_of_strings"][0].as_string() == "1");
    VERIFY(root["array_of_strings"][1].as_string() == "2");
    VERIFY(root["array_of_strings"][2].as_string() == "3");
    VERIFY(root["object"]["array_of_i32"][0].as_int() == 1);
    VERIFY(root["object"]["array_of_i32"][1].as_int() == 2);
    VERIFY(root["object"]["array_of_i32"][2].as_int() == 3);
    VERIFY(root["object"]["d_val"].as_double() == 12.5646);
    VERIFY(root["object"]["i32_val"].as_int() == 123456);
    VERIFY(root["object"]["mixed_array"][0].as_int() == 1);
    VERIFY(root["object"]["mixed_array"][1]["T"].as_bool() == true);
    VERIFY(root["object"]["mixed_array"][1]["F"].as_bool() == false);
    VERIFY(root["object"]["mixed_array"][2].as_string() == "3");
    VERIFY(root["object"]["str_val"].as_string() == "hello");
    VERIFY(root["object"]["bool_val"].as_bool() == true);
    VERIFY(root["null"].is_null());

    uxs::ostringbuf output;
    uxs::db::json::writer(output).write(root);
    VERIFY(output.str() == str);
    return 0;
}

int test_string_json_2() {
    static const std::string_view file_names[] = {
        "fail2.json",
        "fail3.json",
        "fail4.json",
        "fail5.json",
        "fail6.json",
        "fail9.json",
        "fail11.json",
        "fail12.json",
        "fail13.json",
        "fail14.json",
        "fail15.json",
        "fail16.json",
        "fail17.json",
        "fail19.json",
        "fail20.json",
        "fail21.json",
        "fail22.json",
        "fail23.json",
        "fail24.json",
        "fail26.json",
        "fail27.json",
        "fail28.json",
        "fail29.json",
        "fail30.json",
        "fail31.json",
        "fail32.json",
        "fail33.json",
        "pass1.json",
        "pass2.json",
        "pass3.json",
        "fail_invalid_quote.json",
        "fail_test_array_01.json",
        "fail_test_array_02.json",
        "fail_test_object_01.json",
        "legacy_test_array_01.json",
        "legacy_test_array_02.json",
        "legacy_test_array_03.json",
        "legacy_test_array_04.json",
        "legacy_test_array_05.json",
        "legacy_test_array_06.json",
        "legacy_test_array_07.json",
        "legacy_test_basic_01.json",
        "legacy_test_basic_02.json",
        "legacy_test_basic_03.json",
        "legacy_test_basic_04.json",
        "legacy_test_basic_05.json",
        "legacy_test_basic_06.json",
        "legacy_test_basic_07.json",
        "legacy_test_basic_08.json",
        "legacy_test_basic_09.json",
        "legacy_test_comment_00.json",
        "legacy_test_comment_01.json",
        "legacy_test_comment_02.json",
        "legacy_test_complex_01.json",
        "legacy_test_integer_01.json",
        "legacy_test_integer_02.json",
        "legacy_test_integer_03.json",
        "legacy_test_integer_04.json",
        "legacy_test_integer_05.json",
        "legacy_test_integer_06_64bits.json",
        "legacy_test_integer_07_64bits.json",
        "legacy_test_integer_08_64bits.json",
        "legacy_test_large_01.json",
        "legacy_test_object_01.json",
        "legacy_test_object_02.json",
        "legacy_test_object_03.json",
        "legacy_test_object_04.json",
        "legacy_test_real_01.json",
        "legacy_test_real_02.json",
        "legacy_test_real_03.json",
        "legacy_test_real_04.json",
        "legacy_test_real_05.json",
        "legacy_test_real_06.json",
        "legacy_test_real_07.json",
        "legacy_test_real_08.json",
        "legacy_test_real_09.json",
        "legacy_test_real_10.json",
        "legacy_test_real_11.json",
        "legacy_test_real_12.json",
        "legacy_test_real_13.json",
        "legacy_test_string_01.json",
        "legacy_test_string_02.json",
        "legacy_test_string_03.json",
        "legacy_test_string_04.json",
        "legacy_test_string_05.json",
        "legacy_test_string_unicode_01.json",
        "legacy_test_string_unicode_02.json",
        "legacy_test_string_unicode_03.json",
        "legacy_test_string_unicode_04.json",
        "legacy_test_string_unicode_05.json",
        "ext-invalid-0000.json",
        "ext-invalid-0001.json",
        "ext-invalid-0002.json",
        "ext-valid-0000.json",
        "ext-valid-0001.json",
        "ext-valid-0002.json",
        "ext-valid-0003.json",
        "invalid-0000.json",
        "invalid-0001.json",
        "invalid-0002.json",
        "invalid-0003.json",
        "invalid-0004.json",
        "invalid-0005.json",
        "invalid-0006.json",
        "invalid-0007.json",
        "invalid-0008.json",
        "invalid-0009.json",
        "invalid-0010.json",
        "valid-0000.json",
        "valid-0001.json",
        "valid-0002.json",
        "valid-0003.json",
        "valid-0004.json",
        "valid-0005.json",
        "valid-0006.json",
        "valid-0007.json",
        "valid-0008.json",
        "valid-0009.json",
        "valid-0010.json",
        "valid-0011.json",
        "valid-0012.json",
        "valid-0013.json",
        "valid-0014.json",
        "valid-0015.json",
    };

    for (std::string_view file_name : file_names) {
        uxs::db::value root;

        bool is_valid = file_name.find("invalid") == std::string::npos && file_name.find("fail") == std::string::npos;

        std::string full_file_name = g_testdata_path;
        full_file_name += "json/";
        full_file_name += file_name;

        try {
            {  // read
                uxs::filebuf ifile(full_file_name.c_str(), "r");
                VERIFY(ifile);
                root = uxs::db::json::reader(ifile).read();
            }

            VERIFY(is_valid);

            std::string data;
            {  // write
                uxs::ostringbuf out;
                uxs::db::json::writer(out).write(root);
                data = out.str();
            }

            bool skip_round_trip = false;

            {  // check expected
                std::string expected_full_file_name = full_file_name;
                auto pos = expected_full_file_name.rfind('.');
                if (pos != std::string::npos) { expected_full_file_name.resize(pos); }
                expected_full_file_name += ".expected";

                uxs::filebuf ifile(expected_full_file_name.c_str(), "r");
                do {
                    bool nl = true;
                    char ch = ifile.get();
                    while (ifile && (ch != '.' || !nl)) {
                        nl = ch == '\n';
                        ch = ifile.get();
                    }
                    if (!ifile) { break; }
                    std::string val;
                    uxs::db::value* v = &root;
                    bool eol = false;
                    do {
                        switch (ch) {
                            case '[': {
                                size_t i = 0;
                                ch = ifile.get();
                                while (ifile && uxs::is_digit(ch)) {
                                    i = 10u * i + ch - '0';
                                    ch = ifile.get();
                                }
                                VERIFY(ch == ']');
                                v = &(*v)[i];
                                ch = ifile.get();
                            } break;
                            case '.': {
                                std::string name;
                                ch = ifile.get();
                                while (ifile && ch != '.' && ch != '[' && ch != '=' && ch != '\n') {
                                    name += ch;
                                    ch = ifile.get();
                                }
                                if (!name.empty()) {
                                    if (name == "$") { name = ""; }  // empty name marker
                                    v = &(*v)[name];
                                }
                            } break;
                            case '=': {
                                ch = ifile.get();
                                while (ifile && ch != '\n') {
                                    val += ch;
                                    ch = ifile.get();
                                }
                                eol = true;
                            } break;
                            default: VERIFY(false); break;
                        }
                    } while (ifile && !eol);

                    VERIFY(!val.empty());

                    if (val == "null") {
                        VERIFY(v->is_null());
                    } else if (val == "false") {
                        VERIFY(v->is_bool() && v->as_bool() == false);
                    } else if (val == "true") {
                        VERIFY(v->is_bool() && v->as_bool() == true);
                    } else if (val == "[]") {
                        VERIFY(v->is_array());
                    } else if (val == "{}") {
                        VERIFY(v->is_record());
                    } else if (val[0] == '\"' && val.size() >= 2) {
                        VERIFY(v->is_string() && v->as_string_view() == val.substr(1, val.size() - 2));
                    } else if (val.find_first_of(".eEin") != std::string::npos) {  // verify as double
                        VERIFY(v->is_double() && v->as_double() == uxs::from_string<double>(val));
                        // json format does not support `inf` and `nan`
                        skip_round_trip = val.find_first_of("in") != std::string::npos;
                    } else {
                        uint32_t u32;
                        if (uxs::stoval(val, u32) != 0) {  // verify as 32-bit
                            if (val[0] == '-') {
                                VERIFY(v->is_int() && v->as_int() == static_cast<int32_t>(u32));
                            } else {
                                VERIFY(v->is_uint() && v->as_uint() == u32);
                            }
                        } else {  // verify as 64-bit
                            uint64_t u64;
                            if (uxs::stoval(val, u64) != 0) {
                                if (val[0] == '-') {
                                    VERIFY(v->is_int64() && v->as_int64() == static_cast<int64_t>(u64));
                                } else {
                                    VERIFY(v->is_uint64() && v->as_uint64() == u64);
                                }
                            } else {  // verify as double
                                VERIFY(v->is_double() && v->as_double() == uxs::from_string<double>(val));
                            }
                        }
                    }
                } while (ifile);
            }

            if (!skip_round_trip) {  // round-trip
                uxs::istringbuf in(data);
                VERIFY(root == uxs::db::json::reader(in).read());
            }

            {
                std::string output_full_file_name = full_file_name + ".out";
                uxs::filebuf ofile(output_full_file_name.c_str(), "w");
                if (!ofile) { return -1; }
                ofile.write(data);
            }
        } catch (const uxs::db::exception& ex) {
            if (is_valid) { throw std::runtime_error(uxs::format("{}:{}", full_file_name, ex.what())); }
        }
    }
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "json value", test_string_value_null);
ADD_TEST_CASE("", "json value", test_string_value_bool);
ADD_TEST_CASE("", "json value", test_string_value_int);
ADD_TEST_CASE("", "json value", test_string_value_uint);
ADD_TEST_CASE("", "json value", test_string_value_int64);
ADD_TEST_CASE("", "json value", test_string_value_uint64);
ADD_TEST_CASE("", "json value", test_string_value_double);
ADD_TEST_CASE("", "json value", test_string_value_string);
ADD_TEST_CASE("", "json value", test_string_value_array);
ADD_TEST_CASE("", "json reader and writer", test_string_json_1);
ADD_TEST_CASE("", "json reader and writer", test_string_json_2);
