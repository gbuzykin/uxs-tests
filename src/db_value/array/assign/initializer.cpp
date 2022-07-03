#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_initializer_empty_to_empty() {
    std::initializer_list<uxs::db::value> init;
    uxs::db::value v = uxs::db::make_array();
    v = init;
    CHECK_RECORD_EMPTY(v);
    return 0;
}

int test_initializer_not_empty_to_empty() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    uxs::db::value v = uxs::db::make_array();
    v = init;
    CHECK_ARRAY(v, init.size(), init.begin());
    return 0;
}

int test_initializer_more_no_realloc() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    std::initializer_list<uxs::db::value> init2 = {"11", "12", "13", "14", "15", "16", "17"};
    uxs::db::value v(init);
    v.reserve(10);
    auto r = v.as_array();
    v = init2;
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY(v, init2.size(), init2.begin());
    return 0;
}

int test_initializer_more_needs_realloc() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    std::initializer_list<uxs::db::value> init2 = {"11", "12", "13", "14", "15", "16", "17"};
    uxs::db::value v(init);
    auto r = v.as_array();
    v = init2;
    VERIFY(r.data() != v.as_array().data());
    CHECK_ARRAY(v, init2.size(), init2.begin());
    return 0;
}

int test_initializer_less() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5", "6", "7"};
    std::initializer_list<uxs::db::value> init2 = {"11", "12", "13", "14", "15"};
    uxs::db::value v(init);
    auto r = v.as_array();
    v = init2;
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY(v, init2.size(), init2.begin());
    return 0;
}

int test_initializer_same_amount() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    std::initializer_list<uxs::db::value> init2 = {"11", "12", "13", "14", "15"};
    uxs::db::value v(init);
    auto r = v.as_array();
    v = init2;
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY(v, init2.size(), init2.begin());
    return 0;
}

int test_initializer_empty_to_not_empty() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    std::initializer_list<uxs::db::value> init2;
    uxs::db::value v(init);
    v = init2;
    CHECK_RECORD_EMPTY(v);
    return 0;
}

int test_initializer_assign_func() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5", "6", "7"};
    std::initializer_list<uxs::db::value> init2 = {"11", "12", "13", "14", "15"};
    uxs::db::value v(init);
    v.assign(init2);
    CHECK_ARRAY(v, init2.size(), init2.begin());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_initializer_empty_to_empty);
ADD_TEST_CASE("", "db::value", test_initializer_not_empty_to_empty);
ADD_TEST_CASE("", "db::value", test_initializer_more_no_realloc);
ADD_TEST_CASE("", "db::value", test_initializer_more_needs_realloc);
ADD_TEST_CASE("", "db::value", test_initializer_less);
ADD_TEST_CASE("", "db::value", test_initializer_same_amount);
ADD_TEST_CASE("", "db::value", test_initializer_empty_to_not_empty);
