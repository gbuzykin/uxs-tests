#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_not_an_array() {
    {
        uxs::db::value v;
        MUST_THROW(v.remove(0));
    }
    {
        uxs::db::value v("1");
        MUST_THROW(v.remove(0));
    }
    return 0;
}

int test_erase_one() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    std::string_view tst1[] = {"1", "2", "3", "4"};
    std::string_view tst2[] = {"1", "3", "4"};
    std::string_view tst3[] = {"3", "4"};
    uxs::db::value v(init);
    // back
    v.remove(4);
    CHECK_ARRAY(v, 4, tst1);
    // mid
    v.remove(1);
    CHECK_ARRAY(v, 3, tst2);
    // front
    v.remove(0);
    CHECK_ARRAY(v, 2, tst3);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_an_array);
ADD_TEST_CASE("", "db::value", test_erase_one);
