#include "test_suite.h"

#include "uxs/list.h"
#include "uxs/vector.h"
#include "uxs/zip_iterator.h"

#include <forward_list>

namespace {

int test_iterator_0() {
    std::forward_list<int> fl1;
    std::forward_list<double> fl2;
    auto fl_zip = uxs::make_zip_iterator(fl1.begin(), fl2.begin());
    static_assert(std::is_same<typename std::iterator_traits<decltype(fl_zip)>::iterator_category,
                               std::forward_iterator_tag>::value,
                  "");

    uxs::list<int> l1;
    uxs::list<double> l2;
    auto l_zip = uxs::make_zip_iterator(l1.begin(), l2.begin());
    static_assert(std::is_same<typename std::iterator_traits<decltype(l_zip)>::iterator_category,
                               std::bidirectional_iterator_tag>::value,
                  "");

    uxs::vector<int> v1;
    uxs::vector<double> v2;
    auto v_zip = uxs::make_zip_iterator(v1.begin(), v2.begin());
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
    uxs::list<int> l1{1, 5, 11};
    uxs::list<double> l2{1.5, 5.5, 7., 13.};

    size_t c = 0;
    int s1 = 0;
    double s2 = 0;
    for (const auto& item : uxs::zip(l2)) {
        static_assert(std::is_same<decltype(std::get<0>(item)), double&>::value, "");
        s2 += std::get<0>(item), ++c;
    }
    VERIFY(c == 4 && s2 == 27.);

    c = 0, s1 = 0, s2 = 0;
    for (const auto& item : uxs::zip(l1, l2)) {
        static_assert(std::is_same<decltype(std::get<0>(item)), int&>::value, "");
        static_assert(std::is_same<decltype(std::get<1>(item)), double&>::value, "");
        s1 += std::get<0>(item), s2 += std::get<1>(item), ++c;
    }
    VERIFY(c == 3 && s1 == 17 && s2 == 14.);

    c = 0, s1 = 0, s2 = 0;
    for (const auto& item : uxs::zip(l2, l1)) {
        static_assert(std::is_same<decltype(std::get<1>(item)), int&>::value, "");
        static_assert(std::is_same<decltype(std::get<0>(item)), double&>::value, "");
        s2 += std::get<0>(item), s1 += std::get<1>(item), ++c;
    }
    VERIFY(c == 3 && s1 == 17 && s2 == 14.);

    c = 0, s1 = 0, s2 = 0;
    for (const auto& item : uxs::make_reverse_range(uxs::zip(l1, l2))) {
        static_assert(std::is_same<decltype(std::get<0>(item)), int&>::value, "");
        static_assert(std::is_same<decltype(std::get<1>(item)), double&>::value, "");
        s1 += std::get<0>(item), s2 += std::get<1>(item), ++c;
    }
    VERIFY(c == 3 && s1 == 17 && s2 == 25.5);

    auto r = uxs::zip(l1, l2);
    VERIFY(std::distance(r.begin(), r.end()) == 3);
    return 0;
}

int test_iterator_2() {
    uxs::vector<int> v1{1, 5, 11};
    uxs::vector<double> v2{1.5, 5.5, 7., 13.};
    VERIFY(*(uxs::make_zip_iterator(v1.begin(), v2.begin()) + 2) == std::make_tuple(11, 7.));
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "iterator", test_iterator_0);
ADD_TEST_CASE("", "iterator", test_iterator_1);
ADD_TEST_CASE("", "iterator", test_iterator_2);
