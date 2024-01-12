#include "uxs/intrusive/list.h"

#include "test_suite.h"

#include <vector>

namespace {

class Integer : public uxs::intrusive::list_links_t {
 public:
    struct member_hook_with_ptr_1_t : uxs::intrusive::list_links_t {
        Integer* ptr;
    };

    struct member_hook_with_ptr_2_t : uxs::intrusive::list_links_t {
        std::unique_ptr<Integer> ptr;
    };

 private:
    int a_;
    uxs::intrusive::list_links_t member_hook_;
    member_hook_with_ptr_1_t member_hook_with_ptr_1_;
    member_hook_with_ptr_2_t member_hook_with_ptr_2_;

 public:
    struct member_hook_caster {
        static Integer* cast(uxs::intrusive::list_links_t* h);
    };
    using opt_member_hook =
        uxs::intrusive::opt_member_hook<Integer, uxs::intrusive::list_links_t, &Integer::member_hook_>;
    using opt_internal_pointer_1 =
        uxs::intrusive::opt_internal_pointer<member_hook_with_ptr_1_t, Integer*, &member_hook_with_ptr_1_t::ptr>;
    using opt_internal_pointer_2 = uxs::intrusive::opt_internal_pointer<
        member_hook_with_ptr_2_t, std::unique_ptr<Integer>, &member_hook_with_ptr_2_t::ptr>;
    using opt_member_hook_with_ptr_1 =
        uxs::intrusive::opt_member_hook<Integer, member_hook_with_ptr_1_t, &Integer::member_hook_with_ptr_1_>;
    using opt_member_hook_with_ptr_2 =
        uxs::intrusive::opt_member_hook<Integer, member_hook_with_ptr_2_t, &Integer::member_hook_with_ptr_2_>;

    explicit Integer(int a) : a_(a) {}
    int a() const { return a_; }
};

inline Integer* Integer::member_hook_caster::cast(uxs::intrusive::list_links_t* h) {
    return uxs::get_containing_record<Integer, offsetof(Integer, member_hook_)>(h);
}

int test_list_0() {
    std::vector<Integer> vec;
    vec.push_back(Integer{1});
    vec.push_back(Integer{2});
    vec.push_back(Integer{3});
    vec.push_back(Integer{4});
    vec.push_back(Integer{5});

    uxs::intrusive::list<Integer> list;

    for (auto& x : vec) { list.push_back(&x); }

    VERIFY(vec.size() == list.size());

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

    return 0;
}

int test_list_1() {
    std::vector<Integer> vec;
    vec.push_back(Integer{1});
    vec.push_back(Integer{2});
    vec.push_back(Integer{3});
    vec.push_back(Integer{4});
    vec.push_back(Integer{5});

    using list_type = uxs::intrusive::list<
        Integer,
        uxs::intrusive::list_hook_traits<Integer, uxs::intrusive::list_links_t,
                                         uxs::intrusive::opt_hook_caster<Integer::member_hook_caster>>,
        uxs::intrusive::list_hook_getter<Integer::opt_member_hook>>;
    list_type list;

    for (auto& x : vec) { list.push_back(&x); }

    VERIFY(vec.size() == list.size());

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

    return 0;
}

int test_list_2() {
    std::vector<Integer> vec;
    vec.push_back(Integer{1});
    vec.push_back(Integer{2});
    vec.push_back(Integer{3});
    vec.push_back(Integer{4});
    vec.push_back(Integer{5});

    using list_type = uxs::intrusive::list<
        Integer,
        uxs::intrusive::list_hook_traits<Integer, Integer::member_hook_with_ptr_1_t, Integer::opt_internal_pointer_1>,
        uxs::intrusive::list_hook_getter<Integer::opt_member_hook_with_ptr_1>>;
    list_type list;

    for (auto& x : vec) { list.push_back(&x); }

    VERIFY(vec.size() == list.size());

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

    return 0;
}

int test_list_3() {
    std::vector<Integer> vec;
    vec.push_back(Integer{1});
    vec.push_back(Integer{2});
    vec.push_back(Integer{3});
    vec.push_back(Integer{4});
    vec.push_back(Integer{5});

    using list_type = uxs::intrusive::list<
        Integer,
        uxs::intrusive::list_hook_traits<Integer, Integer::member_hook_with_ptr_2_t, Integer::opt_internal_pointer_2>,
        uxs::intrusive::list_hook_getter<Integer::opt_member_hook_with_ptr_2>>;
    list_type list;

    for (const auto& x : vec) { list.push_back(std::make_unique<Integer>(x.a())); }

    VERIFY(vec.size() == list.size());

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

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "intrusive::list", test_list_0);
ADD_TEST_CASE("", "intrusive::list", test_list_1);
ADD_TEST_CASE("", "intrusive::list", test_list_2);
ADD_TEST_CASE("", "intrusive::list", test_list_3);
