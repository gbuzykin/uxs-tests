#define UXS_EXPORT

#include "test_allocators.h"

#include "uxs/impl/db/value_impl.h"  // NOLINT

using namespace uxs_test_suite;

UXS_DB_VALUE_INSTANTIATE_IMPLEMENTATION(char, test_allocator<char>);
UXS_DB_VALUE_INSTANTIATE_IMPLEMENTATION(wchar_t, test_allocator<wchar_t>);
