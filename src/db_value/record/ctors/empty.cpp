#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_empty() {
    uxs::db::value v = uxs::db::make_record();
    VERIFY(!v.is_null());
    VERIFY(!v.is_bool());
    VERIFY(!v.is_int());
    VERIFY(!v.is_uint());
    VERIFY(!v.is_int64());
    VERIFY(!v.is_uint64());
    VERIFY(!v.is_integral());
    VERIFY(!v.is_numeric());
    VERIFY(!v.is_string());
    VERIFY(!v.is_array());
    VERIFY(v.is_record());
    MUST_THROW(v.as_bool());
    MUST_THROW(v.as_int());
    MUST_THROW(v.as_uint());
    MUST_THROW(v.as_int64());
    MUST_THROW(v.as_uint64());
    MUST_THROW(v.as_double());
    MUST_THROW(v.as_string());
    MUST_THROW(v.as_string_view());
    CHECK_RECORD_EMPTY(v);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_empty);
