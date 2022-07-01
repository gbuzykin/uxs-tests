#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_not_an_array() {
    uxs::db::value v("1");
    MUST_THROW(v.reserve(0));
    return 0;
}

int test_reserve_empty() {
    {
        uxs::db::value v;
        v.reserve(0);
        VERIFY(v.as_array().data() == nullptr);
        CHECK_ARRAY_EMPTY(v);
        v.reserve(10);
        CHECK_ARRAY_EMPTY(v);
        auto r = v.as_array();
        size_t n = 0;
        for (auto s : {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}) {
            v.push_back(s);
            VERIFY(++n == v.size());
            VERIFY(r.data() == v.as_array().data());
        }
    }
    {
        uxs::db::value v = uxs::db::make_array();
        v.reserve(0);
        VERIFY(v.as_array().data() == nullptr);
        CHECK_ARRAY_EMPTY(v);
        v.reserve(10);
        CHECK_ARRAY_EMPTY(v);
        auto r = v.as_array();
        size_t n = 0;
        for (auto s : {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}) {
            v.push_back(s);
            VERIFY(++n == v.size());
            VERIFY(r.data() == v.as_array().data());
        }
    }
    return 0;
}

int test_reserve_more() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    uxs::db::value v(init);
    auto r = v.as_array();
    v.reserve(10);
    VERIFY(r.data() != v.as_array().data());
    CHECK_ARRAY(v, init.size(), init.begin());
    r = v.as_array();
    size_t n = 5;
    for (auto s : {"6", "7"}) {
        v.push_back(s);
        VERIFY(++n == v.size());
        VERIFY(r.data() == v.as_array().data());
    }
    return 0;
}

int test_reserve_less() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    uxs::db::value v(init);
    auto r = v.as_array();
    v.reserve(4);
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY(v, init.size(), init.begin());
    v.reserve(0);
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY(v, init.size(), init.begin());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_an_array);
ADD_TEST_CASE("", "db::value", test_reserve_empty);
ADD_TEST_CASE("", "db::value", test_reserve_more);
ADD_TEST_CASE("", "db::value", test_reserve_less);
