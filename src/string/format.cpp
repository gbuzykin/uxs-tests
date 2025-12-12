#if defined(_MSC_VER)
#    define _CRT_SECURE_NO_WARNINGS
#endif

#include "fmt/format.h"

#include "test_suite.h"

#include "uxs/io/oflatbuf.h"

#include <uxs/format_optional.h>
#include <uxs/guid.h>

#if __cplusplus >= 202002L && UXS_HAS_INCLUDE(<format>)
#    include <format>
#    define has_cpp_lib_format 1
#endif

#if __cplusplus >= 201703L && UXS_HAS_INCLUDE(<filesystem>)
#    include <uxs/format_fs.h>
#endif

#define DESIRED_LIBCPP_VERSION 220000
#if __cplusplus >= 202002L && UXS_HAS_INCLUDE(<chrono>)
#    include <uxs/format_chrono.h>
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

static_assert(uxs::sfmt::arg_type_index<bool, char>::value == uxs::sfmt::index_t::boolean, "");
static_assert(uxs::sfmt::arg_type_index<char, char>::value == uxs::sfmt::index_t::character, "");
static_assert(uxs::sfmt::arg_type_index<wchar_t, char>::value == uxs::sfmt::index_t::custom, "");
static_assert(uxs::sfmt::arg_type_index<signed char, char>::value == uxs::sfmt::index_t::integer, "");
static_assert(uxs::sfmt::arg_type_index<signed short, char>::value == uxs::sfmt::index_t::integer, "");
static_assert(uxs::sfmt::arg_type_index<signed, char>::value == uxs::sfmt::index_t::integer, "");
static_assert(sizeof(signed long) == sizeof(signed long long) ?
                  uxs::sfmt::arg_type_index<signed long, char>::value == uxs::sfmt::index_t::long_integer :
                  uxs::sfmt::arg_type_index<signed long, char>::value == uxs::sfmt::index_t::integer,
              "");
static_assert(uxs::sfmt::arg_type_index<signed long long, char>::value == uxs::sfmt::index_t::long_integer, "");
static_assert(uxs::sfmt::arg_type_index<unsigned char, char>::value == uxs::sfmt::index_t::unsigned_integer, "");
static_assert(uxs::sfmt::arg_type_index<unsigned short, char>::value == uxs::sfmt::index_t::unsigned_integer, "");
static_assert(uxs::sfmt::arg_type_index<unsigned, char>::value == uxs::sfmt::index_t::unsigned_integer, "");
static_assert(sizeof(unsigned long) == sizeof(unsigned long long) ?
                  uxs::sfmt::arg_type_index<unsigned long, char>::value == uxs::sfmt::index_t::unsigned_long_integer :
                  uxs::sfmt::arg_type_index<unsigned long, char>::value == uxs::sfmt::index_t::unsigned_integer,
              "");
static_assert(uxs::sfmt::arg_type_index<unsigned long long, char>::value == uxs::sfmt::index_t::unsigned_long_integer,
              "");
static_assert(uxs::sfmt::arg_type_index<float, char>::value == uxs::sfmt::index_t::single_precision, "");
static_assert(uxs::sfmt::arg_type_index<double, char>::value == uxs::sfmt::index_t::double_precision, "");
static_assert(uxs::sfmt::arg_type_index<long double, char>::value == uxs::sfmt::index_t::long_double_precision, "");
static_assert(uxs::sfmt::arg_type_index<uxs::guid, char>::value == uxs::sfmt::index_t::custom, "");

static_assert(uxs::sfmt::arg_type_index<bool, wchar_t>::value == uxs::sfmt::index_t::boolean, "");
static_assert(uxs::sfmt::arg_type_index<char, wchar_t>::value == uxs::sfmt::index_t::character, "");
static_assert(uxs::sfmt::arg_type_index<wchar_t, wchar_t>::value == uxs::sfmt::index_t::character, "");
static_assert(uxs::sfmt::arg_type_index<signed char, wchar_t>::value == uxs::sfmt::index_t::integer, "");
static_assert(uxs::sfmt::arg_type_index<signed short, wchar_t>::value == uxs::sfmt::index_t::integer, "");
static_assert(uxs::sfmt::arg_type_index<signed, wchar_t>::value == uxs::sfmt::index_t::integer, "");
static_assert(sizeof(signed long) == sizeof(signed long long) ?
                  uxs::sfmt::arg_type_index<signed long, wchar_t>::value == uxs::sfmt::index_t::long_integer :
                  uxs::sfmt::arg_type_index<signed long, wchar_t>::value == uxs::sfmt::index_t::integer,
              "");
static_assert(uxs::sfmt::arg_type_index<signed long long, wchar_t>::value == uxs::sfmt::index_t::long_integer, "");
static_assert(uxs::sfmt::arg_type_index<unsigned char, wchar_t>::value == uxs::sfmt::index_t::unsigned_integer, "");
static_assert(uxs::sfmt::arg_type_index<unsigned short, wchar_t>::value == uxs::sfmt::index_t::unsigned_integer, "");
static_assert(uxs::sfmt::arg_type_index<unsigned, wchar_t>::value == uxs::sfmt::index_t::unsigned_integer, "");
static_assert(sizeof(unsigned long) == sizeof(unsigned long long) ?
                  uxs::sfmt::arg_type_index<unsigned long, wchar_t>::value == uxs::sfmt::index_t::unsigned_long_integer :
                  uxs::sfmt::arg_type_index<unsigned long, wchar_t>::value == uxs::sfmt::index_t::unsigned_integer,
              "");
static_assert(uxs::sfmt::arg_type_index<unsigned long long, wchar_t>::value == uxs::sfmt::index_t::unsigned_long_integer,
              "");
static_assert(uxs::sfmt::arg_type_index<float, wchar_t>::value == uxs::sfmt::index_t::single_precision, "");
static_assert(uxs::sfmt::arg_type_index<double, wchar_t>::value == uxs::sfmt::index_t::double_precision, "");
static_assert(uxs::sfmt::arg_type_index<long double, wchar_t>::value == uxs::sfmt::index_t::long_double_precision, "");
static_assert(uxs::sfmt::arg_type_index<uxs::guid, wchar_t>::value == uxs::sfmt::index_t::custom, "");

#if defined(_MSC_VER)
static_assert(uxs::sfmt::arg_type_index<unsigned __int64, char>::value == uxs::sfmt::index_t::unsigned_long_integer, "");
static_assert(uxs::sfmt::arg_type_index<signed __int64, char>::value == uxs::sfmt::index_t::long_integer, "");
static_assert(uxs::sfmt::arg_type_index<unsigned __int64, wchar_t>::value == uxs::sfmt::index_t::unsigned_long_integer,
              "");
static_assert(uxs::sfmt::arg_type_index<signed __int64, wchar_t>::value == uxs::sfmt::index_t::long_integer, "");
#endif  // defined(_MSC_VER)

static_assert(uxs::format_arg_type_index<uxs::format_context, bool>::value == uxs::sfmt::index_t::boolean, "");
static_assert(uxs::format_arg_type_index<uxs::format_context, char>::value == uxs::sfmt::index_t::character, "");
static_assert(uxs::format_arg_type_index<uxs::wformat_context, wchar_t>::value == uxs::sfmt::index_t::character, "");
static_assert(uxs::format_arg_type_index<uxs::format_context, int32_t>::value == uxs::sfmt::index_t::integer, "");
static_assert(uxs::format_arg_type_index<uxs::format_context, int64_t>::value == uxs::sfmt::index_t::long_integer, "");
static_assert(uxs::format_arg_type_index<uxs::format_context, uint32_t>::value == uxs::sfmt::index_t::unsigned_integer,
              "");
static_assert(uxs::format_arg_type_index<uxs::format_context, uint64_t>::value ==
                  uxs::sfmt::index_t::unsigned_long_integer,
              "");
static_assert(uxs::format_arg_type_index<uxs::format_context, float>::value == uxs::sfmt::index_t::single_precision, "");
static_assert(uxs::format_arg_type_index<uxs::format_context, double>::value == uxs::sfmt::index_t::double_precision,
              "");
static_assert(uxs::format_arg_type_index<uxs::format_context, long double>::value ==
                  uxs::sfmt::index_t::long_double_precision,
              "");
static_assert(uxs::format_arg_type_index<uxs::format_context, const void*>::value == uxs::sfmt::index_t::pointer, "");
static_assert(uxs::format_arg_type_index<uxs::format_context, const char*>::value == uxs::sfmt::index_t::z_string, "");
static_assert(uxs::format_arg_type_index<uxs::wformat_context, const wchar_t*>::value == uxs::sfmt::index_t::z_string,
              "");
static_assert(uxs::format_arg_type_index<uxs::format_context, std::string_view>::value == uxs::sfmt::index_t::string,
              "");
static_assert(uxs::format_arg_type_index<uxs::wformat_context, std::wstring_view>::value == uxs::sfmt::index_t::string,
              "");
static_assert(uxs::format_arg_type_index<uxs::format_context, uxs::basic_format_arg<uxs::format_context>::handle>::value ==
                  uxs::sfmt::index_t::custom,
              "");
static_assert(
    uxs::format_arg_type_index<uxs::wformat_context, uxs::basic_format_arg<uxs::wformat_context>::handle>::value ==
        uxs::sfmt::index_t::custom,
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

    ss.seek(0);
    ss.truncate();
    uxs::print(ss, loc, "The answer is {:L}.", 4.2).flush();
    VERIFY(std::string_view(ss.data(), ss.size()) == "The answer is 4_2.");

    wss.seek(0);
    wss.truncate();
    uxs::print(wss, wloc, L"The answer is {:L}.", 4.2).flush();
    VERIFY(std::wstring_view(wss.data(), wss.size()) == L"The answer is 4_2.");

    ss.seek(0);
    ss.truncate();
    uxs::println(ss, "The answer is {}.", 4.2);
    VERIFY(std::string_view(ss.data(), ss.size()) == "The answer is 4.2.\n");

    wss.seek(0);
    wss.truncate();
    uxs::println(wss, L"The answer is {}.", 4.2);
    VERIFY(std::wstring_view(wss.data(), wss.size()) == L"The answer is 4.2.\n");

    ss.seek(0);
    ss.truncate();
    uxs::println(ss, loc, "The answer is {:L}.", 4.2);
    VERIFY(std::string_view(ss.data(), ss.size()) == "The answer is 4_2.\n");

    wss.seek(0);
    wss.truncate();
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
#if WCHAR_MAX <= 0xffff
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

    std::array<int, 4> ints{12, 10, 15, 14};

    VERIFY(uxs::format("{}", ints) == "[12, 10, 15, 14]");
    VERIFY(uxs::format("{::X}", ints) == "[C, A, F, E]");
    VERIFY(uxs::format("{:n:_^4}", ints) == "_12_, _10_, _15_, _14_");

    std::array<std::pair<char, int>, 3> char_pairs{std::make_pair('A', 5), std::make_pair('B', 10),
                                                   std::make_pair('C', 12)};

    VERIFY(uxs::format("{}", char_pairs) == "[('A', 5), ('B', 10), ('C', 12)]");
    VERIFY(uxs::format("{:m}", char_pairs) == "{'A': 5, 'B': 10, 'C': 12}");

    std::array<char, 4> star{'S', 'T', 'A', 'R'};

    VERIFY(uxs::format("{}", star) == "['S', 'T', 'A', 'R']");
    VERIFY(uxs::format("{:s}", star) == "STAR");
    VERIFY(uxs::format("{:?s}", star) == "\"STAR\"");

    return 0;
}

int test_string_format_7() {
    VERIFY(uxs::format("{}", est::optional<int>(est::nullopt())) == "null");
    VERIFY(uxs::format("{:4}", est::make_optional(10)) == "  10");
    VERIFY(uxs::format("{:^5}", est::make_optional("AAA")) == " AAA ");

    return 0;
}

ADD_TEST_CASE("", "string format", test_string_format_0);
ADD_TEST_CASE("", "string format", test_string_format_1);
ADD_TEST_CASE("", "string format", test_string_format_2);
ADD_TEST_CASE("", "string format", test_string_format_3);
ADD_TEST_CASE("", "string format", test_string_format_4);
ADD_TEST_CASE("", "string format", test_string_format_5);
ADD_TEST_CASE("", "string format", test_string_format_6);
ADD_TEST_CASE("", "string format", test_string_format_7);

#if __cplusplus >= 201703L && UXS_HAS_INCLUDE(<filesystem>)
int test_string_format_filesystem() {
    std::filesystem::path p = std::filesystem::current_path();
    VERIFY(uxs::format("{}", p) == uxs::utf_string_adapter<char>{}(p.native()));
    VERIFY(uxs::format("{:g}", p) ==
           uxs::utf_string_adapter<char>{}(p.generic_string<std::filesystem::path::value_type>()));
    return 0;
}
ADD_TEST_CASE("", "string format", test_string_format_filesystem);
#endif

#if __cplusplus >= 202002L && UXS_HAS_INCLUDE(<chrono>) && UXS_HAS_INCLUDE(<format>)
int test_string_format_chrono_year() {
    using namespace std::chrono;

    static constexpr int year_list[] = {-9900, -2001, -101, -100, -99, -1, 0, 1, 99, 100, 101, 1001, 9999};

    static constexpr const char* century_list[] = {"-99", "-21", "-02", "-01", "-01", "-01", "00",
                                                   "00",  "00",  "01",  "01",  "10",  "99"};

    static constexpr const char* year_yy_list[] = {"00", "99", "99", "00", "01", "99", "00",
                                                   "01", "99", "00", "01", "01", "99"};

    static constexpr const char* year_yyyy_list[] = {"-9900", "-2001", "-0101", "-0100", "-0099", "-0001", "0000",
                                                     "0001",  "0099",  "0100",  "0101",  "1001",  "9999"};

    for (unsigned i = 0; i < sizeof(year_list) / sizeof(year_list[0]); ++i) {
        // century
        VERIFY(uxs::format("{:%C}", year{year_list[i]}) == century_list[i]);
#    if defined(_LIBCPP_VERSION) && _LIBCPP_VERSION < DESIRED_LIBCPP_VERSION
        if (year_list[i] >= 0)
#    endif
        {
            VERIFY(uxs::format("{:%C}", year{year_list[i]}) == std::format("{:%C}", year{year_list[i]}));
        }
#    if defined(__GNUC__)
        if (year_list[i] >= 0)
#    endif
        {
            // year yy
            VERIFY(uxs::format("{:%y}", year{year_list[i]}) == year_yy_list[i]);
            VERIFY(uxs::format("{:%y}", year{year_list[i]}) == std::format("{:%y}", year{year_list[i]}));
        }
        // year yyyy
        VERIFY(uxs::format("{:%Y}", year{year_list[i]}) == year_yyyy_list[i]);
        VERIFY(uxs::format("{:%Y}", year{year_list[i]}) == std::format("{:%Y}", year{year_list[i]}));
        // default
        VERIFY(uxs::format("{}", year{year_list[i]}) == std::format("{}", year{year_list[i]}));
    }

    // localized
    VERIFY(uxs::format("{:%EY}", year{101}) == std::format("{:%EY}", year{101}));
    VERIFY(uxs::format("{:%EY}", year{1001}) == std::format("{:%EY}", year{1001}));
    VERIFY(uxs::format("{:%EY}", year{3001}) == std::format("{:%EY}", year{3001}));

    return 0;
}

int test_string_format_chrono_month() {
    using namespace std::chrono;

    static constexpr month month_list[] = {January, February, March,     April,   May,      June,
                                           July,    August,   September, October, November, December};

    static constexpr const char* brief_name_list[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                                      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    static constexpr const char* full_name_list[] = {"January",   "February", "March",    "April",
                                                     "May",       "June",     "July",     "August",
                                                     "September", "October",  "November", "December"};

    static constexpr const char* mm_list[] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};

    for (unsigned i = 0; i < 12; ++i) {
        // month brief
        VERIFY(uxs::format("{:%b}", month_list[i]) == brief_name_list[i]);
        VERIFY(uxs::format("{:%h}", month_list[i]) == brief_name_list[i]);
        VERIFY(uxs::format("{:%b}", month_list[i]) == std::format("{:%b}", month_list[i]));
        VERIFY(uxs::format("{:L%b}", month_list[i]) == std::format("{:L%b}", month_list[i]));
        // month full
        VERIFY(uxs::format("{:%B}", month_list[i]) == full_name_list[i]);
        VERIFY(uxs::format("{:%B}", month_list[i]) == std::format("{:%B}", month_list[i]));
        VERIFY(uxs::format("{:L%B}", month_list[i]) == std::format("{:L%B}", month_list[i]));
        // month mm
        VERIFY(uxs::format("{:%m}", month_list[i]) == mm_list[i]);
        VERIFY(uxs::format("{:%m}", month_list[i]) == std::format("{:%m}", month_list[i]));
        VERIFY(uxs::format("{:%Om}", month_list[i]) == std::format("{:%Om}", month_list[i]));
        // default
        VERIFY(uxs::format("{}", month_list[i]) == std::format("{}", month_list[i]));
    }

    VERIFY(uxs::format("{:%m}", month{0}) == std::format("{:%m}", month{0}));
    VERIFY(uxs::format("{:%m}", month{14}) == std::format("{:%m}", month{14}));

    VERIFY(uxs::format("{}", month{0}) == std::format("{}", month{0}));
    VERIFY(uxs::format("{}", month{14}) == std::format("{}", month{14}));

    MUST_THROW((void)uxs::format("{:%b}", month{0}));
    MUST_THROW((void)uxs::format("{:%B}", month{0}));
    MUST_THROW((void)uxs::format("{:%b}", month{14}));
    MUST_THROW((void)uxs::format("{:%B}", month{14}));

    return 0;
}

int test_string_format_chrono_day() {
    using namespace std::chrono;

    static constexpr unsigned day_list[] = {0, 1, 9, 11, 30, 31, 40, 99};
    static constexpr const char* day_dd_list[] = {"00", "01", "09", "11", "30", "31", "40", "99"};
    static constexpr const char* day_ee_list[] = {" 0", " 1", " 9", "11", "30", "31", "40", "99"};

    // day
    for (unsigned i = 0; i < sizeof(day_list) / sizeof(day_list[0]); ++i) {
        // zeroed
        VERIFY(uxs::format("{:%d}", day{day_list[i]}) == day_dd_list[i]);
        VERIFY(uxs::format("{:%d}", day{day_list[i]}) == std::format("{:%d}", day{day_list[i]}));
        // spaces
        VERIFY(uxs::format("{:%e}", day{day_list[i]}) == day_ee_list[i]);
        VERIFY(uxs::format("{:%e}", day{day_list[i]}) == std::format("{:%e}", day{day_list[i]}));
        // default
        VERIFY(uxs::format("{}", day{day_list[i]}) == std::format("{}", day{day_list[i]}));
    }

    VERIFY(uxs::format("{:%Od}", day{7}) == std::format("{:%Od}", day{7}));

    return 0;
}

int test_string_format_chrono_weekday() {
    using namespace std::chrono;

    static constexpr weekday weekday_list[] = {Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday};

    static constexpr const char* brief_name_list[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    static constexpr const char* full_name_list[] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                                                     "Thursday", "Friday", "Saturday"};

    static constexpr const char* mm_list[] = {"0", "1", "2", "3", "4", "5", "6", "7"};

    for (unsigned i = 0; i < 7; ++i) {
        // weekday brief
        VERIFY(uxs::format("{:%a}", weekday_list[i]) == brief_name_list[i]);
        VERIFY(uxs::format("{:%a}", weekday_list[i]) == std::format("{:%a}", weekday_list[i]));
        VERIFY(uxs::format("{:L%a}", weekday_list[i]) == std::format("{:L%a}", weekday_list[i]));
        // weekday full
        VERIFY(uxs::format("{:%A}", weekday_list[i]) == full_name_list[i]);
        VERIFY(uxs::format("{:%A}", weekday_list[i]) == std::format("{:%A}", weekday_list[i]));
        VERIFY(uxs::format("{:L%A}", weekday_list[i]) == std::format("{:L%A}", weekday_list[i]));
        // weekday mm
        VERIFY(uxs::format("{:%u}", weekday_list[i]) == mm_list[i > 0 ? i : 7]);
        VERIFY(uxs::format("{:%w}", weekday_list[i]) == mm_list[i]);
        VERIFY(uxs::format("{:%w}", weekday_list[i]) == std::format("{:%w}", weekday_list[i]));
        VERIFY(uxs::format("{:%Ow}", weekday_list[i]) == std::format("{:%Ow}", weekday_list[i]));
        // default
        VERIFY(uxs::format("{}", weekday_list[i]) == std::format("{}", weekday_list[i]));
    }

    VERIFY(uxs::format("{}", weekday{8}) == std::format("{}", weekday{8}));
    VERIFY(uxs::format("{}", weekday{100}) == std::format("{}", weekday{100}));

    VERIFY(uxs::format("{}", weekday_indexed{weekday{3}, 2}) == std::format("{}", weekday_indexed{weekday{3}, 2}));
    VERIFY(uxs::format("{:%a}", weekday_indexed{weekday{3}, 2}) == std::format("{:%a}", weekday_indexed{weekday{3}, 2}));
    VERIFY(uxs::format("{}", weekday_indexed{weekday{8}, 2}) == std::format("{}", weekday_indexed{weekday{8}, 2}));
    VERIFY(uxs::format("{}", weekday_indexed{weekday{100}, 2}) == std::format("{}", weekday_indexed{weekday{100}, 2}));
    VERIFY(uxs::format("{}", weekday_indexed{weekday{3}, 10}) == std::format("{}", weekday_indexed{weekday{3}, 10}));

    VERIFY(uxs::format("{}", weekday_last{weekday{3}}) == std::format("{}", weekday_last{weekday{3}}));
    VERIFY(uxs::format("{}", weekday_last{weekday{8}}) == std::format("{}", weekday_last{weekday{8}}));
    VERIFY(uxs::format("{}", weekday_last{weekday{100}}) == std::format("{}", weekday_last{weekday{100}}));

    MUST_THROW((void)uxs::format("{:%a}", weekday{8}));
    MUST_THROW((void)uxs::format("{:%A}", weekday{8}));
    MUST_THROW((void)uxs::format("{:%u}", weekday{8}));
    MUST_THROW((void)uxs::format("{:%w}", weekday{8}));

    return 0;
}

int test_string_format_chrono_compound() {
    using namespace std::chrono;
    using namespace std::chrono_literals;

    VERIFY(uxs::format("{}", month_day{July, 8d}) == std::format("{}", month_day{July, 8d}));
    VERIFY(uxs::format("{}", month_day{month{14}, 40d}) == std::format("{}", month_day{month{14}, 40d}));

    static constexpr month month_list[] = {January, February, March,     April,   May,      June,
                                           July,    August,   September, October, November, December};

    VERIFY(uxs::format("{}", month_day_last{July}) == std::format("{}", month_day_last{July}));
#    if !defined(__GNUC__)
    for (const auto& m : month_list) {
        if (m != February) {
            VERIFY(uxs::format("{:%d}", month_day_last{m}) == std::format("{:%d}", month_day_last{m}));
        }
    }
#    endif
    VERIFY(uxs::format("{}", month_day_last{month{14}}) == std::format("{}", month_day_last{month{14}}));

    VERIFY(uxs::format("{}", month_weekday{July, weekday_indexed{weekday{3}, 2}}) ==
           std::format("{}", month_weekday{July, weekday_indexed{weekday{3}, 2}}));
#    if !defined(_LIBCPP_VERSION) || _LIBCPP_VERSION >= DESIRED_LIBCPP_VERSION
    VERIFY(uxs::format("{}", month_weekday_last{July, weekday_last{weekday{3}}}) ==
           std::format("{}", month_weekday_last{July, weekday_last{weekday{3}}}));
#    endif

    VERIFY(uxs::format("{}", year_month{1982y, July}) == std::format("{}", year_month{1982y, July}));
    VERIFY(uxs::format("{}", year_month{1983y, month{14}}) == std::format("{}", year_month{1983y, month{14}}));

    VERIFY(uxs::format("{}", year_month_day_last{1982y, month_day_last{July}}) ==
           std::format("{}", year_month_day_last{1982y, month_day_last{July}}));
    for (const auto& m : month_list) {
        VERIFY(uxs::format("{:%d}", year_month_day_last{1982y, month_day_last{m}}) ==
               std::format("{:%d}", year_month_day_last{1982y, month_day_last{m}}));
    }
    VERIFY(uxs::format("{}", year_month_day_last{1982y, month_day_last{month{14}}}) ==
           std::format("{}", year_month_day_last{1982y, month_day_last{month{14}}}));

    VERIFY(uxs::format("{}", year_month_weekday{1982y, July, weekday_indexed{weekday{3}, 2}}) ==
           std::format("{}", year_month_weekday{1982y, July, weekday_indexed{weekday{3}, 2}}));
    VERIFY(uxs::format("{}", year_month_weekday_last{1982y, July, weekday_last{weekday{3}}}) ==
           std::format("{}", year_month_weekday_last{1982y, July, weekday_last{weekday{3}}}));

    return 0;
}

int test_string_format_chrono_hours() {
    using namespace std::chrono;
    using namespace std::chrono_literals;

    static constexpr int hours_list[] = {-99, -12, -2, 0, 2, 10, 12, 14, 23, 24, 99};
    static constexpr const char* hours_hh_list[] = {"-99", "-12", "-02", "00", "02", "10",
                                                    "12",  "14",  "23",  "24", "99"};
    static constexpr const char* hours_hh_12_list[] = {"", "", "", "12", "02", "10", "12", "02", "11", "", ""};

    for (unsigned i = 0; i < sizeof(hours_list) / sizeof(hours_list[0]); ++i) {
        // 24-based
        VERIFY(uxs::format("{:%H}", hours{hours_list[i]}) == hours_hh_list[i]);
#    if defined(_LIBCPP_VERSION) && _LIBCPP_VERSION < DESIRED_LIBCPP_VERSION
        if (hours_list[i] >= 0 && hours_list[i] < 24)
#    endif
        {
            VERIFY(uxs::format("{:%H}", hours{hours_list[i]}) == std::format("{:%H}", hours{hours_list[i]}));
        }
        // 12-based
        if (hours_list[i] >= 0 && hours_list[i] < 24) {
            VERIFY(uxs::format("{:%I}", hours{hours_list[i]}) == hours_hh_12_list[i]);
            VERIFY(uxs::format("{:%I}", hours{hours_list[i]}) == std::format("{:%I}", hours{hours_list[i]}));
            // AM/PM
            VERIFY(uxs::format("{:%p}", hours{hours_list[i]}) == std::format("{:%p}", hours{hours_list[i]}));
        }
    }

    return 0;
}

int test_string_format_chrono_minutes() {
    using namespace std::chrono;

    static constexpr int minutes_list[] = {0, 6, 23, 45};
    static constexpr const char* minutes_mm_list[] = {"00", "06", "23", "45"};

    for (unsigned i = 0; i < sizeof(minutes_list) / sizeof(minutes_list[0]); ++i) {
        VERIFY(uxs::format("{:%M}", minutes{minutes_list[i]}) == minutes_mm_list[i]);
        VERIFY(uxs::format("{:%M}", minutes{minutes_list[i]}) == std::format("{:%M}", minutes{minutes_list[i]}));
    }

    return 0;
}

int test_string_format_chrono_seconds() {
    using namespace std::chrono;

    static constexpr int seconds_list[] = {0, 6, 23, 45};
    static constexpr const char* seconds_ss_list[] = {"00", "06", "23", "45"};

    for (unsigned i = 0; i < sizeof(seconds_list) / sizeof(seconds_list[0]); ++i) {
        VERIFY(uxs::format("{:%S}", seconds{seconds_list[i]}) == seconds_ss_list[i]);
        VERIFY(uxs::format("{:%S}", seconds{seconds_list[i]}) == std::format("{:%S}", seconds{seconds_list[i]}));
    }

#    if !defined(_LIBCPP_VERSION) || _LIBCPP_VERSION >= DESIRED_LIBCPP_VERSION
    VERIFY(uxs::format("{:%T}", hh_mm_ss{seconds{23475765}}) == std::format("{:%T}", hh_mm_ss{seconds{23475765}}));
    VERIFY(uxs::format("{:%T}", hh_mm_ss{seconds{-23475765}}) == std::format("{:%T}", hh_mm_ss{seconds{-23475765}}));
    VERIFY(uxs::format("{:%T}", hh_mm_ss{milliseconds{23475765}}) ==
           std::format("{:%T}", hh_mm_ss{milliseconds{23475765}}));
    VERIFY(uxs::format("{:%T}", hh_mm_ss{milliseconds{-23475765}}) ==
           std::format("{:%T}", hh_mm_ss{milliseconds{-23475765}}));
    VERIFY(uxs::format("{:%T}", milliseconds{23475765}) == std::format("{:%T}", milliseconds{23475765}));
    VERIFY(uxs::format("{:%T}", milliseconds{-23475765}) == std::format("{:%T}", milliseconds{-23475765}));
    VERIFY(uxs::format("{:%S}", milliseconds{23475765}) == std::format("{:%S}", milliseconds{23475765}));
    VERIFY(uxs::format("{:%S}", milliseconds{-23475765}) == std::format("{:%S}", milliseconds{-23475765}));
    VERIFY(uxs::format("{:%S}", milliseconds{1}) == std::format("{:%S}", milliseconds{1}));
    VERIFY(uxs::format("{:%S}", milliseconds{-1}) == std::format("{:%S}", milliseconds{-1}));
    VERIFY(uxs::format("{:%OS}", milliseconds{-23475765}) == std::format("{:%OS}", milliseconds{-23475765}));
#    endif

    return 0;
}

int test_string_format_chrono_duration() {
    using namespace std::chrono;

#    if !defined(__GNUC__)
    VERIFY(uxs::format("{}", duration<int, std::atto>{1}) == "1as");
#    endif
    VERIFY(uxs::format("{}", duration<int, std::femto>{1}) == "1fs");
    VERIFY(uxs::format("{}", duration<int, std::pico>{1}) == "1ps");
    VERIFY(uxs::format("{}", duration<int, std::nano>{1}) == "1ns");
    VERIFY(uxs::format("{}", duration<int, std::micro>{1}) == "1us");
    VERIFY(uxs::format("{}", duration<int, std::milli>{1}) == "1ms");
    VERIFY(uxs::format("{}", duration<int, std::deci>{1}) == "1ds");
    VERIFY(uxs::format("{}", duration<int>{1}) == "1s");
    VERIFY(uxs::format("{}", duration<int, std::deca>{1}) == "1das");
    VERIFY(uxs::format("{}", duration<int, std::hecto>{1}) == "1hs");
    VERIFY(uxs::format("{}", duration<int, std::kilo>{1}) == "1ks");
    VERIFY(uxs::format("{}", duration<int, std::mega>{1}) == "1Ms");
    VERIFY(uxs::format("{}", duration<int, std::giga>{1}) == "1Gs");
    VERIFY(uxs::format("{}", duration<int, std::tera>{1}) == "1Ts");
    VERIFY(uxs::format("{}", duration<int, std::peta>{1}) == "1Ps");
    VERIFY(uxs::format("{}", duration<int, std::exa>{1}) == "1Es");
    VERIFY(uxs::format("{}", duration<int, std::ratio<60>>{1}) == "1min");
    VERIFY(uxs::format("{}", duration<int, std::ratio<3600>>{1}) == "1h");
    VERIFY(uxs::format("{}", duration<int, std::ratio<86400>>{1}) == "1d");
    VERIFY(uxs::format("{}", duration<int, std::ratio<45>>{1}) == "1[45]s");
    VERIFY(uxs::format("{}", duration<int, std::ratio<15, 2>>{1}) == "1[15/2]s");
    VERIFY(uxs::format("{}", duration<int, std::ratio<45, 5>>{1}) == "1[9]s");

#    if !defined(__GNUC__)
    VERIFY(std::format("{}", duration<int, std::atto>{1}) == "1as");
    VERIFY(std::format("{}", duration<int, std::femto>{1}) == "1fs");
#    endif
    VERIFY(std::format("{}", duration<int, std::pico>{1}) == "1ps");
    VERIFY(std::format("{}", duration<int, std::nano>{1}) == "1ns");
    // VERIFY(std::format("{}", duration<int, std::micro>{1}) == "1us"); // us?
    VERIFY(std::format("{}", duration<int, std::milli>{1}) == "1ms");
    VERIFY(std::format("{}", duration<int, std::deci>{1}) == "1ds");
    VERIFY(std::format("{}", duration<int>{1}) == "1s");
    VERIFY(std::format("{}", duration<int, std::deca>{1}) == "1das");
    VERIFY(std::format("{}", duration<int, std::hecto>{1}) == "1hs");
    VERIFY(std::format("{}", duration<int, std::kilo>{1}) == "1ks");
    VERIFY(std::format("{}", duration<int, std::mega>{1}) == "1Ms");
    VERIFY(std::format("{}", duration<int, std::giga>{1}) == "1Gs");
    VERIFY(std::format("{}", duration<int, std::tera>{1}) == "1Ts");
    VERIFY(std::format("{}", duration<int, std::peta>{1}) == "1Ps");
    VERIFY(std::format("{}", duration<int, std::exa>{1}) == "1Es");
    VERIFY(std::format("{}", duration<int, std::ratio<60>>{1}) == "1min");
    VERIFY(std::format("{}", duration<int, std::ratio<3600>>{1}) == "1h");
    VERIFY(std::format("{}", duration<int, std::ratio<86400>>{1}) == "1d");
    VERIFY(std::format("{}", duration<int, std::ratio<45>>{1}) == "1[45]s");
    VERIFY(std::format("{}", duration<int, std::ratio<45, 6>>{1}) == "1[15/2]s");
    VERIFY(std::format("{}", duration<int, std::ratio<45, 5>>{1}) == "1[9]s");

    VERIFY(uxs::format("{:.6}", duration<double, std::milli>{2341.456}) == "2341.46ms");
#    if !defined(_LIBCPP_VERSION) || _LIBCPP_VERSION >= DESIRED_LIBCPP_VERSION
    VERIFY(std::format("{:.6}", duration<double, std::milli>{2341.456}) == "2341.46ms");
#    endif

    return 0;
}

template<typename Clock, typename Duration>
auto cast_from_system_clock(std::chrono::sys_time<Duration> t) -> decltype(Clock::from_sys(t)) {
    return Clock::from_sys(t);
}

template<typename Clock, typename Duration>
auto cast_from_system_clock(std::chrono::sys_time<Duration> t) -> decltype(Clock::from_utc(
    decltype(Clock::to_utc(std::chrono::time_point<Clock, Duration>{}))::clock::from_sys(t))) {
    return Clock::from_utc(decltype(Clock::to_utc(std::chrono::time_point<Clock, Duration>{}))::clock::from_sys(t));
}

int test_string_format_chrono_date_time() {
    using namespace std::chrono;
    using namespace std::chrono_literals;

    {
        const time_point<system_clock> epoch;
        const time_point<system_clock> t{sys_days{year_month_day{1940y, June, 26d}} + 3h + 45min + 23s + 15ms};

        VERIFY(uxs::format("{:%F}", epoch) == std::format("{:%F}", epoch));
        VERIFY(uxs::format("{:%R}", epoch) == std::format("{:%R}", epoch));
        VERIFY(uxs::format("{:%T}", epoch) == std::format("{:%T}", epoch));
        VERIFY(uxs::format("{:%z}", epoch) == std::format("{:%z}", epoch));
        VERIFY(uxs::format("{:%Z}", epoch) == std::format("{:%Z}", epoch));
        VERIFY(uxs::format("{}", epoch) == std::format("{}", epoch));
        VERIFY(uxs::format("{:%F}", t) == std::format("{:%F}", t));
        VERIFY(uxs::format("{:%R}", t) == std::format("{:%R}", t));
        VERIFY(uxs::format("{:%T}", t) == std::format("{:%T}", t));
        VERIFY(uxs::format("{:%z}", t) == std::format("{:%z}", t));
        VERIFY(uxs::format("{:%Z}", t) == std::format("{:%Z}", t));
        VERIFY(uxs::format("{}", t) == std::format("{}", t));
    }

#    if _MSC_VER || __GLIBCXX__ >= 20240904 || _LIBCPP_VERSION >= DESIRED_LIBCPP_VERSION
    {
        const time_point<utc_clock> epoch;
        const time_point<utc_clock> t{
            cast_from_system_clock<utc_clock>(sys_days{year_month_day{1940y, June, 26d}} + 3h + 45min + 23s + 15ms)};
        VERIFY(uxs::format("{:%F}", epoch) == std::format("{:%F}", epoch));
        VERIFY(uxs::format("{:%R}", epoch) == std::format("{:%R}", epoch));
        VERIFY(uxs::format("{:%T}", epoch) == std::format("{:%T}", epoch));
        VERIFY(uxs::format("{:%z}", epoch) == std::format("{:%z}", epoch));
        VERIFY(uxs::format("{:%Z}", epoch) == std::format("{:%Z}", epoch));
        VERIFY(uxs::format("{}", epoch) == std::format("{}", epoch));
        VERIFY(uxs::format("{:%F}", t) == std::format("{:%F}", t));
        VERIFY(uxs::format("{:%R}", t) == std::format("{:%R}", t));
        VERIFY(uxs::format("{:%T}", t) == std::format("{:%T}", t));
        VERIFY(uxs::format("{:%z}", t) == std::format("{:%z}", t));
        VERIFY(uxs::format("{:%Z}", t) == std::format("{:%Z}", t));
        VERIFY(uxs::format("{}", t) == std::format("{}", t));
    }

    {
        const time_point<tai_clock> epoch;
        const time_point<tai_clock> t{
            cast_from_system_clock<tai_clock>(sys_days{year_month_day{1940y, June, 26d}} + 3h + 45min + 23s + 15ms)};
        VERIFY(uxs::format("{:%F}", epoch) == std::format("{:%F}", epoch));
        VERIFY(uxs::format("{:%R}", epoch) == std::format("{:%R}", epoch));
        VERIFY(uxs::format("{:%T}", epoch) == std::format("{:%T}", epoch));
        VERIFY(uxs::format("{:%z}", epoch) == std::format("{:%z}", epoch));
        VERIFY(uxs::format("{:%Z}", epoch) == std::format("{:%Z}", epoch));
        VERIFY(uxs::format("{}", epoch) == std::format("{}", epoch));
        VERIFY(uxs::format("{:%F}", t) == std::format("{:%F}", t));
        VERIFY(uxs::format("{:%R}", t) == std::format("{:%R}", t));
        VERIFY(uxs::format("{:%T}", t) == std::format("{:%T}", t));
        VERIFY(uxs::format("{:%z}", t) == std::format("{:%z}", t));
        VERIFY(uxs::format("{:%Z}", t) == std::format("{:%Z}", t));
        VERIFY(uxs::format("{}", t) == std::format("{}", t));
    }

    {
        const time_point<gps_clock> epoch;
        const time_point<gps_clock> t{
            cast_from_system_clock<gps_clock>(sys_days{year_month_day{1940y, June, 26d}} + 3h + 45min + 23s + 15ms)};
        VERIFY(uxs::format("{:%F}", epoch) == std::format("{:%F}", epoch));
        VERIFY(uxs::format("{:%R}", epoch) == std::format("{:%R}", epoch));
        VERIFY(uxs::format("{:%T}", epoch) == std::format("{:%T}", epoch));
        VERIFY(uxs::format("{:%z}", epoch) == std::format("{:%z}", epoch));
        VERIFY(uxs::format("{:%Z}", epoch) == std::format("{:%Z}", epoch));
        VERIFY(uxs::format("{}", epoch) == std::format("{}", epoch));
        VERIFY(uxs::format("{:%F}", t) == std::format("{:%F}", t));
        VERIFY(uxs::format("{:%R}", t) == std::format("{:%R}", t));
        VERIFY(uxs::format("{:%T}", t) == std::format("{:%T}", t));
        VERIFY(uxs::format("{:%z}", t) == std::format("{:%z}", t));
        VERIFY(uxs::format("{:%Z}", t) == std::format("{:%Z}", t));
        VERIFY(uxs::format("{}", t) == std::format("{}", t));
    }
#    endif

    {
        const time_point<file_clock> epoch;
        const time_point<file_clock> t{
            cast_from_system_clock<file_clock>(sys_days{year_month_day{1940y, June, 26d}} + 3h + 45min + 23s + 15ms)};
        VERIFY(uxs::format("{:%F}", epoch) == std::format("{:%F}", epoch));
        VERIFY(uxs::format("{:%R}", epoch) == std::format("{:%R}", epoch));
        VERIFY(uxs::format("{:%T}", epoch) == std::format("{:%T}", epoch));
        VERIFY(uxs::format("{:%z}", epoch) == std::format("{:%z}", epoch));
        VERIFY(uxs::format("{:%Z}", epoch) == std::format("{:%Z}", epoch));
        VERIFY(uxs::format("{}", epoch) == std::format("{}", epoch));
        VERIFY(uxs::format("{:%F}", t) == std::format("{:%F}", t));
        VERIFY(uxs::format("{:%R}", t) == std::format("{:%R}", t));
        VERIFY(uxs::format("{:%T}", t) == std::format("{:%T}", t));
        VERIFY(uxs::format("{:%z}", t) == std::format("{:%z}", t));
        VERIFY(uxs::format("{:%Z}", t) == std::format("{:%Z}", t));
        VERIFY(uxs::format("{}", t) == std::format("{}", t));
    }

    {
        const local_time<system_clock::duration> epoch;
        const local_time<system_clock::duration> t{local_days{year_month_day{1940y, June, 26d}} + 3h + 45min + 23s +
                                                   15ms};
        VERIFY(uxs::format("{:%F}", epoch) == std::format("{:%F}", epoch));
        VERIFY(uxs::format("{:%R}", epoch) == std::format("{:%R}", epoch));
        VERIFY(uxs::format("{:%T}", epoch) == std::format("{:%T}", epoch));
        VERIFY(uxs::format("{}", epoch) == std::format("{}", epoch));
        MUST_THROW((void)uxs::format(uxs::runtime_format("{:%z}"), epoch));
        MUST_THROW((void)uxs::format(uxs::runtime_format("{:%Z}"), epoch));
        VERIFY(uxs::format("{:%F}", t) == std::format("{:%F}", t));
        VERIFY(uxs::format("{:%R}", t) == std::format("{:%R}", t));
        VERIFY(uxs::format("{:%T}", t) == std::format("{:%T}", t));
        VERIFY(uxs::format("{}", t) == std::format("{}", t));
        VERIFY(uxs::format(L"{}", t) == std::format(L"{}", t));
    }

#    if _MSC_VER || __GLIBCXX__ >= 20240904 || _LIBCPP_VERSION >= DESIRED_LIBCPP_VERSION
    {
        const zoned_time<system_clock::duration> epoch;
        const zoned_time<system_clock::duration> t{
            current_zone(), sys_days{year_month_day{1940y, June, 26d}} + 3h + 45min + 23s + 15ms};

        VERIFY(uxs::format("{:%F}", epoch) == std::format("{:%F}", epoch));
        VERIFY(uxs::format("{:%R}", epoch) == std::format("{:%R}", epoch));
        VERIFY(uxs::format("{:%T}", epoch) == std::format("{:%T}", epoch));
        VERIFY(uxs::format("{:%z}", epoch) == std::format("{:%z}", epoch));
        VERIFY(uxs::format("{:%Z}", epoch) == std::format("{:%Z}", epoch));

#        if !defined(__GNUC__)
        VERIFY(uxs::format("{}", epoch) == std::format("{}", epoch));
        VERIFY(uxs::format(L"{}", epoch) == std::format(L"{}", epoch));
#        endif

        VERIFY(uxs::format("{:%F}", t) == std::format("{:%F}", t));
        VERIFY(uxs::format("{:%R}", t) == std::format("{:%R}", t));
        VERIFY(uxs::format("{:%T}", t) == std::format("{:%T}", t));
        VERIFY(uxs::format("{:%z}", t) == std::format("{:%z}", t));
        VERIFY(uxs::format("{:%Z}", t) == std::format("{:%Z}", t));

#        if !defined(__GNUC__)
        VERIFY(uxs::format("{}", t.get_info()) == std::format("{}", t.get_info()));
        VERIFY(uxs::format("L{}", t.get_info()) == std::format("L{}", t.get_info()));
        VERIFY(uxs::format("{:%z}", t.get_info()) == std::format("{:%z}", t.get_info()));
        VERIFY(uxs::format("{:%Z}", t.get_info()) == std::format("{:%Z}", t.get_info()));
#        endif

#        if !defined(__GNUC__)
        VERIFY(uxs::format("{}", t) == std::format("{}", t));
        VERIFY(uxs::format(L"{}", t) == std::format(L"{}", t));
#        endif

#        if !defined(__GNUC__)
        std::chrono::local_info li0{0, t.get_info()};
        std::chrono::local_info li1{1, t.get_info()};
        std::chrono::local_info li2{2, t.get_info()};
        std::chrono::local_info li3{100, t.get_info()};
        std::chrono::local_info li4{-100, t.get_info()};

        VERIFY(uxs::format("{}", li0) == std::format("{}", li0));
        VERIFY(uxs::format("{}", li1) == std::format("{}", li1));
        VERIFY(uxs::format("{}", li2) == std::format("{}", li2));
        VERIFY(uxs::format("{}", li3) == std::format("{}", li3));
        VERIFY(uxs::format("{}", li4) == std::format("{}", li4));
        VERIFY(uxs::format("{:%Z}", li0) == std::format("{:%Z}", li0));
#        endif

        const zoned_time<system_clock::duration> now{current_zone(), system_clock::now()};

#        if !defined(__GNUC__)
        VERIFY(uxs::format("{:%c %Z}.", now) == std::format("{:%c %Z}.", now));
        VERIFY(uxs::format("{}.", now) == std::format("{}.", now));
#        endif
    }
#    endif

    return 0;
}

ADD_TEST_CASE("", "string format", test_string_format_chrono_year);
ADD_TEST_CASE("", "string format", test_string_format_chrono_month);
ADD_TEST_CASE("", "string format", test_string_format_chrono_day);
ADD_TEST_CASE("", "string format", test_string_format_chrono_weekday);
ADD_TEST_CASE("", "string format", test_string_format_chrono_compound);
ADD_TEST_CASE("", "string format", test_string_format_chrono_hours);
ADD_TEST_CASE("", "string format", test_string_format_chrono_minutes);
ADD_TEST_CASE("", "string format", test_string_format_chrono_seconds);
ADD_TEST_CASE("", "string format", test_string_format_chrono_duration);
ADD_TEST_CASE("", "string format", test_string_format_chrono_date_time);
#endif

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
                      [](char* first, char* /*last*/, int i) {
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
                      [](char* first, char* /*last*/, int i) {
                          return std::sprintf(first, "%0.10f:%04d:%+g:%s:%p:%c:%%\n", 1.234 * i, i, 3.13 * i, "str",
                                              reinterpret_cast<void*>(static_cast<uintptr_t>(i)),
                                              static_cast<char>('A' + i % 26));
                      },
                      perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<std::ostringstream> format string", ([]() {
                  return perf_format_to_string(
                      [](char* /*first*/, char* /*last*/, int i) {
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
                      [](char* first, char* /*last*/, int i) {
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
                      [](char* first, char* /*last*/, int i) {
                          return static_cast<size_t>(fmt::format_to(first, "{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234 * i,
                                                                    i, 3.13 * i, "str",
                                                                    reinterpret_cast<void*>(static_cast<uintptr_t>(i)),
                                                                    static_cast<char>('A' + i % 26)) -
                                                     first);
                      },
                      perf_N_secs);
              }));

}  // namespace
