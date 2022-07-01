#include "rbtree_tests.h"

#include "uxs/multimap.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_move_from_not_empty_new_alloc() {
    test_allocator<void> al, al2;

    {  // different allocators -> per-element movement
        std::initializer_list<std::pair<const Ty, Ty>> init = {{1, 10}, {2, 10}, {3, 10}, {4, 10}, {5, 10}};
        uxs::multimap<Ty, Ty, std::less<Ty>, test_allocator<Ty>> ms_from(init, al2);
        uxs::multimap<Ty, Ty, std::less<Ty>, test_allocator<Ty>> ms(std::move(ms_from), al);
        CHECK(ms, init.size(), init.begin());
        VERIFY(ms.get_allocator() == al);
        VERIFY(ms_from.size() == 0);
        VERIFY(ms_from.get_allocator() == al2);
        VERIFY(T::instance_count == 20);
        VERIFY(T::not_empty_count == 20);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al2.get_alloc_detected() == 5);
        VERIFY(al2.get_alloc_count() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multimap", test_move_from_not_empty_new_alloc);
