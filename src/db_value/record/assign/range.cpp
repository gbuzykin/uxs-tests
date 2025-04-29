#include "db_value_tests.h"

#include "uxs/list.h"
#include "uxs/vector.h"

using namespace uxs_test_suite;

namespace {

template<typename Src>
int test_range_empty_to_empty() {
    Src src;
    uxs::db::value v = uxs::db::make_record();
    v.assign(src.begin(), src.end());
    CHECK_RECORD_EMPTY(v);
    return 0;
}

template<typename Src>
int test_range_not_empty_to_empty() {
    Src src = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    uxs::db::value v = uxs::db::make_record();
    v.assign(src.begin(), src.end());
    CHECK_RECORD(v, src.size(), src.begin());
    return 0;
}

template<typename Src>
int test_range_more() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    Src src = {{"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"}, {"15", "e"}, {"16", "f"}, {"17", "g"}};
    uxs::db::value v(init);
    v.assign(src.begin(), src.end());
    CHECK_RECORD(v, src.size(), src.begin());
    return 0;
}

template<typename Src>
int test_range_less() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"},
                                                  {"5", "E"}, {"6", "F"}, {"7", "G"}};
    Src src = {{"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"}, {"15", "e"}};
    uxs::db::value v(init);
    v.assign(src.begin(), src.end());
    CHECK_RECORD(v, src.size(), src.begin());
    return 0;
}

template<typename Src>
int test_range_same_amount() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    Src src = {{"11", "a"}, {"12", "b"}, {"13", "c"}, {"14", "d"}, {"15", "e"}};
    uxs::db::value v(init);
    v.assign(src.begin(), src.end());
    CHECK_RECORD(v, src.size(), src.begin());
    return 0;
}

template<typename Src>
int test_range_empty_to_not_empty() {
    std::initializer_list<uxs::db::value> init = {{"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", "D"}, {"5", "E"}};
    Src src;
    uxs::db::value v(init);
    v.assign(src.begin(), src.end());
    CHECK_RECORD_EMPTY(v);
    return 0;
}

int test_range_empty_to_empty_random_access_range() {
    return test_range_empty_to_empty<uxs::vector<std::pair<std::string_view, uxs::db::value>>>();
}
int test_range_not_empty_to_empty_random_access_range() {
    return test_range_not_empty_to_empty<uxs::vector<std::pair<std::string_view, uxs::db::value>>>();
}
int test_range_more_random_access_range() {
    return test_range_more<uxs::vector<std::pair<std::string_view, uxs::db::value>>>();
}
int test_range_less_random_access_range() {
    return test_range_less<uxs::vector<std::pair<std::string_view, uxs::db::value>>>();
}
int test_range_same_amount_random_access_range() {
    return test_range_same_amount<uxs::vector<std::pair<std::string_view, uxs::db::value>>>();
}
int test_range_empty_to_not_empty_random_access_range() {
    return test_range_empty_to_not_empty<uxs::vector<std::pair<std::string_view, uxs::db::value>>>();
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_range_empty_to_empty_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_not_empty_to_empty_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_more_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_less_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_same_amount_random_access_range);
ADD_TEST_CASE("", "db::value", test_range_empty_to_not_empty_random_access_range);
