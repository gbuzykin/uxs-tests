#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_not_a_record() {
    uxs::db::value v("1");
    MUST_THROW(v.insert({{"1", "A"}, {"2", "B"}, {"3", "C"}}));
    return 0;
}

int test_insert_to_empty() {
    std::initializer_list<std::pair<std::string_view, uxs::db::value>> ins;
    std::initializer_list<std::pair<std::string_view, uxs::db::value>> ins2 = {{"1", "A"}, {"2", "B"}, {"3", "C"}};
    {
        uxs::db::value v;
        // insert empty
        v.insert(ins);
        CHECK_RECORD_EMPTY(v);
        // insert empty to not empty :
        v.insert(ins2);
        CHECK_RECORD(v, ins2.size(), ins2.begin());
        v.insert(ins);
        CHECK_RECORD(v, ins2.size(), ins2.begin());
    }
    {
        uxs::db::value v = uxs::db::make_record();
        // insert empty
        v.insert(ins.begin(), ins.end());
        CHECK_RECORD_EMPTY(v);
        // insert empty to not empty :
        v.insert(ins2.begin(), ins2.end());
        CHECK_RECORD(v, ins2.size(), ins2.begin());
        v.insert(ins.begin(), ins.end());
        CHECK_RECORD(v, ins2.size(), ins2.begin());
    }
    return 0;
}

int test_insert() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::initializer_list<std::pair<std::string_view, uxs::db::value>> ins = {{"6", "F"}, {"7", "G"}, {"8", "H"}};
    uxs::db::value v(init);
    std::pair<std::string_view, std::string_view> tst[] = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"},
                                                           {"5", "E"}, {"6", "F"}, {"7", "G"}, {"8", "H"}};
    v.insert(ins);
    CHECK_RECORD(v, 8, tst);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_a_record);
ADD_TEST_CASE("", "db::value", test_insert_to_empty);
ADD_TEST_CASE("", "db::value", test_insert);
