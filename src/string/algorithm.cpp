#include "test_suite.h"

#include "uxs/regex.h"
#include "uxs/stringalg.h"

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
        throw std::runtime_error(uxs_test_suite::report_error(__FILE__, __LINE__, "string list mismatched")); \
    }

namespace {

int test_string_alg_0() {
    CHECK(uxs::split_string("", uxs::sfind(',')), {""});
    CHECK(uxs::split_string(",", uxs::sfind(',')), {"", ""});
    CHECK(uxs::split_string("1234,", uxs::sfind(',')), {"1234", ""});
    CHECK(uxs::split_string("1234,34646", uxs::sfind(',')), {"1234", "34646"});
    CHECK(uxs::split_string(",1234,34646,", uxs::sfind(',')), {"", "1234", "34646", ""});
    CHECK(uxs::split_string(",1234,34646,,", uxs::sfind(',')), {"", "1234", "34646", "", ""});
    CHECK(uxs::split_string(",1234\\,34646,,", uxs::sfind(',')), {"", "1234\\,34646", "", ""});
    CHECK(uxs::split_string(",1234,34646,\\,", uxs::sfind(',')), {"", "1234", "34646", "\\,"});
    CHECK(uxs::split_string(",1234,34646,,\\", uxs::sfind(',')), {"", "1234", "34646", "", "\\"});

    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfind(','), 0, 0) == "");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfind(','), 1, 1) == "1234");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfind(','), 2, 2) == "34646");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfind(','), 3, 3) == "");

    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfind(','), 1, 2) == "1234,34646");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfind(','), 0, 2) == ",1234,34646");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfind(','), 2) == "34646,,");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfind(','), 4, 4) == "");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfind(','), 10) == "");
    VERIFY(uxs::string_section<uxs::split_opts::kSkipEmpty>(",1234,,,34646,,", uxs::sfind(','), 0, 1) ==
           "1234,,,34646");

    VERIFY(uxs::string_section(",1234,34646,,124", uxs::rsfind(','), 0) == "124");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfind(','), 0) == "");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfind(','), 1, 1) == "");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfind(','), 2, 2) == "34646");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfind(','), 2, 1) == "34646,");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfind(','), 2) == "34646,,");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfind(','), 3) == "1234,34646,,");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfind(','), 10, 3) == ",1234");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfind(','), 10, 10) == "");
    VERIFY(uxs::string_section<uxs::split_opts::kSkipEmpty>(",1234,,,34646,,", uxs::rsfind(','), 1) == "1234,,,34646");
    VERIFY(uxs::string_section(",1234\\,34646,,", uxs::rsfind(','), 3) == ",1234\\,34646,,");

    std::string csv = "forename,middlename,surname,phone";
    std::string path = "/usr/local/bin/myapp";  // First field is empty
    std::string data = "forename**middlename**surname**phone";
    std::string line = "forename\tmiddlename  surname \t \t phone";

    VERIFY(uxs::string_section(csv, uxs::sfind(','), 2, 2) == "surname");
    VERIFY(uxs::string_section(path, uxs::sfind('/'), 3, 4) == "bin/myapp");
    VERIFY(uxs::string_section<uxs::split_opts::kSkipEmpty>(path, uxs::sfind('/'), 3, 3) == "myapp");

    VERIFY(uxs::string_section(csv, uxs::rsfind(','), 2, 1) == "middlename,surname");
    VERIFY(uxs::string_section(path, uxs::rsfind('/'), 0) == "myapp");

    VERIFY(uxs::string_section(data, uxs::sfind("**"), 2, 2) == "surname");
    VERIFY(uxs::string_section(data, uxs::rsfind("**"), 2, 1) == "middlename**surname");

    static const std::regex sep("[ \\t]+");
    VERIFY(uxs::string_section(line, uxs::sfind(sep), 2, 2) == "surname");
    VERIFY(uxs::string_section(line, uxs::rsfind(sep), 2, 1) == "middlename  surname");
    return 0;
}

int test_string_alg_1() {
    std::vector<std::string_view> v = {"111", "222", "333"};
    VERIFY(uxs::join_strings(v, '-', "abcd") == "abcd111-222-333");
    return 0;
}

int test_string_alg_2() {
    CHECK(uxs::string_to_words("", ','), {});
    CHECK(uxs::string_to_words("   ", ','), {});

    CHECK(uxs::string_to_words("   ,", ','), {"", ""});
    CHECK(uxs::string_to_words("   ,   ", ','), {"", ""});
    CHECK(uxs::string_to_words(",", ','), {"", ""});

    CHECK(uxs::string_to_words("234", ','), {"234"});
    CHECK(uxs::string_to_words("  234", ','), {"234"});
    CHECK(uxs::string_to_words("234   ", ','), {"234"});
    CHECK(uxs::string_to_words("  234   ", ','), {"234"});

    CHECK(uxs::string_to_words(",234", ','), {"", "234"});
    CHECK(uxs::string_to_words(" ,234", ','), {"", "234"});
    CHECK(uxs::string_to_words(" , 234", ','), {"", "234"});

    CHECK(uxs::string_to_words(",,234", ','), {"", "", "234"});
    CHECK(uxs::string_to_words(" ,,234", ','), {"", "", "234"});
    CHECK(uxs::string_to_words(", ,234", ','), {"", "", "234"});
    CHECK(uxs::string_to_words(",, 234", ','), {"", "", "234"});
    CHECK(uxs::string_to_words(" , ,234", ','), {"", "", "234"});
    CHECK(uxs::string_to_words(", , 234", ','), {"", "", "234"});
    CHECK(uxs::string_to_words(" , , 234", ','), {"", "", "234"});

    CHECK(uxs::string_to_words("234  64", ','), {"234", "64"});
    CHECK(uxs::string_to_words("234,64", ','), {"234", "64"});
    CHECK(uxs::string_to_words("234 ,64", ','), {"234", "64"});
    CHECK(uxs::string_to_words("234, 64", ','), {"234", "64"});
    CHECK(uxs::string_to_words("234 , 64", ','), {"234", "64"});
    CHECK(uxs::string_to_words("234,,64", ','), {"234", "", "64"});
    CHECK(uxs::string_to_words("234 ,,64", ','), {"234", "", "64"});
    CHECK(uxs::string_to_words("234, ,64", ','), {"234", "", "64"});
    CHECK(uxs::string_to_words("234,, 64", ','), {"234", "", "64"});
    CHECK(uxs::string_to_words("234 , ,64", ','), {"234", "", "64"});
    CHECK(uxs::string_to_words("234, , 64", ','), {"234", "", "64"});
    CHECK(uxs::string_to_words("234 , , 64", ','), {"234", "", "64"});
    CHECK(uxs::string_to_words("234,,,64", ','), {"234", "", "", "64"});

    CHECK(uxs::string_to_words("234,", ','), {"234", ""});
    CHECK(uxs::string_to_words("234 ,", ','), {"234", ""});
    CHECK(uxs::string_to_words("234, ", ','), {"234", ""});
    CHECK(uxs::string_to_words("234 , ", ','), {"234", ""});
    CHECK(uxs::string_to_words("234,,", ','), {"234", "", ""});
    CHECK(uxs::string_to_words("234 ,,", ','), {"234", "", ""});
    CHECK(uxs::string_to_words("234, ,", ','), {"234", "", ""});
    CHECK(uxs::string_to_words("234,, ", ','), {"234", "", ""});
    CHECK(uxs::string_to_words("234 , ,", ','), {"234", "", ""});
    CHECK(uxs::string_to_words("234, , ", ','), {"234", "", ""});
    CHECK(uxs::string_to_words("234 , , ", ','), {"234", "", ""});
    CHECK(uxs::string_to_words("234,,,", ','), {"234", "", "", ""});

    CHECK(uxs::string_to_words("  234 , 64,  8765   ,72346,87  ", ','), {"234", "64", "8765", "72346", "87"});
    CHECK(uxs::string_to_words(",   234 ,  644 ,, 6778,", ','), {"", "234", "644", "", "6778", ""});
    CHECK(uxs::string_to_words("  ,   234 ,  644,   , 6778,   ", ','), {"", "234", "644", "", "6778", ""});
    CHECK(uxs::string_to_words("  ,   234 ,  644 ,   ,6778  ,   ", ','), {"", "234", "644", "", "6778", ""});
    CHECK(uxs::string_to_words("  ,   234 ,  644,,   ,,6778  ,   ", ','), {"", "234", "644", "", "", "", "6778", ""});
    CHECK(uxs::string_to_words("  ,   234\\ ,  644\\,\\,   ,,6778  ,   ", ','),
          {"", "234\\ ", "644\\,\\,", "", "6778", ""});
    return 0;
}

int test_string_alg_3() {
    CHECK(uxs::unpack_strings("", ';'), {});
    CHECK(uxs::unpack_strings(";", ';'), {""});
    CHECK(uxs::unpack_strings("12;3", ';'), {"12", "3"});
    CHECK(uxs::unpack_strings("12;3;", ';'), {"12", "3"});
    CHECK(uxs::unpack_strings("12;3;456", ';'), {"12", "3", "456"});
    CHECK(uxs::unpack_strings(";12;3;456", ';'), {"", "12", "3", "456"});
    CHECK(uxs::unpack_strings(";;12;3;;456;;", ';'), {"", "", "12", "3", "", "456", ""});

    CHECK(uxs::unpack_strings("12\\\\323\\;64567;434553;", ';'), {"12\\323;64567", "434553"});
    CHECK(uxs::unpack_strings("12\\\\323\\;64567;434553;;;", ';'), {"12\\323;64567", "434553", "", ""});
    CHECK(uxs::unpack_strings("12\\\\323\\;64567;434553;\\", ';'), {"12\\323;64567", "434553"});
    CHECK(uxs::unpack_strings("12\\\\323\\;64567;434553;\\\\", ';'), {"12\\323;64567", "434553", "\\"});
    CHECK(uxs::unpack_strings("12\\\\323\\;\\\\64567;434553\\\\;", ';'), {"12\\323;\\64567", "434553\\"});

    VERIFY(uxs::pack_strings(uxs::unpack_strings("12\\\\323\\;64567;434553;\\", ';'), ';', "") ==
           "12\\\\323\\;64567;434553");
    VERIFY(uxs::pack_strings(uxs::unpack_strings("12\\\\323\\;64567;434553;;", ';'), ';', "") ==
           "12\\\\323\\;64567;434553;;");
    return 0;
}

int test_string_alg_4() {
    VERIFY(uxs::trim_string("asdf") == "asdf");
    VERIFY(uxs::trim_string("   asdf") == "asdf");
    VERIFY(uxs::trim_string("   asdf  ") == "asdf");
    return 0;
}

int test_string_alg_5() {
    VERIFY(uxs::encode_escapes("1234\\467;;", "\\;", "\\;") == "1234\\\\467\\;\\;");
    VERIFY(uxs::decode_escapes("1234\\\\467\\;\\;", "", "") == "1234\\467;;");
    VERIFY(uxs::decode_escapes("1234\\\\467\\;\\;\\", "", "") == "1234\\467;;");
    VERIFY(uxs::decode_escapes("1234\\\\467\\;\\;\\", "", "") == "1234\\467;;");
    VERIFY(uxs::decode_escapes("\\n\\n1234\\\\467\\;\\;\\", "\n", "n") == "\n\n1234\\467;;");
    return 0;
}

int test_string_alg_6() {
    VERIFY(uxs::replace_strings("1234***2345***678", uxs::sfind("***"), "abcdef") == "1234abcdef2345abcdef678");
    VERIFY(uxs::replace_strings("1234***2345***678***", uxs::sfind("***"), "abcdef") ==
           "1234abcdef2345abcdef678abcdef");
    VERIFY(uxs::replace_strings("***1234***2345***678***", uxs::sfind("***"), "abcdef") ==
           "abcdef1234abcdef2345abcdef678abcdef");
    return 0;
}

int test_string_alg_7() {
    VERIFY(uxs::from_utf8_to_wide(
               "\xD0\x94\xD0\xBE\xD0\xB1\xD1\x80\xD1\x8B\xD0\xB9\x20\xD0\xB4\xD0\xB5\xD0\xBD\xD1\x8C\x21") ==
           L"\x0414\x043e\x0431\x0440\x044b\x0439\x0020\x0434\x0435\x043d\x044c\x0021");
    VERIFY(uxs::from_wide_to_utf8(L"\x0414\x043e\x0431\x0440\x044b\x0439\x0020\x0434\x0435\x043d\x044c\x0021") ==
           "\xD0\x94\xD0\xBE\xD0\xB1\xD1\x80\xD1\x8B\xD0\xB9\x20\xD0\xB4\xD0\xB5\xD0\xBD\xD1\x8C\x21");
    VERIFY(uxs::from_utf8_to_wide("\xE4\xB8\x8B\xE5\x8D\x88\xE5\xA5\xBD") == L"\x4e0b\x5348\x597d");
    VERIFY(uxs::from_wide_to_utf8(L"\x4e0b\x5348\x597d") == "\xE4\xB8\x8B\xE5\x8D\x88\xE5\xA5\xBD");
    return 0;
}

int test_string_alg_8() {
    std::string s1;
    std::string_view s2;
    bool r = s1 == s1 || s2 == s2 || s1 == s2 || s2 == s1;
    (void)r;
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "string algorithm", test_string_alg_0);
ADD_TEST_CASE("", "string algorithm", test_string_alg_1);
ADD_TEST_CASE("", "string algorithm", test_string_alg_2);
ADD_TEST_CASE("", "string algorithm", test_string_alg_3);
ADD_TEST_CASE("", "string algorithm", test_string_alg_4);
ADD_TEST_CASE("", "string algorithm", test_string_alg_5);
ADD_TEST_CASE("", "string algorithm", test_string_alg_6);
ADD_TEST_CASE("", "string algorithm", test_string_alg_7);
ADD_TEST_CASE("", "string algorithm", test_string_alg_8);
