#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_assign() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}};
    std::initializer_list<uxs::db::value> init2 = {{"5", "E"}, {"6", "F"}, {"7", "G"}};

    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v).at("1") == &std::as_const(v2).at("1"));

    v2 = init2;

    CHECK_RECORD(v, init.size(), init.begin());
    CHECK_RECORD(v2, init2.size(), init2.begin());
    return 0;
}

int test_insert() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::initializer_list<std::pair<std::string_view, uxs::db::value>> ins = {{"6", "F"}, {"7", "G"}, {"8", "H"}};
    std::initializer_list<uxs::db::value> tst = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"},
                                                 {"5", "E"}, {"6", "F"}, {"7", "G"}, {"8", "H"}};
    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v).at("1") == &std::as_const(v2).at("1"));

    v2.insert(ins);

    CHECK_RECORD(v, init.size(), init.begin());
    CHECK_RECORD(v2, tst.size(), tst.begin());
    return 0;
}

int test_emplace() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::initializer_list<uxs::db::value> tst = {{"1", "A"}, {"2", "B"}, {"3", "C"},
                                                 {"4", "D"}, {"5", "E"}, {"6", "F"}};

    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v).at("1") == &std::as_const(v2).at("1"));

    v2.emplace("6", "F").value();

    CHECK_RECORD(v, init.size(), init.begin());
    CHECK_RECORD(v2, tst.size(), tst.begin());
    return 0;
}

int test_clear() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};

    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v).at("1") == &std::as_const(v2).at("1"));

    v2.clear();

    CHECK_RECORD(v, init.size(), init.begin());
    CHECK_RECORD_EMPTY(v2);
    return 0;
}

int test_erase() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::initializer_list<uxs::db::value> tst = {{"1", "A"}, {"2", "B"}, {"4", "D"}, {"5", "E"}};

    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v).at("1") == &std::as_const(v2).at("1"));

    v2.erase("3");

    CHECK_RECORD(v, init.size(), init.begin());
    CHECK_RECORD(v2, tst.size(), tst.begin());
    return 0;
}

int test_erase2() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::initializer_list<uxs::db::value> tst = {{"1", "A"}, {"2", "B"}, {"4", "D"}, {"5", "E"}};

    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v).at("1") == &std::as_const(v2).at("1"));

    auto it = std::as_const(v2).find("3");

    v2.erase(it);

    CHECK_RECORD(v, init.size(), init.begin());
    CHECK_RECORD(v2, tst.size(), tst.begin());
    return 0;
}

int test_access() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::initializer_list<uxs::db::value> tst = {{"1", "A"}, {"2", "B"}, {"3", "Z"}, {"4", "D"}, {"5", "E"}};

    uxs::db::value v(init);
    uxs::db::value v2(v);
    VERIFY(&std::as_const(v).at("1") == &std::as_const(v2).at("1"));

    v2.at("3") = "Z";

    CHECK_RECORD(v, init.size(), init.begin());
    CHECK_RECORD(v2, tst.size(), tst.begin());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_assign);
ADD_TEST_CASE("", "db::value", test_insert);
ADD_TEST_CASE("", "db::value", test_emplace);
ADD_TEST_CASE("", "db::value", test_clear);
ADD_TEST_CASE("", "db::value", test_erase);
ADD_TEST_CASE("", "db::value", test_erase2);
ADD_TEST_CASE("", "db::value", test_access);
