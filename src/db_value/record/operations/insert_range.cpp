#include "db_value_tests.h"

#include <uxs-legacy/vector.h>

using namespace uxs_test_suite;

namespace {

template<typename Src>
int test_not_a_record() {
    uxs::db::value v("1");
    Src ins = {{"1", "A"}, {"2", "B"}, {"3", "C"}};
    MUST_THROW(v.insert(ins.begin(), ins.end()));
    return 0;
}

template<typename Src>
int test_insert_empty() {
    Src ins;
    Src ins2 = {{"1", "A"}, {"2", "B"}, {"3", "C"}};
    {
        uxs::db::value v;
        // insert empty
        v.insert(ins.begin(), ins.end());
        CHECK_RECORD_EMPTY(v);
        // insert empty to not empty :
        v.insert(ins2.begin(), ins2.end());
        CHECK_RECORD(v, ins2.size(), ins2.begin());
        v.insert(ins.begin(), ins.end());
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

template<typename Src>
int test_insert() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    Src ins = {{"6", "F"}, {"7", "G"}, {"8", "H"}};
    uxs::db::value v(init);
    std::initializer_list<uxs::db::value> tst = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"},
                                                 {"5", "E"}, {"6", "F"}, {"7", "G"}, {"8", "H"}};
    v.insert(ins.begin(), ins.end());
    CHECK_RECORD(v, tst.size(), tst.begin());
    return 0;
}

int test_not_a_record_random_access_range_assignable() {
    return test_not_a_record<uxs::vector<std::pair<std::string_view, uxs::db::value>>>();
}
int test_insert_empty_random_access_range_assignable() {
    return test_insert_empty<uxs::vector<std::pair<std::string_view, uxs::db::value>>>();
}
int test_insert_random_access_range_assignable() {
    return test_insert<uxs::vector<std::pair<std::string_view, uxs::db::value>>>();
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_a_record_random_access_range_assignable);
ADD_TEST_CASE("", "db::value", test_insert_empty_random_access_range_assignable);
ADD_TEST_CASE("", "db::value", test_insert_random_access_range_assignable);
