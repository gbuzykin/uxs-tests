#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_not_an_array() {
    uxs::db::value v("1");
    MUST_THROW(v.emplace("10"));
    return 0;
}

int test_emplace_to_empty() {
    std::initializer_list<uxs::db::value> tst = {"10"};
    {
        uxs::db::value v;
        auto* p = &v.emplace(0, "10").value();
        VERIFY(p == &v[0]);
        CHECK_ARRAY(v, tst.size(), tst.begin());
    }
    {
        uxs::db::value v = uxs::db::make_array();
        auto* p = &v.emplace(0, "10").value();
        VERIFY(p == &v[0]);
        CHECK_ARRAY(v, tst.size(), tst.begin());
    }
    return 0;
}

int test_emplace_no_realloc() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    std::initializer_list<uxs::db::value> tst1 = {"1", "2", "3", "4", "5", "6"};
    std::initializer_list<uxs::db::value> tst2 = {"1", "2", "3", "7", "4", "5", "6"};
    std::initializer_list<uxs::db::value> tst3 = {"8", "1", "2", "3", "7", "4", "5", "6"};
    uxs::db::value v(init);
    v.reserve(10);
    // back
    auto r = v.as_array();
    auto* p = &v.emplace(5, "6").value();
    VERIFY(r.data() == v.as_array().data());
    VERIFY(p == &v[5]);
    CHECK_ARRAY(v, tst1.size(), tst1.begin());
    // mid
    p = &v.emplace(3, "7").value();
    VERIFY(r.data() == v.as_array().data());
    VERIFY(p == &v[3]);
    CHECK_ARRAY(v, tst2.size(), tst2.begin());
    // front
    p = &v.emplace(0, "8").value();
    VERIFY(r.data() == v.as_array().data());
    VERIFY(p == &v[0]);
    CHECK_ARRAY(v, tst3.size(), tst3.begin());
    return 0;
}

int test_emplace_needs_realloc() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4"};
    {
        std::initializer_list<uxs::db::value> tst = {"1", "2", "3", "4", "10"};
        uxs::db::value v(init);
        // back
        auto r = v.as_array();
        auto* p = &v.emplace(5, "10").value();
        VERIFY(r.data() != v.as_array().data());
        VERIFY(p == &v[4]);
        CHECK_ARRAY(v, tst.size(), tst.begin());
    }
    {
        std::initializer_list<uxs::db::value> tst = {"1", "2", "3", "10", "4"};
        uxs::db::value v(init);
        // back
        auto r = v.as_array();
        auto* p = &v.emplace(3, "10").value();
        VERIFY(r.data() != v.as_array().data());
        VERIFY(p == &v[3]);
        CHECK_ARRAY(v, tst.size(), tst.begin());
    }
    {
        std::initializer_list<uxs::db::value> tst = {"10", "1", "2", "3", "4"};
        uxs::db::value v(init);
        // front
        auto r = v.as_array();
        auto* p = &v.emplace(0, "10").value();
        VERIFY(r.data() != v.as_array().data());
        VERIFY(p == &v[0]);
        CHECK_ARRAY(v, tst.size(), tst.begin());
    }
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_an_array);
ADD_TEST_CASE("", "db::value", test_emplace_to_empty);
ADD_TEST_CASE("", "db::value", test_emplace_no_realloc);
ADD_TEST_CASE("", "db::value", test_emplace_needs_realloc);
