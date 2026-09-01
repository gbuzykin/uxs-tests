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
        uxs::db::value v = uxs::db::make_object();
        CHECK_RECORD_EMPTY(v);
    }
    return 0;
}

int test_initializer_not_empty() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    uxs::db::value v(init);
    CHECK_OBJECT(v, init.size(), init.begin());
    return 0;
}

int test_initializer_make_object_empty() {
    {
        std::initializer_list<std::pair<std::string_view, uxs::db::value>> init;
        uxs::db::value v = uxs::db::make_object(init);
        CHECK_RECORD_EMPTY(v);
    }
    {
        uxs::db::value v = uxs::db::make_object();
        CHECK_RECORD_EMPTY(v);
    }
    return 0;
}

int test_initializer_make_object_not_empty() {
    std::initializer_list<std::pair<std::string_view, uxs::db::value>> init = {{"1", "A"}, {"2", 3.1415}};
    uxs::db::value v = uxs::db::make_object(init);
    VERIFY(v.is_object() && v.size() == 2);
    VERIFY(v["2"].as_double() == 3.1415);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_initializer_empty);
ADD_TEST_CASE("", "db::value", test_initializer_not_empty);
ADD_TEST_CASE("", "db::value", test_initializer_make_object_empty);
ADD_TEST_CASE("", "db::value", test_initializer_make_object_not_empty);
