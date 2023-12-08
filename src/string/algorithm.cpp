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

int test_string_view() {
    VERIFY(std::string_view{"hello*********"}.find('h') == 0);
    VERIFY(std::string_view{"hello*********"}.find('h', 0) == 0);
    VERIFY(std::string_view{"hello*********"}.find('h', 1) == std::string::npos);
    VERIFY(std::string_view{"hello*********"}.find('l', 0) == 2);
    VERIFY(std::string_view{"hello*********"}.find('l', 1) == 2);
    VERIFY(std::string_view{"hello*********"}.find('l', 2) == 2);
    VERIFY(std::string_view{"hello*********"}.find('l', 3) == 3);
    VERIFY(std::string_view{"hello********1"}.find('l', 4) == std::string::npos);
    VERIFY(std::string_view{"hello********1"}.find('1', 13) == 13);
    VERIFY(std::string_view{"hello********1"}.find('1', 14) == std::string::npos);
    VERIFY(std::string_view{"hello********1"}.find('2') == std::string::npos);

    VERIFY(std::string_view{"hello*********"}.find("hello") == 0);
    VERIFY(std::string_view{"hello*********"}.find("hello", 0) == 0);
    VERIFY(std::string_view{"hello*********"}.find("hello", 1) == std::string::npos);
    VERIFY(std::string_view{"hello**hello**"}.find("hello", 1) == 7);
    VERIFY(std::string_view{"hello****hello"}.find("hello", 1) == 9);
    VERIFY(std::string_view{"hello****hello"}.find("hello", 10) == std::string::npos);
    VERIFY(std::string_view{"hello****hello"}.find("hello", 14) == std::string::npos);
    VERIFY(std::string_view{"hello****hello"}.find("") == 0);
    VERIFY(std::string_view{"hello****hello"}.find("", 7) == 7);
    VERIFY(std::string_view{"hello****hello"}.find("", 14) == 14);
    VERIFY(std::string_view{"hello****hello"}.find("", 15) == std::string::npos);
    VERIFY(std::string_view{""}.find("") == 0);
    VERIFY(std::string_view{""}.find("", 1) == std::string::npos);
    VERIFY(std::string_view{""}.find("hello") == std::string::npos);
    VERIFY(std::string_view{"hell"}.find("hello") == std::string::npos);

    VERIFY(std::string_view{"*********hello"}.rfind('o') == 13);
    VERIFY(std::string_view{"*********hello"}.rfind('o', 13) == 13);
    VERIFY(std::string_view{"*********hello"}.rfind('o', 12) == std::string::npos);
    VERIFY(std::string_view{"*********hello"}.rfind('l', 13) == 12);
    VERIFY(std::string_view{"*********hello"}.rfind('l', 12) == 12);
    VERIFY(std::string_view{"*********hello"}.rfind('l', 11) == 11);
    VERIFY(std::string_view{"*********hello"}.rfind('l', 10) == std::string::npos);
    VERIFY(std::string_view{"1********hello"}.rfind('1') == 0);
    VERIFY(std::string_view{"1********hello"}.rfind('1', 0) == 0);
    VERIFY(std::string_view{"*1*******hello"}.rfind('1', 0) == std::string::npos);
    VERIFY(std::string_view{"1********hello"}.rfind('2') == std::string::npos);

    VERIFY(std::string_view{"*********hello"}.rfind("hello") == 9);
    VERIFY(std::string_view{"*********hello"}.rfind("hello", 13) == 9);
    VERIFY(std::string_view{"*********hello"}.rfind("hello", 9) == 9);
    VERIFY(std::string_view{"*********hello"}.rfind("hello", 8) == std::string::npos);
    VERIFY(std::string_view{"**hello**hello"}.rfind("hello", 8) == 2);
    VERIFY(std::string_view{"**hello**hello"}.rfind("hello", 1) == std::string::npos);
    VERIFY(std::string_view{"hello****hello"}.rfind("hello", 8) == 0);
    VERIFY(std::string_view{"hello****hello"}.rfind("hello", 1) == 0);
    VERIFY(std::string_view{"hello****hello"}.rfind("hello", 0) == 0);
    VERIFY(std::string_view{"hello****hello"}.rfind("") == 14);
    VERIFY(std::string_view{"hello****hello"}.rfind("", 7) == 7);
    VERIFY(std::string_view{"hello****hello"}.rfind("", 14) == 14);
    VERIFY(std::string_view{"hello****hello"}.rfind("", 15) == 14);
    VERIFY(std::string_view{""}.rfind("") == 0);
    VERIFY(std::string_view{""}.rfind("", 0) == 0);
    VERIFY(std::string_view{""}.rfind("hello") == std::string::npos);
    VERIFY(std::string_view{"hell"}.rfind("hello") == std::string::npos);

    return 0;
}

int test_sfinder() {
    std::string_view s1{"12\\34\\56"};
    VERIFY(uxs::sfinder('1')(s1.begin(), s1.end()) == std::make_pair(s1.begin(), s1.begin() + 1));
    VERIFY(uxs::sfinder('2')(s1.begin(), s1.end()) == std::make_pair(s1.begin() + 1, s1.begin() + 2));
    VERIFY(uxs::sfinder('3')(s1.begin(), s1.end()) == std::make_pair(s1.end(), s1.end()));
    VERIFY(uxs::sfinder('4')(s1.begin(), s1.end()) == std::make_pair(s1.begin() + 4, s1.begin() + 5));
    VERIFY(uxs::sfinder('5')(s1.begin(), s1.end()) == std::make_pair(s1.end(), s1.end()));
    VERIFY(uxs::sfinder('6')(s1.begin(), s1.end()) == std::make_pair(s1.begin() + 7, s1.begin() + 8));
    VERIFY(uxs::sfinder('7')(s1.begin(), s1.end()) == std::make_pair(s1.end(), s1.end()));

    VERIFY(uxs::rsfinder('1')(s1.begin(), s1.end()) == std::make_pair(s1.begin(), s1.begin() + 1));
    VERIFY(uxs::rsfinder('2')(s1.begin(), s1.end()) == std::make_pair(s1.begin() + 1, s1.begin() + 2));
    VERIFY(uxs::rsfinder('3')(s1.begin(), s1.end()) == std::make_pair(s1.begin(), s1.begin()));
    VERIFY(uxs::rsfinder('4')(s1.begin(), s1.end()) == std::make_pair(s1.begin() + 4, s1.begin() + 5));
    VERIFY(uxs::rsfinder('5')(s1.begin(), s1.end()) == std::make_pair(s1.begin(), s1.begin()));
    VERIFY(uxs::rsfinder('6')(s1.begin(), s1.end()) == std::make_pair(s1.begin() + 7, s1.begin() + 8));
    VERIFY(uxs::rsfinder('7')(s1.begin(), s1.end()) == std::make_pair(s1.begin(), s1.begin()));

    std::string_view s2{"hello**hello**"};
    VERIFY(uxs::sfinder("hello")(s2.begin(), s2.end()) == std::make_pair(s2.begin(), s2.begin() + 5));
    VERIFY(uxs::sfinder("hello")(s2.begin() + 1, s2.end()) == std::make_pair(s2.begin() + 7, s2.begin() + 12));
    VERIFY(uxs::sfinder("hello")(s2.begin() + 1, s2.begin() + 12) == std::make_pair(s2.begin() + 7, s2.begin() + 12));
    VERIFY(uxs::sfinder("hello")(s2.begin() + 1, s2.begin() + 11) == std::make_pair(s2.begin() + 11, s2.begin() + 11));
    VERIFY(uxs::sfinder("hello")(s2.begin(), s2.begin() + 4) == std::make_pair(s2.begin() + 4, s2.begin() + 4));
    VERIFY(uxs::sfinder("")(s2.begin(), s2.end()) == std::make_pair(s2.begin(), s2.begin()));
    VERIFY(uxs::sfinder("###")(s2.begin(), s2.end()) == std::make_pair(s2.end(), s2.end()));
    VERIFY(uxs::sfinder("")(s2.begin(), s2.begin()) == std::make_pair(s2.begin(), s2.begin()));
    VERIFY(uxs::sfinder("###")(s2.begin(), s2.begin()) == std::make_pair(s2.begin(), s2.begin()));

    VERIFY(uxs::rsfinder("hello")(s2.begin(), s2.end()) == std::make_pair(s2.begin() + 7, s2.begin() + 12));
    VERIFY(uxs::rsfinder("hello")(s2.begin(), s2.end() - 2) == std::make_pair(s2.begin() + 7, s2.begin() + 12));
    VERIFY(uxs::rsfinder("hello")(s2.begin(), s2.end() - 3) == std::make_pair(s2.begin(), s2.begin() + 5));
    VERIFY(uxs::rsfinder("")(s2.begin(), s2.end()) == std::make_pair(s2.end(), s2.end()));
    VERIFY(uxs::rsfinder("###")(s2.begin(), s2.end()) == std::make_pair(s2.begin(), s2.begin()));
    VERIFY(uxs::rsfinder("")(s2.begin(), s2.begin()) == std::make_pair(s2.begin(), s2.begin()));
    VERIFY(uxs::rsfinder("###")(s2.begin(), s2.begin()) == std::make_pair(s2.begin(), s2.begin()));

    return 0;
}

int test_string_alg_0() {
    CHECK(uxs::split_string("", uxs::sfinder(',')), {""});
    CHECK(uxs::split_string(",", uxs::sfinder(',')), {"", ""});
    CHECK(uxs::split_string("1234,", uxs::sfinder(',')), {"1234", ""});
    CHECK(uxs::split_string("1234,34646", uxs::sfinder(',')), {"1234", "34646"});
    CHECK(uxs::split_string(",1234,34646,", uxs::sfinder(',')), {"", "1234", "34646", ""});
    CHECK(uxs::split_string(",1234,34646,,", uxs::sfinder(',')), {"", "1234", "34646", "", ""});
    CHECK(uxs::split_string(",1234\\,34646,,", uxs::sfinder(',')), {"", "1234\\,34646", "", ""});
    CHECK(uxs::split_string(",1234,34646,\\,", uxs::sfinder(',')), {"", "1234", "34646", "\\,"});
    CHECK(uxs::split_string(",1234,34646,,\\", uxs::sfinder(',')), {"", "1234", "34646", "", "\\"});

    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfinder(','), 0, 0) == "");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfinder(','), 1, 1) == "1234");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfinder(','), 2, 2) == "34646");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfinder(','), 3, 3) == "");

    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfinder(','), 1, 2) == "1234,34646");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfinder(','), 0, 2) == ",1234,34646");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfinder(','), 2) == "34646,,");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfinder(','), 4, 4) == "");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::sfinder(','), 10) == "");
    VERIFY(uxs::string_section<uxs::split_opts::skip_empty>(",1234,,,34646,,", uxs::sfinder(','), 0, 1) ==
           "1234,,,34646");

    VERIFY(uxs::string_section(",1234,34646,,124", uxs::rsfinder(','), 0) == "124");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfinder(','), 0) == "");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfinder(','), 1, 1) == "");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfinder(','), 2, 2) == "34646");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfinder(','), 2, 1) == "34646,");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfinder(','), 2) == "34646,,");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfinder(','), 3) == "1234,34646,,");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfinder(','), 10, 3) == ",1234");
    VERIFY(uxs::string_section(",1234,34646,,", uxs::rsfinder(','), 10, 10) == "");
    VERIFY(uxs::string_section<uxs::split_opts::skip_empty>(",1234,,,34646,,", uxs::rsfinder(','), 1) ==
           "1234,,,34646");
    VERIFY(uxs::string_section(",1234\\,34646,,", uxs::rsfinder(','), 3) == ",1234\\,34646,,");

    std::string csv = "forename,middlename,surname,phone";
    std::string path = "/usr/local/bin/myapp";  // First field is empty
    std::string data = "forename**middlename**surname**phone";
    std::string line = "forename\tmiddlename  surname \t \t phone";

    VERIFY(uxs::string_section(csv, uxs::sfinder(','), 2, 2) == "surname");
    VERIFY(uxs::string_section(path, uxs::sfinder('/'), 3, 4) == "bin/myapp");
    VERIFY(uxs::string_section<uxs::split_opts::skip_empty>(path, uxs::sfinder('/'), 3, 3) == "myapp");

    VERIFY(uxs::string_section(csv, uxs::rsfinder(','), 2, 1) == "middlename,surname");
    VERIFY(uxs::string_section(path, uxs::rsfinder('/'), 0) == "myapp");

    VERIFY(uxs::string_section(data, uxs::sfinder("**"), 2, 2) == "surname");
    VERIFY(uxs::string_section(data, uxs::rsfinder("**"), 2, 1) == "middlename**surname");

    static const std::regex sep("[ \\t]+");
    VERIFY(uxs::string_section(line, uxs::sfinder(sep), 2, 2) == "surname");
    VERIFY(uxs::string_section(line, uxs::rsfinder(sep), 2, 1) == "middlename  surname");
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
    VERIFY(uxs::replace_strings("1234***2345***678", uxs::sfinder("***"), "abcdef") == "1234abcdef2345abcdef678");
    VERIFY(uxs::replace_strings("1234***2345***678***", uxs::sfinder("***"), "abcdef") ==
           "1234abcdef2345abcdef678abcdef");
    VERIFY(uxs::replace_strings("***1234***2345***678***", uxs::sfinder("***"), "abcdef") ==
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
    VERIFY(uxs::from_utf8_to_wide("hello, world") == L"hello, world");
    VERIFY(uxs::from_wide_to_utf8(L"hello, world") == "hello, world");
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

ADD_TEST_CASE("", "string algorithm", test_string_view);
ADD_TEST_CASE("", "string algorithm", test_sfinder);
ADD_TEST_CASE("", "string algorithm", test_string_alg_0);
ADD_TEST_CASE("", "string algorithm", test_string_alg_1);
ADD_TEST_CASE("", "string algorithm", test_string_alg_2);
ADD_TEST_CASE("", "string algorithm", test_string_alg_3);
ADD_TEST_CASE("", "string algorithm", test_string_alg_4);
ADD_TEST_CASE("", "string algorithm", test_string_alg_5);
ADD_TEST_CASE("", "string algorithm", test_string_alg_6);
ADD_TEST_CASE("", "string algorithm", test_string_alg_7);
ADD_TEST_CASE("", "string algorithm", test_string_alg_8);
