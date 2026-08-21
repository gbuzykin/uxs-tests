#include "test_suite.h"

#include <uxs/function_output_iterator.h>
#include <uxs/functional.h>

#include <uxs-legacy/vector.h>

namespace {

uxs::vector<std::string> g_vec;

void g_func(const std::string& s) { g_vec.emplace_back(s); }

int test_functional_1() {
    struct OutoutIt : uxs::function_output_iterator_facade<OutoutIt> {
        explicit OutoutIt(void (*fn)(const std::string&)) : fn(fn) {}
        void (*fn)(const std::string&);
        void operator()(const std::string& s) const { fn(s); }
    };

    uxs::vector<std::string> strs{"aaa", "bbb", "ccc"};
    g_vec.clear();
    std::copy(strs.begin(), strs.end(), OutoutIt(&g_func));
    VERIFY(g_vec[0] == "aaa");
    VERIFY(g_vec[1] == "bbb");
    VERIFY(g_vec[2] == "ccc");
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "functional", test_functional_1);
