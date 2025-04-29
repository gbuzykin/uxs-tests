#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_not_an_array() {
    uxs::db::value v(1);
    v.clear();
    VERIFY(v.as_int() == 1);
    return 0;
}

int test_clear_empty() {
    uxs::db::value v = uxs::db::make_array();
    v.clear();
    CHECK_ARRAY_EMPTY(v);
    return 0;
}

int test_clear_not_empty() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    uxs::db::value v(init);
    v.clear();
    CHECK_ARRAY_EMPTY(v);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_an_array);
ADD_TEST_CASE("", "db::value", test_clear_empty);
ADD_TEST_CASE("", "db::value", test_clear_not_empty);
