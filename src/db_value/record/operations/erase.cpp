#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_not_a_record() {
    {
        uxs::db::value v;
        MUST_THROW(v.erase("1"));
    }
    {
        uxs::db::value v("1");
        MUST_THROW(v.erase("1"));
    }
    {
        uxs::db::value v = uxs::db::make_array();
        MUST_THROW(v.erase("1"));
    }
    return 0;
}

int test_erase_from_empty() {
    uxs::db::value v = uxs::db::make_record();
    v.erase("1");
    CHECK_RECORD_EMPTY(v);
    return 0;
}

int test_erase() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"2", "B1"},
                                                  {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::pair<std::string_view, std::string_view> tst1[] = {{"1", "A"}, {"2", "B"}, {"2", "B1"}, {"3", "C"}, {"4", "D"}};
    std::pair<std::string_view, std::string_view> tst2[] = {{"1", "A"}, {"3", "C"}, {"4", "D"}};
    std::pair<std::string_view, std::string_view> tst3[] = {{"3", "C"}, {"4", "D"}};
    uxs::db::value v(init);
    // back
    v.erase("5");
    CHECK_RECORD(v, 5, tst1);
    // mid
    v.erase("2");
    CHECK_RECORD(v, 3, tst2);
    VERIFY(v.find("2") == v.end());
    // front
    v.erase("1");
    CHECK_RECORD(v, 2, tst3);
    // not find
    v.erase("1");
    CHECK_RECORD(v, 2, tst3);
    return 0;
}

int test_erase_one() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"2", "B1"},
                                                  {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::pair<std::string_view, std::string_view> tst1[] = {{"1", "A"}, {"2", "B"}, {"2", "B1"}, {"3", "C"}, {"4", "D"}};
    std::pair<std::string_view, std::string_view> tst2[] = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}};
    std::pair<std::string_view, std::string_view> tst2a[] = {{"1", "A"}, {"3", "C"}, {"4", "D"}};
    std::pair<std::string_view, std::string_view> tst3[] = {{"3", "C"}, {"4", "D"}};
    uxs::db::value v(init);
    // back
    v.erase(v.find("5"));
    CHECK_RECORD(v, 5, tst1);
    // mid
    v.erase(v.find("2"));
    CHECK_RECORD(v, 4, tst2);
    v.erase(v.find("2"));
    CHECK_RECORD(v, 3, tst2a);
    VERIFY(v.find("2") == v.end());
    // front
    v.erase(v.find("1"));
    CHECK_RECORD(v, 2, tst3);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_a_record);
ADD_TEST_CASE("", "db::value", test_erase_from_empty);
ADD_TEST_CASE("", "db::value", test_erase);
ADD_TEST_CASE("", "db::value", test_erase_one);
