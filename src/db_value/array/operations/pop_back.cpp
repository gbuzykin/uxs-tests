#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_not_an_array() {
    uxs::db::value v("1");
    MUST_THROW(v.pop_back());
    return 0;
}

int test_pop_back() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    std::string_view tst[] = {"1", "2", "3", "4"};
    uxs::db::value v(init);
    v.pop_back();
    CHECK_ARRAY(v, 4, tst);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_an_array);
ADD_TEST_CASE("", "db::value", test_pop_back);
