#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_move_empty_to_empty() {
    uxs::db::value v = uxs::db::make_array(), v_from = uxs::db::make_array();
    v = std::move(v_from);
    VERIFY(v.as_array().data() == nullptr);
    CHECK_ARRAY_EMPTY(v);
    VERIFY(v_from.is_null());
    return 0;
}

int test_move_not_empty_to_empty() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    uxs::db::value v = uxs::db::make_array(), v_from(init);
    auto r = v_from.as_array();
    v = std::move(v_from);
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY(v, init.size(), init.begin());
    VERIFY(v_from.is_null());
    return 0;
}

int test_move_not_empty_to_not_empty() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    std::initializer_list<uxs::db::value> init2 = {"11", "12", "13", "14", "15", "16", "17"};
    uxs::db::value v(init), v_from(init2);
    auto r = v_from.as_array();
    v = std::move(v_from);
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY(v, init2.size(), init2.begin());
    VERIFY(v_from.is_null());
    return 0;
}

int test_move_empty_to_not_empty() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    uxs::db::value v(init), v_from = uxs::db::make_array();
    auto r = v_from.as_array();
    v = std::move(v_from);
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY_EMPTY(v);
    VERIFY(v_from.is_null());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_move_empty_to_empty);
ADD_TEST_CASE("", "db::value", test_move_not_empty_to_empty);
ADD_TEST_CASE("", "db::value", test_move_not_empty_to_not_empty);
ADD_TEST_CASE("", "db::value", test_move_empty_to_not_empty);
