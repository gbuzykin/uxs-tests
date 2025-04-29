#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_not_an_array() {
    uxs::db::value v("1");
    MUST_THROW(v.resize(0));
    return 0;
}

int test_resize_empty_to_empty() {
    {
        uxs::db::value v;
        v.resize(0);
        VERIFY(v.as_array().data() == nullptr);
        CHECK_ARRAY_EMPTY(v);
    }
    {
        uxs::db::value v = uxs::db::make_array();
        v.resize(0);
        VERIFY(v.as_array().data() == nullptr);
        CHECK_ARRAY_EMPTY(v);
    }
    return 0;
}

int test_resize_not_empty_to_empty() {
    std::initializer_list<uxs::db::value> tst = {nullptr, nullptr, nullptr, nullptr, nullptr};
    {
        uxs::db::value v;
        v.resize(5);
        CHECK_ARRAY(v, tst.size(), tst.begin());
    }
    {
        uxs::db::value v = uxs::db::make_array();
        v.resize(5);
        CHECK_ARRAY(v, tst.size(), tst.begin());
    }
    return 0;
}

int test_resize_more_no_realloc() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    std::initializer_list<uxs::db::value> tst = {"1", "2", "3", "4", "5", nullptr, nullptr};
    uxs::db::value v(init);
    v.reserve(10);
    auto r = v.as_array();
    v.resize(7);
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY(v, tst.size(), tst.begin());
    return 0;
}

int test_resize_more_needs_realloc() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    std::initializer_list<uxs::db::value> tst = {"1",     "2",     "3",     "4",     "5",     nullptr,
                                                 nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    uxs::db::value v(init);
    auto r = v.as_array();
    v.resize(12);
    VERIFY(r.data() != v.as_array().data());
    CHECK_ARRAY(v, tst.size(), tst.begin());
    return 0;
}

int test_resize_less() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5", "6", "7"};
    std::initializer_list<uxs::db::value> tst = {"1", "2", "3", "4", "5"};
    uxs::db::value v(init);
    auto r = v.as_array();
    v.resize(5);
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY(v, tst.size(), tst.begin());
    return 0;
}

int test_resize_same_amount() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    uxs::db::value v(init);
    auto r = v.as_array();
    v.resize(5);
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY(v, init.size(), init.begin());
    return 0;
}

int test_resize_empty_to_not_empty() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    uxs::db::value v(init);
    auto r = v.as_array();
    v.resize(0);
    VERIFY(r.data() == v.as_array().data());
    CHECK_ARRAY_EMPTY(v);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_an_array);
ADD_TEST_CASE("", "db::value", test_resize_empty_to_empty);
ADD_TEST_CASE("", "db::value", test_resize_not_empty_to_empty);
ADD_TEST_CASE("", "db::value", test_resize_more_no_realloc);
ADD_TEST_CASE("", "db::value", test_resize_more_needs_realloc);
ADD_TEST_CASE("", "db::value", test_resize_less);
ADD_TEST_CASE("", "db::value", test_resize_same_amount);
ADD_TEST_CASE("", "db::value", test_resize_empty_to_not_empty);
