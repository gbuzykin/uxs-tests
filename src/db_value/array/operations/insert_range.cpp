#include "db_value_tests.h"

#include "uxs/list.h"
#include "uxs/vector.h"

using namespace uxs_test_suite;

namespace {

template<typename Src>
int test_not_an_array() {
    uxs::db::value v("1");
    Src ins = {"1", "2", "3"};
    MUST_THROW(v.insert(0, ins.begin(), ins.end()));
    return 0;
}

template<typename Src>
int test_insert_empty() {
    Src ins;
    Src ins2 = {"1", "2", "3"};
    {
        uxs::db::value v;
        // insert empty
        v.insert(v.size(), ins.begin(), ins.end());
        CHECK_ARRAY_EMPTY(v);
        // insert empty to not empty :
        v.insert(v.size(), ins2.begin(), ins2.end());
        CHECK_ARRAY(v, ins2.size(), ins2.begin());
        // back
        v.insert(v.size(), ins.begin(), ins.end());
        CHECK_ARRAY(v, ins2.size(), ins2.begin());
        // mid
        v.insert(1, ins.begin(), ins.end());
        CHECK_ARRAY(v, ins2.size(), ins2.begin());
        // front
        v.insert(0, ins.begin(), ins.end());
        CHECK_ARRAY(v, ins2.size(), ins2.begin());
    }
    {
        uxs::db::value v = uxs::db::make_array();
        // insert empty
        v.insert(v.size(), ins.begin(), ins.end());
        CHECK_ARRAY_EMPTY(v);
        // insert empty to not empty :
        v.insert(v.size(), ins2.begin(), ins2.end());
        CHECK_ARRAY(v, ins2.size(), ins2.begin());
        // back
        v.insert(v.size(), ins.begin(), ins.end());
        CHECK_ARRAY(v, ins2.size(), ins2.begin());
        // mid
        v.insert(1, ins.begin(), ins.end());
        CHECK_ARRAY(v, ins2.size(), ins2.begin());
        // front
        v.insert(0, ins.begin(), ins.end());
        CHECK_ARRAY(v, ins2.size(), ins2.begin());
    }
    return 0;
}

template<typename Src>
int test_insert_no_realloc() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    Src ins = {"6", "7", "8"};
    {  // back
        uxs::db::value v(init);
        std::string_view tst[] = {"1", "2", "3", "4", "5", "6", "7", "8"};
        v.reserve(10);
        auto r = v.as_array();
        v.insert(v.size(), ins.begin(), ins.end());
        VERIFY(r.data() == v.as_array().data());
        CHECK_ARRAY(v, 8, tst);
    }
    {  // mid
        uxs::db::value v(init);
        std::string_view tst[] = {"1", "2", "3", "6", "7", "8", "4", "5"};
        v.reserve(10);
        auto r = v.as_array();
        v.insert(3, ins.begin(), ins.end());
        VERIFY(r.data() == v.as_array().data());
        CHECK_ARRAY(v, 8, tst);
    }
    {  // front
        uxs::db::value v(init);
        std::string_view tst[] = {"6", "7", "8", "1", "2", "3", "4", "5"};
        v.reserve(10);
        auto r = v.as_array();
        v.insert(0, ins.begin(), ins.end());
        VERIFY(r.data() == v.as_array().data());
        CHECK_ARRAY(v, 8, tst);
    }
    return 0;
}

template<typename Src>
int test_insert_needs_realloc() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    Src ins = {"10", "11", "12", "13", "14", "15", "16"};
    {  // back
        uxs::db::value v(init);
        std::string_view tst[] = {"1", "2", "3", "4", "5", "10", "11", "12", "13", "14", "15", "16"};
        auto r = v.as_array();
        v.insert(v.size(), ins.begin(), ins.end());
        VERIFY(r.data() != v.as_array().data());
        CHECK_ARRAY(v, 12, tst);
    }
    {  // mid
        uxs::db::value v(init);
        std::string_view tst[] = {"1", "2", "3", "10", "11", "12", "13", "14", "15", "16", "4", "5"};
        auto r = v.as_array();
        v.insert(3, ins.begin(), ins.end());
        VERIFY(r.data() != v.as_array().data());
        CHECK_ARRAY(v, 12, tst);
    }
    {  // front
        uxs::db::value v(init);
        std::string_view tst[] = {"10", "11", "12", "13", "14", "15", "16", "1", "2", "3", "4", "5"};
        auto r = v.as_array();
        v.insert(0, ins.begin(), ins.end());
        VERIFY(r.data() != v.as_array().data());
        CHECK_ARRAY(v, 12, tst);
    }
    return 0;
}

int test_not_an_array_random_access_range_assignable() { return test_not_an_array<uxs::vector<std::string_view>>(); }
int test_insert_empty_random_access_range_assignable() { return test_insert_empty<uxs::vector<std::string_view>>(); }
int test_insert_no_realloc_random_access_range_assignable() {
    return test_insert_no_realloc<uxs::vector<std::string_view>>();
}
int test_insert_needs_realloc_random_access_range_assignable() {
    return test_insert_needs_realloc<uxs::vector<std::string_view>>();
}

int test_not_an_array_generic_input_range_assignable() { return test_not_an_array<uxs::list<std::string_view>>(); }
int test_insert_empty_generic_input_range_assignable() { return test_insert_empty<uxs::list<std::string_view>>(); }
int test_insert_no_realloc_generic_input_range_assignable() {
    return test_insert_no_realloc<uxs::list<std::string_view>>();
}
int test_insert_needs_realloc_generic_input_range_assignable() {
    return test_insert_needs_realloc<uxs::list<std::string_view>>();
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_an_array_random_access_range_assignable);
ADD_TEST_CASE("", "db::value", test_insert_empty_random_access_range_assignable);
ADD_TEST_CASE("", "db::value", test_insert_no_realloc_random_access_range_assignable);
ADD_TEST_CASE("", "db::value", test_insert_needs_realloc_random_access_range_assignable);

ADD_TEST_CASE("", "db::value", test_not_an_array_generic_input_range_assignable);
ADD_TEST_CASE("", "db::value", test_insert_empty_generic_input_range_assignable);
ADD_TEST_CASE("", "db::value", test_insert_no_realloc_generic_input_range_assignable);
ADD_TEST_CASE("", "db::value", test_insert_needs_realloc_generic_input_range_assignable);
