#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_assign() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4"};
    std::initializer_list<uxs::db::value> init2 = {"5", "6", "7"};

    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v)[0] == &std::as_const(v2)[0]);

    v2 = init2;

    CHECK_ARRAY(v, init.size(), init.begin());
    CHECK_ARRAY(v2, init2.size(), init2.begin());
    return 0;
}

int test_assign_string() {
    uxs::db::value v("abc");
    uxs::db::value v2(v);
    VERIFY(v.as_string_view().data() == v2.as_string_view().data());

    v2 = "asdf";

    VERIFY(v.as_string_view() == "abc");
    VERIFY(v2.as_string_view() == "asdf");
    return 0;
}

int test_append_string() {
    uxs::db::value v("abc");
    uxs::db::value v2(v);
    VERIFY(v.as_string_view().data() == v2.as_string_view().data());

    v2.append_string("asdf");

    VERIFY(v.as_string_view() == "abc");
    VERIFY(v2.as_string_view() == "abcasdf");
    return 0;
}

int test_insert() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4"};
    std::initializer_list<uxs::db::value> ins = {"5", "6", "7"};
    std::initializer_list<uxs::db::value> tst = {"1", "5", "6", "7", "2", "3", "4"};

    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v)[0] == &std::as_const(v2)[0]);

    v2.insert(1, ins);

    CHECK_ARRAY(v, init.size(), init.begin());
    CHECK_ARRAY(v2, tst.size(), tst.begin());
    return 0;
}

int test_emplace_back() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4"};
    std::initializer_list<uxs::db::value> tst = {"1", "2", "3", "4", "5"};

    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v)[0] == &std::as_const(v2)[0]);

    v2.emplace_back("5");

    CHECK_ARRAY(v, init.size(), init.begin());
    CHECK_ARRAY(v2, tst.size(), tst.begin());
    return 0;
}

int test_pop_back() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4"};
    std::initializer_list<uxs::db::value> tst = {"1", "2", "3"};

    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v)[0] == &std::as_const(v2)[0]);

    v2.pop_back();

    CHECK_ARRAY(v, init.size(), init.begin());
    CHECK_ARRAY(v2, tst.size(), tst.begin());
    return 0;
}

int test_clear() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4"};

    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v)[0] == &std::as_const(v2)[0]);

    v2.clear();

    CHECK_ARRAY(v, init.size(), init.begin());
    CHECK_ARRAY_EMPTY(v2);
    return 0;
}

int test_erase() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4"};
    std::initializer_list<uxs::db::value> tst = {"1", "3", "4"};

    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v)[0] == &std::as_const(v2)[0]);

    v2.erase(1);

    CHECK_ARRAY(v, init.size(), init.begin());
    CHECK_ARRAY(v2, tst.size(), tst.begin());
    return 0;
}

int test_reserve() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4"};

    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v)[0] == &std::as_const(v2)[0]);

    v2.reserve(1000);

    VERIFY(&v[0] != &v2[0]);
    CHECK_ARRAY(v, init.size(), init.begin());
    CHECK_ARRAY(v2, init.size(), init.begin());
    return 0;
}

int test_resize() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4"};
    std::initializer_list<uxs::db::value> tst = {"1", "2", "3", "4", "0"};

    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v)[0] == &std::as_const(v2)[0]);

    v2.resize(5, "0");

    CHECK_ARRAY(v, init.size(), init.begin());
    CHECK_ARRAY(v2, tst.size(), tst.begin());
    return 0;
}

int test_access() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4"};
    std::initializer_list<uxs::db::value> tst = {"1", "5", "3", "4"};

    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v)[0] == &std::as_const(v2)[0]);

    v2[1] = "5";

    CHECK_ARRAY(v, init.size(), init.begin());
    CHECK_ARRAY(v2, tst.size(), tst.begin());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_assign);
ADD_TEST_CASE("", "db::value", test_assign_string);
ADD_TEST_CASE("", "db::value", test_append_string);
ADD_TEST_CASE("", "db::value", test_insert);
ADD_TEST_CASE("", "db::value", test_emplace_back);
ADD_TEST_CASE("", "db::value", test_pop_back);
ADD_TEST_CASE("", "db::value", test_clear);
ADD_TEST_CASE("", "db::value", test_erase);
ADD_TEST_CASE("", "db::value", test_reserve);
ADD_TEST_CASE("", "db::value", test_resize);
ADD_TEST_CASE("", "db::value", test_access);
