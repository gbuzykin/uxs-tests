#include <uxs/optional.h>

#include <cstdlib>
#include <memory>
#include <string>
#include <type_traits>

static_assert(std::is_trivially_copy_constructible<est::optional<int>>::value, "");
static_assert(std::is_trivially_copy_assignable<est::optional<int>>::value, "");
static_assert(std::is_trivially_move_constructible<est::optional<int>>::value, "");
static_assert(std::is_trivially_move_assignable<est::optional<int>>::value, "");
static_assert(std::is_trivially_destructible<est::optional<int>>::value, "");

static_assert(!std::is_trivially_copy_constructible<est::optional<std::string>>::value, "");
static_assert(!std::is_trivially_copy_assignable<est::optional<std::string>>::value, "");
static_assert(!std::is_trivially_move_constructible<est::optional<std::string>>::value, "");
static_assert(!std::is_trivially_move_assignable<est::optional<std::string>>::value, "");
static_assert(!std::is_trivially_destructible<est::optional<std::string>>::value, "");

static_assert(std::is_copy_constructible<est::optional<std::string>>::value, "");
static_assert(std::is_copy_assignable<est::optional<std::string>>::value, "");
static_assert(!std::is_nothrow_copy_constructible<est::optional<std::string>>::value, "");
static_assert(!std::is_nothrow_copy_assignable<est::optional<std::string>>::value, "");
static_assert(std::is_nothrow_move_constructible<est::optional<std::string>>::value, "");
static_assert(std::is_nothrow_move_assignable<est::optional<std::string>>::value, "");

static_assert(!std::is_copy_constructible<est::optional<std::unique_ptr<int>>>::value, "");
static_assert(!std::is_copy_assignable<est::optional<std::unique_ptr<int>>>::value, "");
static_assert(std::is_nothrow_move_constructible<est::optional<std::unique_ptr<int>>>::value, "");
static_assert(std::is_nothrow_move_assignable<est::optional<std::unique_ptr<int>>>::value, "");
