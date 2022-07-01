#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_not_an_array() {
    uxs::db::value v("1");
    MUST_THROW(v.emplace_back("10"));
    return 0;
}

int test_emplace_back_to_empty() {
    std::string_view tst[] = {"10"};
    {
        uxs::db::value v;
        auto* p = &v.emplace_back("10");
        VERIFY(p == &v[0]);
        CHECK_ARRAY(v, 1, tst);
    }
    {
        uxs::db::value v = uxs::db::make_array();
        auto* p = &v.emplace_back("10");
        VERIFY(p == &v[0]);
        CHECK_ARRAY(v, 1, tst);
    }
    return 0;
}

int test_emplace_back_no_realloc() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    std::string_view tst[] = {"1", "2", "3", "4", "5", "6"};
    uxs::db::value v(init);
    v.reserve(10);
    auto r = v.as_array();
    auto* p = &v.emplace_back("6");
    VERIFY(r.data() == v.as_array().data());
    VERIFY(p == &v[5]);
    CHECK_ARRAY(v, 6, tst);
    return 0;
}

int test_emplace_back_needs_realloc() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    std::string_view tst[] = {"1", "2", "3", "4", "5", "6"};
    uxs::db::value v(init);
    auto r = v.as_array();
    auto* p = &v.emplace_back("6");
    VERIFY(r.data() != v.as_array().data());
    VERIFY(p == &v[5]);
    CHECK_ARRAY(v, 6, tst);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_an_array);
ADD_TEST_CASE("", "db::value", test_emplace_back_to_empty);
ADD_TEST_CASE("", "db::value", test_emplace_back_no_realloc);
ADD_TEST_CASE("", "db::value", test_emplace_back_needs_realloc);
