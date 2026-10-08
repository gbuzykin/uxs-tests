#include "test_suite.h"

#include "uxs/db/value.h"

#include <vector>

using namespace uxs_test_suite;

static_assert(
    uxs::db::detail::is_object_item<char, std::allocator<char>, std::pair<std::string_view, uxs::db::value>>::value, "");
static_assert(uxs::db::detail::is_object_item<wchar_t, std::allocator<wchar_t>,
                                              std::pair<std::wstring_view, uxs::db::basic_value<wchar_t>>>::value,
              "");
static_assert(
    !uxs::db::detail::is_object_item<char, std::allocator<char>, std::pair<std::wstring_view, uxs::db::value>>::value,
    "");
static_assert(!uxs::db::detail::is_object_item<char, int, int>::value, "");

static_assert(std::is_same<decltype(uxs::db::detail::get<0>(
                               std::declval<uxs::db::detail::object_item<char, std::allocator<char>>&>())),
                           std::string_view>::value,
              "");
static_assert(std::is_same<decltype(uxs::db::detail::get<1>(
                               std::declval<uxs::db::detail::object_item<char, std::allocator<char>>&>())),
                           uxs::db::value&>::value,
              "");
static_assert(std::is_same<decltype(uxs::db::detail::get<1>(
                               std::declval<const uxs::db::detail::object_item<char, std::allocator<char>>&>())),
                           const uxs::db::value&>::value,
              "");
static_assert(std::is_same<decltype(uxs::db::detail::get<1>(
                               std::declval<uxs::db::detail::object_item<char, std::allocator<char>>&&>())),
                           uxs::db::value&&>::value,
              "");
static_assert(std::is_same<decltype(uxs::db::detail::get<1>(
                               std::declval<const uxs::db::detail::object_item<char, std::allocator<char>>&&>())),
                           const uxs::db::value&>::value,
              "");

namespace {

int test_array_iterator() {
    std::vector<std::string_view> keys{"0", "1", "2", "3"};
    std::vector<int> init{1, 2, 3, 4};
    uxs::db::value v(init.begin(), init.end());
    size_t n = 0;

    n = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
        VERIFY((*it).value().as_int() == init[n]);
        VERIFY((*it).key() == keys[n]);
        ++n;
    }

    n = init.size();
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        VERIFY((*it).value().as_int() == init[n - 1]);
        VERIFY((*it).key() == keys[n - 1]);
        --n;
    }

    n = 0;
    for (const auto& el : v) {
        VERIFY(el.value().as_int() == init[n]);
        VERIFY(el.key() == keys[n]);
        ++n;
    }

    n = 0;
    for (auto&& el : v) {
        VERIFY(el.value().as_int() == init[n]);
        VERIFY(el.key() == keys[n]);
        el.value() = init[n];
        ++n;
    }

    n = init.size();
    for (const auto& el : est::make_reverse_range(v)) {
        VERIFY(el.value().as_int() == init[n - 1]);
        VERIFY(el.key() == keys[n - 1]);
        --n;
    }

    n = init.size();
    for (auto&& el : est::make_reverse_range(v)) {
        VERIFY(el.value().as_int() == init[n - 1]);
        VERIFY(el.key() == keys[n - 1]);
        el.value() = init[n - 1];
        --n;
    }

#if __cplusplus >= 201703L
    n = 0;
    for (const auto& [key, v] : v) {
        VERIFY(v.as_int() == init[n]);
        VERIFY(key == keys[n]);
        ++n;
    }

    n = 0;
    for (auto&& [key, v] : v) {
        VERIFY(v.as_int() == init[n]);
        VERIFY(key == keys[n]);
        v = init[n];
        ++n;
    }

    n = init.size();
    for (const auto& [key, v] : est::make_reverse_range(v)) {
        VERIFY(v.as_int() == init[n - 1]);
        VERIFY(key == keys[n - 1]);
        --n;
    }

    n = init.size();
    for (auto&& [key, v] : est::make_reverse_range(v)) {
        VERIFY(v.as_int() == init[n - 1]);
        VERIFY(key == keys[n - 1]);
        v = init[n - 1];
        --n;
    }

    for (auto&& [key, v] : v) {
        (void)key;
        v = "foo";
    }

    for (const auto& [key, v] : v) {
        (void)key;
        VERIFY(v.as_string_view() == "foo");
    }

    for (auto&& [key, v] : v) {
        (void)key;
        auto v_foo = std::move(v);
    }

    for (const auto& [key, v] : v) {
        (void)key;
        VERIFY(v.type() == uxs::db::dtype::null);
    }

    n = 0;
    for (auto&& [key, v] : v) {
        (void)key;
        v = init[n];
        ++n;
    }
#endif  // __cplusplus >= 201703L

    return 0;
}

int test_object_iterator() {
    std::vector<std::pair<std::string, int>> init{{"A", 1}, {"B", 2}, {"C", 3}, {"D", 4}};
    uxs::db::value v(init.begin(), init.end());
    size_t n = 0;

    n = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
        VERIFY((*it).value().as_int() == init[n].second);
        VERIFY((*it).key() == init[n].first);
        ++n;
    }

    n = init.size();
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        VERIFY((*it).value().as_int() == init[n - 1].second);
        VERIFY((*it).key() == init[n - 1].first);
        --n;
    }

    n = 0;
    for (const auto& el : v) {
        VERIFY(el.value().as_int() == init[n].second);
        VERIFY(el.key() == init[n].first);
        ++n;
    }

    n = 0;
    for (auto&& el : v) {
        VERIFY(el.value().as_int() == init[n].second);
        VERIFY(el.key() == init[n].first);
        el.value() = init[n].second;
        ++n;
    }

    n = init.size();
    for (const auto& el : est::make_reverse_range(v)) {
        VERIFY(el.value().as_int() == init[n - 1].second);
        VERIFY(el.key() == init[n - 1].first);
        --n;
    }

    n = init.size();
    for (auto&& el : est::make_reverse_range(v)) {
        VERIFY(el.value().as_int() == init[n - 1].second);
        VERIFY(el.key() == init[n - 1].first);
        el.value() = init[n - 1].second;
        --n;
    }

#if __cplusplus >= 201703L
    n = 0;
    for (const auto& [key, v] : v.as_object()) {
        VERIFY(v.as_int() == init[n].second);
        VERIFY(key == init[n].first);
        ++n;
    }

    n = 0;
    for (auto&& [key, v] : v.as_object()) {
        VERIFY(v.as_int() == init[n].second);
        VERIFY(key == init[n].first);
        v = init[n].second;
        ++n;
    }

    n = init.size();
    for (const auto& [key, v] : est::make_reverse_range(v.as_object())) {
        VERIFY(v.as_int() == init[n - 1].second);
        VERIFY(key == init[n - 1].first);
        --n;
    }

    n = init.size();
    for (auto&& [key, v] : est::make_reverse_range(v.as_object())) {
        VERIFY(v.as_int() == init[n - 1].second);
        VERIFY(key == init[n - 1].first);
        v = init[n - 1].second;
        --n;
    }

    for (auto&& [key, v] : v.as_object()) {
        (void)key;
        v = "foo";
    }

    for (const auto& [key, v] : v.as_object()) {
        (void)key;
        VERIFY(v.as_string_view() == "foo");
    }

    for (auto&& [key, v] : v.as_object()) {
        (void)key;
        auto v_foo = std::move(v);
    }

    for (const auto& [key, v] : v.as_object()) {
        (void)key;
        VERIFY(v.type() == uxs::db::dtype::null);
    }

    n = 0;
    for (auto&& [key, v] : v.as_object()) {
        (void)key;
        v = init[n].second;
        ++n;
    }

    n = 0;
    for (const auto& [key, v] : v) {
        VERIFY(v.as_int() == init[n].second);
        VERIFY(key == init[n].first);
        ++n;
    }

    n = 0;
    for (auto&& [key, v] : v) {
        VERIFY(v.as_int() == init[n].second);
        VERIFY(key == init[n].first);
        v = init[n].second;
        ++n;
    }

    n = init.size();
    for (const auto& [key, v] : est::make_reverse_range(v)) {
        VERIFY(v.as_int() == init[n - 1].second);
        VERIFY(key == init[n - 1].first);
        --n;
    }

    n = init.size();
    for (auto&& [key, v] : est::make_reverse_range(v)) {
        VERIFY(v.as_int() == init[n - 1].second);
        VERIFY(key == init[n - 1].first);
        v = init[n - 1].second;
        --n;
    }

    for (auto&& [key, v] : v) {
        (void)key;
        v = "foo";
    }

    for (const auto& [key, v] : v) {
        (void)key;
        VERIFY(v.as_string_view() == "foo");
    }

    for (auto&& [key, v] : v) {
        (void)key;
        auto v_foo = std::move(v);
    }

    for (const auto& [key, v] : v) {
        (void)key;
        VERIFY(v.type() == uxs::db::dtype::null);
    }

    n = 0;
    for (auto&& [key, v] : v) {
        (void)key;
        v = init[n].second;
        ++n;
    }
#endif  // __cplusplus >= 201703L

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_array_iterator);
ADD_TEST_CASE("", "db::value", test_object_iterator);
