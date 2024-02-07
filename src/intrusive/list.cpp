#include "uxs/intrusive/list.h"

#include "test_suite.h"

#include <vector>

namespace {

struct Integer0 : uxs::intrusive::list_links_t {
    static size_t counter;
    bool valid_;
    int a_;
    explicit Integer0(int a) : valid_(true), a_(a) { ++counter; }
    Integer0(const Integer0& a) : valid_(true), a_(a.a_) { ++counter; }
    Integer0(Integer0&& a) : valid_(a.valid_), a_(a.a_) { a.valid_ = false; }
    ~Integer0() {
        if (valid_) { --counter; }
    }
    int a() const { return a_; }
};

struct Integer1 {
    bool valid_;
    int a_;
    uxs::intrusive::list_links_t member_hook_;

    using opt_member_hook =
        uxs::intrusive::opt_member_hook<Integer1, uxs::intrusive::list_links_t, &Integer1::member_hook_>;

    struct member_hook_caster {
        Integer1* cast(uxs::intrusive::list_links_t* h) const;
    };

    explicit Integer1(int a) : valid_(true), a_(a) { ++Integer0::counter; }
    Integer1(const Integer1& a) : valid_(true), a_(a.a_) { ++Integer0::counter; }
    Integer1(Integer1&& a) : valid_(a.valid_), a_(a.a_) { a.valid_ = false; }
    ~Integer1() {
        if (valid_) { --Integer0::counter; }
    }
    int a() const { return a_; }
};

static_assert(std::is_standard_layout<uxs::intrusive::list_links_t>::value, "");
static_assert(std::is_standard_layout<Integer1>::value, "");

struct Integer {
    bool valid_;
    int a_;

    struct member_hook_with_ptr_t : uxs::intrusive::list_links_t {
        Integer* ptr;
    };

    member_hook_with_ptr_t member_hook_with_ptr_;

    struct member_hook_with_unique_ptr_t : uxs::intrusive::list_links_t {
        std::unique_ptr<Integer> ptr;
    };

    member_hook_with_unique_ptr_t member_hook_with_unique_ptr_;

    using opt_internal_pointer =
        uxs::intrusive::opt_internal_pointer<member_hook_with_ptr_t, Integer*, &member_hook_with_ptr_t::ptr>;
    using opt_member_hook_with_ptr =
        uxs::intrusive::opt_member_hook<Integer, member_hook_with_ptr_t, &Integer::member_hook_with_ptr_>;
    using opt_internal_unique_pointer = uxs::intrusive::opt_internal_pointer<
        member_hook_with_unique_ptr_t, std::unique_ptr<Integer>, &member_hook_with_unique_ptr_t::ptr>;
    using opt_member_hook_with_unique_ptr =
        uxs::intrusive::opt_member_hook<Integer, member_hook_with_unique_ptr_t, &Integer::member_hook_with_unique_ptr_>;

    explicit Integer(int a) : valid_(true), a_(a) { ++Integer0::counter; }
    Integer(const Integer& a) : valid_(true), a_(a.a_) { ++Integer0::counter; }
    Integer(Integer&& a) : valid_(a.valid_), a_(a.a_) { a.valid_ = false; }
    ~Integer() {
        if (valid_) { --Integer0::counter; }
    }
    int a() const { return a_; }
};

size_t Integer0::counter = 0;

inline Integer1* Integer1::member_hook_caster::cast(uxs::intrusive::list_links_t* h) const {
    return uxs::get_containing_record<Integer1, offsetof(Integer1, member_hook_)>(h);
}

int test_list_0() {
    Integer0::counter = 0;

    std::vector<Integer0> vec;
    vec.push_back(Integer0{1});
    vec.push_back(Integer0{2});
    vec.push_back(Integer0{3});
    vec.push_back(Integer0{4});
    vec.push_back(Integer0{5});

    uxs::intrusive::list<Integer0> list;

    for (auto& x : vec) { list.push_back(&x); }
    VERIFY(vec.size() == list.size());
    VERIFY(Integer0::counter == 5);

    size_t n = 0;
    for (const auto& x : list) { VERIFY(x.a() == vec[n++].a()); }

    auto it = list.erase(list.begin());
    VERIFY(it == list.begin());
    VERIFY(list.size() == 4);
    VERIFY(list.begin()->a() == 2);
    VERIFY(std::next(list.begin())->a() == 3);
    VERIFY(std::next(list.begin(), 2)->a() == 4);
    VERIFY(std::next(list.begin(), 3)->a() == 5);

    list.pop_back();
    VERIFY(list.size() == 3);
    VERIFY(list.begin()->a() == 2);
    VERIFY(std::next(list.begin())->a() == 3);
    VERIFY(std::next(list.begin(), 2)->a() == 4);

    list.pop_front();
    VERIFY(list.size() == 2);
    VERIFY(list.begin()->a() == 3);
    VERIFY(std::next(list.begin())->a() == 4);

    auto p1 = list.extract_front();
    VERIFY(list.size() == 1);
    VERIFY(p1->a() == 3);
    VERIFY(list.begin()->a() == 4);

    auto p2 = list.extract_back();
    VERIFY(list.size() == 0 && list.empty());
    VERIFY(p2->a() == 4);

    VERIFY(Integer0::counter == 5);
    return 0;
}

int test_list_1() {
    Integer0::counter = 0;

    std::vector<Integer1> vec;
    vec.push_back(Integer1{1});
    vec.push_back(Integer1{2});
    vec.push_back(Integer1{3});
    vec.push_back(Integer1{4});
    vec.push_back(Integer1{5});

    using list_type = uxs::intrusive::list<
        Integer1,
        uxs::intrusive::list_hook_traits<Integer1, uxs::intrusive::list_links_t,
                                         uxs::intrusive::opt_hook_caster<Integer1::member_hook_caster>>,
        uxs::intrusive::list_hook_getter<Integer1::opt_member_hook>>;

    list_type list;

    for (auto& x : vec) { list.push_back(&x); }
    VERIFY(vec.size() == list.size());
    VERIFY(Integer0::counter == 5);

    size_t n = 0;
    for (const auto& x : list) { VERIFY(x.a() == vec[n++].a()); }

    auto it = list.erase(list.begin());
    VERIFY(it == list.begin());
    VERIFY(list.size() == 4);
    VERIFY(list.begin()->a() == 2);
    VERIFY(std::next(list.begin())->a() == 3);
    VERIFY(std::next(list.begin(), 2)->a() == 4);
    VERIFY(std::next(list.begin(), 3)->a() == 5);

    list.pop_back();
    VERIFY(list.size() == 3);
    VERIFY(list.begin()->a() == 2);
    VERIFY(std::next(list.begin())->a() == 3);
    VERIFY(std::next(list.begin(), 2)->a() == 4);

    list.pop_front();
    VERIFY(list.size() == 2);
    VERIFY(list.begin()->a() == 3);
    VERIFY(std::next(list.begin())->a() == 4);

    auto p1 = list.extract_front();
    VERIFY(list.size() == 1);
    VERIFY(p1->a() == 3);
    VERIFY(list.begin()->a() == 4);

    auto p2 = list.extract_back();
    VERIFY(list.size() == 0 && list.empty());
    VERIFY(p2->a() == 4);

    VERIFY(Integer0::counter == 5);
    return 0;
}

int test_list_2() {
    Integer0::counter = 0;

    std::vector<Integer> vec;
    vec.push_back(Integer{1});
    vec.push_back(Integer{2});
    vec.push_back(Integer{3});
    vec.push_back(Integer{4});
    vec.push_back(Integer{5});

    using list_type = uxs::intrusive::list<
        Integer,
        uxs::intrusive::list_hook_traits<Integer, Integer::member_hook_with_ptr_t, Integer::opt_internal_pointer>,
        uxs::intrusive::list_hook_getter<Integer::opt_member_hook_with_ptr>>;

    list_type list;

    for (auto& x : vec) { list.push_back(&x); }
    VERIFY(vec.size() == list.size());
    VERIFY(Integer0::counter == 5);

    size_t n = 0;
    for (const auto& x : list) { VERIFY(x.a() == vec[n++].a()); }

    auto it = list.erase(list.begin());
    VERIFY(it == list.begin());
    VERIFY(list.size() == 4);
    VERIFY(list.begin()->a() == 2);
    VERIFY(std::next(list.begin())->a() == 3);
    VERIFY(std::next(list.begin(), 2)->a() == 4);
    VERIFY(std::next(list.begin(), 3)->a() == 5);

    list.pop_back();
    VERIFY(list.size() == 3);
    VERIFY(list.begin()->a() == 2);
    VERIFY(std::next(list.begin())->a() == 3);
    VERIFY(std::next(list.begin(), 2)->a() == 4);

    list.pop_front();
    VERIFY(list.size() == 2);
    VERIFY(list.begin()->a() == 3);
    VERIFY(std::next(list.begin())->a() == 4);

    auto p1 = list.extract_front();
    VERIFY(list.size() == 1);
    VERIFY(p1->a() == 3);
    VERIFY(list.begin()->a() == 4);
    VERIFY(p1->member_hook_with_ptr_.ptr == nullptr);

    auto p2 = list.extract_back();
    VERIFY(list.size() == 0 && list.empty());
    VERIFY(p2->a() == 4);
    VERIFY(p2->member_hook_with_ptr_.ptr == nullptr);

    VERIFY(Integer0::counter == 5);
    return 0;
}

int test_list_3() {
    Integer0::counter = 0;

    std::vector<Integer> vec;
    vec.push_back(Integer{1});
    vec.push_back(Integer{2});
    vec.push_back(Integer{3});
    vec.push_back(Integer{4});
    vec.push_back(Integer{5});

    using list_type =
        uxs::intrusive::list<Integer,
                             uxs::intrusive::list_hook_traits<Integer, Integer::member_hook_with_unique_ptr_t,
                                                              Integer::opt_internal_unique_pointer>,
                             uxs::intrusive::list_hook_getter<Integer::opt_member_hook_with_unique_ptr>>;

    list_type list;

    for (const auto& x : vec) { list.push_back(std::unique_ptr<Integer>(new Integer(x.a()))); }
    VERIFY(vec.size() == list.size());
    VERIFY(Integer0::counter == 10);

    size_t n = 0;
    for (const auto& x : list) { VERIFY(x.a() == vec[n++].a()); }

    auto it = list.erase(list.begin());
    VERIFY(it == list.begin());
    VERIFY(list.size() == 4);
    VERIFY(list.begin()->a() == 2);
    VERIFY(std::next(list.begin())->a() == 3);
    VERIFY(std::next(list.begin(), 2)->a() == 4);
    VERIFY(std::next(list.begin(), 3)->a() == 5);

    VERIFY(Integer0::counter == 9);

    list.pop_back();
    VERIFY(list.size() == 3);
    VERIFY(list.begin()->a() == 2);
    VERIFY(std::next(list.begin())->a() == 3);
    VERIFY(std::next(list.begin(), 2)->a() == 4);

    VERIFY(Integer0::counter == 8);

    list.pop_front();
    VERIFY(list.size() == 2);
    VERIFY(list.begin()->a() == 3);
    VERIFY(std::next(list.begin())->a() == 4);

    VERIFY(Integer0::counter == 7);

    auto p1 = list.extract_front();
    VERIFY(list.size() == 1);
    VERIFY(p1->a() == 3);
    VERIFY(list.begin()->a() == 4);
    VERIFY(!p1->member_hook_with_unique_ptr_.ptr.get());

    auto p2 = list.extract_back();
    VERIFY(list.size() == 0 && list.empty());
    VERIFY(p2->a() == 4);
    VERIFY(!p2->member_hook_with_unique_ptr_.ptr.get());

    VERIFY(Integer0::counter == 7);

    p1.reset();
    p2.reset();
    VERIFY(Integer0::counter == 5);
    return 0;
}

int test_list_4() {
    Integer0::counter = 0;

    std::vector<Integer1> vec;
    vec.push_back(Integer1{1});
    vec.push_back(Integer1{2});
    vec.push_back(Integer1{3});
    vec.push_back(Integer1{4});
    vec.push_back(Integer1{5});

    struct list_hook_traits
        : uxs::intrusive::list_hook_traits<Integer1, uxs::intrusive::list_links_t,
                                           uxs::intrusive::opt_hook_caster<Integer1::member_hook_caster>> {
        void dispose(Integer1* p) const { delete p; }
    };

    using list_type =
        uxs::intrusive::list<Integer1, list_hook_traits, uxs::intrusive::list_hook_getter<Integer1::opt_member_hook>>;
    list_type list;

    for (const auto& x : vec) { list.push_back(new Integer1(x.a())); }
    VERIFY(vec.size() == list.size());
    VERIFY(Integer0::counter == 10);

    size_t n = 0;
    for (const auto& x : list) { VERIFY(x.a() == vec[n++].a()); }

    auto it = list.erase(list.begin());
    VERIFY(it == list.begin());
    VERIFY(list.size() == 4);
    VERIFY(list.begin()->a() == 2);
    VERIFY(std::next(list.begin())->a() == 3);
    VERIFY(std::next(list.begin(), 2)->a() == 4);
    VERIFY(std::next(list.begin(), 3)->a() == 5);

    VERIFY(Integer0::counter == 9);

    list.pop_back();
    VERIFY(list.size() == 3);
    VERIFY(list.begin()->a() == 2);
    VERIFY(std::next(list.begin())->a() == 3);
    VERIFY(std::next(list.begin(), 2)->a() == 4);

    VERIFY(Integer0::counter == 8);

    list.pop_front();
    VERIFY(list.size() == 2);
    VERIFY(list.begin()->a() == 3);
    VERIFY(std::next(list.begin())->a() == 4);

    VERIFY(Integer0::counter == 7);

    std::unique_ptr<Integer1> p1(list.extract_front());
    VERIFY(list.size() == 1);
    VERIFY(p1->a() == 3);
    VERIFY(list.begin()->a() == 4);

    std::unique_ptr<Integer1> p2(list.extract_back());
    VERIFY(list.size() == 0 && list.empty());
    VERIFY(p2->a() == 4);

    VERIFY(Integer0::counter == 7);

    p1.reset();
    p2.reset();
    VERIFY(Integer0::counter == 5);
    return 0;
}

int test_list_5() {
    Integer0::counter = 0;

    std::vector<Integer> vec;
    vec.push_back(Integer{1});
    vec.push_back(Integer{2});
    vec.push_back(Integer{3});
    vec.push_back(Integer{4});
    vec.push_back(Integer{5});

    struct list_hook_traits
        : uxs::intrusive::list_hook_traits<Integer, Integer::member_hook_with_ptr_t, Integer::opt_internal_pointer> {
        void dispose(Integer* p) const { delete p; }
    };

    using list_type = uxs::intrusive::list<Integer, list_hook_traits,
                                           uxs::intrusive::list_hook_getter<Integer::opt_member_hook_with_ptr>>;
    list_type list;

    for (const auto& x : vec) { list.push_back(new Integer(x.a())); }
    VERIFY(vec.size() == list.size());
    VERIFY(Integer0::counter == 10);

    size_t n = 0;
    for (const auto& x : list) { VERIFY(x.a() == vec[n++].a()); }

    auto it = list.erase(list.begin());
    VERIFY(it == list.begin());
    VERIFY(list.size() == 4);
    VERIFY(list.begin()->a() == 2);
    VERIFY(std::next(list.begin())->a() == 3);
    VERIFY(std::next(list.begin(), 2)->a() == 4);
    VERIFY(std::next(list.begin(), 3)->a() == 5);

    VERIFY(Integer0::counter == 9);

    list.pop_back();
    VERIFY(list.size() == 3);
    VERIFY(list.begin()->a() == 2);
    VERIFY(std::next(list.begin())->a() == 3);
    VERIFY(std::next(list.begin(), 2)->a() == 4);

    VERIFY(Integer0::counter == 8);

    list.pop_front();
    VERIFY(list.size() == 2);
    VERIFY(list.begin()->a() == 3);
    VERIFY(std::next(list.begin())->a() == 4);

    VERIFY(Integer0::counter == 7);

    std::unique_ptr<Integer> p1(list.extract_front());
    VERIFY(list.size() == 1);
    VERIFY(p1->a() == 3);
    VERIFY(list.begin()->a() == 4);
    VERIFY(p1->member_hook_with_ptr_.ptr == nullptr);

    std::unique_ptr<Integer> p2(list.extract_back());
    VERIFY(list.size() == 0 && list.empty());
    VERIFY(p2->a() == 4);
    VERIFY(p2->member_hook_with_ptr_.ptr == nullptr);

    VERIFY(Integer0::counter == 7);

    p1.reset();
    p2.reset();
    VERIFY(Integer0::counter == 5);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "intrusive::list", test_list_0);
ADD_TEST_CASE("", "intrusive::list", test_list_1);
ADD_TEST_CASE("", "intrusive::list", test_list_2);
ADD_TEST_CASE("", "intrusive::list", test_list_3);
ADD_TEST_CASE("", "intrusive::list", test_list_4);
ADD_TEST_CASE("", "intrusive::list", test_list_5);
