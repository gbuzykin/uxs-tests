#include "uxs_testsuite/suite.h"

#include <uxs/iterator.h>
#include <uxs/zip_iterator.h>

#include <algorithm>
#include <forward_list>
#include <iterator>
#include <list>
#include <tuple>
#include <vector>

using namespace uxs_testsuite;

static_assert(est::is_output_iterator<int*, int>::value, "");
static_assert(est::is_output_iterator<std::string::iterator, char>::value, "");
static_assert(!est::is_output_iterator<std::string::const_iterator, char>::value, "");
static_assert(!est::is_output_iterator<std::string::iterator, std::string>::value, "");
static_assert(!est::is_output_iterator<int, int>::value, "");
static_assert(std::is_same<est::iterator_value_t<std::string::iterator>, char>::value, "");
static_assert(!std::is_same<est::iterator_value_t<std::string::iterator>, int>::value, "");
static_assert(is_defined<est::iterator_value<std::string::iterator>>::value, "");
static_assert(!is_defined<est::iterator_value<int>>::value, "");
static_assert(std::is_same<est::range_element_t<std::string>, char>::value, "");
static_assert(!std::is_same<est::range_element_t<std::string>, int>::value, "");
static_assert(is_defined<est::range_element<std::string>>::value, "");
static_assert(!is_defined<est::range_element<int>>::value, "");
static_assert(est::is_contiguous_range<std::string, const char>::value, "");
static_assert(!est::is_contiguous_range<std::string, int>::value, "");
static_assert(est::is_contiguous_range<std::vector<int>, int>::value, "");
static_assert(!est::is_contiguous_range<std::list<int>, int>::value, "");

namespace {

int test_iterator_0() {
    std::forward_list<int> fl1;
    std::forward_list<double> fl2;
    auto fl_zip = est::make_zip_iterator(fl1.begin(), fl2.begin());
    static_assert(std::is_same<typename std::iterator_traits<decltype(fl_zip)>::iterator_category,
                               std::forward_iterator_tag>::value,
                  "");

    std::list<int> l1;
    std::list<double> l2;
    auto l_zip = est::make_zip_iterator(l1.begin(), l2.begin());
    static_assert(std::is_same<typename std::iterator_traits<decltype(l_zip)>::iterator_category,
                               std::bidirectional_iterator_tag>::value,
                  "");

    std::vector<int> v1;
    std::vector<double> v2;
    auto v_zip = est::make_zip_iterator(v1.begin(), v2.begin());
    static_assert(std::is_same<typename std::iterator_traits<decltype(v_zip)>::iterator_category,
                               std::random_access_iterator_tag>::value,
                  "");

    static_assert(std::is_same<decltype(std::get<0>(*fl_zip)), int&>::value, "");
    static_assert(std::is_same<decltype(std::get<0>(*l_zip)), int&>::value, "");
    static_assert(std::is_same<decltype(std::get<0>(*v_zip)), int&>::value, "");

    static_assert(std::is_same<decltype(std::get<1>(*fl_zip)), double&>::value, "");
    static_assert(std::is_same<decltype(std::get<1>(*l_zip)), double&>::value, "");
    static_assert(std::is_same<decltype(std::get<1>(*v_zip)), double&>::value, "");

    static_assert(std::is_same<decltype(fl_zip.base<0>()), decltype(fl1.begin())>::value, "");
    static_assert(std::is_same<decltype(l_zip.base<0>()), decltype(l1.begin())>::value, "");
    static_assert(std::is_same<decltype(v_zip.base<0>()), decltype(v1.begin())>::value, "");

    static_assert(std::is_same<decltype(fl_zip.base<1>()), decltype(fl2.begin())>::value, "");
    static_assert(std::is_same<decltype(l_zip.base<1>()), decltype(l2.begin())>::value, "");
    static_assert(std::is_same<decltype(v_zip.base<1>()), decltype(v2.begin())>::value, "");
    return 0;
}

int test_iterator_1() {
    std::list<int> l1{1, 5, 11};
    std::list<double> l2{1.5, 5.5, 7., 13.};

    size_t c = 0;
    int s1 = 0;
    double s2 = 0;
    for (const auto& item : est::zip(l2)) {
        static_assert(std::is_same<decltype(std::get<0>(item)), double&>::value, "");
        s2 += std::get<0>(item), ++c;
    }
    VERIFY(c == 4 && s2 == 27.);

    c = 0, s1 = 0, s2 = 0;
    for (const auto& item : est::zip(l1, est::make_range(l2.begin(), std::prev(l2.end())))) {
        static_assert(std::is_same<decltype(std::get<0>(item)), int&>::value, "");
        static_assert(std::is_same<decltype(std::get<1>(item)), double&>::value, "");
        s1 += std::get<0>(item), s2 += std::get<1>(item), ++c;
    }
    VERIFY(c == 3 && s1 == 17 && s2 == 14.);

    c = 0, s1 = 0, s2 = 0;
    for (const auto& item : est::zip(est::make_range(l2.begin(), std::prev(l2.end())), l1)) {
        static_assert(std::is_same<decltype(std::get<1>(item)), int&>::value, "");
        static_assert(std::is_same<decltype(std::get<0>(item)), double&>::value, "");
        s2 += std::get<0>(item), s1 += std::get<1>(item), ++c;
    }
    VERIFY(c == 3 && s1 == 17 && s2 == 14.);

    c = 0, s1 = 0, s2 = 0;
    for (const auto& item : est::make_reverse_range(est::zip(l1, est::make_range(std::next(l2.begin()), l2.end())))) {
        static_assert(std::is_same<decltype(std::get<0>(item)), int&>::value, "");
        static_assert(std::is_same<decltype(std::get<1>(item)), double&>::value, "");
        s1 += std::get<0>(item), s2 += std::get<1>(item), ++c;
    }
    VERIFY(c == 3 && s1 == 17 && s2 == 25.5);

    auto r = est::zip(l1, l2);
    VERIFY(std::distance(r.begin(), r.end()) == 3);
    return 0;
}

int test_iterator_2() {
    std::vector<int> v1{1, 5, 11};
    std::vector<double> v2{1.5, 5.5, 7., 13.};
    auto r = est::zip(v1, v2);
    VERIFY(std::distance(r.begin(), r.end()) == 3);
    VERIFY(*(est::make_zip_iterator(v1.begin(), v2.begin()) + 2) == std::make_tuple(11, 7.));
    return 0;
}

std::vector<std::string> g_vec;

void g_func(const std::string& s) { g_vec.emplace_back(s); }

int test_output_iterator_0() {
    struct OutoutIt : est::output_iterator_facade<OutoutIt> {
        explicit OutoutIt(void (*fn)(const std::string&)) : fn(fn) {}
        void (*fn)(const std::string&);
        void operator()(const std::string& s) const { fn(s); }
    };

    std::vector<std::string> strs{"aaa", "bbb", "ccc"};
    g_vec.clear();
    std::copy(strs.begin(), strs.end(), OutoutIt(&g_func));
    VERIFY(g_vec[0] == "aaa");
    VERIFY(g_vec[1] == "bbb");
    VERIFY(g_vec[2] == "ccc");
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "iterator", test_iterator_0);
ADD_TEST_CASE("", "iterator", test_iterator_1);
ADD_TEST_CASE("", "iterator", test_iterator_2);
ADD_TEST_CASE("", "iterator", test_output_iterator_0);
