#if defined(_MSC_VER)
#    define _CRT_SECURE_NO_WARNINGS
#endif

#include "fmt/format.h"

#include "test_suite.h"

#include "uxs/format_ranges.h"
#include "uxs/guid.h"
#include "uxs/io/oflatbuf.h"

#if __cplusplus >= 202002L && UXS_HAS_INCLUDE(<format>)
#    include <format>
#    define has_cpp_lib_format 1
#endif

#define MUST_THROW(x) \
    try { \
        x; \
        VERIFY(false); \
    } catch (const uxs::format_error&) {}

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <locale>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using namespace uxs_test_suite;

static_assert(uxs::sfmt::arg_type_index<bool, char>::value == uxs::sfmt::type_index::boolean, "");
static_assert(uxs::sfmt::arg_type_index<char, char>::value == uxs::sfmt::type_index::character, "");
static_assert(uxs::sfmt::arg_type_index<wchar_t, char>::value == uxs::sfmt::type_index::custom, "");
static_assert(uxs::sfmt::arg_type_index<signed char, char>::value == uxs::sfmt::type_index::integer, "");
static_assert(uxs::sfmt::arg_type_index<signed short, char>::value == uxs::sfmt::type_index::integer, "");
static_assert(uxs::sfmt::arg_type_index<signed, char>::value == uxs::sfmt::type_index::integer, "");
static_assert(sizeof(signed long) == sizeof(signed long long) ?
                  uxs::sfmt::arg_type_index<signed long, char>::value == uxs::sfmt::type_index::long_integer :
                  uxs::sfmt::arg_type_index<signed long, char>::value == uxs::sfmt::type_index::integer,
              "");
static_assert(uxs::sfmt::arg_type_index<signed long long, char>::value == uxs::sfmt::type_index::long_integer, "");
static_assert(uxs::sfmt::arg_type_index<unsigned char, char>::value == uxs::sfmt::type_index::unsigned_integer, "");
static_assert(uxs::sfmt::arg_type_index<unsigned short, char>::value == uxs::sfmt::type_index::unsigned_integer, "");
static_assert(uxs::sfmt::arg_type_index<unsigned, char>::value == uxs::sfmt::type_index::unsigned_integer, "");
static_assert(sizeof(unsigned long) == sizeof(unsigned long long) ?
                  uxs::sfmt::arg_type_index<unsigned long, char>::value == uxs::sfmt::type_index::unsigned_long_integer :
                  uxs::sfmt::arg_type_index<unsigned long, char>::value == uxs::sfmt::type_index::unsigned_integer,
              "");
static_assert(uxs::sfmt::arg_type_index<unsigned long long, char>::value == uxs::sfmt::type_index::unsigned_long_integer,
              "");
static_assert(uxs::sfmt::arg_type_index<float, char>::value == uxs::sfmt::type_index::single_precision, "");
static_assert(uxs::sfmt::arg_type_index<double, char>::value == uxs::sfmt::type_index::double_precision, "");
static_assert(uxs::sfmt::arg_type_index<long double, char>::value == uxs::sfmt::type_index::long_double_precision, "");
static_assert(uxs::sfmt::arg_type_index<uxs::guid, char>::value == uxs::sfmt::type_index::custom, "");

static_assert(uxs::sfmt::arg_type_index<bool, wchar_t>::value == uxs::sfmt::type_index::boolean, "");
static_assert(uxs::sfmt::arg_type_index<char, wchar_t>::value == uxs::sfmt::type_index::character, "");
static_assert(uxs::sfmt::arg_type_index<wchar_t, wchar_t>::value == uxs::sfmt::type_index::character, "");
static_assert(uxs::sfmt::arg_type_index<signed char, wchar_t>::value == uxs::sfmt::type_index::integer, "");
static_assert(uxs::sfmt::arg_type_index<signed short, wchar_t>::value == uxs::sfmt::type_index::integer, "");
static_assert(uxs::sfmt::arg_type_index<signed, wchar_t>::value == uxs::sfmt::type_index::integer, "");
static_assert(sizeof(signed long) == sizeof(signed long long) ?
                  uxs::sfmt::arg_type_index<signed long, wchar_t>::value == uxs::sfmt::type_index::long_integer :
                  uxs::sfmt::arg_type_index<signed long, wchar_t>::value == uxs::sfmt::type_index::integer,
              "");
static_assert(uxs::sfmt::arg_type_index<signed long long, wchar_t>::value == uxs::sfmt::type_index::long_integer, "");
static_assert(uxs::sfmt::arg_type_index<unsigned char, wchar_t>::value == uxs::sfmt::type_index::unsigned_integer, "");
static_assert(uxs::sfmt::arg_type_index<unsigned short, wchar_t>::value == uxs::sfmt::type_index::unsigned_integer, "");
static_assert(uxs::sfmt::arg_type_index<unsigned, wchar_t>::value == uxs::sfmt::type_index::unsigned_integer, "");
static_assert(sizeof(unsigned long) == sizeof(unsigned long long) ?
                  uxs::sfmt::arg_type_index<unsigned long, wchar_t>::value ==
                      uxs::sfmt::type_index::unsigned_long_integer :
                  uxs::sfmt::arg_type_index<unsigned long, wchar_t>::value == uxs::sfmt::type_index::unsigned_integer,
              "");
static_assert(uxs::sfmt::arg_type_index<unsigned long long, wchar_t>::value ==
                  uxs::sfmt::type_index::unsigned_long_integer,
              "");
static_assert(uxs::sfmt::arg_type_index<float, wchar_t>::value == uxs::sfmt::type_index::single_precision, "");
static_assert(uxs::sfmt::arg_type_index<double, wchar_t>::value == uxs::sfmt::type_index::double_precision, "");
static_assert(uxs::sfmt::arg_type_index<long double, wchar_t>::value == uxs::sfmt::type_index::long_double_precision,
              "");
static_assert(uxs::sfmt::arg_type_index<uxs::guid, wchar_t>::value == uxs::sfmt::type_index::custom, "");

#if defined(_MSC_VER)
static_assert(uxs::sfmt::arg_type_index<unsigned __int64, char>::value == uxs::sfmt::type_index::unsigned_long_integer,
              "");
static_assert(uxs::sfmt::arg_type_index<signed __int64, char>::value == uxs::sfmt::type_index::long_integer, "");
static_assert(uxs::sfmt::arg_type_index<unsigned __int64, wchar_t>::value ==
                  uxs::sfmt::type_index::unsigned_long_integer,
              "");
static_assert(uxs::sfmt::arg_type_index<signed __int64, wchar_t>::value == uxs::sfmt::type_index::long_integer, "");
#endif  // defined(_MSC_VER)

static_assert(uxs::format_arg_type_index<uxs::format_context, bool>::value == uxs::sfmt::type_index::boolean, "");
static_assert(uxs::format_arg_type_index<uxs::format_context, char>::value == uxs::sfmt::type_index::character, "");
static_assert(uxs::format_arg_type_index<uxs::wformat_context, wchar_t>::value == uxs::sfmt::type_index::character, "");
static_assert(uxs::format_arg_type_index<uxs::format_context, int32_t>::value == uxs::sfmt::type_index::integer, "");
static_assert(uxs::format_arg_type_index<uxs::format_context, int64_t>::value == uxs::sfmt::type_index::long_integer,
              "");
static_assert(uxs::format_arg_type_index<uxs::format_context, uint32_t>::value ==
                  uxs::sfmt::type_index::unsigned_integer,
              "");
static_assert(uxs::format_arg_type_index<uxs::format_context, uint64_t>::value ==
                  uxs::sfmt::type_index::unsigned_long_integer,
              "");
static_assert(uxs::format_arg_type_index<uxs::format_context, float>::value == uxs::sfmt::type_index::single_precision,
              "");
static_assert(uxs::format_arg_type_index<uxs::format_context, double>::value == uxs::sfmt::type_index::double_precision,
              "");
static_assert(uxs::format_arg_type_index<uxs::format_context, long double>::value ==
                  uxs::sfmt::type_index::long_double_precision,
              "");
static_assert(uxs::format_arg_type_index<uxs::format_context, const void*>::value == uxs::sfmt::type_index::pointer, "");
static_assert(uxs::format_arg_type_index<uxs::format_context, const char*>::value == uxs::sfmt::type_index::z_string,
              "");
static_assert(uxs::format_arg_type_index<uxs::wformat_context, const wchar_t*>::value == uxs::sfmt::type_index::z_string,
              "");
static_assert(uxs::format_arg_type_index<uxs::format_context, std::string_view>::value == uxs::sfmt::type_index::string,
              "");
static_assert(uxs::format_arg_type_index<uxs::wformat_context, std::wstring_view>::value ==
                  uxs::sfmt::type_index::string,
              "");
static_assert(uxs::format_arg_type_index<uxs::format_context, uxs::basic_format_arg<uxs::format_context>::handle>::value ==
                  uxs::sfmt::type_index::custom,
              "");
static_assert(
    uxs::format_arg_type_index<uxs::wformat_context, uxs::basic_format_arg<uxs::wformat_context>::handle>::value ==
        uxs::sfmt::type_index::custom,
    "");

static_assert(uxs::formattable<bool, char>::value, "");
static_assert(uxs::formattable<char, char>::value, "");
static_assert(!uxs::formattable<wchar_t, char>::value, "");
static_assert(uxs::formattable<const char*, char>::value, "");
static_assert(!uxs::formattable<const wchar_t*, char>::value, "");
static_assert(uxs::formattable<std::string_view, char>::value, "");
static_assert(!uxs::formattable<std::wstring_view, char>::value, "");
static_assert(uxs::formattable<int32_t, char>::value, "");
static_assert(uxs::formattable<int64_t, char>::value, "");
static_assert(uxs::formattable<uint32_t, char>::value, "");
static_assert(uxs::formattable<uint64_t, char>::value, "");
static_assert(uxs::formattable<float, char>::value, "");
static_assert(uxs::formattable<double, char>::value, "");
static_assert(uxs::formattable<long double, char>::value, "");
static_assert(uxs::formattable<const void*, char>::value, "");
static_assert(uxs::formattable<uxs::guid, char>::value, "");

static_assert(uxs::formattable<bool, wchar_t>::value, "");
static_assert(uxs::formattable<char, wchar_t>::value, "");
static_assert(uxs::formattable<wchar_t, wchar_t>::value, "");
static_assert(!uxs::formattable<const char*, wchar_t>::value, "");
static_assert(uxs::formattable<const wchar_t*, wchar_t>::value, "");
static_assert(uxs::formattable<std::string_view, wchar_t>::value, "");
static_assert(uxs::formattable<std::wstring_view, wchar_t>::value, "");
static_assert(uxs::formattable<int32_t, wchar_t>::value, "");
static_assert(uxs::formattable<int64_t, wchar_t>::value, "");
static_assert(uxs::formattable<uint32_t, wchar_t>::value, "");
static_assert(uxs::formattable<uint64_t, wchar_t>::value, "");
static_assert(uxs::formattable<float, wchar_t>::value, "");
static_assert(uxs::formattable<double, wchar_t>::value, "");
static_assert(uxs::formattable<long double, wchar_t>::value, "");
static_assert(uxs::formattable<const void*, wchar_t>::value, "");
static_assert(uxs::formattable<uxs::guid, wchar_t>::value, "");

namespace {

template<typename CharT>
bool str_equal(const CharT* lhs, std::string_view rhs) {
    size_t len = 0;
    for (const CharT* p = lhs; *p; ++p, ++len) {}
    return len == rhs.size() && std::equal(rhs.begin(), rhs.end(), lhs);
}

int test_string_format_0() {
    struct grouping : std::numpunct<char> {
        char do_decimal_point() const override { return '_'; }
    };

    struct wgrouping : std::numpunct<wchar_t> {
        wchar_t do_decimal_point() const override { return L'_'; }
    };

    std::locale loc{std::locale::classic(), new grouping};
    std::locale wloc{std::locale::classic(), new wgrouping};

    VERIFY(uxs::format("The answer is {}.", 4.2) == "The answer is 4.2.");
    VERIFY(uxs::format(L"The answer is {}.", 4.2) == L"The answer is 4.2.");
    VERIFY(uxs::format(loc, "The answer is {:L}.", 4.2) == "The answer is 4_2.");
    VERIFY(uxs::format(wloc, L"The answer is {:L}.", 4.2) == L"The answer is 4_2.");

    char buf[256];
    wchar_t wbuf[256];
    int ibuf[245];

    *uxs::format_to(buf, "The answer is {}.", 4.2) = '\0';
    VERIFY(str_equal(buf, "The answer is 4.2."));
    *uxs::format_to(wbuf, "The answer is {}.", 4.2) = '\0';
    VERIFY(str_equal(wbuf, "The answer is 4.2."));

    *uxs::format_to(wbuf, L"The answer is {}.", 4.2) = '\0';
    VERIFY(str_equal(wbuf, "The answer is 4.2."));
    *uxs::format_to(ibuf, L"The answer is {}.", 4.2) = '\0';
    VERIFY(str_equal(ibuf, "The answer is 4.2."));

    *uxs::format_to(buf, loc, "The answer is {:L}.", 4.2) = '\0';
    VERIFY(str_equal(buf, "The answer is 4_2."));
    *uxs::format_to(wbuf, loc, "The answer is {:L}.", 4.2) = '\0';
    VERIFY(str_equal(wbuf, "The answer is 4_2."));

    *uxs::format_to(wbuf, wloc, L"The answer is {:L}.", 4.2) = '\0';
    VERIFY(str_equal(wbuf, "The answer is 4_2."));
    *uxs::format_to(ibuf, wloc, L"The answer is {:L}.", 4.2) = '\0';
    VERIFY(str_equal(ibuf, "The answer is 4_2."));

    const double pi = 3.14159265358979323846;

    {
        auto result = uxs::format_to_n(buf, 5, "{}", pi);
        *result.out = '\0';
        VERIFY(result.size == 17 && str_equal(buf, "3.141"));
    }
    {
        auto result = uxs::format_to_n(wbuf, 5, "{}", pi);
        *result.out = '\0';
        VERIFY(result.size == 17 && str_equal(wbuf, "3.141"));
    }

    {
        auto result = uxs::format_to_n(wbuf, 5, L"{}", pi);
        *result.out = '\0';
        VERIFY(result.size == 17 && str_equal(wbuf, "3.141"));
    }
    {
        auto result = uxs::format_to_n(ibuf, 5, L"{}", pi);
        *result.out = '\0';
        VERIFY(result.size == 17 && str_equal(ibuf, "3.141"));
    }

    {
        auto result = uxs::format_to_n(buf, 5, loc, "{:L}", pi);
        *result.out = '\0';
        VERIFY(result.size == 17 && str_equal(buf, "3_141"));
    }
    {
        auto result = uxs::format_to_n(wbuf, 5, loc, "{:L}", pi);
        *result.out = '\0';
        VERIFY(result.size == 17 && str_equal(wbuf, "3_141"));
    }

    {
        auto result = uxs::format_to_n(wbuf, 5, wloc, L"{:L}", pi);
        *result.out = '\0';
        VERIFY(result.size == 17 && str_equal(wbuf, "3_141"));
    }
    {
        auto result = uxs::format_to_n(ibuf, 5, wloc, L"{:L}", pi);
        *result.out = '\0';
        VERIFY(result.size == 17 && str_equal(ibuf, "3_141"));
    }

    {
        auto result = uxs::format_to_n(wbuf, 20, wloc, L"{:L}", pi);
        *result.out = '\0';
        VERIFY(result.size == 17 && str_equal(wbuf, "3_141592653589793"));
    }
    {
        auto result = uxs::format_to_n(ibuf, 20, wloc, L"{:L}", pi);
        *result.out = '\0';
        VERIFY(result.size == 17 && str_equal(ibuf, "3_141592653589793"));
    }

    uxs::oflatbuf ss;
    uxs::print(ss, "The answer is {}.", 4.2).flush();
    VERIFY(std::string_view(ss.data(), ss.size()) == "The answer is 4.2.");

    uxs::woflatbuf wss;
    uxs::print(wss, L"The answer is {}.", 4.2).flush();
    VERIFY(std::wstring_view(wss.data(), wss.size()) == L"The answer is 4.2.");

    ss.truncate(0);
    uxs::print(ss, loc, "The answer is {:L}.", 4.2).flush();
    VERIFY(std::string_view(ss.data(), ss.size()) == "The answer is 4_2.");

    wss.truncate(0);
    uxs::print(wss, wloc, L"The answer is {:L}.", 4.2).flush();
    VERIFY(std::wstring_view(wss.data(), wss.size()) == L"The answer is 4_2.");

    ss.truncate(0);
    uxs::println(ss, "The answer is {}.", 4.2);
    VERIFY(std::string_view(ss.data(), ss.size()) == "The answer is 4.2.\n");

    wss.truncate(0);
    uxs::println(wss, L"The answer is {}.", 4.2);
    VERIFY(std::wstring_view(wss.data(), wss.size()) == L"The answer is 4.2.\n");

    ss.truncate(0);
    uxs::println(ss, loc, "The answer is {:L}.", 4.2);
    VERIFY(std::string_view(ss.data(), ss.size()) == "The answer is 4_2.\n");

    wss.truncate(0);
    uxs::println(wss, wloc, L"The answer is {:L}.", 4.2);
    VERIFY(std::wstring_view(wss.data(), wss.size()) == L"The answer is 4_2.\n");

    VERIFY(uxs::format("{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234, 42, 3.13, "str", (void*)1000, 'X') ==
           "1.2340000000:0042:+3.13:str:0x3e8:X:%\n");
    return 0;
}

int test_string_format_1() {
    VERIFY(uxs::format("abcdefghi") == "abcdefghi");
    VERIFY(uxs::format("{}abcdefghi", "A") == "Aabcdefghi");
    VERIFY(uxs::format("{}abcdefghi{}", "A", "B") == "AabcdefghiB");
    VERIFY(uxs::format("{}{}abcdefghi{}", "A", "C", "B") == "ACabcdefghiB");

    VERIFY(uxs::format("{}{}{}{}", "1", "2", "3", "4") == "1234");
    VERIFY(uxs::format("{:*>8}", 1) == "*******1");
    VERIFY(uxs::format("{:*>8.2f}", 2.34) == "****2.34");
    VERIFY(uxs::format("{:.2f}", 2.34) == "2.34");

    VERIFY(uxs::format("{3}{2}{1}{0}", "1", "2", "3", "4") == "4321");
    VERIFY(uxs::format("{2}{1}{1}{0}", "1", "2", "3") == "3221");
    VERIFY(uxs::format("{2:=^{1}.{0}f}", 3, 7, 1.23) == "=1.230=");

    const char* s1 = "world";
    char* ncs1 = const_cast<char*>("world");
    std::string_view s2 = "world";
    std::string s3 = "world";
    VERIFY(uxs::format("Hello, {}!", "world") == "Hello, world!");
    VERIFY(uxs::format("Hello, {}!", s1) == "Hello, world!");
    VERIFY(uxs::format("Hello, {}!", ncs1) == "Hello, world!");
    VERIFY(uxs::format("Hello, {}!", s2) == "Hello, world!");
    VERIFY(uxs::format("Hello, {}!", s3) == "Hello, world!");
    VERIFY(uxs::format("Hello, {}!", s3 + "123") == "Hello, world123!");
    VERIFY(uxs::format("Hello, {}!", (void*)1000) == "Hello, 0x3e8!");
    VERIFY(uxs::format("Hello, {}!", (const void*)1000) == "Hello, 0x3e8!");
    VERIFY(uxs::format("Hello, {}!", (const int*)1000) == "Hello, 0x3e8!");

    VERIFY(uxs::format("{:.^15}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(uxs::format("{:.^15.6}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(uxs::format("{:.^15.8}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(uxs::format("{:.^15.4}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           ".....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2......");
    VERIFY(uxs::format("{:.^15.0}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") == "...............");
    VERIFY(uxs::format("{:.6}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82");
    VERIFY(uxs::format("{:.4}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2");
    VERIFY(uxs::format("{:.0}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") == "");
    VERIFY(uxs::format("{:2.4}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2");

    VERIFY(uxs::format("{:.^15}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0....");
    VERIFY(uxs::format("{:.^15.6}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(uxs::format("{:.^15.7}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0....");
    VERIFY(uxs::format("{:.^15.8}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0....");

    {
        uxs::inline_dynbuffer s;
        uxs::basic_vformat(s, "{} {} {} {} {}", uxs::make_format_args(true, 'A', "hello", 100, 3.1415));
        VERIFY(std::string(s.data(), s.size()) == "true A hello 100 3.1415");
    }
    {
        std::string s;
        VERIFY(uxs::basic_vformat(s, "{} {} {} {} {}", uxs::make_format_args(true, 'A', "hello", 100, 3.1415)) ==
               "true A hello 100 3.1415");
    }
    {
        uxs::inline_dynbuffer s;
        uxs::basic_vformat(s, std::locale{}, "{} {} {} {} {}", uxs::make_format_args(true, 'A', "hello", 100, 3.1415));
        VERIFY(std::string(s.data(), s.size()) == "true A hello 100 3.1415");
    }
    {
        std::string s;
        VERIFY(uxs::basic_vformat(s, std::locale{}, "{} {} {} {} {}",
                                  uxs::make_format_args(true, 'A', "hello", 100, 3.1415)) == "true A hello 100 3.1415");
    }

    {
        uxs::inline_dynbuffer s;
        uxs::basic_vformat(s, "{} {} {} {} {}", uxs::make_format_args(true, 'A', "hello", 100, 3.1415));
        VERIFY(std::string(s.data(), s.size()) == "true A hello 100 3.1415");
    }
    {
        std::string s;
        VERIFY(uxs::basic_format(s, "{} {} {} {} {}", true, 'A', "hello", 100, 3.1415) == "true A hello 100 3.1415");
    }
    {
        uxs::inline_dynbuffer s;
        uxs::basic_format(s, std::locale{}, "{} {} {} {} {}", true, 'A', "hello", 100, 3.1415);
        VERIFY(std::string(s.data(), s.size()) == "true A hello 100 3.1415");
    }
    {
        std::string s;
        VERIFY(uxs::basic_format(s, std::locale{}, "{} {} {} {} {}", true, 'A', "hello", 100, 3.1415) ==
               "true A hello 100 3.1415");
    }

    return 0;
}

template<typename T>
const T& unmove(T&& x) {
    return x;
}

int test_string_format_2() {
    MUST_THROW((void)uxs::vformat("{", uxs::make_format_args(unmove(123))));
    MUST_THROW((void)uxs::vformat("}", uxs::make_format_args(unmove(123))));
    MUST_THROW((void)uxs::vformat("}{", uxs::make_format_args(unmove(123))));

    // -- no type specifier
    VERIFY(uxs::format("{}", true) == "true");  // boolean
    VERIFY(uxs::format("{:L}", true) == "true");
    MUST_THROW((void)uxs::vformat("{:+}", uxs::make_format_args(unmove(true))));
    MUST_THROW((void)uxs::vformat("{:-}", uxs::make_format_args(unmove(true))));
    MUST_THROW((void)uxs::vformat("{:0}", uxs::make_format_args(unmove(true))));
    MUST_THROW((void)uxs::vformat("{:#}", uxs::make_format_args(unmove(true))));
    MUST_THROW((void)uxs::vformat("{:.3}", uxs::make_format_args(unmove(true))));
    VERIFY(uxs::format("{}", 'A') == "A");  // character
    VERIFY(uxs::format("{:L}", 'A') == "A");
    MUST_THROW((void)uxs::vformat("{:+}", uxs::make_format_args(unmove('A'))));
    MUST_THROW((void)uxs::vformat("{:-}", uxs::make_format_args(unmove('A'))));
    MUST_THROW((void)uxs::vformat("{:0}", uxs::make_format_args(unmove('A'))));
    MUST_THROW((void)uxs::vformat("{:#}", uxs::make_format_args(unmove('A'))));
    MUST_THROW((void)uxs::vformat("{:.3}", uxs::make_format_args(unmove('A'))));
    VERIFY(uxs::format("{}", 123) == "123");  // integer
    VERIFY(uxs::format("{:L}", 123) == "123");
    VERIFY(uxs::format("{:+}", 123) == "+123");
    VERIFY(uxs::format("{:-}", 123) == "123");
    VERIFY(uxs::format("{:0}", 123) == "123");
    VERIFY(uxs::format("{:#}", 123) == "123");
    MUST_THROW((void)uxs::vformat("{:.3}", uxs::make_format_args(unmove(123))));
    VERIFY(uxs::format("{}", 123u) == "123");  // unsigned integer
    VERIFY(uxs::format("{:L}", 123u) == "123");
    VERIFY(uxs::format("{:+}", 123u) == "+123");
    VERIFY(uxs::format("{:-}", 123u) == "123");
    VERIFY(uxs::format("{:0}", 123u) == "123");
    VERIFY(uxs::format("{:#}", 123u) == "123");
    MUST_THROW((void)uxs::vformat("{:.3}", uxs::make_format_args(unmove(123u))));
    VERIFY(uxs::format("{}", 123.) == "123");  // double
    VERIFY(uxs::format("{:L}", 123.) == "123");
    VERIFY(uxs::format("{:+}", 123.) == "+123");
    VERIFY(uxs::format("{:-}", 123.) == "123");
    VERIFY(uxs::format("{:0}", 123.) == "123");
    VERIFY(uxs::format("{:#}", 123.) == "123.");
    VERIFY(uxs::format("{:.3}", 123.) == "123");
    VERIFY(uxs::format("{}", reinterpret_cast<void*>(0x123)) == "0x123");  // pointer
    MUST_THROW((void)uxs::vformat("{:L}", uxs::make_format_args(unmove(reinterpret_cast<void*>(0x123)))));
    MUST_THROW((void)uxs::vformat("{:+}", uxs::make_format_args(unmove(reinterpret_cast<void*>(0x123)))));
    MUST_THROW((void)uxs::vformat("{:-}", uxs::make_format_args(unmove(reinterpret_cast<void*>(0x123)))));
    VERIFY(uxs::format("{:0}", reinterpret_cast<void*>(0x123)) == "0x123");
    MUST_THROW((void)uxs::vformat("{:#}", uxs::make_format_args(unmove(reinterpret_cast<void*>(0x123)))));
    MUST_THROW((void)uxs::vformat("{:.3}", uxs::make_format_args(unmove(reinterpret_cast<void*>(0x123)))));
    VERIFY(uxs::format("{}", "hello") == "hello");  // string
    MUST_THROW((void)uxs::vformat("{:L}", uxs::make_format_args(unmove("hello"))));
    MUST_THROW((void)uxs::vformat("{:+}", uxs::make_format_args(unmove("hello"))));
    MUST_THROW((void)uxs::vformat("{:-}", uxs::make_format_args(unmove("hello"))));
    MUST_THROW((void)uxs::vformat("{:0}", uxs::make_format_args(unmove("hello"))));
    MUST_THROW((void)uxs::vformat("{:#}", uxs::make_format_args(unmove("hello"))));
    VERIFY(uxs::format("{:.3}", "hello") == "hel");

    // -- integer specifier
    VERIFY(uxs::format("{:d}", true) == "1");  // boolean
    VERIFY(uxs::format("{:+d}", true) == "+1");
    VERIFY(uxs::format("{:-d}", true) == "1");
    VERIFY(uxs::format("{:0d}", true) == "1");
    VERIFY(uxs::format("{:#d}", true) == "1");
    MUST_THROW((void)uxs::vformat("{:.3d}", uxs::make_format_args(unmove(true))));
    VERIFY(uxs::format("{:d}", 'A') == "65");  // character
    VERIFY(uxs::format("{:+d}", 'A') == "+65");
    VERIFY(uxs::format("{:-d}", 'A') == "65");
    VERIFY(uxs::format("{:0d}", 'A') == "65");
    VERIFY(uxs::format("{:#d}", 'A') == "65");
    MUST_THROW((void)uxs::vformat("{:.3d}", uxs::make_format_args(unmove('A'))));
    VERIFY(uxs::format("{:d}", 123) == "123");  // integer
    VERIFY(uxs::format("{:+d}", 123) == "+123");
    VERIFY(uxs::format("{:-d}", 123) == "123");
    VERIFY(uxs::format("{:0d}", 123) == "123");
    VERIFY(uxs::format("{:#d}", 123) == "123");
    MUST_THROW((void)uxs::vformat("{:.3d}", uxs::make_format_args(unmove(123))));
    VERIFY(uxs::format("{:d}", 123u) == "123");  // unsigned integer
    VERIFY(uxs::format("{:+d}", 123u) == "+123");
    VERIFY(uxs::format("{:-d}", 123u) == "123");
    VERIFY(uxs::format("{:0d}", 123u) == "123");
    VERIFY(uxs::format("{:#d}", 123u) == "123");
    MUST_THROW((void)uxs::vformat("{:.3d}", uxs::make_format_args(unmove(123u))));
    MUST_THROW((void)uxs::vformat("{:d}", uxs::make_format_args(unmove(123.))));                            // double
    MUST_THROW((void)uxs::vformat("{:d}", uxs::make_format_args(unmove(reinterpret_cast<void*>(0x123)))));  // pointer
    MUST_THROW((void)uxs::vformat("{:d}", uxs::make_format_args(unmove("hello"))));                         // string

    // -- float specifier
    MUST_THROW((void)uxs::vformat("{:f}", uxs::make_format_args(unmove(true))));  // boolean
    MUST_THROW((void)uxs::vformat("{:f}", uxs::make_format_args(unmove('A'))));   // character
    MUST_THROW((void)uxs::vformat("{:f}", uxs::make_format_args(unmove(123))));   // integer
    MUST_THROW((void)uxs::vformat("{:f}", uxs::make_format_args(unmove(123u))));  // unsigned integer
    VERIFY(uxs::format("{:f}", 123.) == "123.000000");                            // double
    VERIFY(uxs::format("{:+f}", 123.) == "+123.000000");
    VERIFY(uxs::format("{:-f}", 123.) == "123.000000");
    VERIFY(uxs::format("{:0f}", 123.) == "123.000000");
    VERIFY(uxs::format("{:#f}", 123.) == "123.000000");
    VERIFY(uxs::format("{:.3f}", 123.) == "123.000");
    MUST_THROW((void)uxs::vformat("{:f}", uxs::make_format_args(unmove(reinterpret_cast<void*>(0x123)))));  // pointer
    MUST_THROW((void)uxs::vformat("{:f}", uxs::make_format_args(unmove("hello"))));                         // string

    // -- character specifier
    MUST_THROW((void)uxs::vformat("{:c}", uxs::make_format_args(unmove(true))));  // boolean
    VERIFY(uxs::format("{:c}", 'A') == "A");                                      // character
    MUST_THROW((void)uxs::vformat("{:+c}", uxs::make_format_args(unmove('A'))));
    MUST_THROW((void)uxs::vformat("{:-c}", uxs::make_format_args(unmove('A'))));
    MUST_THROW((void)uxs::vformat("{:0c}", uxs::make_format_args(unmove('A'))));
    MUST_THROW((void)uxs::vformat("{:#c}", uxs::make_format_args(unmove('A'))));
    MUST_THROW((void)uxs::vformat("{:.3c}", uxs::make_format_args(unmove('A'))));
    VERIFY(uxs::format("{:c}", 123) == "{");  // integer
    MUST_THROW((void)uxs::vformat("{:+c}", uxs::make_format_args(unmove(123))));
    MUST_THROW((void)uxs::vformat("{:-c}", uxs::make_format_args(unmove(123))));
    MUST_THROW((void)uxs::vformat("{:0c}", uxs::make_format_args(unmove(123))));
    MUST_THROW((void)uxs::vformat("{:#c}", uxs::make_format_args(unmove(123))));
    MUST_THROW((void)uxs::vformat("{:.3c}", uxs::make_format_args(unmove(123))));
    VERIFY(uxs::format("{:c}", 123u) == "{");  // unsigned integer
    MUST_THROW((void)uxs::vformat("{:+c}", uxs::make_format_args(unmove(123u))));
    MUST_THROW((void)uxs::vformat("{:-c}", uxs::make_format_args(unmove(123u))));
    MUST_THROW((void)uxs::vformat("{:0c}", uxs::make_format_args(unmove(123u))));
    MUST_THROW((void)uxs::vformat("{:#c}", uxs::make_format_args(unmove(123u))));
    MUST_THROW((void)uxs::vformat("{:.3c}", uxs::make_format_args(unmove(123u))));
    MUST_THROW((void)uxs::vformat("{:c}", uxs::make_format_args(unmove(123.))));                            // double
    MUST_THROW((void)uxs::vformat("{:c}", uxs::make_format_args(unmove(reinterpret_cast<void*>(0x123)))));  // pointer
    MUST_THROW((void)uxs::vformat("{:c}", uxs::make_format_args(unmove("hello"))));                         // string

    // -- pointer specifier
    MUST_THROW((void)uxs::vformat("{:p}", uxs::make_format_args(unmove(true))));  // boolean
    MUST_THROW((void)uxs::vformat("{:p}", uxs::make_format_args(unmove('A'))));   // character
    MUST_THROW((void)uxs::vformat("{:p}", uxs::make_format_args(unmove(123))));   // integer
    MUST_THROW((void)uxs::vformat("{:p}", uxs::make_format_args(unmove(123u))));  // unsigned integer
    MUST_THROW((void)uxs::vformat("{:p}", uxs::make_format_args(unmove(123.))));  // double
    VERIFY(uxs::format("{:p}", reinterpret_cast<void*>(0x123)) == "0x123");       // pointer
    MUST_THROW((void)uxs::vformat("{:+p}", uxs::make_format_args(unmove(reinterpret_cast<void*>(0x123)))));
    MUST_THROW((void)uxs::vformat("{:-p}", uxs::make_format_args(unmove(reinterpret_cast<void*>(0x123)))));
    VERIFY(uxs::format("{:0p}", reinterpret_cast<void*>(0x123)) == "0x123");
    MUST_THROW((void)uxs::vformat("{:#p}", uxs::make_format_args(unmove(reinterpret_cast<void*>(0x123)))));
    MUST_THROW((void)uxs::vformat("{:.3p}", uxs::make_format_args(unmove(reinterpret_cast<void*>(0x123)))));
    MUST_THROW((void)uxs::vformat("{:p}", uxs::make_format_args(unmove("hello"))));  // string

    // -- string specifier
    VERIFY(uxs::format("{:s}", true) == "true");  // boolean
    MUST_THROW((void)uxs::vformat("{:+s}", uxs::make_format_args(unmove(true))));
    MUST_THROW((void)uxs::vformat("{:-s}", uxs::make_format_args(unmove(true))));
    MUST_THROW((void)uxs::vformat("{:0s}", uxs::make_format_args(unmove(true))));
    MUST_THROW((void)uxs::vformat("{:#s}", uxs::make_format_args(unmove(true))));
    MUST_THROW((void)uxs::vformat("{:.3s}", uxs::make_format_args(unmove(true))));
    MUST_THROW((void)uxs::vformat("{:s}", uxs::make_format_args(unmove('A'))));   // character
    MUST_THROW((void)uxs::vformat("{:s}", uxs::make_format_args(unmove(123))));   // integer
    MUST_THROW((void)uxs::vformat("{:s}", uxs::make_format_args(unmove(123u))));  // unsigned integer
    MUST_THROW((void)uxs::vformat("{:s}", uxs::make_format_args(unmove(123.))));  // double
    MUST_THROW((void)uxs::vformat("{:s}", uxs::make_format_args(unmove(reinterpret_cast<void*>(0x123)))));  // pointer
    VERIFY(uxs::format("{:s}", "hello") == "hello");                                                        // string
    MUST_THROW((void)uxs::vformat("{:+s}", uxs::make_format_args(unmove("hello"))));
    MUST_THROW((void)uxs::vformat("{:-s}", uxs::make_format_args(unmove("hello"))));
    MUST_THROW((void)uxs::vformat("{:0s}", uxs::make_format_args(unmove("hello"))));
    MUST_THROW((void)uxs::vformat("{:#s}", uxs::make_format_args(unmove("hello"))));
    VERIFY(uxs::format("{:.3s}", "hello") == "hel");

    VERIFY(uxs::format(L"{}", 'A') == L"A");
    MUST_THROW((void)uxs::vformat(L"{:+}", uxs::make_wformat_args(unmove('A'))));
    MUST_THROW((void)uxs::vformat(L"{:0}", uxs::make_wformat_args(unmove('A'))));
    VERIFY(uxs::format(L"{}", L'A') == L"A");
    MUST_THROW((void)uxs::vformat(L"{:+}", uxs::make_wformat_args(unmove(L'A'))));
    MUST_THROW((void)uxs::vformat(L"{:0}", uxs::make_wformat_args(unmove(L'A'))));
    VERIFY(uxs::format(L"{}", L"hello") == L"hello");
    MUST_THROW((void)uxs::vformat(L"{:+}", uxs::make_wformat_args(unmove(L"hello"))));
    MUST_THROW((void)uxs::vformat(L"{:0}", uxs::make_wformat_args(unmove(L"hello"))));
    VERIFY(uxs::format(L"{:c}", 123) == L"{");
    MUST_THROW((void)uxs::vformat(L"{:+c}", uxs::make_wformat_args(unmove(123))));
    MUST_THROW((void)uxs::vformat(L"{:0c}", uxs::make_wformat_args(unmove(123))));

    VERIFY(uxs::format(L"{:c}", 'A') == L"A");
    MUST_THROW((void)uxs::vformat(L"{:+c}", uxs::make_wformat_args(unmove('A'))));
    MUST_THROW((void)uxs::vformat(L"{:0c}", uxs::make_wformat_args(unmove('A'))));
    VERIFY(uxs::format(L"{:c}", L'A') == L"A");
    MUST_THROW((void)uxs::vformat(L"{:+c}", uxs::make_wformat_args(unmove(L'A'))));
    MUST_THROW((void)uxs::vformat(L"{:0c}", uxs::make_wformat_args(unmove(L'A'))));
    VERIFY(uxs::format(L"{:s}", L"hello") == L"hello");
    MUST_THROW((void)uxs::vformat(L"{:+s}", uxs::make_wformat_args(unmove(L"hello"))));
    MUST_THROW((void)uxs::vformat(L"{:0s}", uxs::make_wformat_args(unmove(L"hello"))));

    MUST_THROW((void)uxs::vformat("{:c}", uxs::make_format_args(unmove(1230))));
#if !defined(WCHAR_MAX) || WCHAR_MAX <= 0xffff
    MUST_THROW((void)uxs::vformat(L"{:c}", uxs::make_wformat_args(unmove(123000))));
#endif

    VERIFY(uxs::to_wstring(123.4556) == L"123.4556");
    VERIFY(uxs::format(L"{} {} {}", 123.4556, L"aaa", 567) == L"123.4556 aaa 567");

    std::string s{"{"};
    MUST_THROW((void)uxs::vformat(s, uxs::make_format_args()));
    return 0;
}

int test_string_format_3() {
    uxs::guid id(0x17364152, 0x36b4, 0x4b3e, 0x81, 0xba, 0x5e, 0x79, 0xa6, 0x81, 0xba, 0xee);
    VERIFY(uxs::format("{}", id) == "{17364152-36b4-4b3e-81ba-5e79a681baee}");
    VERIFY(uxs::format("{:=^42}", id) == "=={17364152-36b4-4b3e-81ba-5e79a681baee}==");
    VERIFY(uxs::format("{:=^42x}", id) == "=={17364152-36b4-4b3e-81ba-5e79a681baee}==");
    VERIFY(uxs::format("{:=^42X}", id) == "=={17364152-36B4-4B3E-81BA-5E79A681BAEE}==");
    MUST_THROW((void)uxs::vformat("{:=^+42}", uxs::make_format_args(id)));
    MUST_THROW((void)uxs::vformat("{:=^+42b}", uxs::make_format_args(id)));
    MUST_THROW((void)uxs::vformat("{:=^+42f}", uxs::make_format_args(id)));
    MUST_THROW((void)uxs::vformat("{:=^+42s}", uxs::make_format_args(id)));
    MUST_THROW((void)uxs::vformat("{:=^+42n}", uxs::make_format_args(id)));
    MUST_THROW((void)uxs::vformat("{:=^042}", uxs::make_format_args(id)));
    return 0;
}

int test_string_format_4() {
    {
        char c = 120;
        VERIFY(uxs::format("{:6}", 42) == "    42");
        VERIFY(uxs::format("{:6}", 'x') == "x     ");
        VERIFY(uxs::format("{:*<6}", 'x') == "x*****");
        VERIFY(uxs::format("{:*>6}", 'x') == "*****x");
        VERIFY(uxs::format("{:*^6}", 'x') == "**x***");
        VERIFY(uxs::format("{:6d}", c) == "   120");
        VERIFY(uxs::format("{:6}", true) == "true  ");
    }
    {
        double inf = std::numeric_limits<double>::infinity();
        double nan = std::numeric_limits<double>::quiet_NaN();
        VERIFY(uxs::format("{0:},{0:+},{0:-},{0: }", 1) == "1,+1,1, 1");
        VERIFY(uxs::format("{0:},{0:+},{0:-},{0: }", -1) == "-1,-1,-1,-1");
        VERIFY(uxs::format("{0:},{0:+},{0:-},{0: }", inf) == "inf,+inf,inf, inf");
        VERIFY(uxs::format("{0:},{0:+},{0:-},{0: }", nan) == "nan,+nan,nan, nan");
    }
    {
        char c = 120;
        VERIFY(uxs::format("{:+06d}", c) == "+00120");
        VERIFY(uxs::format("{:#06x}", 0xa) == "0x000a");
        VERIFY(uxs::format("{:<06}", -42) == "-42   ");
    }
    {
        float pi = 3.14f;
        VERIFY(uxs::format("{:10f}", pi) == "  3.140000");
        VERIFY(uxs::format("{:{}f}", pi, 10) == "  3.140000");
        VERIFY(uxs::format("{:.5f}", pi) == "3.14000");
        VERIFY(uxs::format("{:.{}f}", pi, 5) == "3.14000");
        VERIFY(uxs::format("{:10.5f}", pi) == "   3.14000");
        VERIFY(uxs::format("{:{}.{}f}", pi, 10, 5) == "   3.14000");
        MUST_THROW((void)uxs::vformat("{:{}f}", uxs::make_format_args(unmove(pi), unmove(10.0))));
        MUST_THROW((void)uxs::vformat("{:{}f}", uxs::make_format_args(unmove(pi), unmove(-10))));
        MUST_THROW((void)uxs::vformat("{:.{}f}", uxs::make_format_args(unmove(pi), unmove(5.0))));
    }

    VERIFY(uxs::format("{}", nullptr) == "0x0");
    VERIFY(uxs::format("{:6}", nullptr) == "   0x0");
    VERIFY(uxs::format("{:6}", reinterpret_cast<void*>(1)) == "   0x1");
    VERIFY(uxs::format("{:6}", std::numeric_limits<double>::quiet_NaN()) == "   nan");
    VERIFY(uxs::format("{:6f}", std::numeric_limits<double>::quiet_NaN()) == "   nan");
    VERIFY(uxs::format("{:06f}", std::numeric_limits<double>::quiet_NaN()) == "   nan");
    VERIFY(uxs::format("{:6}", std::numeric_limits<double>::infinity()) == "   inf");
    VERIFY(uxs::format("{:<6}", std::numeric_limits<double>::infinity()) == "inf   ");
    VERIFY(uxs::format("{:^6}", std::numeric_limits<double>::infinity()) == " inf  ");
    VERIFY(uxs::format("{:>6}", std::numeric_limits<double>::infinity()) == "   inf");
    VERIFY(uxs::format("{:6e}", std::numeric_limits<double>::infinity()) == "   inf");
    VERIFY(uxs::format("{:06e}", std::numeric_limits<double>::infinity()) == "   inf");
    VERIFY(uxs::format("{:<06e}", std::numeric_limits<double>::infinity()) == "inf   ");
    VERIFY(uxs::format("{:^06e}", std::numeric_limits<double>::infinity()) == " inf  ");
    VERIFY(uxs::format("{:>06e}", std::numeric_limits<double>::infinity()) == "   inf");
    VERIFY(uxs::format("{:6}", true) == "true  ");
    VERIFY(uxs::format("{:10}", "hello") == "hello     ");

    return 0;
}

int test_string_format_5() {
    struct grouping : std::numpunct<char> {
        char do_decimal_point() const override { return ','; }
        char do_thousands_sep() const override { return '\''; }
        std::string do_grouping() const override { return "\1\2\3"; }
    };

    std::locale loc{std::locale::classic(), new grouping};

    VERIFY(uxs::format(loc, "{:#Lb}", -25510) == "-0b110'001'110'100'11'0");
    VERIFY(uxs::format(loc, "{:#Lo}", -1452346510) == "-012'644'206'21'6");
    VERIFY(uxs::format(loc, "{:#Lx}", -1452346510) == "-0x56'910'c8'e");
    VERIFY(uxs::format(loc, "{:#017Lx}", -1452346510) == "-0x00056'910'c8'e");
    VERIFY(uxs::format(loc, "{:L}", 1452346510u) == "1'452'346'51'0");
    VERIFY(uxs::format(loc, "{:L}", -1452346510) == "-1'452'346'51'0");
    VERIFY(uxs::format(loc, "{:020L}", -1452346510) == "-000001'452'346'51'0");
    VERIFY(uxs::format(loc, "{:Lf}", 1452346510.0) == "1'452'346'51'0,000000");

    return 0;
}

static_assert(uxs::range_formattable<std::map<short, double>>::value == uxs::range_format::map, "");
static_assert(uxs::range_formattable<std::set<short>>::value == uxs::range_format::set, "");
static_assert(uxs::range_formattable<std::vector<short>>::value == uxs::range_format::sequence, "");

#if 0
template<typename Func>
void classify_utf(const Func& fn) {
    enum class type_t { normal = 0, special, ill_formed };
    type_t last_type = type_t::normal;
    const std::uint32_t max_code = 0x200000;
    std::uint32_t code_first = 0;
    for (std::uint32_t code = 0; code < max_code; ++code) {
        auto s = fn(code);
        type_t curr_type = type_t::normal;
        if (code < 0x20 || s.starts_with("\"\\u")) {
            curr_type = type_t::special;
        } else if (s.starts_with("\"\\x")) {
            curr_type = type_t::ill_formed;
        }
        if (curr_type != last_type) {
            if (last_type != type_t::normal) { uxs::println("{}-{}", fn(code_first), fn(code - 1)); }
            last_type = curr_type;
            code_first = code;
        }
    }
    if (last_type != type_t::normal) { uxs::println("{}-{}", fn(code_first), fn(max_code - 1)); }
}

template<typename OutputIt>
unsigned to_utf8(std::uint32_t code, OutputIt out) {
    if (code < 0x80) {
        *out++ = static_cast<std::uint8_t>(code);
        return 1;
    }
    std::uint8_t ch[4];
    const std::uint8_t mask[] = {0, 0x1f, 0xf, 0x7};
    const std::uint8_t hdr[] = {0, 0xc0, 0xe0, 0xf0};
    unsigned count = 0;
    do { ch[count] = 0x80 | (code & 0x3f); } while ((code >>= 6) > mask[++count]);
    *out++ = static_cast<std::uint8_t>(hdr[count] | code);
    const unsigned n_written = count + 1;
    do { *out++ = ch[--count]; } while (count > 0);
    return n_written;
}
#endif

int test_string_format_6() {
#if 0
    auto fn = [](std::uint32_t code) {
        char buf[5];
        unsigned sz = to_utf8(code, static_cast<char*>(buf));
        return uxs::format("{:?}", std::string_view(buf, sz));
    };
    classify_utf(fn);
#endif

    auto p = std::make_pair<short, double>(3, 3.1415);
    VERIFY(uxs::format("{}", p) == "(3, 3.1415)");
    VERIFY(uxs::format("{:n}", p) == "3, 3.1415");
    VERIFY(uxs::format("{:m}", p) == "3: 3.1415");
    VERIFY(uxs::format("{:=^15}", p) == "==(3, 3.1415)==");
    VERIFY(uxs::format("{:=<15}", p) == "(3, 3.1415)====");
    VERIFY(uxs::format("{:=>15}", p) == "====(3, 3.1415)");
    VERIFY(uxs::format("{:=>7}", p) == "(3, 3.1415)");
    VERIFY(uxs::format("{:15}", p) == "(3, 3.1415)    ");

    std::map<short, double> m = {{3, 3.1415}, {4, 4.1415}, {5, 5.1415}};
    std::set<short> s = {3, 4, 5};
    std::vector<short> v = {3, 4, 5};
    std::vector<std::pair<short, double>> vp = {{3, 3.1415}, {4, 4.1415}, {5, 5.1415}};
    VERIFY(uxs::format("{}", m) == "{3: 3.1415, 4: 4.1415, 5: 5.1415}");
    VERIFY(uxs::format("{}", s) == "{3, 4, 5}");
    VERIFY(uxs::format("{}", v) == "[3, 4, 5]");
    VERIFY(uxs::format("{}", vp) == "[(3, 3.1415), (4, 4.1415), (5, 5.1415)]");
    VERIFY(uxs::format("{:m}", m) == "{3: 3.1415, 4: 4.1415, 5: 5.1415}");
    VERIFY(uxs::format("{:nm}", m) == "3: 3.1415, 4: 4.1415, 5: 5.1415");
    VERIFY(uxs::format("{:m}", vp) == "{3: 3.1415, 4: 4.1415, 5: 5.1415}");
    VERIFY(uxs::format("{:nm}", vp) == "3: 3.1415, 4: 4.1415, 5: 5.1415");

    MUST_THROW((void)uxs::vformat("{:m}", uxs::make_format_args(unmove(v))));

    std::vector<char> str2({'H', 'e', 'l', 'l', 'o'});
    VERIFY(uxs::format("{:s}", str2) == "Hello");
    MUST_THROW((void)uxs::vformat("{:s}", uxs::make_format_args(unmove(v))));

    VERIFY(uxs::format("{:10.3?}", "hello") == "\"he       ");
    VERIFY(uxs::format("{:10.3?}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") == "\"\xd0\x9f\xd1\x80       ");

    VERIFY(uxs::format("{:.^15}", std::make_tuple("\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82")) ==
           "..(\"\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\")...");

    str2 = {'\xd0', '\x9f', '\xd1', '\x80', '\xd0', '\xb8', '\xd0', '\xb2', '\xd0', '\xb5', '\xd1', '\x82'};
    VERIFY(uxs::format("{:10?s}", str2) == "\"\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\"  ");
    VERIFY(uxs::format("{:10.3?s}", str2) == "\"\xd0\x9f\xd1\x80       ");

    auto t = std::make_tuple<short, double, char>(3, 3.1415, 'A');
    VERIFY(uxs::format("{}", t) == "(3, 3.1415, 'A')");
    MUST_THROW((void)uxs::vformat("{:m}", uxs::make_format_args(unmove(t))));

    VERIFY(uxs::format("{::#5x:+010.2E}", p) == "(  0x3, +03.14E+00)");
    return 0;
}

ADD_TEST_CASE("", "string format", test_string_format_0);
ADD_TEST_CASE("", "string format", test_string_format_1);
ADD_TEST_CASE("", "string format", test_string_format_2);
ADD_TEST_CASE("", "string format", test_string_format_3);
ADD_TEST_CASE("", "string format", test_string_format_4);
ADD_TEST_CASE("", "string format", test_string_format_5);
ADD_TEST_CASE("", "string format", test_string_format_6);

//-----------------------------------------------------------------------------
// Performance tests

const int perf_N_secs = 5;

template<typename Func>
int perf_format_to_string(const Func& fn, int n_secs) {
    std::array<char, 128> buf;

    size_t len = 0;
    const int estimate_loop_count = 10000;
    const auto start0 = curr_clock_t::now();
    for (int i = 0; i < estimate_loop_count; ++i) {
        len = std::max<size_t>(len, fn(buf.data(), buf.data() + buf.size(), i));
    }
    const auto start = curr_clock_t::now();
    const int loop_count = static_cast<int>(
        std::ceil((estimate_loop_count * n_secs * 1000000000.0) / as_ns_duration(start - start0)));
    for (int i = 0; i < loop_count; ++i) {
        len = std::max<size_t>(len, fn(buf.data(), buf.data() + buf.size(), i % estimate_loop_count));
    }
    return len ? static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) / loop_count) : 0;
}

ADD_TEST_CASE("2-perf", "format string", ([]() {
                  return perf_format_to_string(
                      [](char* first, char* last, int i) {
                          return static_cast<size_t>(uxs::format_to(first, "{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234 * i,
                                                                    i, 3.13 * i, "str",
                                                                    reinterpret_cast<void*>(static_cast<uintptr_t>(i)),
                                                                    static_cast<char>('A' + i % 26)) -
                                                     first);
                      },
                      perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<libc> format string", ([]() {
                  return perf_format_to_string(
                      [](char* first, char* last, int i) {
                          return std::sprintf(first, "%0.10f:%04d:%+g:%s:%p:%c:%%\n", 1.234 * i, i, 3.13 * i, "str",
                                              reinterpret_cast<void*>(static_cast<uintptr_t>(i)),
                                              static_cast<char>('A' + i % 26));
                      },
                      perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<std::ostringstream> format string", ([]() {
                  return perf_format_to_string(
                      [](char* first, char* last, int i) {
                          std::ostringstream ss;
                          ss << std::setprecision(10) << std::fixed << 1.234 * i << ':'
                             << std::resetiosflags(std::ios::floatfield) << std::setw(4) << std::setfill('0') << i
                             << std::setfill(' ') << ':' << std::setiosflags(std::ios::showpos) << 3.13 * i
                             << std::resetiosflags(std::ios::showpos) << ':' << "str" << ':'
                             << reinterpret_cast<void*>(static_cast<uintptr_t>(i)) << ':'
                             << static_cast<char>('A' + i % 26) << ":%\n"
                             << std::flush;
                          std::string s = std::move(ss).str();
                          return s.size();
                      },
                      perf_N_secs);
              }));
#if defined(has_cpp_lib_format)
ADD_TEST_CASE("2-perf", "<std::format_to> format string", ([]() {
                  return perf_format_to_string(
                      [](char* first, char* last, int i) {
                          return static_cast<size_t>(std::format_to(first, "{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234 * i,
                                                                    i, 3.13 * i, "str",
                                                                    reinterpret_cast<void*>(static_cast<uintptr_t>(i)),
                                                                    static_cast<char>('A' + i % 26)) -
                                                     first);
                      },
                      perf_N_secs);
              }));
#endif
ADD_TEST_CASE("2-perf", "<{fmt}> format string", ([]() {
                  return perf_format_to_string(
                      [](char* first, char* last, int i) {
                          return static_cast<size_t>(fmt::format_to(first, "{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234 * i,
                                                                    i, 3.13 * i, "str",
                                                                    reinterpret_cast<void*>(static_cast<uintptr_t>(i)),
                                                                    static_cast<char>('A' + i % 26)) -
                                                     first);
                      },
                      perf_N_secs);
              }));

}  // namespace
