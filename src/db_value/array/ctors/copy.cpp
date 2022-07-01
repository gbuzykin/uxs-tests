#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_copy_from_empty() {
    uxs::db::value v_from = uxs::db::make_array();
    uxs::db::value v(v_from);
    VERIFY(v.as_array().data() == nullptr);
    CHECK_ARRAY_EMPTY(v);
    return 0;
}

int test_copy_from_not_empty() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    uxs::db::value v_from(init);
    uxs::db::value v(v_from);
    CHECK_ARRAY(v, init.size(), init.begin());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_copy_from_empty);
ADD_TEST_CASE("", "db::value", test_copy_from_not_empty);
