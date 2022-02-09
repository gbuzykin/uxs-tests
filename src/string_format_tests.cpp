#include "test_suite.h"
#include "util/format.h"

namespace {

int test_string_format_0() {
    VERIFY(util::sformat("abcdefghi").str() == "abcdefghi");
    VERIFY(util::sformat("%1abcdefghi").arg("A").str() == "Aabcdefghi");
    VERIFY(util::sformat("%1abcdefghi%2").arg("A").arg("B").str() == "AabcdefghiB");
    VERIFY(util::sformat("%1%3abcdefghi%2").arg("A").arg("B").arg("C").str() == "ACabcdefghiB");
    VERIFY(util::sformat("%1%3abcdefghi%2%").arg("A").arg("B").arg("C").str() == "ACabcdefghiB");
    VERIFY(util::sformat("%1%3%abcdefghi%2%").arg("A").arg("B").arg("C").str() == "ACabcdefghiB");
    VERIFY(util::sformat("%1%3%4abcdefghi%2%").arg("A").arg("B").arg("C").arg("D").str() == "ACDabcdefghiB");
    VERIFY(util::sformat("%1%3%4%%abcdefghi%2%%").arg("A").arg("B").arg("C").arg("D").str() == "ACD%abcdefghiB%");

    VERIFY(util::sformat("%1%2%3%4").arg("1").arg("2").arg("3").arg("4").str() == "1234");
    VERIFY(util::sformat("%4%3%2%1").arg("1").arg("2").arg("3").arg("4").str() == "4321");
    VERIFY(util::sformat("%4%3%2%1").arg("1").arg("2").str() == "21");
    VERIFY(util::sformat("%4%3%2%1").str() == "");
    VERIFY(util::sformat("%4%1%3%2").arg("1").arg("2").arg("3").arg("4").str() == "4132");
    VERIFY(util::sformat("%1").arg(1, util::sfield(8, '*')).str() == "*******1");
    VERIFY(util::sformat("%1").arg(2.34, util::sfield(8, '*'), util::scvt_fp::kFixed, 2).str() == "****2.34");
    VERIFY(util::sformat("%1").arg(2.34, util::scvt_fp::kFixed, 2).str() == "2.34");
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "string format", test_string_format_0);
