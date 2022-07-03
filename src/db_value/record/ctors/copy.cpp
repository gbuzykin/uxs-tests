#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_copy_from_empty() {
    uxs::db::value v_from = uxs::db::make_record();
    uxs::db::value v(v_from);
    CHECK_RECORD_EMPTY(v);
    return 0;
}

int test_copy_from_not_empty() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::pair<std::string_view, std::string_view> tst[] = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    uxs::db::value v_from(init);
    uxs::db::value v(v_from);
    CHECK_RECORD(v, 5, tst);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_copy_from_empty);
ADD_TEST_CASE("", "db::value", test_copy_from_not_empty);
