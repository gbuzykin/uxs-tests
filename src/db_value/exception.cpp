#include "db_value_tests.h"

#include <list>
#include <vector>

namespace {

int test_exception_safety() {
    std::string_view bad_str("A", std::numeric_limits<size_t>::max());
    // bad string init & assignment
    try {
        uxs::db::value v(bad_str);
        VERIFY(false);
    } catch (const std::exception&) {}
    try {
        uxs::db::value v;
        v = bad_str;
        VERIFY(false);
    } catch (const std::exception&) {}
    // initialize with array with bad string (from random access range)
    try {
        std::vector<std::string_view> init{"1", "2", "3", bad_str};
        uxs::db::value v(init.begin(), init.end());
        VERIFY(false);
    } catch (const std::exception&) {}
    try {
        std::vector<std::string_view> init{"1", "2", "3", bad_str};
        uxs::db::value v = uxs::db::make_array(init.begin(), init.end());
        VERIFY(false);
    } catch (const std::exception&) {}
    // initialize with array with bad string (from generic access range)
    try {
        std::list<std::string_view> init{"1", "2", "3", bad_str};
        uxs::db::value v(init.begin(), init.end());
        VERIFY(false);
    } catch (const std::exception&) {}
    try {
        std::list<std::string_view> init{"1", "2", "3", bad_str};
        uxs::db::value v = uxs::db::make_array(init.begin(), init.end());
        VERIFY(false);
    } catch (const std::exception&) {}
    // assignment of array with bad string
    try {
        std::vector<std::string_view> init{"1", "2", "3", bad_str};
        uxs::db::value v{"1", "2", "3"};
        v.assign(init.begin(), init.end());
        VERIFY(false);
    } catch (const std::exception&) {}
    try {
        std::vector<std::string_view> init{"1", bad_str};
        uxs::db::value v{"1", "2", "3"};
        v.assign(init.begin(), init.end());
        VERIFY(false);
    } catch (const std::exception&) {}
    // initialize with record with bad string
    try {
        std::vector<std::pair<std::string_view, std::string_view>> init{
            {"1", "A"}, {"2", "B"}, {"3", "C"}, {bad_str, "D"}};
        uxs::db::value v(init.begin(), init.end());
        VERIFY(false);
    } catch (const std::exception&) {}
    try {
        std::vector<std::pair<std::string_view, std::string_view>> init{
            {"1", "A"}, {"2", "B"}, {"3", "C"}, {bad_str, "D"}};
        uxs::db::value v = uxs::db::make_record(init.begin(), init.end());
        VERIFY(false);
    } catch (const std::exception&) {}
    try {
        std::vector<std::pair<std::string_view, std::string_view>> init{
            {"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", bad_str}};
        uxs::db::value v(init.begin(), init.end());
        VERIFY(false);
    } catch (const std::exception&) {}
    try {
        std::vector<std::pair<std::string_view, std::string_view>> init{
            {"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", bad_str}};
        uxs::db::value v = uxs::db::make_record(init.begin(), init.end());
        VERIFY(false);
    } catch (const std::exception&) {}
    // initialize with record initializer with bad string
    try {
        std::initializer_list<std::pair<std::string_view, uxs::db::value>> init{
            {"1", "A"}, {"2", "B"}, {"3", "C"}, {bad_str, "D"}};
        uxs::db::value v = uxs::db::make_record(init);
        VERIFY(false);
    } catch (const std::exception&) {}
    // assignment of record with bad string
    try {
        std::vector<std::pair<std::string_view, std::string_view>> init{
            {"1", "A"}, {"2", "B"}, {"3", "C"}, {bad_str, "D"}};
        uxs::db::value v{{"1", "A"}, {"2", "B"}, {"3", "C"}};
        v.assign(init.begin(), init.end());
        VERIFY(false);
    } catch (const std::exception&) {}
    try {
        std::vector<std::pair<std::string_view, std::string_view>> init{{"1", "A"}, {bad_str, "D"}};
        uxs::db::value v{{"1", "A"}, {"2", "B"}, {"3", "C"}};
        v.assign(init.begin(), init.end());
        VERIFY(false);
    } catch (const std::exception&) {}
    try {
        std::vector<std::pair<std::string_view, std::string_view>> init{
            {"1", "A"}, {"2", "B"}, {"3", "C"}, {"4", bad_str}};
        uxs::db::value v{{"1", "A"}, {"2", "B"}, {"3", "C"}};
        v.assign(init.begin(), init.end());
        VERIFY(false);
    } catch (const std::exception&) {}
    try {
        std::vector<std::pair<std::string_view, std::string_view>> init{{"1", "A"}, {"4", bad_str}};
        uxs::db::value v{{"1", "A"}, {"2", "B"}, {"3", "C"}};
        v.assign(init.begin(), init.end());
        VERIFY(false);
    } catch (const std::exception&) {}
    // insert of record initializer with bad string
    try {
        std::initializer_list<std::pair<std::string_view, uxs::db::value>> init{
            {"1", "A"}, {"2", "B"}, {"3", "C"}, {bad_str, "D"}};
        uxs::db::value v;
        v.insert(init);
        VERIFY(false);
    } catch (const std::exception&) {}
    try {
        std::initializer_list<std::pair<std::string_view, uxs::db::value>> init{
            {"1", "A"}, {"2", "B"}, {"3", "C"}, {bad_str, "D"}};
        uxs::db::value v{{"1", "A"}, {"2", "B"}, {"3", "C"}};
        v.insert(init);
        VERIFY(false);
    } catch (const std::exception&) {}
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_exception_safety);
