#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_initializer_empty_to_empty() {
    std::initializer_list<uxs::db::value> init;
    uxs::db::value v = uxs::db::make_record();
    v = init;
    CHECK_RECORD_EMPTY(v);
    return 0;
}

int test_initializer_not_empty_to_empty() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::pair<std::string_view, std::string_view> tst[] = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    uxs::db::value v = uxs::db::make_record();
    v = init;
    CHECK_RECORD(v, 5, tst);
    return 0;
}

int test_initializer_more() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::initializer_list<uxs::db::value> init2 = {{"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"},
                                                   {"15", "e"}, {"16", "f"}, {"17", "g"}};
    std::pair<std::string_view, std::string_view> tst[] = {{"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"},
                                                           {"15", "e"}, {"16", "f"}, {"17", "g"}};
    uxs::db::value v(init);
    v = init2;
    CHECK_RECORD(v, 7, tst);
    return 0;
}

int test_initializer_less() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"},
                                                  {"5", "E"}, {"6", "F"}, {"7", "G"}};
    std::initializer_list<uxs::db::value> init2 = {{"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"}, {"15", "e"}};
    std::pair<std::string_view, std::string_view> tst[] = {
        {"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"}, {"15", "e"}};
    uxs::db::value v(init);
    v = init2;
    CHECK_RECORD(v, 5, tst);
    return 0;
}

int test_initializer_same_amount() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::initializer_list<uxs::db::value> init2 = {{"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"}, {"15", "e"}};
    std::pair<std::string_view, std::string_view> tst[] = {
        {"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"}, {"15", "e"}};
    uxs::db::value v(init);
    v = init2;
    CHECK_RECORD(v, 5, tst);
    return 0;
}

int test_initializer_empty_to_not_empty() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::initializer_list<uxs::db::value> init2;
    uxs::db::value v(init);
    v = init2;
    CHECK_RECORD_EMPTY(v);
    return 0;
}

int test_initializer_assign_func() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"},
                                                  {"5", "E"}, {"6", "F"}, {"7", "G"}};
    std::initializer_list<uxs::db::value> init2 = {{"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"}, {"15", "e"}};
    std::pair<std::string_view, std::string_view> tst[] = {
        {"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"}, {"15", "e"}};
    uxs::db::value v(init);
    v.assign(init2);
    CHECK_RECORD(v, 5, tst);
    return 0;
}

int test_smart_initializer() {
    uxs::db::value v = {{"pi", 3.141},
                        {"happy", true},
                        {"name", "Niels"},
                        {"nothing", nullptr},
                        {"answer", {{"everything", 42}}},
                        {"list", {1, 0, 2}},
                        {"object", {{"currency", "USD"}, {"value", 42.99}}}};
    VERIFY(v["pi"].as_double() == 3.141);
    VERIFY(v["happy"].as_bool() == true);
    VERIFY(v["name"].as_string_view() == "Niels");
    VERIFY(v["nothing"].is_null());
    VERIFY(v["answer"]["everything"].as_int() == 42);
    VERIFY(v["list"][0].as_int() == 1);
    VERIFY(v["list"][1].as_int() == 0);
    VERIFY(v["list"][2].as_int() == 2);
    VERIFY(v["object"]["currency"].as_string_view() == "USD");
    VERIFY(v["object"]["value"].as_double() == 42.99);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_initializer_empty_to_empty);
ADD_TEST_CASE("", "db::value", test_initializer_not_empty_to_empty);
ADD_TEST_CASE("", "db::value", test_initializer_more);
ADD_TEST_CASE("", "db::value", test_initializer_less);
ADD_TEST_CASE("", "db::value", test_initializer_same_amount);
ADD_TEST_CASE("", "db::value", test_initializer_empty_to_not_empty);
ADD_TEST_CASE("", "db::value", test_initializer_assign_func);
ADD_TEST_CASE("", "db::value", test_smart_initializer);
