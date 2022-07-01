#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_initializer_not_empty() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    uxs::db::value v(init);
    CHECK_ARRAY(v, init.size(), init.begin());
    return 0;
}

int test_initializer_make_array_empty() {
    {
        uxs::db::value v = uxs::db::make_array();
        VERIFY(v.as_array().data() == nullptr);
        CHECK_ARRAY_EMPTY(v);
    }
    {
        std::initializer_list<uxs::db::value> init;
        uxs::db::value v = uxs::db::make_array(init);
        VERIFY(v.as_array().data() == nullptr);
        CHECK_ARRAY_EMPTY(v);
    }
    return 0;
}

int test_initializer_make_array_not_empty() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", 3.1415}};
    uxs::db::value v = uxs::db::make_array(init);
    VERIFY(v.is_array() && v.size() == 2);
    VERIFY(v[0][0].as_string_view() == "1");
    VERIFY(v[0][1].as_string_view() == "A");
    VERIFY(v[1][0].as_string_view() == "2");
    VERIFY(v[1][1].as_double() == 3.1415);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_initializer_not_empty);
ADD_TEST_CASE("", "db::value", test_initializer_make_array_empty);
ADD_TEST_CASE("", "db::value", test_initializer_make_array_not_empty);
