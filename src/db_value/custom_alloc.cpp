#ifndef _MSC_VER

#    include "test_allocators.h"

#    include <uxs/impl/db/value_impl.h>

using namespace uxs_test_suite;

namespace uxs {
namespace db {
template class basic_value<char, test_allocator<char>>;
template class basic_value<wchar_t, test_allocator<wchar_t>>;
}  // namespace db
}  // namespace uxs

#endif
