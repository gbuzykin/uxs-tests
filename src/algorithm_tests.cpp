#include "test_suite.h"

#include "uxs/algorithm.h"
#include "uxs/list.h"
#include "uxs/map.h"
#include "uxs/multiset.h"
#include "uxs/set.h"
#include "uxs/vector.h"

namespace {

int test_algorithm_0() {
    uxs::vector<std::string> v{"a", "b", "c", "d", "e"};
    VERIFY(uxs::find(v, "d") == std::make_pair(std::find(v.begin(), v.end(), "d"), true));
    VERIFY(uxs::find(v, "f") == std::make_pair(v.end(), false));
    VERIFY(uxs::contains(v, "d"));
    VERIFY(!uxs::contains(v, "f"));
    VERIFY(uxs::find_if(v, [](const std::string& s) { return s >= "d"; }) ==
           std::make_pair(std::find(v.begin(), v.end(), "d"), true));
    VERIFY(uxs::find_if(v, [](const std::string& s) { return s >= "f"; }) == std::make_pair(v.end(), false));
    VERIFY(uxs::any_of(v, [](const std::string& s) { return s >= "d"; }));
    VERIFY(!uxs::any_of(v, [](const std::string& s) { return s >= "f"; }));
    VERIFY(uxs::all_of(v, [](const std::string& s) { return s >= "a"; }));
    VERIFY(!uxs::all_of(v, [](const std::string& s) { return s >= "b"; }));
    VERIFY(uxs::none_of(v, [](const std::string& s) { return s >= "f"; }));
    VERIFY(!uxs::none_of(v, [](const std::string& s) { return s >= "d"; }));

    uxs::map<std::string, int> m{{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}};
    using value_type = std::decay_t<decltype(m)>::value_type;
    VERIFY(uxs::find(m, "d") == std::make_pair(m.find("d"), true));
    VERIFY(uxs::find(m, value_type{"d", 4}) == std::make_pair(m.find("d"), true));
    VERIFY(uxs::find(m, value_type{"d", 5}) == std::make_pair(m.end(), false));
    VERIFY(uxs::find(m, "f") == std::make_pair(m.end(), false));
    VERIFY(uxs::contains(m, "d"));
    VERIFY(uxs::contains(m, value_type{"d", 4}));
    VERIFY(!uxs::contains(m, value_type{"d", 5}));
    VERIFY(!uxs::contains(m, "f"));

    uxs::vector<std::string> v1{"a", "b", "c", "d"};
    uxs::vector<std::string> v2{"d", "c", "b", "a"};
    VERIFY(uxs::equal(uxs::reverse_range(v1), v2.begin()) == true);

    uxs::vector<std::string> v3{"a", "b", "b", "c", "d", "b"};
    VERIFY(uxs::count(v3, "b") == 3);
    VERIFY(uxs::count_if(v3, [](decltype(*v1.cbegin()) el) { return el == "b"; }) == 3);
    return 0;
}

int test_algorithm_1() {
    uxs::vector<std::string> v{"a", "b", "b", "b", "c", "c", "c", "c", "d", "e"};
    VERIFY(*uxs::erase_one(v, "d") == "e");
    VERIFY(uxs::erase_one(v, "f") == v.end());
    VERIFY(uxs::erase(v, "b") == 3 && v.size() == 6);
    VERIFY(uxs::erase_if(v, [](const std::string& s) { return s == "c"; }) == 4 && v.size() == 2);

    uxs::list<std::string> l{"a", "b", "b", "b", "c", "c", "c", "c", "d", "e"};
    VERIFY(*uxs::erase_one(l, "d") == "e");
    VERIFY(uxs::erase_one(l, "f") == l.end());
    VERIFY(uxs::erase(l, "b") == 3 && l.size() == 6);
    VERIFY(uxs::erase_if(l, [](const std::string& s) { return s == "c"; }) == 4 && l.size() == 2);

    uxs::map<std::string, int> m{{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}};
    using value_type = std::decay_t<decltype(m)>::value_type;
    VERIFY(*uxs::erase_one(m, "d") == value_type{"e", 5});
    VERIFY(uxs::erase_one(m, "f") == m.end());
    VERIFY(uxs::erase(m, value_type{"b", 2}) == 1 && m.size() == 3);
    VERIFY(uxs::erase(m, value_type{"c", 4}) == 0 && m.size() == 3);
    VERIFY(uxs::erase_if(m, [](const value_type& i) { return i.first == "c"; }) == 1 && m.size() == 2);

    uxs::multiset<std::string> ms{"a", "b", "b", "b", "c", "c", "c", "c", "d", "e"};
    VERIFY(uxs::erase_range(ms, uxs::make_range(ms.equal_range("b"))) == 3 && ms.size() == 7);

    uxs::set<std::string> s;
    // uxs::erase(s, "a");
    uxs::erase_range(s, s);

    uxs::vector<std::string> v1;
    uxs::erase_range(v1, v1);
    uxs::erase(v1, "aa" /*, "aa"*/);
    return 0;
}

int test_algorithm_2() {
    uxs::vector<std::string> v{"a", "b", "b", "b", "c", "c", "c", "c", "d", "e"};
    uxs::unique(v);
    VERIFY(v == decltype(v){"a", "b", "c", "d", "e"});

    uxs::list<std::string> l{"a", "b", "b", "b", "c", "c", "c", "c", "d", "e"};
    uxs::unique(l);
    VERIFY(l == decltype(l){"a", "b", "c", "d", "e"});
    return 0;
}

int test_algorithm_3() {
    uxs::vector<std::string> v{"a", "b", "c", "d", "e"};
    uxs::emplace_at(v, 3, "f");
    VERIFY(v == decltype(v){"a", "b", "c", "f", "d", "e"});
    uxs::erase_at(v, 2);
    VERIFY(v == decltype(v){"a", "b", "f", "d", "e"});
    return 0;
}

int test_algorithm_4() {
    uxs::vector<std::pair<std::string, int>> v{{"a", 0}, {"b", 1}, {"b", 2}, {"b", 3}, {"c", 4},
                                               {"c", 5}, {"c", 6}, {"c", 7}, {"d", 8}, {"e", 9}};

    VERIFY(uxs::lower_bound(v, "b") == uxs::find_if(v, uxs::is_equal_to("b")).first);
    VERIFY(uxs::upper_bound(v, "b") == uxs::find_if(v, uxs::is_equal_to("c")).first);
    VERIFY(uxs::equal_range(v, "b") ==
           uxs::make_range(uxs::find_if(v, uxs::is_equal_to("b")).first, uxs::find_if(v, uxs::is_equal_to("c")).first));

    VERIFY(uxs::lower_bound(v, "cc") == uxs::find_if(v, uxs::is_equal_to("d")).first);
    VERIFY(uxs::upper_bound(v, "cc") == uxs::find_if(v, uxs::is_equal_to("d")).first);
    VERIFY(uxs::equal_range(v, "cc") ==
           uxs::make_range(uxs::find_if(v, uxs::is_equal_to("d")).first, uxs::find_if(v, uxs::is_equal_to("d")).first));

    VERIFY(uxs::binary_find(v, "c") == uxs::find_if(v, uxs::is_equal_to("c")));
    VERIFY(uxs::binary_find(v, "cc") == std::make_pair(uxs::find_if(v, uxs::is_equal_to("d")).first, false));

    VERIFY(uxs::binary_contains(v, "c"));
    VERIFY(!uxs::binary_contains(v, "cc"));
    return 0;
}

int test_algorithm_5() {
    uxs::vector<std::pair<std::string, int>> v{{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}};
    using value_type = std::decay_t<decltype(v)>::value_type;

    auto result = uxs::binary_insert_unique(v, std::make_pair("cc", 30));
    VERIFY(result.second && v.size() == 6 && *result.first == value_type{"cc", 30} &&
           *std::next(result.first) == value_type{"d", 4});

    result = uxs::binary_insert_unique(v, std::make_pair("c", 35));
    VERIFY(!result.second && v.size() == 6 && *result.first == value_type{"c", 3} &&
           *std::next(result.first) == value_type{"cc", 30});

    uxs::binary_access_unique(v, "bb").second = 40;
    result = uxs::binary_find(v, "bb");
    VERIFY(result.second && v.size() == 7 && *result.first == value_type{"bb", 40} &&
           *std::next(result.first) == value_type{"c", 3});

    uxs::binary_access_unique(v, "bb").second = 41;
    result = uxs::binary_find(v, "bb");
    VERIFY(result.second && v.size() == 7 && *result.first == value_type{"bb", 41} &&
           *std::next(result.first) == value_type{"c", 3});

    auto it = uxs::binary_insert_new(v, std::make_pair("cc", 35));
    VERIFY(v.size() == 8 && *it == value_type{"cc", 35} && *std::next(it) == value_type{"cc", 30});

    uxs::binary_access_new(v, "bb").second = 45;
    result = uxs::binary_find(v, "bb");
    VERIFY(result.second && v.size() == 9 && *result.first == value_type{"bb", 45} &&
           *std::next(result.first) == value_type{"bb", 41});

    VERIFY(*uxs::binary_erase_one(v, "cc") == value_type{"cc", 30});
    VERIFY(v.size() == 8);

    VERIFY(uxs::erase_range(v, uxs::equal_range(v, "bb")) == 2);
    VERIFY(v.size() == 6);

    VERIFY(*uxs::binary_erase_one(v, "bb") == value_type{"c", 3});
    VERIFY(v.size() == 6);

    uxs::vector<std::tuple<std::string, int, double>> v1, v2;
    for (const auto& item : v1) { uxs::binary_insert_new(v2, item); }
    return 0;
}

void test_algorithm_binary_search(int iter_count) {
    std::array<int, 500> a;

    srand(0);

    for (int iter = 0, perc0 = -1; iter < iter_count; ++iter) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
            perc0 = perc;
        }

        for (size_t i = 0; i < a.size(); ++i) { a[i] = rand() % 100000; }
        std::sort(a.begin(), a.end());

        int k = rand() % 100000;
        auto lower = uxs::lower_bound(a, k);
        auto upper = uxs::upper_bound(a, k);
        VERIFY(std::lower_bound(a.begin(), a.end(), k) == lower);
        VERIFY(std::upper_bound(a.begin(), a.end(), k) == upper);
        VERIFY(uxs::equal_range(a, k) == uxs::make_range(lower, upper));
    }
}

int test_bruteforce_binary_search() {
#if defined(NDEBUG)
    const int N = 1000000;
#else   // defined(NDEBUG)
    const int N = 100000;
#endif  // defined(NDEBUG)
    test_algorithm_binary_search(N);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "algorithm", test_algorithm_0);
ADD_TEST_CASE("", "algorithm", test_algorithm_1);
ADD_TEST_CASE("", "algorithm", test_algorithm_2);
ADD_TEST_CASE("", "algorithm", test_algorithm_3);
ADD_TEST_CASE("", "algorithm", test_algorithm_4);
ADD_TEST_CASE("", "algorithm", test_algorithm_5);

ADD_TEST_CASE("1-bruteforce", "algorithm", test_bruteforce_binary_search);
