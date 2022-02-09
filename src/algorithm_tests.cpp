#include "test_suite.h"
#include "util/algorithm.h"
#include "util/list.h"
#include "util/map.h"
#include "util/multiset.h"
#include "util/set.h"
#include "util/vector.h"

namespace {

int test_algorithm_0() {
    util::vector<std::string> v{"a", "b", "c", "d", "e"};
    VERIFY(util::find(v, "d") == std::make_pair(std::find(v.begin(), v.end(), "d"), true));
    VERIFY(util::find(v, "f") == std::make_pair(v.end(), false));
    VERIFY(util::contains(v, "d"));
    VERIFY(!util::contains(v, "f"));
    VERIFY(util::find_if(v, [](const std::string& s) { return s >= "d"; }) ==
           std::make_pair(std::find(v.begin(), v.end(), "d"), true));
    VERIFY(util::find_if(v, [](const std::string& s) { return s >= "f"; }) == std::make_pair(v.end(), false));
    VERIFY(util::any_of(v, [](const std::string& s) { return s >= "d"; }));
    VERIFY(!util::any_of(v, [](const std::string& s) { return s >= "f"; }));
    VERIFY(util::all_of(v, [](const std::string& s) { return s >= "a"; }));
    VERIFY(!util::all_of(v, [](const std::string& s) { return s >= "b"; }));
    VERIFY(util::none_of(v, [](const std::string& s) { return s >= "f"; }));
    VERIFY(!util::none_of(v, [](const std::string& s) { return s >= "d"; }));

    util::map<std::string, int> m{{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}};
    using value_type = std::decay_t<decltype(m)>::value_type;
    VERIFY(util::find(m, "d") == std::make_pair(m.find("d"), true));
    VERIFY(util::find(m, value_type{"d", 4}) == std::make_pair(m.find("d"), true));
    VERIFY(util::find(m, value_type{"d", 5}) == std::make_pair(m.end(), false));
    VERIFY(util::find(m, "f") == std::make_pair(m.end(), false));
    VERIFY(util::contains(m, "d"));
    VERIFY(util::contains(m, value_type{"d", 4}));
    VERIFY(!util::contains(m, value_type{"d", 5}));
    VERIFY(!util::contains(m, "f"));

    util::vector<std::string> v1{"a", "b", "c", "d"};
    util::vector<std::string> v2{"d", "c", "b", "a"};
    VERIFY(util::equal(util::reverse_range(v1), v2.begin()) == true);

    util::vector<std::string> v3{"a", "b", "b", "c", "d", "b"};
    VERIFY(util::count(v3, "b") == 3);
    VERIFY(util::count_if(v3, [](decltype(*v1.cbegin()) el) { return el == "b"; }) == 3);
    return 0;
}

int test_algorithm_1() {
    util::vector<std::string> v{"a", "b", "b", "b", "c", "c", "c", "c", "d", "e"};
    VERIFY(*util::erase_one(v, "d") == "e");
    VERIFY(util::erase_one(v, "f") == v.end());
    VERIFY(util::erase(v, "b") == 3 && v.size() == 6);
    VERIFY(util::erase_if(v, [](const std::string& s) { return s == "c"; }) == 4 && v.size() == 2);

    util::list<std::string> l{"a", "b", "b", "b", "c", "c", "c", "c", "d", "e"};
    VERIFY(*util::erase_one(l, "d") == "e");
    VERIFY(util::erase_one(l, "f") == l.end());
    VERIFY(util::erase(l, "b") == 3 && l.size() == 6);
    VERIFY(util::erase_if(l, [](const std::string& s) { return s == "c"; }) == 4 && l.size() == 2);

    util::map<std::string, int> m{{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}};
    using value_type = std::decay_t<decltype(m)>::value_type;
    VERIFY(*util::erase_one(m, "d") == value_type{"e", 5});
    VERIFY(util::erase_one(m, "f") == m.end());
    VERIFY(util::erase(m, value_type{"b", 2}) == 1 && m.size() == 3);
    VERIFY(util::erase(m, value_type{"c", 4}) == 0 && m.size() == 3);
    VERIFY(util::erase_if(m, [](const value_type& i) { return i.first == "c"; }) == 1 && m.size() == 2);

    util::multiset<std::string> ms{"a", "b", "b", "b", "c", "c", "c", "c", "d", "e"};
    VERIFY(util::erase(ms, util::make_range(ms.equal_range("b"))) == 3 && ms.size() == 7);

    util::set<std::string> s;
    // util::erase(s, "a");
    util::erase(s, s);

    util::vector<std::string> v1;
    util::erase(v1, v1);
    util::erase(v1, "aa" /*, "aa"*/);
    return 0;
}

int test_algorithm_2() {
    util::vector<std::string> v{"a", "b", "b", "b", "c", "c", "c", "c", "d", "e"};
    util::unique(v);
    VERIFY(v == decltype(v){"a", "b", "c", "d", "e"});

    util::list<std::string> l{"a", "b", "b", "b", "c", "c", "c", "c", "d", "e"};
    util::unique(l);
    VERIFY(l == decltype(l){"a", "b", "c", "d", "e"});
    return 0;
}

int test_algorithm_3() {
    util::vector<std::string> v{"a", "b", "c", "d", "e"};
    util::emplace_at(v, 3, "f");
    VERIFY(v == decltype(v){"a", "b", "c", "f", "d", "e"});
    util::erase_at(v, 2);
    VERIFY(v == decltype(v){"a", "b", "f", "d", "e"});
    return 0;
}

int test_algorithm_4() {
    util::vector<std::pair<std::string, int>> v{{"a", 0}, {"b", 1}, {"b", 2}, {"b", 3}, {"c", 4},
                                                {"c", 5}, {"c", 6}, {"c", 7}, {"d", 8}, {"e", 9}};

    VERIFY(util::lower_bound(v, "b") == util::find_if(v, util::is_equal_to("b")).first);
    VERIFY(util::upper_bound(v, "b") == util::find_if(v, util::is_equal_to("c")).first);
    VERIFY(util::equal_range(v, "b") == util::make_range(util::find_if(v, util::is_equal_to("b")).first,
                                                         util::find_if(v, util::is_equal_to("c")).first));

    VERIFY(util::lower_bound(v, "cc") == util::find_if(v, util::is_equal_to("d")).first);
    VERIFY(util::upper_bound(v, "cc") == util::find_if(v, util::is_equal_to("d")).first);
    VERIFY(util::equal_range(v, "cc") == util::make_range(util::find_if(v, util::is_equal_to("d")).first,
                                                          util::find_if(v, util::is_equal_to("d")).first));

    VERIFY(util::binary_find(v, "c") == util::find_if(v, util::is_equal_to("c")));
    VERIFY(util::binary_find(v, "cc") == std::make_pair(util::find_if(v, util::is_equal_to("d")).first, false));

    VERIFY(util::binary_contains(v, "c"));
    VERIFY(!util::binary_contains(v, "cc"));
    return 0;
}

int test_algorithm_5() {
    util::vector<std::pair<std::string, int>> v{{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}};
    using value_type = std::decay_t<decltype(v)>::value_type;

    auto result = util::binary_insert_unique(v, std::make_pair("cc", 30));
    VERIFY(result.second && v.size() == 6 && *result.first == value_type{"cc", 30} &&
           *std::next(result.first) == value_type{"d", 4});

    result = util::binary_insert_unique(v, std::make_pair("c", 35));
    VERIFY(!result.second && v.size() == 6 && *result.first == value_type{"c", 3} &&
           *std::next(result.first) == value_type{"cc", 30});

    util::binary_access_unique(v, "bb").second = 40;
    result = util::binary_find(v, "bb");
    VERIFY(result.second && v.size() == 7 && *result.first == value_type{"bb", 40} &&
           *std::next(result.first) == value_type{"c", 3});

    util::binary_access_unique(v, "bb").second = 41;
    result = util::binary_find(v, "bb");
    VERIFY(result.second && v.size() == 7 && *result.first == value_type{"bb", 41} &&
           *std::next(result.first) == value_type{"c", 3});

    auto it = util::binary_insert_new(v, std::make_pair("cc", 35));
    VERIFY(v.size() == 8 && *it == value_type{"cc", 35} && *std::next(it) == value_type{"cc", 30});

    util::binary_access_new(v, "bb").second = 45;
    result = util::binary_find(v, "bb");
    VERIFY(result.second && v.size() == 9 && *result.first == value_type{"bb", 45} &&
           *std::next(result.first) == value_type{"bb", 41});

    VERIFY(*util::binary_erase_one(v, "cc") == value_type{"cc", 30});
    VERIFY(v.size() == 8);

    VERIFY(util::erase(v, util::equal_range(v, "bb")) == 2);
    VERIFY(v.size() == 6);

    VERIFY(*util::binary_erase_one(v, "bb") == value_type{"c", 3});
    VERIFY(v.size() == 6);

    util::vector<std::tuple<std::string, int, double>> v1, v2;
    for (const auto& item : v1) { util::binary_insert_new(v2, item); }
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "algorithm", test_algorithm_0);
ADD_TEST_CASE("", "algorithm", test_algorithm_1);
ADD_TEST_CASE("", "algorithm", test_algorithm_2);
ADD_TEST_CASE("", "algorithm", test_algorithm_3);
ADD_TEST_CASE("", "algorithm", test_algorithm_4);
ADD_TEST_CASE("", "algorithm", test_algorithm_5);
