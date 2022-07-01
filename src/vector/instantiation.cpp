#include "vector_tests.h"

using namespace uxs_test_suite;

namespace uxs {
template class vector<T>;
template class vector<T_ThrowingMove>;
}  // namespace uxs
