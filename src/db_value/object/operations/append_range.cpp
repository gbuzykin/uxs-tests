#include "db_value_tests.h"

#include <uxs-legacy/vector.h>

using namespace uxs_test_suite;

namespace {

template<typename Src>
int test_not_a_object() {
    uxs::db::value v("1");
    Src ins = {{"1", "A"}, {"2", "B"}, {"3", "C"}};
    MUST_THROW(v.append(ins.begin(), ins.end()));
    return 0;
}

template<typename Src>
int test_append_empty() {
    Src ins;
    Src ins2 = {{"1", "A"}, {"2", "B"}, {"3", "C"}};
    {
        uxs::db::value v;
        // append empty
        v.append(ins.begin(), ins.end());
        CHECK_RECORD_EMPTY(v);
        // append empty to not empty :
        v.append(ins2.begin(), ins2.end());
        CHECK_OBJECT(v, ins2.size(), ins2.begin());
        v.append(ins.begin(), ins.end());
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

template<typename Src>
int test_append() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    Src ins = {{"6", "F"}, {"7", "G"}, {"8", "H"}};
    uxs::db::value v(init);
    std::initializer_list<uxs::db::value> tst = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"},
                                                 {"5", "E"}, {"6", "F"}, {"7", "G"}, {"8", "H"}};
    v.append(ins.begin(), ins.end());
    CHECK_OBJECT(v, tst.size(), tst.begin());
    return 0;
}

int test_not_a_object_random_access_range_assignable() {
    return test_not_a_object<uxs::vector<std::pair<std::string_view, uxs::db::value>>>();
}
int test_append_empty_random_access_range_assignable() {
    return test_append_empty<uxs::vector<std::pair<std::string_view, uxs::db::value>>>();
}
int test_append_random_access_range_assignable() {
    return test_append<uxs::vector<std::pair<std::string_view, uxs::db::value>>>();
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_a_object_random_access_range_assignable);
ADD_TEST_CASE("", "db::value", test_append_empty_random_access_range_assignable);
ADD_TEST_CASE("", "db::value", test_append_random_access_range_assignable);
