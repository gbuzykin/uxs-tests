#include "test_allocators.h"
#include "test_suite.h"
#include "test_types.h"

#include "uxs/map.h"
#include "uxs/multimap.h"
#include "uxs/multiset.h"
#include "uxs/pool_allocator.h"
#include "uxs/set.h"

#include <set>
#include <vector>

using namespace uxs_test_suite;

bool check_balance(uxs::rbtree_node_t* node, int& black, int cur_black = 0,
                   uxs::rbtree_node_t::color_t color = uxs::rbtree_node_t::color_t::kRed) {
    if ((color != uxs::rbtree_node_t::color_t::kBlack) && (node->color != uxs::rbtree_node_t::color_t::kBlack)) {
        return false;
    }
    if (node->color == uxs::rbtree_node_t::color_t::kBlack) { ++cur_black; }

    if (node->left) {
        if (!check_balance(node->left, black, cur_black, node->color)) { return false; }
    } else if (black > 0) {
        if (cur_black != black) { return false; }
    } else {
        black = cur_black;
    }

    if (node->right) {
        if (!check_balance(node->right, black, cur_black, node->color)) { return false; }
    } else if (black > 0) {
        if (cur_black != black) { return false; }
    } else {
        black = cur_black;
    }

    return true;
}

template<typename NodeTy, typename Alloc, typename Comp, typename InputIt>
bool check_rbtree(const uxs::detail::rbtree_base<NodeTy, Alloc, Comp>& t, size_t sz, InputIt src) {
    int black = 0;
    if (t.size() != sz) { return false; }
    if (t.begin() != t.cbegin()) { return false; }
    if (t.end() != t.cend()) { return false; }
    if (!t.empty() && !check_balance(t.end().node()->left, black)) { return false; }
    if (std::distance(t.begin(), t.end()) != static_cast<ptrdiff_t>(sz)) { return false; }
    for (auto it = t.begin(); it != t.end(); ++it) {
        if (!(*it == *src++)) { return false; }
    }
    for (auto it = t.rbegin(); it != t.rend(); ++it) {
        if (!(*it == *(--src))) { return false; }
    }
    return true;
}

#define CHECK(...) \
    if (!check_rbtree(__VA_ARGS__)) { throw std::runtime_error(report_error(__FILE__, __LINE__, "set mismatched")); }

#define CHECK_EMPTY(...) \
    if (((__VA_ARGS__).size() != 0) || ((__VA_ARGS__).begin() != (__VA_ARGS__).end())) { \
        throw std::runtime_error(report_error(__FILE__, __LINE__, "set is not empty")); \
    }

namespace uxs {
template class set<T>;
template class set<T_ThrowingMove>;
template class map<T, std::string>;
template class map<T_ThrowingMove, std::string>;
template class multiset<T>;
template class multiset<T_ThrowingMove>;
template class multimap<T, std::string>;
template class multimap<T_ThrowingMove, std::string>;
}  // namespace uxs

namespace {

int test_0() {  // empty set
    VERIFY(uxs::is_random_access_iterator<std::multiset<T>::iterator>::value == false);
    VERIFY(uxs::is_random_access_iterator<uxs::multiset<T>::iterator>::value == false);

    test_allocator<void> al;

    uxs::multiset<T, std::less<T>, test_allocator<T>> s;
    CHECK_EMPTY(s);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s1(al);
    CHECK_EMPTY(s1);
    VERIFY(s1.get_allocator() == al);
    return 0;
}

int test_3() {  // set initialized with iterator range
    test_allocator<void> al;

    T tst0[] = {1};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s0(tst0, tst0, al);
    CHECK_EMPTY(s0);
    VERIFY(s0.get_allocator() == al);

    T tst[] = {1, 2, 3, 4, 5};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s(tst, tst + 5, al);
    CHECK(s, 5, tst);
    VERIFY(s.get_allocator() == al);
    return 0;
}

int test_4() {  // set with initializer
    test_allocator<void> al;

    std::initializer_list<T> tst0;
    uxs::multiset<T, std::less<T>, test_allocator<T>> s0(tst0, al);
    CHECK_EMPTY(s0);
    VERIFY(s0.get_allocator() == al);

    std::initializer_list<T> tst = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s(tst, al);
    CHECK(s, tst.size(), tst.begin());
    VERIFY(s.get_allocator() == al);
    return 0;
}

int test_5() {  // initializer assignment
    uxs::multiset<T, std::less<T>, test_allocator<T>> s;

    std::initializer_list<T> tst0;
    s = tst0;  // from empty to empty
    CHECK_EMPTY(s);

    std::initializer_list<T> tst1 = {1, 2, 3, 4, 5};
    s = tst1;  // from empty
    CHECK(s, tst1.size(), tst1.begin());

    std::initializer_list<T> tst3 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    s = tst3;  // from non-empty
    CHECK(s, tst3.size(), tst3.begin());

    std::initializer_list<T> tst4 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    s = tst4;  // to less size
    CHECK(s, tst4.size(), tst4.begin());

    std::initializer_list<T> tst5 = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    s = tst5;  // to the same size
    CHECK(s, tst5.size(), tst5.begin());

    std::initializer_list<T> tst6;
    s = tst6;  // to empty
    CHECK_EMPTY(s);
    return 0;
}

// --------------------------------------------

int test_6() {  // copy constructor
    test_allocator<void> al1, al2;

    std::initializer_list<T> tst = {1, 2, 3, 4, 5};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s0(al1), s(tst, al1);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s01(s0);
    CHECK_EMPTY(s01);
    VERIFY(s01.get_allocator() == al1);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s02(s0, al2);
    CHECK_EMPTY(s02);
    VERIFY(s02.get_allocator() == al2);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s1(s);
    CHECK(s1, tst.size(), tst.begin());
    VERIFY(s1.get_allocator() == al1);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s2(s, al2);
    CHECK(s2, tst.size(), tst.begin());
    VERIFY(s2.get_allocator() == al2);
    return 0;
}

int test_7() {  // move constructor
    test_allocator<void> al1, al2;

    std::initializer_list<T> tst = {1, 2, 3, 4, 5};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s0(al1), s(tst, al1);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s01(std::move(s0));
    CHECK_EMPTY(s01);
    VERIFY(s01.get_allocator() == al1);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s02(std::move(s01), al2);
    CHECK_EMPTY(s02);
    VERIFY(s02.get_allocator() == al2);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s03(std::move(s02), al2);
    CHECK_EMPTY(s03);
    VERIFY(s03.get_allocator() == al2);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s1(std::move(s));
    CHECK(s1, tst.size(), tst.begin());
    VERIFY(s1.get_allocator() == al1);
    CHECK_EMPTY(s);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s2(std::move(s1), al2);
    CHECK(s2, tst.size(), tst.begin());
    VERIFY(s2.get_allocator() == al2);
    VERIFY(s1.size() == s2.size());  // different allocators -> per-element movement

    uxs::multiset<T, std::less<T>, test_allocator<T>> s3(std::move(s2), al2);
    CHECK(s3, tst.size(), tst.begin());
    VERIFY(s3.get_allocator() == al2);
    CHECK_EMPTY(s2);
    return 0;
}

// --------------------------------------------

int test_8() {  // copy assignment, same allocator
    test_allocator<void> al;
    uxs::multiset<T, std::less<T>, test_allocator<T>> s(al);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s0(al);
    s = s0;  // from empty to empty
    CHECK_EMPTY(s);
    VERIFY(s.get_allocator() == al);

    std::initializer_list<T> tst1 = {1, 2, 3, 4, 5};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s1(tst1, al);
    s = s1;  // from empty
    CHECK(s, tst1.size(), tst1.begin());
    VERIFY(s.get_allocator() == al);

    std::initializer_list<T> tst3 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s3(tst3, al);
    s = s3;  // from non-empty
    CHECK(s, tst3.size(), tst3.begin());
    VERIFY(s.get_allocator() == al);

    std::initializer_list<T> tst4 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s4(tst4, al);
    s = s4;  // to less size
    CHECK(s, tst4.size(), tst4.begin());
    VERIFY(s.get_allocator() == al);

    std::initializer_list<T> tst5 = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s5(tst5, al);
    s = s5;  // to the same size
    CHECK(s, tst5.size(), tst5.begin());
    VERIFY(s.get_allocator() == al);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s6(al);
    s = s6;  // to empty
    CHECK_EMPTY(s);
    VERIFY(s.get_allocator() == al);
    return 0;
}

int test_9() {  // copy assignment, different allocators, friendly allocator
    test_allocator<void> al1, al2;
    uxs::multiset<T, std::less<T>, test_allocator<T>> s(al1);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s0(al2);
    s = s0;  // from empty to empty
    CHECK_EMPTY(s);
    VERIFY(s.get_allocator() == al1);

    std::initializer_list<T> tst1 = {1, 2, 3, 4, 5};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s1(tst1, al2);
    s = s1;  // from empty
    CHECK(s, tst1.size(), tst1.begin());
    VERIFY(s.get_allocator() == al1);

    std::initializer_list<T> tst3 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s3(tst3, al2);
    s = s3;  // from non-empty
    CHECK(s, tst3.size(), tst3.begin());
    VERIFY(s.get_allocator() == al1);

    std::initializer_list<T> tst4 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s4(tst4, al2);
    s = s4;  // to less size
    CHECK(s, tst4.size(), tst4.begin());
    VERIFY(s.get_allocator() == al1);

    std::initializer_list<T> tst5 = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s5(tst5, al2);
    s = s5;  // to the same size
    CHECK(s, tst5.size(), tst5.begin());
    VERIFY(s.get_allocator() == al1);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s6(al2);
    s = s6;  // to empty
    CHECK_EMPTY(s);
    VERIFY(s.get_allocator() == al1);
    return 0;
}

int test_10() {  // copy assignment, different allocators, unfriendly allocator
    unfriendly_test_allocator<void> al1, al2;
    uxs::multiset<T, std::less<T>, unfriendly_test_allocator<T>> s(al1);

    uxs::multiset<T, std::less<T>, unfriendly_test_allocator<T>> s0(al2);
    s = s0;  // from empty to empty
    CHECK_EMPTY(s);
    VERIFY(s.get_allocator() == al2);

    std::initializer_list<T> tst1 = {1, 2, 3, 4, 5};
    uxs::multiset<T, std::less<T>, unfriendly_test_allocator<T>> s1(tst1, al1);
    s = s1;  // from empty
    CHECK(s, tst1.size(), tst1.begin());
    VERIFY(s.get_allocator() == al1);

    std::initializer_list<T> tst2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    uxs::multiset<T, std::less<T>, unfriendly_test_allocator<T>> s2(tst2, al2);
    s = s2;  // from non-empty
    CHECK(s, tst2.size(), tst2.begin());
    VERIFY(s.get_allocator() == al2);

    uxs::multiset<T, std::less<T>, unfriendly_test_allocator<T>> s3(al1);
    s = s3;  // to empty
    CHECK_EMPTY(s);
    VERIFY(s.get_allocator() == al1);
    return 0;
}

// --------------------------------------------

int test_11() {  // move assignment, same allocator
    test_allocator<void> al;
    uxs::multiset<T, std::less<T>, test_allocator<T>> s(al);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s0(al);
    s = std::move(s0);  // from empty to empty
    CHECK_EMPTY(s);
    VERIFY(s.get_allocator() == al);

    std::initializer_list<T> tst1 = {1, 2, 3, 4, 5};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s1(tst1, al);
    s = std::move(s1);  // from empty
    CHECK(s, tst1.size(), tst1.begin());
    VERIFY(s.get_allocator() == al);
    CHECK_EMPTY(s1);

    std::initializer_list<T> tst2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s2(tst2, al);
    s = std::move(s2);  // from non-empty
    CHECK(s, tst2.size(), tst2.begin());
    VERIFY(s.get_allocator() == al);
    CHECK_EMPTY(s2);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s3(al);
    s = std::move(s3);  // to empty
    CHECK_EMPTY(s);
    VERIFY(s.get_allocator() == al);
    return 0;
}

int test_12() {  // move assignment, different allocators, friendly allocator
    test_allocator<void> al1, al2;
    uxs::multiset<T, std::less<T>, test_allocator<T>> s(al1);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s0(al2);
    s = std::move(s0);  // from empty to empty
    CHECK_EMPTY(s);
    VERIFY(s.get_allocator() == al2);

    std::initializer_list<T> tst1 = {1, 2, 3, 4, 5};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s1(tst1, al1);
    s = std::move(s1);  // from empty
    CHECK(s, tst1.size(), tst1.begin());
    VERIFY(s.get_allocator() == al1);
    CHECK_EMPTY(s1);

    std::initializer_list<T> tst2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    uxs::multiset<T, std::less<T>, test_allocator<T>> s2(tst2, al2);
    s = std::move(s2);  // from non-empty
    CHECK(s, tst2.size(), tst2.begin());
    VERIFY(s.get_allocator() == al2);
    CHECK_EMPTY(s2);

    uxs::multiset<T, std::less<T>, test_allocator<T>> s3(al1);
    s = std::move(s3);  // to empty
    CHECK_EMPTY(s);
    VERIFY(s.get_allocator() == al1);
    return 0;
}

int test_13() {  // move assignment, different allocators, unfriendly allocator
    unfriendly_test_allocator<void> al1, al2;
    uxs::multiset<T, std::less<T>, unfriendly_test_allocator<T>> s(al1);

    uxs::multiset<T, std::less<T>, unfriendly_test_allocator<T>> s0(al2);
    s = std::move(s0);  // from empty to empty
    CHECK_EMPTY(s);
    VERIFY(s.get_allocator() == al1);
    VERIFY(s0.size() == s.size());

    std::initializer_list<T> tst1 = {1, 2, 3, 4, 5};
    uxs::multiset<T, std::less<T>, unfriendly_test_allocator<T>> s1(tst1, al2);
    s = std::move(s1);  // from empty
    CHECK(s, tst1.size(), tst1.begin());
    VERIFY(s.get_allocator() == al1);
    VERIFY(s1.size() == s.size());

    std::initializer_list<T> tst3 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    uxs::multiset<T, std::less<T>, unfriendly_test_allocator<T>> s3(tst3, al2);
    s = std::move(s3);  // from non-empty
    CHECK(s, tst3.size(), tst3.begin());
    VERIFY(s.get_allocator() == al1);
    VERIFY(s3.size() == s.size());

    std::initializer_list<T> tst4 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uxs::multiset<T, std::less<T>, unfriendly_test_allocator<T>> s4(tst4, al2);
    s = std::move(s4);  // to less size
    CHECK(s, tst4.size(), tst4.begin());
    VERIFY(s.get_allocator() == al1);
    VERIFY(s4.size() == s.size());

    std::initializer_list<T> tst5 = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    uxs::multiset<T, std::less<T>, unfriendly_test_allocator<T>> s5(tst5, al2);
    s = std::move(s5);  // to the same size
    CHECK(s, tst5.size(), tst5.begin());
    VERIFY(s.get_allocator() == al1);
    VERIFY(s5.size() == s.size());

    uxs::multiset<T, std::less<T>, unfriendly_test_allocator<T>> s6(al2);
    s = std::move(s6);  // to empty
    CHECK_EMPTY(s);
    VERIFY(s.get_allocator() == al1);
    VERIFY(s6.size() == s.size());
    return 0;
}

// --------------------------------------------

int test_18() {  // swap
    test_allocator<void> al1, al2;
    uxs::multiset<T, std::less<T>, test_allocator<T>> s1(al1), s2(al2);

    s1.swap(s2);
    CHECK_EMPTY(s1);
    CHECK_EMPTY(s2);

    std::initializer_list<T> tst1 = {1, 2, 3, 4, 5};
    s1 = tst1;
    s1.swap(s2);
    CHECK_EMPTY(s1);
    CHECK(s2, tst1.size(), tst1.begin());

    s1.swap(s2);
    CHECK(s1, tst1.size(), tst1.begin());
    CHECK_EMPTY(s2);

    std::initializer_list<T> tst2 = {10, 11, 12, 13, 14, 15, 16, 17};
    s2 = tst2;
    s1.swap(s2);
    CHECK(s1, tst2.size(), tst2.begin());
    CHECK(s2, tst1.size(), tst1.begin());
    return 0;
}

// --------------------------------------------

int test_19() {
    uxs::map<int, int> m;
    auto comp = m.value_comp();
    auto b = comp(std::make_pair(4, 2), std::make_pair(2, 3));
    (void)b;
    return 0;
}

int test_20() {
    test_allocator<void> al;
    uxs::multiset<T, std::less<T>, test_allocator<T>> s1(al), s2(al);

    std::initializer_list<T> tst1 = {1, 2, 3, 4, 5};
    s1 = tst1;
    auto nh = s1.extract(3);
    s2.insert(std::move(nh));

    std::initializer_list<T> tst2 = {1, 2, 4, 5};
    CHECK(s1, tst2.size(), tst2.begin());

    std::initializer_list<T> tst3 = {3};
    CHECK(s2, tst3.size(), tst3.begin());

    decltype(nh) nh2;
    decltype(nh) nh3{s1.extract(4)};

    uxs::set<T, std::less<T>, test_allocator<T>> s3(al);
    nh = s1.extract(5);
    auto result = s3.insert(std::move(nh));
    VERIFY(result.inserted);

    s1.emplace(5);
    nh = s1.extract(5);
    result = s3.insert(std::move(nh));
    VERIFY(!result.inserted);

    using value_type = std::decay_t<decltype(nh)>::value_type;
    value_type v = std::move(nh3.value());
    VERIFY(v == 4);

    uxs::map<T, std::string, std::less<T>, test_allocator<std::pair<const T, std::string>>> m(al);
    m.emplace(1, "aaa");
    auto nh4 = m.extract(1);
    using key_type = std::decay_t<decltype(nh4)>::key_type;
    using mapped_type = std::decay_t<decltype(nh4)>::mapped_type;
    key_type k = std::move(nh4.key());
    mapped_type obj = std::move(nh4.mapped());
    VERIFY(k == 1);
    VERIFY(obj == "aaa");
    return 0;
}

int test_21() {
    uxs::map<std::string, std::string> m;
    m.insert_or_assign("a", "apple");
    m.insert_or_assign("b", "bannana");
    m.insert_or_assign("c", "cherry");
    m.insert_or_assign("c", "clementine");

    VERIFY(m["a"] == "apple");
    VERIFY(m["b"] == "bannana");
    VERIFY(m["c"] == "clementine");
    return 0;
}

int test_22() {
    uxs::map<std::string, std::string> m;

    m.try_emplace("a", "a");
    m.try_emplace("b", "abcd");
    m.try_emplace("c", 10, 'c');
    m.try_emplace("c", "Won't be inserted");

    VERIFY(m["a"] == "a");
    VERIFY(m["b"] == "abcd");
    VERIFY(m["c"] == std::string(10, 'c'));
    return 0;
}

int test_23() {
    test_allocator<void> al;
    uxs::map<int, std::string, std::less<int>, test_allocator<std::pair<const int, std::string>>> u(al);
    using init_type = std::initializer_list<std::decay_t<decltype(u)>::value_type>;
    uxs::map<int, std::string, std::less<int>, test_allocator<std::pair<const int, std::string>>> ma(
        init_type{{1, "apple"}, {5, "pear"}, {10, "banana"}}, al);
    uxs::map<int, std::string, std::less<int>, test_allocator<std::pair<const int, std::string>>> mb(
        init_type{{2, "zorro"}, {4, "batman"}, {5, "X"}, {8, "alpaca"}}, al);
    u.merge(ma);
    u.merge(mb);
    CHECK_EMPTY(ma);
    VERIFY(mb[5] == "X");

    VERIFY(u[1] == "apple");
    VERIFY(u[2] == "zorro");
    VERIFY(u[4] == "batman");
    VERIFY(u[5] == "pear");
    VERIFY(u[8] == "alpaca");
    VERIFY(u[10] == "banana");

    u.insert(std::make_pair(11, "dddd"));
    VERIFY(u[11] == "dddd");
    return 0;
}

// --------------------------------------------

template<typename SetType>
void rbtree_test(int iter_count, bool log = false) {
    using value_type = typename SetType::value_type;
    using key_compare = typename SetType::key_compare;
    using allocator_type = typename SetType::allocator_type;

    using RefSetType =
        typename std::conditional<std::is_same<SetType, uxs::set<value_type, key_compare, allocator_type>>::value,
                                  std::set<value_type>, std::multiset<value_type>>::type;

    srand(0);

    for (int iter = 0, perc0 = -1; iter < iter_count; ++iter) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
            perc0 = perc;
        }

        SetType s, s1, s2, s3;
        RefSetType s_ref;

        for (size_t i = 0; i < 100; ++i) { s1.emplace(rand() % 500); }
        for (size_t i = 0; i < 1100; ++i) { s2.emplace(rand() % 500); }
        for (size_t i = 0; i < 1000; ++i) {
            int val = rand() % 500;
            s3.emplace(val);
            s_ref.emplace(val);
            CHECK(s3, s_ref.size(), s_ref.begin());
        }

        s = s1;
        s = s2;
        s = s3;
        CHECK(s, s_ref.size(), s_ref.begin());

        s = decltype(s)();
        CHECK_EMPTY(s);

        s = s3;

        for (size_t i = 0; i < 250; ++i) {
            int val = rand() % 500;

            auto it = s.find(val);
            auto lower = s.lower_bound(val);
            auto upper = s.upper_bound(val);
            auto range = s.equal_range(val);

            VERIFY(lower == range.first);
            VERIFY(upper == range.second);

            auto it_ref = s_ref.find(val);
            auto lower_ref = s_ref.lower_bound(val);
            auto upper_ref = s_ref.upper_bound(val);

            VERIFY(s.count(val) == s_ref.count(val));

            VERIFY(std::distance(s.begin(), it) == std::distance(s_ref.begin(), it_ref));
            VERIFY(std::distance(s.begin(), lower) == std::distance(s_ref.begin(), lower_ref));
            VERIFY(std::distance(s.begin(), upper) == std::distance(s_ref.begin(), upper_ref));

            if ((lower != s.end()) && (lower == upper)) {
                it = s.erase(lower);
            } else {
                it = s.erase(lower, upper);
            }

            if ((lower_ref != s_ref.end()) && (lower_ref == upper_ref)) {
                it_ref = s_ref.erase(lower_ref);
            } else {
                it_ref = s_ref.erase(lower_ref, upper_ref);
            }

            VERIFY(std::distance(s.begin(), it) == std::distance(s_ref.begin(), it_ref));
            CHECK(s, s_ref.size(), s_ref.begin());
        }

        VERIFY(s.empty() == s_ref.empty());

        for (size_t i = 0; i < 500; ++i) {
            int val = rand() % 500;
            VERIFY(s.erase(val) == s_ref.erase(val));
            CHECK(s, s_ref.size(), s_ref.begin());
        }

        VERIFY(s.empty() == s_ref.empty());

        while (!s_ref.empty()) {
            int val = rand() % 500;
            auto it = s.lower_bound(val);
            if (it != s.end()) { it = s.erase(it); }

            auto it_ref = s_ref.lower_bound(val);
            if (it_ref != s_ref.end()) { it_ref = s_ref.erase(it_ref); }

            VERIFY(std::distance(s.begin(), it) == std::distance(s_ref.begin(), it_ref));
            CHECK(s, s_ref.size(), s_ref.begin());
        }
    }
}

#if defined(NDEBUG)
static const int brute_N = 50000;
#else   // defined(NDEBUG)
static const int brute_N = 500;
#endif  // defined(NDEBUG)

int test_bruteforce_unique() {
    rbtree_test<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(brute_N);
    return 0;
}

int test_bruteforce_multi() {
    rbtree_test<uxs::multiset<T, std::less<T>, uxs::global_pool_allocator<T>>>(brute_N);
    return 0;
}

// --------------------------------------------

template<typename SetType>
int perf_insert(int iter_count) {
    srand(0);

    std::vector<int> v;
    v.reserve(1000);
    for (size_t i = 0; i < 1000; ++i) { v.push_back(rand() % 500); }

    auto start = std::clock();
    SetType s;
    for (int iter = 0; iter < iter_count; ++iter) {
        s.clear();
        for (size_t i = 0; i < 1000; ++i) { s.emplace(v[i]); }
    }
    return static_cast<int>(std::clock() - start);
}

template<typename SetType>
int perf_forward(int iter_count) {
    srand(0);

    SetType s;
    for (size_t i = 0; i < 1000; ++i) { s.emplace(rand()); }

    auto start = std::clock();
    int result = 0;
    for (int iter = 0; iter < iter_count; ++iter) {
        for (auto it = s.begin(); it != s.end(); ++it) { result += static_cast<int>(*it); }
    }
    return result ? static_cast<int>(std::clock() - start) : 0;
}

template<typename SetType>
int perf_backward(int iter_count) {
    srand(0);

    SetType s;
    for (size_t i = 0; i < 1000; ++i) { s.emplace(rand()); }

    auto start = std::clock();
    int result = 0;
    for (int iter = 0; iter < iter_count; ++iter) {
        for (auto it = s.rbegin(); it != s.rend(); ++it) { result += static_cast<int>(*it); }
    }
    return result ? static_cast<int>(std::clock() - start) : 0;
}

template<typename SetType>
int perf_find(int iter_count) {
    srand(0);

    SetType s;
    for (size_t i = 0; i < 1000; ++i) { s.emplace(rand()); }

    std::vector<int> v;
    v.reserve(iter_count);
    for (int iter = 0; iter < iter_count; ++iter) { v.push_back(rand()); }

    auto start = std::clock();
    int result = 0;
    for (int iter = 0; iter < iter_count; ++iter) {
        int val = v[iter];

        auto lower = s.lower_bound(val);
        auto upper = s.upper_bound(val);
        auto range = s.equal_range(val);

        result += lower != s.end() ? static_cast<int>(*lower) : 0;
        result += upper != s.end() ? static_cast<int>(*upper) : 0;
        result += range.first != s.end() ? static_cast<int>(*range.first) : 0;
        result += range.second != s.end() ? static_cast<int>(*range.second) : 0;
    }

    return result ? static_cast<int>(std::clock() - start) : 0;
}

template<typename SetType>
int perf_integral(int iter_count) {
    srand(0);

    std::vector<int> v1, v2;
    v1.reserve(1000);
    v2.reserve(1000);
    for (size_t i = 0; i < 1000; ++i) {
        v1.push_back(rand() % 500);
        v2.push_back(rand() % 500);
    }

    auto start = std::clock();

    SetType s;
    int result = 0;
    for (int iter = 0; iter < iter_count; ++iter) {
        s.clear();
        for (size_t i = 0; i < 1000; ++i) { s.emplace(v1[i]); }

        for (size_t i = 0; i < 30; ++i) {
            for (auto it = s.begin(); it != s.end(); ++it) { result += static_cast<int>(*it); }
            for (auto it = s.rbegin(); it != s.rend(); ++it) { result += static_cast<int>(*it); }
        }

        for (size_t i = 0; i < 1000; ++i) {
            int val = v2[i];

            auto lower = s.lower_bound(val);
            auto upper = s.upper_bound(val);
            auto range = s.equal_range(val);

            result += lower != s.end() ? static_cast<int>(*lower) : 0;
            result += upper != s.end() ? static_cast<int>(*upper) : 0;
            result += range.first != s.end() ? static_cast<int>(*range.first) : 0;
            result += range.second != s.end() ? static_cast<int>(*range.second) : 0;

            s.erase(lower, upper);
        }
    }

    return result ? static_cast<int>(std::clock() - start) : 0;
}

const int perf_N = 1500;

int test_perf_insert_T_std_alloc() { return perf_insert<uxs::set<T>>(3 * perf_N); }
int test_perf_insert_T_global_pool() {
    return perf_insert<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(3 * perf_N);
}
int test_perf_insert_T_pool() { return perf_insert<uxs::set<T, std::less<T>, uxs::pool_allocator<T>>>(3 * perf_N); }
int test_perf_insert_int_std_alloc() { return perf_insert<uxs::set<int>>(3 * perf_N); }
int test_perf_insert_int_global_pool() {
    return perf_insert<uxs::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(3 * perf_N);
}
int test_perf_insert_int_pool() {
    return perf_insert<uxs::set<int, std::less<int>, uxs::pool_allocator<int>>>(3 * perf_N);
}

int test_perf_insert_T_std_alloc_std() { return perf_insert<std::set<T>>(3 * perf_N); }
int test_perf_insert_T_global_pool_std() {
    return perf_insert<std::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(3 * perf_N);
}
int test_perf_insert_T_pool_std() { return perf_insert<std::set<T, std::less<T>, uxs::pool_allocator<T>>>(3 * perf_N); }
int test_perf_insert_int_std_alloc_std() { return perf_insert<std::set<int>>(3 * perf_N); }
int test_perf_insert_int_global_pool_std() {
    return perf_insert<std::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(3 * perf_N);
}
int test_perf_insert_int_pool_std() {
    return perf_insert<std::set<int, std::less<int>, uxs::pool_allocator<int>>>(3 * perf_N);
}

int test_perf_forward_T_std_alloc() { return perf_forward<uxs::set<T>>(100 * perf_N); }
int test_perf_forward_T_global_pool() {
    return perf_forward<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(100 * perf_N);
}
int test_perf_forward_T_pool() { return perf_forward<uxs::set<T, std::less<T>, uxs::pool_allocator<T>>>(100 * perf_N); }
int test_perf_forward_int_std_alloc() { return perf_forward<uxs::set<int>>(100 * perf_N); }
int test_perf_forward_int_global_pool() {
    return perf_forward<uxs::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(100 * perf_N);
}
int test_perf_forward_int_pool() {
    return perf_forward<uxs::set<int, std::less<int>, uxs::pool_allocator<int>>>(100 * perf_N);
}

int test_perf_forward_T_std_alloc_std() { return perf_forward<std::set<T>>(100 * perf_N); }
int test_perf_forward_T_global_pool_std() {
    return perf_forward<std::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(100 * perf_N);
}
int test_perf_forward_T_pool_std() {
    return perf_forward<std::set<T, std::less<T>, uxs::pool_allocator<T>>>(100 * perf_N);
}
int test_perf_forward_int_std_alloc_std() { return perf_forward<std::set<int>>(100 * perf_N); }
int test_perf_forward_int_global_pool_std() {
    return perf_forward<std::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(100 * perf_N);
}
int test_perf_forward_int_pool_std() {
    return perf_forward<std::set<int, std::less<int>, uxs::pool_allocator<int>>>(100 * perf_N);
}

int test_perf_backward_T_std_alloc() { return perf_backward<uxs::set<T>>(100 * perf_N); }
int test_perf_backward_T_global_pool() {
    return perf_backward<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(100 * perf_N);
}
int test_perf_backward_T_pool() {
    return perf_backward<uxs::set<T, std::less<T>, uxs::pool_allocator<T>>>(100 * perf_N);
}
int test_perf_backward_int_std_alloc() { return perf_backward<uxs::set<int>>(100 * perf_N); }
int test_perf_backward_int_global_pool() {
    return perf_backward<uxs::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(100 * perf_N);
}
int test_perf_backward_int_pool() {
    return perf_backward<uxs::set<int, std::less<int>, uxs::pool_allocator<int>>>(100 * perf_N);
}

int test_perf_backward_T_std_alloc_std() { return perf_backward<std::set<T>>(100 * perf_N); }
int test_perf_backward_T_global_pool_std() {
    return perf_backward<std::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(100 * perf_N);
}
int test_perf_backward_T_pool_std() {
    return perf_backward<std::set<T, std::less<T>, uxs::pool_allocator<T>>>(100 * perf_N);
}
int test_perf_backward_int_std_alloc_std() { return perf_backward<std::set<int>>(100 * perf_N); }
int test_perf_backward_int_global_pool_std() {
    return perf_backward<std::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(100 * perf_N);
}
int test_perf_backward_int_pool_std() {
    return perf_backward<std::set<int, std::less<int>, uxs::pool_allocator<int>>>(100 * perf_N);
}

int test_perf_find_T_std_alloc() { return perf_find<uxs::set<T>>(1000 * perf_N); }
int test_perf_find_T_global_pool() {
    return perf_find<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(1000 * perf_N);
}
int test_perf_find_T_pool() { return perf_find<uxs::set<T, std::less<T>, uxs::pool_allocator<T>>>(1000 * perf_N); }
int test_perf_find_int_std_alloc() { return perf_find<uxs::set<int>>(1000 * perf_N); }
int test_perf_find_int_global_pool() {
    return perf_find<uxs::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(1000 * perf_N);
}
int test_perf_find_int_pool() {
    return perf_find<uxs::set<int, std::less<int>, uxs::pool_allocator<int>>>(1000 * perf_N);
}

int test_perf_find_T_std_alloc_std() { return perf_find<std::set<T>>(1000 * perf_N); }
int test_perf_find_T_global_pool_std() {
    return perf_find<std::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(1000 * perf_N);
}
int test_perf_find_T_pool_std() { return perf_find<std::set<T, std::less<T>, uxs::pool_allocator<T>>>(1000 * perf_N); }
int test_perf_find_int_std_alloc_std() { return perf_find<std::set<int>>(1000 * perf_N); }
int test_perf_find_int_global_pool_std() {
    return perf_find<std::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(1000 * perf_N);
}
int test_perf_find_int_pool_std() {
    return perf_find<std::set<int, std::less<int>, uxs::pool_allocator<int>>>(1000 * perf_N);
}

int test_perf_T_std_alloc() { return perf_integral<uxs::set<T>>(perf_N); }
int test_perf_T_global_pool() {
    return perf_integral<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N);
}
int test_perf_T_pool() { return perf_integral<uxs::set<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N); }
int test_perf_int_std_alloc() { return perf_integral<uxs::set<int>>(perf_N); }
int test_perf_int_global_pool() {
    return perf_integral<uxs::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N);
}
int test_perf_int_pool() { return perf_integral<uxs::set<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N); }

int test_perf_T_std_alloc_std() { return perf_integral<std::set<T>>(perf_N); }
int test_perf_T_global_pool_std() {
    return perf_integral<std::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(perf_N);
}
int test_perf_T_pool_std() { return perf_integral<std::set<T, std::less<T>, uxs::pool_allocator<T>>>(perf_N); }
int test_perf_int_std_alloc_std() { return perf_integral<std::set<int>>(perf_N); }
int test_perf_int_global_pool_std() {
    return perf_integral<std::set<int, std::less<int>, uxs::global_pool_allocator<int>>>(perf_N);
}
int test_perf_int_pool_std() { return perf_integral<std::set<int, std::less<int>, uxs::pool_allocator<int>>>(perf_N); }

// --------------------------------------------

int test_info_sizeof_T_std_alloc() { return sizeof(uxs::set<T>); }
int test_info_sizeof_T_global_pool() { return sizeof(uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>); }
int test_info_sizeof_T_pool() { return sizeof(uxs::set<T, std::less<T>, uxs::pool_allocator<T>>); }
int test_info_sizeof_T_iterator() { return sizeof(uxs::set<T>::iterator); }
int test_info_sizeof_T_node() { return sizeof(uxs::detail::set_node_type<T>); }

int test_info_sizeof_T_std_alloc_std() { return sizeof(std::set<T>); }
int test_info_sizeof_T_global_pool_std() { return sizeof(std::set<T, std::less<T>, uxs::global_pool_allocator<T>>); }
int test_info_sizeof_T_pool_std() { return sizeof(std::set<T, std::less<T>, uxs::pool_allocator<T>>); }
int test_info_sizeof_T_iterator_std() { return sizeof(std::set<T>::iterator); }

}  // namespace

ADD_TEST_CASE("", "rbtree", test_0);
ADD_TEST_CASE("", "rbtree", test_3);
ADD_TEST_CASE("", "rbtree", test_4);
ADD_TEST_CASE("", "rbtree", test_5);
ADD_TEST_CASE("", "rbtree", test_6);
ADD_TEST_CASE("", "rbtree", test_7);
ADD_TEST_CASE("", "rbtree", test_8);
ADD_TEST_CASE("", "rbtree", test_9);
ADD_TEST_CASE("", "rbtree", test_10);
ADD_TEST_CASE("", "rbtree", test_11);
ADD_TEST_CASE("", "rbtree", test_12);
ADD_TEST_CASE("", "rbtree", test_13);
ADD_TEST_CASE("", "rbtree", test_18);
ADD_TEST_CASE("", "rbtree", test_19);
ADD_TEST_CASE("", "rbtree", test_20);
ADD_TEST_CASE("", "rbtree", test_21);
ADD_TEST_CASE("", "rbtree", test_22);
ADD_TEST_CASE("", "rbtree", test_23);

ADD_TEST_CASE("1-bruteforce", "rbtree", test_bruteforce_unique);
ADD_TEST_CASE("1-bruteforce", "rbtree", test_bruteforce_multi);

ADD_TEST_CASE("2-perf", "rbtree:T: insert", test_perf_insert_T_std_alloc);
ADD_TEST_CASE("2-perf", "rbtree:T:global_pool_allocator: insert", test_perf_insert_T_global_pool);
ADD_TEST_CASE("2-perf", "rbtree:T:pool_allocator: insert", test_perf_insert_T_pool);
ADD_TEST_CASE("2-perf", "rbtree:int: insert", test_perf_insert_int_std_alloc);
ADD_TEST_CASE("2-perf", "rbtree:int:global_pool_allocator: insert", test_perf_insert_int_global_pool);
ADD_TEST_CASE("2-perf", "rbtree:int:pool_allocator: insert", test_perf_insert_int_pool);
ADD_TEST_CASE("2-perf", "<std> rbtree:T: insert", test_perf_insert_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:T:global_pool_allocator: insert", test_perf_insert_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:T:pool_allocator: insert", test_perf_insert_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:int: insert", test_perf_insert_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:int:global_pool_allocator: insert", test_perf_insert_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:int:pool_allocator: insert", test_perf_insert_int_pool_std);

ADD_TEST_CASE("2-perf", "rbtree:T: forward", test_perf_forward_T_std_alloc);
ADD_TEST_CASE("2-perf", "rbtree:T:global_pool_allocator: forward", test_perf_forward_T_global_pool);
ADD_TEST_CASE("2-perf", "rbtree:T:pool_allocator: forward", test_perf_forward_T_pool);
ADD_TEST_CASE("2-perf", "rbtree:int: forward", test_perf_forward_int_std_alloc);
ADD_TEST_CASE("2-perf", "rbtree:int:global_pool_allocator: forward", test_perf_forward_int_global_pool);
ADD_TEST_CASE("2-perf", "rbtree:int:pool_allocator: forward", test_perf_forward_int_pool);
ADD_TEST_CASE("2-perf", "<std> rbtree:T: forward", test_perf_forward_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:T:global_pool_allocator: forward", test_perf_forward_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:T:pool_allocator: forward", test_perf_forward_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:int: forward", test_perf_forward_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:int:global_pool_allocator: forward", test_perf_forward_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:int:pool_allocator: forward", test_perf_forward_int_pool_std);

ADD_TEST_CASE("2-perf", "rbtree:T: backward", test_perf_backward_T_std_alloc);
ADD_TEST_CASE("2-perf", "rbtree:T:global_pool_allocator: backward", test_perf_backward_T_global_pool);
ADD_TEST_CASE("2-perf", "rbtree:T:pool_allocator: backward", test_perf_backward_T_pool);
ADD_TEST_CASE("2-perf", "rbtree:int: backward", test_perf_backward_int_std_alloc);
ADD_TEST_CASE("2-perf", "rbtree:int:global_pool_allocator: backward", test_perf_backward_int_global_pool);
ADD_TEST_CASE("2-perf", "rbtree:int:pool_allocator: backward", test_perf_backward_int_pool);
ADD_TEST_CASE("2-perf", "<std> rbtree:T: backward", test_perf_backward_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:T:global_pool_allocator: backward", test_perf_backward_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:T:pool_allocator: backward", test_perf_backward_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:int: backward", test_perf_backward_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:int:global_pool_allocator: backward", test_perf_backward_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:int:pool_allocator: backward", test_perf_backward_int_pool_std);

ADD_TEST_CASE("2-perf", "rbtree:T: find", test_perf_find_T_std_alloc);
ADD_TEST_CASE("2-perf", "rbtree:T:global_pool_allocator: find", test_perf_find_T_global_pool);
ADD_TEST_CASE("2-perf", "rbtree:T:pool_allocator: find", test_perf_find_T_pool);
ADD_TEST_CASE("2-perf", "rbtree:int: find", test_perf_find_int_std_alloc);
ADD_TEST_CASE("2-perf", "rbtree:int:global_pool_allocator: find", test_perf_find_int_global_pool);
ADD_TEST_CASE("2-perf", "rbtree:int:pool_allocator: find", test_perf_find_int_pool);
ADD_TEST_CASE("2-perf", "<std> rbtree:T: find", test_perf_find_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:T:global_pool_allocator: find", test_perf_find_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:T:pool_allocator: find", test_perf_find_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:int: find", test_perf_find_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:int:global_pool_allocator: find", test_perf_find_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:int:pool_allocator: find", test_perf_find_int_pool_std);

ADD_TEST_CASE("2-perf", "rbtree:T", test_perf_T_std_alloc);
ADD_TEST_CASE("2-perf", "rbtree:T:global_pool_allocator", test_perf_T_global_pool);
ADD_TEST_CASE("2-perf", "rbtree:T:pool_allocator", test_perf_T_pool);
ADD_TEST_CASE("2-perf", "rbtree:int", test_perf_int_std_alloc);
ADD_TEST_CASE("2-perf", "rbtree:int:global_pool_allocator", test_perf_int_global_pool);
ADD_TEST_CASE("2-perf", "rbtree:int:pool_allocator", test_perf_int_pool);
ADD_TEST_CASE("2-perf", "<std> rbtree:T", test_perf_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:T:global_pool_allocator", test_perf_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:T:pool_allocator", test_perf_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:int", test_perf_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:int:global_pool_allocator", test_perf_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> rbtree:int:pool_allocator", test_perf_int_pool_std);

ADD_TEST_CASE("3-info", "size: rbtree:T", test_info_sizeof_T_std_alloc);
ADD_TEST_CASE("3-info", "size: rbtree:T:global_pool_allocator", test_info_sizeof_T_global_pool);
ADD_TEST_CASE("3-info", "size: rbtree:T:pool_allocator", test_info_sizeof_T_pool);
ADD_TEST_CASE("3-info", "size: rbtree:T:iterator", test_info_sizeof_T_iterator);
ADD_TEST_CASE("3-info", "size: rbtree:T:node", test_info_sizeof_T_node);
ADD_TEST_CASE("3-info", "<std> size: rbtree:T", test_info_sizeof_T_std_alloc_std);
ADD_TEST_CASE("3-info", "<std> size: rbtree:T:global_pool_allocator", test_info_sizeof_T_global_pool_std);
ADD_TEST_CASE("3-info", "<std> size: rbtree:T:pool_allocator", test_info_sizeof_T_pool_std);
ADD_TEST_CASE("3-info", "<std> size: rbtree:T:iterator", test_info_sizeof_T_iterator_std);
