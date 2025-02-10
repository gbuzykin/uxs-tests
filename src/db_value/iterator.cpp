#include "db_value_tests.h"

#include "uxs/db/value.h"

#include <vector>

using namespace uxs_test_suite;

namespace {

int test_array_iterator() {
    std::vector<int> init{1, 2, 3, 4};
    uxs::db::value v(init.begin(), init.end());
    size_t n = 0;

    n = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
        VERIFY(it.value().as_int() == init[n]);
        MUST_THROW(it.key());
        ++n;
    }

    n = init.size();
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        VERIFY(it.value().as_int() == init[n - 1]);
        MUST_THROW(it.key());
        --n;
    }

    n = 0;
    for (const auto& el : v) {
        VERIFY(el.value().as_int() == init[n]);
        MUST_THROW(el.key());
        ++n;
    }

    n = init.size();
    for (const auto& el : uxs::make_reverse_range(v)) {
        VERIFY(el.value().as_int() == init[n - 1]);
        MUST_THROW(el.key());
        --n;
    }

    return 0;
}

int test_record_iterator() {
    std::vector<std::pair<std::string, int>> init{{"A", 1}, {"B", 2}, {"C", 3}, {"D", 4}};
    uxs::db::value v(init.begin(), init.end());
    size_t n = 0;

    n = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
        VERIFY(it.value().as_int() == init[n].second);
        VERIFY(it.key() == init[n].first);
        ++n;
    }

    n = init.size();
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        VERIFY(it.value().as_int() == init[n - 1].second);
        VERIFY(it.key() == init[n - 1].first);
        --n;
    }

    n = 0;
    for (const auto& el : v) {
        VERIFY(el.value().as_int() == init[n].second);
        VERIFY(el.key() == init[n].first);
        ++n;
    }

    n = init.size();
    for (const auto& el : uxs::make_reverse_range(v)) {
        VERIFY(el.value().as_int() == init[n - 1].second);
        VERIFY(el.key() == init[n - 1].first);
        --n;
    }

#if __cplusplus >= 201703L
    n = 0;
    for (const auto& [key, v] : v.as_record()) {
        VERIFY(v.as_int() == init[n].second);
        VERIFY(key == init[n].first);
        ++n;
    }

    n = init.size();
    for (const auto& [key, v] : uxs::make_reverse_range(v.as_record())) {
        VERIFY(v.as_int() == init[n - 1].second);
        VERIFY(key == init[n - 1].first);
        --n;
    }

    n = 0;
    for (const auto& [key, v] : v) {
        VERIFY(v.as_int() == init[n].second);
        VERIFY(key == init[n].first);
        ++n;
    }

    n = init.size();
    for (const auto& [key, v] : uxs::make_reverse_range(v)) {
        VERIFY(v.as_int() == init[n - 1].second);
        VERIFY(key == init[n - 1].first);
        --n;
    }
#endif  // __cplusplus >= 201703L
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_array_iterator);
ADD_TEST_CASE("", "db::value", test_record_iterator);
