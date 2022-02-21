#include "test_suite.h"
#include "util/regex_ext.h"
#include "util/string_ext.h"

template<typename Ty>
bool check_string_list(const Ty& v, std::initializer_list<std::string_view> tst) {
    if (v.size() != tst.size()) { return false; }
    auto it = tst.begin();
    for (const auto& el : v) {
        if (!(el == *it++)) { return false; }
    }
    return true;
}

#define CHECK(...) \
    if (!check_string_list(__VA_ARGS__)) { \
        throw std::runtime_error(util_test_suite::report_error(__FILE__, __LINE__, "string list mismatched")); \
    }

namespace {

int test_string_ext_0() {
    CHECK(util::split_string("", util::sfind(',')), {""});
    CHECK(util::split_string(",", util::sfind(',')), {"", ""});
    CHECK(util::split_string("1234,", util::sfind(',')), {"1234", ""});
    CHECK(util::split_string("1234,34646", util::sfind(',')), {"1234", "34646"});
    CHECK(util::split_string(",1234,34646,", util::sfind(',')), {"", "1234", "34646", ""});
    CHECK(util::split_string(",1234,34646,,", util::sfind(',')), {"", "1234", "34646", "", ""});
    CHECK(util::split_string(",1234\\,34646,,", util::sfind(',')), {"", "1234\\,34646", "", ""});
    CHECK(util::split_string(",1234,34646,\\,", util::sfind(',')), {"", "1234", "34646", "\\,"});
    CHECK(util::split_string(",1234,34646,,\\", util::sfind(',')), {"", "1234", "34646", "", "\\"});

    VERIFY(util::string_section(",1234,34646,,", util::sfind(','), 0, 0) == "");
    VERIFY(util::string_section(",1234,34646,,", util::sfind(','), 1, 1) == "1234");
    VERIFY(util::string_section(",1234,34646,,", util::sfind(','), 2, 2) == "34646");
    VERIFY(util::string_section(",1234,34646,,", util::sfind(','), 3, 3) == "");

    VERIFY(util::string_section(",1234,34646,,", util::sfind(','), 1, 2) == "1234,34646");
    VERIFY(util::string_section(",1234,34646,,", util::sfind(','), 0, 2) == ",1234,34646");
    VERIFY(util::string_section(",1234,34646,,", util::sfind(','), 2) == "34646,,");
    VERIFY(util::string_section(",1234,34646,,", util::sfind(','), 4, 4) == "");
    VERIFY(util::string_section(",1234,34646,,", util::sfind(','), 10) == "");
    VERIFY(util::string_section<util::split_flags::kSkipEmpty>(",1234,,,34646,,", util::sfind(','), 0, 1) ==
           "1234,,,34646");

    VERIFY(util::string_section(",1234,34646,,124", util::rsfind(','), 0) == "124");
    VERIFY(util::string_section(",1234,34646,,", util::rsfind(','), 0) == "");
    VERIFY(util::string_section(",1234,34646,,", util::rsfind(','), 1, 1) == "");
    VERIFY(util::string_section(",1234,34646,,", util::rsfind(','), 2, 2) == "34646");
    VERIFY(util::string_section(",1234,34646,,", util::rsfind(','), 2, 1) == "34646,");
    VERIFY(util::string_section(",1234,34646,,", util::rsfind(','), 2) == "34646,,");
    VERIFY(util::string_section(",1234,34646,,", util::rsfind(','), 3) == "1234,34646,,");
    VERIFY(util::string_section(",1234,34646,,", util::rsfind(','), 10, 3) == ",1234");
    VERIFY(util::string_section(",1234,34646,,", util::rsfind(','), 10, 10) == "");
    VERIFY(util::string_section<util::split_flags::kSkipEmpty>(",1234,,,34646,,", util::rsfind(','), 1) ==
           "1234,,,34646");
    VERIFY(util::string_section(",1234\\,34646,,", util::rsfind(','), 3) == ",1234\\,34646,,");

    std::string csv = "forename,middlename,surname,phone";
    std::string path = "/usr/local/bin/myapp";  // First field is empty
    std::string data = "forename**middlename**surname**phone";
    std::string line = "forename\tmiddlename  surname \t \t phone";

    VERIFY(util::string_section(csv, util::sfind(','), 2, 2) == "surname");
    VERIFY(util::string_section(path, util::sfind('/'), 3, 4) == "bin/myapp");
    VERIFY(util::string_section<util::split_flags::kSkipEmpty>(path, util::sfind('/'), 3, 3) == "myapp");

    VERIFY(util::string_section(csv, util::rsfind(','), 2, 1) == "middlename,surname");
    VERIFY(util::string_section(path, util::rsfind('/'), 0) == "myapp");

    VERIFY(util::string_section(data, util::sfind("**"), 2, 2) == "surname");
    VERIFY(util::string_section(data, util::rsfind("**"), 2, 1) == "middlename**surname");

    static const std::regex sep("[ \\t]+");
    VERIFY(util::string_section(line, util::sfind(sep), 2, 2) == "surname");
    VERIFY(util::string_section(line, util::rsfind(sep), 2, 1) == "middlename  surname");
    return 0;
}

int test_string_ext_1() {
    std::vector<std::string_view> v = {"111", "222", "333"};
    VERIFY(util::join_strings(v, '-', "abcd") == "abcd111-222-333");
    return 0;
}

int test_string_ext_2() {
    CHECK(util::separate_words("", ','), {});
    CHECK(util::separate_words("   ", ','), {});

    CHECK(util::separate_words("   ,", ','), {"", ""});
    CHECK(util::separate_words("   ,   ", ','), {"", ""});
    CHECK(util::separate_words(",", ','), {"", ""});

    CHECK(util::separate_words("234", ','), {"234"});
    CHECK(util::separate_words("  234", ','), {"234"});
    CHECK(util::separate_words("234   ", ','), {"234"});
    CHECK(util::separate_words("  234   ", ','), {"234"});

    CHECK(util::separate_words(",234", ','), {"", "234"});
    CHECK(util::separate_words(" ,234", ','), {"", "234"});
    CHECK(util::separate_words(" , 234", ','), {"", "234"});

    CHECK(util::separate_words(",,234", ','), {"", "", "234"});
    CHECK(util::separate_words(" ,,234", ','), {"", "", "234"});
    CHECK(util::separate_words(", ,234", ','), {"", "", "234"});
    CHECK(util::separate_words(",, 234", ','), {"", "", "234"});
    CHECK(util::separate_words(" , ,234", ','), {"", "", "234"});
    CHECK(util::separate_words(", , 234", ','), {"", "", "234"});
    CHECK(util::separate_words(" , , 234", ','), {"", "", "234"});

    CHECK(util::separate_words("234  64", ','), {"234", "64"});
    CHECK(util::separate_words("234,64", ','), {"234", "64"});
    CHECK(util::separate_words("234 ,64", ','), {"234", "64"});
    CHECK(util::separate_words("234, 64", ','), {"234", "64"});
    CHECK(util::separate_words("234 , 64", ','), {"234", "64"});
    CHECK(util::separate_words("234,,64", ','), {"234", "", "64"});
    CHECK(util::separate_words("234 ,,64", ','), {"234", "", "64"});
    CHECK(util::separate_words("234, ,64", ','), {"234", "", "64"});
    CHECK(util::separate_words("234,, 64", ','), {"234", "", "64"});
    CHECK(util::separate_words("234 , ,64", ','), {"234", "", "64"});
    CHECK(util::separate_words("234, , 64", ','), {"234", "", "64"});
    CHECK(util::separate_words("234 , , 64", ','), {"234", "", "64"});
    CHECK(util::separate_words("234,,,64", ','), {"234", "", "", "64"});

    CHECK(util::separate_words("234,", ','), {"234", ""});
    CHECK(util::separate_words("234 ,", ','), {"234", ""});
    CHECK(util::separate_words("234, ", ','), {"234", ""});
    CHECK(util::separate_words("234 , ", ','), {"234", ""});
    CHECK(util::separate_words("234,,", ','), {"234", "", ""});
    CHECK(util::separate_words("234 ,,", ','), {"234", "", ""});
    CHECK(util::separate_words("234, ,", ','), {"234", "", ""});
    CHECK(util::separate_words("234,, ", ','), {"234", "", ""});
    CHECK(util::separate_words("234 , ,", ','), {"234", "", ""});
    CHECK(util::separate_words("234, , ", ','), {"234", "", ""});
    CHECK(util::separate_words("234 , , ", ','), {"234", "", ""});
    CHECK(util::separate_words("234,,,", ','), {"234", "", "", ""});

    CHECK(util::separate_words("  234 , 64,  8765   ,72346,87  ", ','), {"234", "64", "8765", "72346", "87"});
    CHECK(util::separate_words(",   234 ,  644 ,, 6778,", ','), {"", "234", "644", "", "6778", ""});
    CHECK(util::separate_words("  ,   234 ,  644,   , 6778,   ", ','), {"", "234", "644", "", "6778", ""});
    CHECK(util::separate_words("  ,   234 ,  644 ,   ,6778  ,   ", ','), {"", "234", "644", "", "6778", ""});
    CHECK(util::separate_words("  ,   234 ,  644,,   ,,6778  ,   ", ','), {"", "234", "644", "", "", "", "6778", ""});
    CHECK(util::separate_words("  ,   234\\ ,  644\\,\\,   ,,6778  ,   ", ','),
          {"", "234\\ ", "644\\,\\,", "", "6778", ""});
    return 0;
}

int test_string_ext_3() {
    CHECK(util::unpack_strings("", ';'), {});
    CHECK(util::unpack_strings(";", ';'), {""});
    CHECK(util::unpack_strings("12;3", ';'), {"12", "3"});
    CHECK(util::unpack_strings("12;3;", ';'), {"12", "3"});
    CHECK(util::unpack_strings("12;3;456", ';'), {"12", "3", "456"});
    CHECK(util::unpack_strings(";12;3;456", ';'), {"", "12", "3", "456"});
    CHECK(util::unpack_strings(";;12;3;;456;;", ';'), {"", "", "12", "3", "", "456", ""});

    CHECK(util::unpack_strings("12\\\\323\\;64567;434553;", ';'), {"12\\323;64567", "434553"});
    CHECK(util::unpack_strings("12\\\\323\\;64567;434553;;;", ';'), {"12\\323;64567", "434553", "", ""});
    CHECK(util::unpack_strings("12\\\\323\\;64567;434553;\\", ';'), {"12\\323;64567", "434553"});
    CHECK(util::unpack_strings("12\\\\323\\;64567;434553;\\\\", ';'), {"12\\323;64567", "434553", "\\"});
    CHECK(util::unpack_strings("12\\\\323\\;\\\\64567;434553\\\\;", ';'), {"12\\323;\\64567", "434553\\"});

    VERIFY(util::pack_strings(util::unpack_strings("12\\\\323\\;64567;434553;\\", ';'), ';', {}) ==
           "12\\\\323\\;64567;434553");
    VERIFY(util::pack_strings(util::unpack_strings("12\\\\323\\;64567;434553;;", ';'), ';', {}) ==
           "12\\\\323\\;64567;434553;;");
    return 0;
}

int test_string_ext_4() {
    VERIFY(util::trim_string("asdf") == "asdf");
    VERIFY(util::trim_string("   asdf") == "asdf");
    VERIFY(util::trim_string("   asdf  ") == "asdf");
    return 0;
}

int test_string_ext_5() {
    VERIFY(util::encode_escapes("1234\\467;;", "\\;", "\\;") == "1234\\\\467\\;\\;");
    VERIFY(util::decode_escapes("1234\\\\467\\;\\;", "", "") == "1234\\467;;");
    VERIFY(util::decode_escapes("1234\\\\467\\;\\;\\", "", "") == "1234\\467;;");
    VERIFY(util::decode_escapes("1234\\\\467\\;\\;\\", "", "") == "1234\\467;;");
    VERIFY(util::decode_escapes("\\n\\n1234\\\\467\\;\\;\\", "\n", "n") == "\n\n1234\\467;;");
    return 0;
}

int test_string_ext_6() {
    VERIFY(util::replace_strings("1234***2345***678", util::sfind("***"), "abcdef") == "1234abcdef2345abcdef678");
    VERIFY(util::replace_strings("1234***2345***678***", util::sfind("***"), "abcdef") ==
           "1234abcdef2345abcdef678abcdef");
    VERIFY(util::replace_strings("***1234***2345***678***", util::sfind("***"), "abcdef") ==
           "abcdef1234abcdef2345abcdef678abcdef");
    return 0;
}

int test_string_ext_7() {
    VERIFY(util::from_utf8_to_wide(
               "\xD0\x94\xD0\xBE\xD0\xB1\xD1\x80\xD1\x8B\xD0\xB9\x20\xD0\xB4\xD0\xB5\xD0\xBD\xD1\x8C\x21") ==
           L"\x0414\x043e\x0431\x0440\x044b\x0439\x0020\x0434\x0435\x043d\x044c\x0021");
    VERIFY(util::from_wide_to_utf8(L"\x0414\x043e\x0431\x0440\x044b\x0439\x0020\x0434\x0435\x043d\x044c\x0021") ==
           "\xD0\x94\xD0\xBE\xD0\xB1\xD1\x80\xD1\x8B\xD0\xB9\x20\xD0\xB4\xD0\xB5\xD0\xBD\xD1\x8C\x21");
    VERIFY(util::from_utf8_to_wide("\xE4\xB8\x8B\xE5\x8D\x88\xE5\xA5\xBD") == L"\x4e0b\x5348\x597d");
    VERIFY(util::from_wide_to_utf8(L"\x4e0b\x5348\x597d") == "\xE4\xB8\x8B\xE5\x8D\x88\xE5\xA5\xBD");
    return 0;
}

int test_string_ext_8() {
    std::string s1;
    std::string_view s2;
    bool r = s1 == s1 || s2 == s2 || s1 == s2 || s2 == s1;
    (void)r;
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "string extension", test_string_ext_0);
ADD_TEST_CASE("", "string extension", test_string_ext_1);
ADD_TEST_CASE("", "string extension", test_string_ext_2);
ADD_TEST_CASE("", "string extension", test_string_ext_3);
ADD_TEST_CASE("", "string extension", test_string_ext_4);
ADD_TEST_CASE("", "string extension", test_string_ext_5);
ADD_TEST_CASE("", "string extension", test_string_ext_6);
ADD_TEST_CASE("", "string extension", test_string_ext_7);
ADD_TEST_CASE("", "string extension", test_string_ext_8);
