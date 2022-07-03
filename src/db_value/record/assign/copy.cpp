#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_copy_empty_to_empty() {
    uxs::db::value v = uxs::db::make_record(), v_from = uxs::db::make_record();
    v = v_from;
    CHECK_RECORD_EMPTY(v);
    return 0;
}

int test_copy_not_empty_to_empty() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::pair<std::string_view, std::string_view> tst[] = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    uxs::db::value v = uxs::db::make_record(), v_from(init);
    v = v_from;
    CHECK_RECORD(v, 5, tst);
    return 0;
}

int test_copy_more() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::initializer_list<uxs::db::value> init2 = {{"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"},
                                                   {"15", "e"}, {"16", "f"}, {"17", "g"}};
    std::pair<std::string_view, std::string_view> tst[] = {{"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"},
                                                           {"15", "e"}, {"16", "f"}, {"17", "g"}};
    uxs::db::value v(init), v_from(init2);
    v = v_from;
    CHECK_RECORD(v, 7, tst);
    return 0;
}

int test_copy_less() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"},
                                                  {"5", "E"}, {"6", "F"}, {"7", "G"}};
    std::initializer_list<uxs::db::value> init2 = {{"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"}, {"15", "e"}};
    std::pair<std::string_view, std::string_view> tst[] = {
        {"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"}, {"15", "e"}};
    uxs::db::value v(init), v_from(init2);
    v = v_from;
    CHECK_RECORD(v, 5, tst);
    return 0;
}

int test_copy_same_amount() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::initializer_list<uxs::db::value> init2 = {{"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"}, {"15", "e"}};
    std::pair<std::string_view, std::string_view> tst[] = {
        {"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"}, {"15", "e"}};
    uxs::db::value v(init), v_from(init2);
    v = v_from;
    CHECK_RECORD(v, 5, tst);
    return 0;
}

int test_copy_empty_to_not_empty() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    uxs::db::value v(init), v_from = uxs::db::make_record();
    v = v_from;
    CHECK_RECORD_EMPTY(v);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_copy_empty_to_empty);
ADD_TEST_CASE("", "db::value", test_copy_not_empty_to_empty);
ADD_TEST_CASE("", "db::value", test_copy_more);
ADD_TEST_CASE("", "db::value", test_copy_less);
ADD_TEST_CASE("", "db::value", test_copy_same_amount);
ADD_TEST_CASE("", "db::value", test_copy_empty_to_not_empty);
