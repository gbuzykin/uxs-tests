#include "vector_tests.h"

using namespace uxs_test_suite;

#if defined(__GNUC__)
#    pragma GCC diagnostic ignored "-Wunused-function"
#elif defined(__clang__)
#    pragma clang diagnostic ignored "-Wunused-function"
#endif

static_assert(uxs::is_input_iterator<uxs::vector<T>::iterator>::value, "");
static_assert(uxs::is_random_access_iterator<uxs::vector<T>::iterator>::value, "");

static_assert(!std::is_nothrow_default_constructible<T>::value, "");
static_assert(!std::is_nothrow_copy_constructible<T>::value, "");
static_assert(std::is_nothrow_move_constructible<T>::value, "");
static_assert(std::is_copy_assignable<T>::value, "");
static_assert(std::is_move_assignable<T>::value, "");
static_assert(!std::is_assignable<T&, const int&>::value, "");
static_assert(!std::is_assignable<T&, int&&>::value, "");
static_assert(!std::is_assignable<T&, int>::value, "");

static_assert(!std::is_nothrow_default_constructible<T_ThrowingMove>::value, "");
static_assert(!std::is_nothrow_copy_constructible<T_ThrowingMove>::value, "");
static_assert(!std::is_nothrow_move_constructible<T_ThrowingMove>::value, "");
static_assert(std::is_copy_assignable<T_ThrowingMove>::value, "");
static_assert(std::is_move_assignable<T_ThrowingMove>::value, "");
static_assert(!std::is_assignable<T_ThrowingMove&, const int&>::value, "");
static_assert(!std::is_assignable<T_ThrowingMove&, int&&>::value, "");
static_assert(!std::is_assignable<T_ThrowingMove&, int>::value, "");

static_assert(uxs::is_allocator<std::allocator<T>>::value, "");
static_assert(uxs::is_allocator<test_allocator<T>>::value, "");
static_assert(uxs::is_allocator<unfriendly_test_allocator<T>>::value, "");
static_assert(!uxs::is_allocator<int>::value, "");

static_assert(std::allocator_traits<std::allocator<T>>::is_always_equal::value, "");
static_assert(!std::allocator_traits<test_allocator<T>>::is_always_equal::value, "");
static_assert(!std::allocator_traits<unfriendly_test_allocator<T>>::is_always_equal::value, "");
