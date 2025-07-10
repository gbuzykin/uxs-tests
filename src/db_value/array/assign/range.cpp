#include "db_value_tests.h"

#include <uxs-legacy/list.h>
#include <uxs-legacy/vector.h>

using namespace uxs_test_suite;

namespace {

template<typename Src>
int test_range_empty_to_empty() {
    Src src;
    uxs::db::value v = uxs::db::make_array();
    v.assign(src.begin(), src.end());
    VERIFY(v.as_array().data() == nullptr);
    CHECK_ARRAY_EMPTY(v);
    return 0;
}

template<typename Src>
int test_range_not_empty_to_empty() {
    Src src = {"1", "2", "3", "4", "5"};
    uxs::db::value v = uxs::db::make_array();
    v.assign(src.begin(), src.end());
    CHECK_ARRAY(v, src.size(), src.begin());
    return 0;
}

template<typename Src>
int test_range_more_no_realloc() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    Src src = {"11", "12", "13", "14", "15", "16", "17"};
    uxs::db::value v(init);
    v.reserve(10);
    auto r = v.as_array();
    v.assign(src.begin(), src.end());
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY(v, src.size(), src.begin());
    return 0;
}

template<typename Src>
int test_range_more_needs_realloc() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4"};
    Src src = {"11", "12", "13", "14", "15", "16", "17"};
    uxs::db::value v(init);
    auto r = v.as_array();
    v.assign(src.begin(), src.end());
    VERIFY(r.data() != v.as_array().data());
    CHECK_ARRAY(v, src.size(), src.begin());
    return 0;
}

template<typename Src>
int test_range_less() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5", "6", "7"};
    Src src = {"11", "12", "13", "14", "15"};
    uxs::db::value v(init);
    auto r = v.as_array();
    v.assign(src.begin(), src.end());
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY(v, src.size(), src.begin());
    return 0;
}

template<typename Src>
int test_range_same_amount() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    Src src = {"11", "12", "13", "14", "15"};
    uxs::db::value v(init);
    auto r = v.as_array();
    v.assign(src.begin(), src.end());
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY(v, src.size(), src.begin());
    return 0;
}

template<typename Src>
int test_range_empty_to_not_empty() {
    std::initializer_list<uxs::db::value> init = {1, 2, 3, 4, 5};
    Src src;
    uxs::db::value v(init);
    auto r = v.as_array();
    v.assign(src.begin(), src.end());
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY_EMPTY(v);
    return 0;
}

int test_range_empty_to_empty_random_access_range() { return test_range_empty_to_empty<uxs::vector<uxs::db::value>>(); }
int test_range_not_empty_to_empty_random_access_range() {
    return test_range_not_empty_to_empty<uxs::vector<uxs::db::value>>();
}
int test_range_more_no_realloc_random_access_range() {
    return test_range_more_no_realloc<uxs::vector<uxs::db::value>>();
}
int test_range_more_needs_realloc_random_access_range() {
    return test_range_more_needs_realloc<uxs::vector<uxs::db::value>>();
}
int test_range_less_random_access_range() { return test_range_less<uxs::vector<uxs::db::value>>(); }
int test_range_same_amount_random_access_range() { return test_range_same_amount<uxs::vector<uxs::db::value>>(); }
int test_range_empty_to_not_empty_random_access_range() {
    return test_range_empty_to_not_empty<uxs::vector<uxs::db::value>>();
}

int test_range_empty_to_empty_generic_input_range() { return test_range_empty_to_empty<uxs::list<uxs::db::value>>(); }
int test_range_not_empty_to_empty_generic_input_range() {
    return test_range_not_empty_to_empty<uxs::list<uxs::db::value>>();
}
int test_range_more_no_realloc_generic_input_range() { return test_range_more_no_realloc<uxs::list<uxs::db::value>>(); }
int test_range_more_needs_realloc_generic_input_range() {
    return test_range_more_needs_realloc<uxs::list<uxs::db::value>>();
}
int test_range_less_generic_input_range() { return test_range_less<uxs::list<uxs::db::value>>(); }
int test_range_same_amount_generic_input_range() { return test_range_same_amount<uxs::list<uxs::db::value>>(); }
int test_range_empty_to_not_empty_generic_input_range() {
    return test_range_empty_to_not_empty<uxs::list<uxs::db::value>>();
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_range_empty_to_empty_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_not_empty_to_empty_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_more_no_realloc_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_more_needs_realloc_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_less_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_same_amount_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_empty_to_not_empty_random_access_range);

ADD_TEST_CASE("", "db::value", test_range_empty_to_empty_generic_input_range);
ADD_TEST_CASE("", "db::value", test_range_not_empty_to_empty_generic_input_range);
ADD_TEST_CASE("", "db::value", test_range_more_no_realloc_generic_input_range);
ADD_TEST_CASE("", "db::value", test_range_more_needs_realloc_generic_input_range);
ADD_TEST_CASE("", "db::value", test_range_less_generic_input_range);
ADD_TEST_CASE("", "db::value", test_range_same_amount_generic_input_range);
ADD_TEST_CASE("", "db::value", test_range_empty_to_not_empty_generic_input_range);
