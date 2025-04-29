#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_move_from_empty() {
    uxs::db::value v_from = uxs::db::make_record();
    uxs::db::value v(std::move(v_from));
    CHECK_RECORD_EMPTY(v);
    VERIFY(v_from.is_null());
    return 0;
}

int test_move_from_not_empty() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    uxs::db::value v_from(init);
    uxs::db::value v(std::move(v_from));
    CHECK_RECORD(v, init.size(), init.begin());
    VERIFY(v_from.is_null());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_move_from_empty);
ADD_TEST_CASE("", "db::value", test_move_from_not_empty);
