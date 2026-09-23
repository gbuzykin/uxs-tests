#include "test_types.h"

#include <uxs-legacy/vector.h>

using namespace uxs_test_suite;

namespace uxs {
template class vector<T>;
template class vector<T_ThrowingMove>;
}  // namespace uxs
