#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_not_a_object() {
    uxs::db::value v("1");
    MUST_THROW(v.append({{"1", "A"}, {"2", "B"}, {"3", "C"}}));
    return 0;
}

int test_append_to_empty() {
    std::initializer_list<std::pair<std::string_view, uxs::db::value>> ins;
    std::initializer_list<std::pair<std::string_view, uxs::db::value>> ins2 = {{"1", "A"}, {"2", "B"}, {"3", "C"}};
    {
        uxs::db::value v;
        // append empty
        v.append(uxs::db::object_tag, ins);
        CHECK_RECORD_EMPTY(v);
        // append empty to not empty :
        v.append(uxs::db::object_tag, ins2);
        CHECK_OBJECT(v, ins2.size(), ins2.begin());
        v.append(uxs::db::object_tag, ins);
        CHECK_OBJECT(v, ins2.size(), ins2.begin());
    }
    {
        uxs::db::value v(uxs::db::object_tag);
        // append empty
        v.append(ins.begin(), ins.end());
        CHECK_RECORD_EMPTY(v);
        // append empty to not empty :
        v.append(ins2.begin(), ins2.end());
        CHECK_OBJECT(v, ins2.size(), ins2.begin());
        v.append(ins.begin(), ins.end());
        CHECK_OBJECT(v, ins2.size(), ins2.begin());
    }
    return 0;
}

int test_append() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::initializer_list<std::pair<std::string_view, uxs::db::value>> ins = {{"6", "F"}, {"7", "G"}, {"8", "H"}};
    uxs::db::value v(init);
    std::initializer_list<uxs::db::value> tst = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"},
                                                 {"5", "E"}, {"6", "F"}, {"7", "G"}, {"8", "H"}};
    v.append(uxs::db::object_tag, ins);
    CHECK_OBJECT(v, tst.size(), tst.begin());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_a_object);
ADD_TEST_CASE("", "db::value", test_append_to_empty);
ADD_TEST_CASE("", "db::value", test_append);
