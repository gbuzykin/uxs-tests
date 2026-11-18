#define UXS_EXPORT

#include "uxs_testsuite/allocators.h"

#include <uxs/impl/db/value_impl.h>  // IWYU pragma: keep

using namespace uxs_testsuite;

UXS_DB_VALUE_INSTANTIATE_IMPLEMENTATION(char, test_allocator<char>);
UXS_DB_VALUE_INSTANTIATE_IMPLEMENTATION(wchar_t, test_allocator<wchar_t>);
