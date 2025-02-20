#include "db_value_tests.h"

#include "uxs/list.h"
#include "uxs/vector.h"

using namespace uxs_test_suite;

namespace {

template<typename Src>
int test_range_empty() {
    Src init;
    uxs::db::value v(init.begin(), init.end());
    VERIFY(v.as_array().data() == nullptr);
    CHECK_ARRAY_EMPTY(v);
    return 0;
}

template<typename Src>
int test_range_not_empty() {
    Src init = {"1", "2", "3", "4", "5"};
    uxs::db::value v(init.begin(), init.end());
    CHECK_ARRAY(v, init.size(), init.begin());
    return 0;
}

template<typename Src>
int test_range_make_array_empty() {
    Src init;
    uxs::db::value v = uxs::db::make_array(init.begin(), init.end());
    VERIFY(v.as_array().data() == nullptr);
    CHECK_ARRAY_EMPTY(v);
    return 0;
}

template<typename Src>
int test_range_make_array_not_empty() {
    Src init = {"1", "2"};
    uxs::db::value v = uxs::db::make_array(init.begin(), init.end());
    CHECK_ARRAY(v, init.size(), init.begin());
    VERIFY(v[0].as_string_view() == "1");
    VERIFY(v[1].as_string_view() == "2");
    return 0;
}

int test_range_empty_random_access_range() { return test_range_empty<uxs::vector<std::string_view>>(); }
int test_range_not_empty_random_access_range() { return test_range_not_empty<uxs::vector<std::string_view>>(); }
int test_range_empty_generic_input_range() { return test_range_empty<uxs::list<std::string_view>>(); }
int test_range_not_empty_generic_input_range() { return test_range_not_empty<uxs::list<std::string_view>>(); }

int test_range_make_array_empty_random_access_range() {
    return test_range_make_array_empty<uxs::vector<std::string_view>>();
}
int test_range_make_array_not_empty_random_access_range() {
    return test_range_make_array_not_empty<uxs::vector<std::string_view>>();
}
int test_range_make_array_empty_generic_input_range() {
    return test_range_make_array_empty<uxs::list<std::string_view>>();
}
int test_range_make_array_not_empty_generic_input_range() {
    return test_range_make_array_not_empty<uxs::list<std::string_view>>();
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_range_empty_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_not_empty_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_empty_generic_input_range);
ADD_TEST_CASE("", "db::value", test_range_not_empty_generic_input_range);
ADD_TEST_CASE("", "db::value", test_range_make_array_empty_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_make_array_not_empty_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_make_array_empty_generic_input_range);
ADD_TEST_CASE("", "db::value", test_range_make_array_not_empty_generic_input_range);
