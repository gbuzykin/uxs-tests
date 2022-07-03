#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_initializer_empty() {
    {
        std::initializer_list<uxs::db::value> init;
        uxs::db::value v(init);
        CHECK_RECORD_EMPTY(v);
    }
    {
        std::initializer_list<uxs::db::value> init;
        uxs::db::value v = uxs::db::make_record();
        CHECK_RECORD_EMPTY(v);
    }
    return 0;
}

int test_initializer_not_empty() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::pair<std::string_view, std::string_view> tst[] = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    uxs::db::value v(init);
    CHECK_RECORD(v, 5, tst);
    return 0;
}

int test_initializer_make_record_empty() {
    {
        std::initializer_list<std::pair<std::string_view, uxs::db::value>> init;
        uxs::db::value v = uxs::db::make_record(init);
        CHECK_RECORD_EMPTY(v);
    }
    {
        uxs::db::value v = uxs::db::make_record();
        CHECK_RECORD_EMPTY(v);
    }
    return 0;
}

int test_initializer_make_record_not_empty() {
    std::initializer_list<std::pair<std::string_view, uxs::db::value>> init = {{"1", "A"}, {"2", 3.1415}};
    uxs::db::value v = uxs::db::make_record(init);
    VERIFY(v.is_record() && v.size() == 2);
    VERIFY(v["2"].as_double() == 3.1415);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_initializer_empty);
ADD_TEST_CASE("", "db::value", test_initializer_not_empty);
ADD_TEST_CASE("", "db::value", test_initializer_make_record_empty);
ADD_TEST_CASE("", "db::value", test_initializer_make_record_not_empty);
