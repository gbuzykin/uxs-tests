#include "db_value_tests.h"

using namespace uxs_test_suite;

namespace {

int test_not_an_array() {
    {
        uxs::db::value v;
        MUST_THROW(v.erase(0));
    }
    {
        uxs::db::value v("1");
        MUST_THROW(v.erase(0));
    }
    return 0;
}

int test_erase_one() {
    std::initializer_list<uxs::db::value> init = {"1", "2", "3", "4", "5"};
    std::initializer_list<uxs::db::value> tst1 = {"1", "2", "3", "4"};
    std::initializer_list<uxs::db::value> tst2 = {"1", "3", "4"};
    std::initializer_list<uxs::db::value> tst3 = {"3", "4"};
    uxs::db::value v(init);
    // back
    v.erase(4);
    CHECK_ARRAY(v, tst1.size(), tst1.begin());
    // mid
    v.erase(1);
    CHECK_ARRAY(v, tst2.size(), tst2.begin());
    // front
    v.erase(0);
    CHECK_ARRAY(v, tst3.size(), tst3.begin());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "db::value", test_not_an_array);
ADD_TEST_CASE("", "db::value", test_erase_one);
