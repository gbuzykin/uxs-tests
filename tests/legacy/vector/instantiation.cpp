#include "uxs_testsuite/types.h"

#include <uxs-legacy/vector.h>

using namespace uxs_testsuite;

namespace uxs {
template class vector<T>;
template class vector<T_ThrowingMove>;
}  // namespace uxs
