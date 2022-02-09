#include "test_suite.h"
#include "util/functional.h"
#include "util/vector.h"

namespace {

int test_functional_0() {
    std::pair<std::string, int> p1{"Hello", 10};
    std::tuple<std::string, int, double> p2{"Hello", 10, 2.5};
    std::string p3{"Hello"};

    VERIFY(std::is_same<std::decay_t<decltype(util::get_n<0>()(p1))>, std::string>::value);
    VERIFY(std::is_same<std::decay_t<decltype(util::get_n<1>()(p1))>, int>::value);
    VERIFY(util::get_n<0>()(p1) == "Hello");

    VERIFY(std::is_same<std::decay_t<decltype(util::get_n<0>()(p2))>, std::string>::value);
    VERIFY(std::is_same<std::decay_t<decltype(util::get_n<1>()(p2))>, int>::value);
    VERIFY(std::is_same<std::decay_t<decltype(util::get_n<2>()(p2))>, double>::value);
    VERIFY(util::get_n<0>()(p2) == "Hello");

    VERIFY(std::is_same<std::decay_t<decltype(util::get_n<0>()(p3))>, std::string>::value);
    VERIFY(util::get_n<0>()(p3) == "Hello");

    return 0;
}

util::vector<std::string> g_vec;

void g_func(const std::string& s) { g_vec.emplace_back(s); }

int test_functional_1() {
    auto it3 = util::function_caller(&g_func);
    auto it4 = util::function_caller(&g_func);
    it3 = it4;

    g_vec.clear();
    *it3 = "aaa";
    VERIFY(g_vec[0] == "aaa");

    std::string s2;
    auto l2 = [=](const std::string& s) { g_vec.emplace_back(s2 + s); };

    auto it5 = util::function_caller(l2);
    auto it6 = util::function_caller(l2);
    it5 = it6;

    g_vec.clear();
    *it5 = "bbb";
    VERIFY(g_vec[0] == "bbb");

    auto l = [](const std::string& s) { g_vec.emplace_back(s); };

    auto it7 = util::function_caller(l);
    auto it8 = util::function_caller(l);
    it7 = it8;

    g_vec.clear();
    *it7 = "ccc";
    VERIFY(g_vec[0] == "ccc");

    struct F {
        void operator()(const std::string& s) const { g_vec.emplace_back(s); }
    } f;

    auto it9 = util::function_caller(f);
    auto it10 = util::function_caller(f);
    it9 = it10;

    g_vec.clear();
    *it9 = "ddd";
    VERIFY(g_vec[0] == "ddd");

    return 0;
}

int test_functional_2() {
    util::vector<std::string> strs{"aaa", "bbb", "ccc"};
    g_vec.clear();
    std::copy(strs.begin(), strs.end(), util::function_caller([](const std::string& s) { g_vec.emplace_back(s); }));
    VERIFY(g_vec[0] == "aaa");
    VERIFY(g_vec[1] == "bbb");
    VERIFY(g_vec[2] == "ccc");
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "functional", test_functional_0);
ADD_TEST_CASE("", "functional", test_functional_1);
ADD_TEST_CASE("", "functional", test_functional_2);
