#include "db_value_tests.h"

#include "uxs/list.h"
#include "uxs/vector.h"

using namespace uxs_test_suite;

namespace {

template<typename Src>
int test_range_empty() {
    Src init;
    uxs::db::value v(init.begin(), init.end());
    CHECK_RECORD_EMPTY(v);
    return 0;
}

template<typename Src>
int test_range_not_empty() {
    Src init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    std::pair<std::string_view, std::string_view> tst[] = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    uxs::db::value v(init.begin(), init.end());
    CHECK_RECORD(v, init.size(), init.begin());
    return 0;
}

template<typename Src>
int test_range_make_record_empty() {
    Src init;
    uxs::db::value v = uxs::db::make_record(init.begin(), init.end());
    CHECK_RECORD_EMPTY(v);
    return 0;
}

template<typename Src>
int test_initializer_make_record_not_empty() {
    Src init = {{"1", "A"}, {"2", 3.1415}};
    uxs::db::value v = uxs::db::make_record(init);
    VERIFY(v.is_record() && v.size() == 2);
    VERIFY(v["2"].as_double() == 3.1415);
    return 0;
}

int test_range_empty_random_access_range() {
    return test_range_empty<uxs::vector<std::pair<std::string_view, std::string_view>>>();
}
int test_range_not_empty_random_access_range() {
    return test_range_not_empty<uxs::vector<std::pair<std::string_view, std::string_view>>>();
}
int test_range_make_record_empty_random_access_range() {
    return test_range_empty<uxs::vector<std::pair<std::string_view, std::string_view>>>();
}
int test_range_make_record_not_empty_random_access_range() {
    return test_range_empty<uxs::vector<std::pair<std::string_view, std::string_view>>>();
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_range_empty_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_not_empty_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_make_record_empty_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_make_record_not_empty_random_access_range);
