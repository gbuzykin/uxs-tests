#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

#if defined(__GNUC__)
#    pragma GCC diagnostic ignored "-Wunused-function"
#elif defined(__clang__)
#    pragma clang diagnostic ignored "-Wunused-function"
#endif

void test_types() {
    static_assert(uxs::is_input_iterator<uxs::vector<T>::iterator>::value == true, "");
    static_assert(uxs::is_random_access_iterator<uxs::vector<T>::iterator>::value == true, "");

    static_assert(std::is_nothrow_default_constructible<T>::value == false, "");
    static_assert(std::is_nothrow_copy_constructible<T>::value == false, "");
    static_assert(std::is_nothrow_move_constructible<T>::value == true, "");
    static_assert(std::is_copy_assignable<T>::value == true, "");
    static_assert(std::is_move_assignable<T>::value == true, "");
    static_assert(std::is_assignable<T&, const int&>::value == false, "");
    static_assert(std::is_assignable<T&, int&&>::value == false, "");
    static_assert(std::is_assignable<T&, int>::value == false, "");

    static_assert(std::is_nothrow_default_constructible<T_ThrowingMove>::value == false, "");
    static_assert(std::is_nothrow_copy_constructible<T_ThrowingMove>::value == false, "");
    static_assert(std::is_nothrow_move_constructible<T_ThrowingMove>::value == false, "");
    static_assert(std::is_copy_assignable<T_ThrowingMove>::value == true, "");
    static_assert(std::is_move_assignable<T_ThrowingMove>::value == true, "");
    static_assert(std::is_assignable<T_ThrowingMove&, const int&>::value == false, "");
    static_assert(std::is_assignable<T_ThrowingMove&, int&&>::value == false, "");
    static_assert(std::is_assignable<T_ThrowingMove&, int>::value == false, "");

    static_assert(uxs::is_alloc_always_equal<test_allocator<T>>::value == false, "");
    static_assert(uxs::is_alloc_always_equal<unfriendly_test_allocator<T>>::value == false, "");
}

}  // namespace
