#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_clear_empty() {
    uxs::db::value v = uxs::db::make_record();
    v.clear();
    CHECK_RECORD_EMPTY(v);
    return 0;
}

int test_clear_not_empty() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    uxs::db::value v(init);
    v.clear();
    CHECK_RECORD_EMPTY(v);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_clear_empty);
ADD_TEST_CASE("", "db::value", test_clear_not_empty);
