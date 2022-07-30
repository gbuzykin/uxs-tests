#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_not_a_record() {
    uxs::db::value v("1");
    MUST_THROW(v.emplace("1", "A"));
    return 0;
}

int test_emplace_to_empty() {
    std::pair<std::string_view, std::string_view> tst[] = {{"1", "A"}};
    {
        uxs::db::value v;
        auto* p = &v.emplace("1", "A")->second;
        VERIFY(p == &v["1"]);
        CHECK_RECORD(v, 1, tst);
    }
    {
        uxs::db::value v = uxs::db::make_record();
        auto* p = &v.emplace("1", "A")->second;
        VERIFY(p == &v["1"]);
        CHECK_RECORD(v, 1, tst);
    }
    return 0;
}

int test_emplace() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::pair<std::string_view, std::string_view> tst[] = {{"1", "A"}, {"2", "B"}, {"3", "C"},
                                                           {"4", "D"}, {"5", "E"}, {"6", "F"}};
    uxs::db::value v(init);
    auto* p = &v.emplace("6", "F")->second;
    VERIFY(p == &v["6"]);
    CHECK_RECORD(v, 6, tst);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_a_record);
ADD_TEST_CASE("", "db::value", test_emplace_to_empty);
ADD_TEST_CASE("", "db::value", test_emplace);
