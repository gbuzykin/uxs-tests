#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_not_an_array() {
    uxs::db::value v("1");
    MUST_THROW(v.insert(0, std::initializer_list<uxs::db::value>{"1", "2", "3"}));
    return 0;
}

int test_insert_to_empty() {
    std::initializer_list<uxs::db::value> ins = {"1", "2", "3"};
    {
        uxs::db::value v;
        v.insert(0, ins);
        CHECK_ARRAY(v, ins.size(), ins.begin());
    }
    {
        uxs::db::value v = uxs::db::make_array();
        v.insert(0, ins);
        CHECK_ARRAY(v, ins.size(), ins.begin());
    }
    return 0;
}

int test_insert_no_realloc() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    std::initializer_list<uxs::db::value> ins = {"6", "7", "8"};
    {  // back
        uxs::db::value v(init);
        v.reserve(10);
        std::string_view tst[] = {"1", "2", "3", "4", "5", "6", "7", "8"};
        auto r = v.as_array();
        v.insert(v.size(), ins);
        VERIFY(r.data() == v.as_array().data());
        CHECK_ARRAY(v, 8, tst);
    }
    {  // mid
        uxs::db::value v(init);
        std::string_view tst[] = {"1", "2", "3", "6", "7", "8", "4", "5"};
        v.reserve(10);
        auto r = v.as_array();
        v.insert(3, ins);
        VERIFY(r.data() == v.as_array().data());
        CHECK_ARRAY(v, 8, tst);
    }
    {  // front
        uxs::db::value v(init);
        std::string_view tst[] = {"6", "7", "8", "1", "2", "3", "4", "5"};
        v.reserve(10);
        auto r = v.as_array();
        v.insert(0, ins);
        VERIFY(r.data() == v.as_array().data());
        CHECK_ARRAY(v, 8, tst);
    }
    return 0;
}

int test_insert_needs_realloc() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    std::initializer_list<uxs::db::value> ins = {"10", "11", "12", "13", "14", "15", "16"};
    {  // back
        uxs::db::value v(init);
        std::string_view tst[] = {"1", "2", "3", "4", "5", "10", "11", "12", "13", "14", "15", "16"};
        auto r = v.as_array();
        v.insert(v.size(), ins);
        VERIFY(r.data() != v.as_array().data());
        CHECK_ARRAY(v, 12, tst);
    }
    {  // mid
        uxs::db::value v(init);
        std::string_view tst[] = {"1", "2", "3", "10", "11", "12", "13", "14", "15", "16", "4", "5"};
        auto r = v.as_array();
        v.insert(3, ins);
        VERIFY(r.data() != v.as_array().data());
        CHECK_ARRAY(v, 12, tst);
    }
    {  // front
        uxs::db::value v(init);
        std::string_view tst[] = {"10", "11", "12", "13", "14", "15", "16", "1", "2", "3", "4", "5"};
        auto r = v.as_array();
        v.insert(0, ins);
        VERIFY(r.data() != v.as_array().data());
        CHECK_ARRAY(v, 12, tst);
    }
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_an_array);
ADD_TEST_CASE("", "db::value", test_insert_to_empty);
ADD_TEST_CASE("", "db::value", test_insert_no_realloc);
ADD_TEST_CASE("", "db::value", test_insert_needs_realloc);
