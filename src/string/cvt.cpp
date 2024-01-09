#if defined(_MSC_VER)
#    define _CRT_SECURE_NO_WARNINGS
#endif

#include "fmt/compile.h"
#include "fmt/format.h"
#include "milo/dtoa_milo.h"
#include "test_suite.h"

#include "uxs/vector.h"

#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <locale>
#include <random>
#include <thread>

#if __cplusplus >= 201703L && UXS_HAS_INCLUDE(<charconv>)
#    include <charconv>
#    define has_cpp_lib_to_chars 1
#    if !defined(_LIBCPP_VERSION) || _LIBCPP_VERSION >= 180000
#        define has_from_chars_implementation_for_floats
#    endif
#endif

#if __cplusplus >= 202002L && UXS_HAS_INCLUDE(<format>)
#    include <format>
#    define has_cpp_lib_format 1
#endif

using namespace uxs_test_suite;

extern unsigned g_proc_num;

static_assert(uxs::has_string_parser<bool, char>::value, "");
static_assert(uxs::has_string_parser<char, char>::value, "");
static_assert(!uxs::has_string_parser<wchar_t, char>::value, "");
static_assert(uxs::has_string_parser<signed char, char>::value, "");
static_assert(uxs::has_string_parser<signed short, char>::value, "");
static_assert(uxs::has_string_parser<signed, char>::value, "");
static_assert(uxs::has_string_parser<signed long, char>::value, "");
static_assert(uxs::has_string_parser<signed long long, char>::value, "");
static_assert(uxs::has_string_parser<unsigned char, char>::value, "");
static_assert(uxs::has_string_parser<unsigned short, char>::value, "");
static_assert(uxs::has_string_parser<unsigned, char>::value, "");
static_assert(uxs::has_string_parser<unsigned long, char>::value, "");
static_assert(uxs::has_string_parser<unsigned long long, char>::value, "");
static_assert(uxs::has_string_parser<float, char>::value, "");
static_assert(uxs::has_string_parser<double, char>::value, "");
static_assert(uxs::has_string_parser<long double, char>::value, "");

static_assert(uxs::has_string_parser<bool, wchar_t>::value, "");
static_assert(!uxs::has_string_parser<char, wchar_t>::value, "");
static_assert(uxs::has_string_parser<wchar_t, wchar_t>::value, "");
static_assert(uxs::has_string_parser<signed char, wchar_t>::value, "");
static_assert(uxs::has_string_parser<signed short, wchar_t>::value, "");
static_assert(uxs::has_string_parser<signed, wchar_t>::value, "");
static_assert(uxs::has_string_parser<signed long, wchar_t>::value, "");
static_assert(uxs::has_string_parser<signed long long, wchar_t>::value, "");
static_assert(uxs::has_string_parser<unsigned char, wchar_t>::value, "");
static_assert(uxs::has_string_parser<unsigned short, wchar_t>::value, "");
static_assert(uxs::has_string_parser<unsigned, wchar_t>::value, "");
static_assert(uxs::has_string_parser<unsigned long, wchar_t>::value, "");
static_assert(uxs::has_string_parser<unsigned long long, wchar_t>::value, "");
static_assert(uxs::has_string_parser<float, wchar_t>::value, "");
static_assert(uxs::has_string_parser<double, wchar_t>::value, "");
static_assert(uxs::has_string_parser<long double, wchar_t>::value, "");

static_assert(uxs::has_formatter<bool, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<char, uxs::membuffer>::value, "");
static_assert(!uxs::has_formatter<wchar_t, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<signed char, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<signed short, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<signed, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<signed long, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<signed long long, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<unsigned char, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<unsigned short, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<unsigned, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<unsigned long, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<unsigned long long, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<float, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<double, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<long double, uxs::membuffer>::value, "");

static_assert(uxs::has_formatter<bool, uxs::wmembuffer>::value, "");
static_assert(!uxs::has_formatter<char, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<wchar_t, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<signed char, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<signed short, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<signed, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<signed long, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<signed long long, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<unsigned char, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<unsigned short, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<unsigned, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<unsigned long, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<unsigned long long, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<float, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<double, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<long double, uxs::wmembuffer>::value, "");

#if defined(_MSC_VER)
static_assert(uxs::has_string_parser<signed __int64, char>::value, "");
static_assert(uxs::has_string_parser<unsigned __int64, char>::value, "");
static_assert(uxs::has_string_parser<signed __int64, wchar_t>::value, "");
static_assert(uxs::has_string_parser<unsigned __int64, wchar_t>::value, "");
static_assert(uxs::has_formatter<signed __int64, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<unsigned __int64, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<signed __int64, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<unsigned __int64, uxs::wmembuffer>::value, "");
#endif  // defined(_MSC_VER)

template<typename TyTo, typename TyFrom>
TyTo safe_reinterpret(const TyFrom& v) {
    static_assert(sizeof(TyTo) == sizeof(TyFrom), "bad reinterpret");
    return *reinterpret_cast<const TyTo*>(&v);
}

namespace {

//-----------------------------------------------------------------------------
// Sanity tests

int test_string_cvt_dec() {  // decimal representation
    // no alignment
    VERIFY(uxs::format("{:d}", 1452346510) == "1452346510");
    VERIFY(uxs::format("{:+d}", 1452346510) == "+1452346510");
    VERIFY(uxs::format("{: d}", 1452346510) == " 1452346510");
    VERIFY(uxs::format("{:d}", 1452346510u) == "1452346510");
    VERIFY(uxs::format("{:d}", -25510) == "-25510");
    VERIFY(uxs::format("{:d}", static_cast<unsigned>(-25510)) == "4294941786");
    VERIFY(uxs::format("{:#d}", 1452346510) == "1452346510");
    VERIFY(uxs::format("{:+#d}", 1452346510) == "+1452346510");
    VERIFY(uxs::format("{: #d}", 1452346510) == " 1452346510");
    VERIFY(uxs::format("{:#d}", 1452346510u) == "1452346510");
    VERIFY(uxs::format("{:#d}", -25510) == "-25510");
    VERIFY(uxs::format("{:#d}", static_cast<unsigned>(-25510)) == "4294941786");

    // small width
    VERIFY(uxs::format("{:4d}", 1452346510) == "1452346510");
    VERIFY(uxs::format("{:+4d}", 1452346510) == "+1452346510");
    VERIFY(uxs::format("{: 4d}", 1452346510) == " 1452346510");
    VERIFY(uxs::format("{:4d}", 1452346510u) == "1452346510");
    VERIFY(uxs::format("{:4d}", -25510) == "-25510");
    VERIFY(uxs::format("{:4d}", static_cast<unsigned>(-25510)) == "4294941786");
    VERIFY(uxs::format("{:#4d}", 1452346510) == "1452346510");
    VERIFY(uxs::format("{:+#4d}", 1452346510) == "+1452346510");
    VERIFY(uxs::format("{: #4d}", 1452346510) == " 1452346510");
    VERIFY(uxs::format("{:#4d}", 1452346510u) == "1452346510");
    VERIFY(uxs::format("{:#4d}", -25510) == "-25510");
    VERIFY(uxs::format("{:#4d}", static_cast<unsigned>(-25510)) == "4294941786");

    // default alignment
    VERIFY(uxs::format("{:15d}", 1452346510) == "     1452346510");
    VERIFY(uxs::format("{:+15d}", 1452346510) == "    +1452346510");
    VERIFY(uxs::format("{: 15d}", 1452346510) == "     1452346510");
    VERIFY(uxs::format("{:15d}", 1452346510u) == "     1452346510");
    VERIFY(uxs::format("{:15d}", -25510) == "         -25510");
    VERIFY(uxs::format("{:15d}", static_cast<unsigned>(-25510)) == "     4294941786");
    VERIFY(uxs::format("{:#15d}", 1452346510) == "     1452346510");
    VERIFY(uxs::format("{:+#15d}", 1452346510) == "    +1452346510");
    VERIFY(uxs::format("{: #15d}", 1452346510) == "     1452346510");
    VERIFY(uxs::format("{:#15d}", 1452346510u) == "     1452346510");
    VERIFY(uxs::format("{:#15d}", -25510) == "         -25510");
    VERIFY(uxs::format("{:#15d}", static_cast<unsigned>(-25510)) == "     4294941786");

    // right alignment
    VERIFY(uxs::format("{:>15d}", 1452346510) == "     1452346510");
    VERIFY(uxs::format("{:>+15d}", 1452346510) == "    +1452346510");
    VERIFY(uxs::format("{:> 15d}", 1452346510) == "     1452346510");
    VERIFY(uxs::format("{:>15d}", 1452346510u) == "     1452346510");
    VERIFY(uxs::format("{:>15d}", -25510) == "         -25510");
    VERIFY(uxs::format("{:>15d}", static_cast<unsigned>(-25510)) == "     4294941786");
    VERIFY(uxs::format("{:>#15d}", 1452346510) == "     1452346510");
    VERIFY(uxs::format("{:>+#15d}", 1452346510) == "    +1452346510");
    VERIFY(uxs::format("{:> #15d}", 1452346510) == "     1452346510");
    VERIFY(uxs::format("{:>#15d}", 1452346510u) == "     1452346510");
    VERIFY(uxs::format("{:>#15d}", -25510) == "         -25510");
    VERIFY(uxs::format("{:>#15d}", static_cast<unsigned>(-25510)) == "     4294941786");

    // mid alignment
    VERIFY(uxs::format("{:^15d}", 1452346510) == "  1452346510   ");
    VERIFY(uxs::format("{:^+15d}", 1452346510) == "  +1452346510  ");
    VERIFY(uxs::format("{:^ 15d}", 1452346510) == "   1452346510  ");
    VERIFY(uxs::format("{:^15d}", 1452346510u) == "  1452346510   ");
    VERIFY(uxs::format("{:^15d}", -25510) == "    -25510     ");
    VERIFY(uxs::format("{:^15d}", static_cast<unsigned>(-25510)) == "  4294941786   ");
    VERIFY(uxs::format("{:^#15d}", 1452346510) == "  1452346510   ");
    VERIFY(uxs::format("{:^+#15d}", 1452346510) == "  +1452346510  ");
    VERIFY(uxs::format("{:^ #15d}", 1452346510) == "   1452346510  ");
    VERIFY(uxs::format("{:^#15d}", 1452346510u) == "  1452346510   ");
    VERIFY(uxs::format("{:^#15d}", -25510) == "    -25510     ");
    VERIFY(uxs::format("{:^#15d}", static_cast<unsigned>(-25510)) == "  4294941786   ");

    // left alignment
    VERIFY(uxs::format("{:<15d}", 1452346510) == "1452346510     ");
    VERIFY(uxs::format("{:<+15d}", 1452346510) == "+1452346510    ");
    VERIFY(uxs::format("{:< 15d}", 1452346510) == " 1452346510    ");
    VERIFY(uxs::format("{:<15d}", 1452346510u) == "1452346510     ");
    VERIFY(uxs::format("{:<15d}", -25510) == "-25510         ");
    VERIFY(uxs::format("{:<15d}", static_cast<unsigned>(-25510)) == "4294941786     ");
    VERIFY(uxs::format("{:<#15d}", 1452346510) == "1452346510     ");
    VERIFY(uxs::format("{:<+#15d}", 1452346510) == "+1452346510    ");
    VERIFY(uxs::format("{:< #15d}", 1452346510) == " 1452346510    ");
    VERIFY(uxs::format("{:<#15d}", 1452346510u) == "1452346510     ");
    VERIFY(uxs::format("{:<#15d}", -25510) == "-25510         ");
    VERIFY(uxs::format("{:<#15d}", static_cast<unsigned>(-25510)) == "4294941786     ");

    // zero fill
    VERIFY(uxs::format("{:015d}", 1452346510) == "000001452346510");
    VERIFY(uxs::format("{:+015d}", 1452346510) == "+00001452346510");
    VERIFY(uxs::format("{: 015d}", 1452346510) == " 00001452346510");
    VERIFY(uxs::format("{:015d}", 1452346510u) == "000001452346510");
    VERIFY(uxs::format("{:015d}", -25510) == "-00000000025510");
    VERIFY(uxs::format("{:015d}", static_cast<unsigned>(-25510)) == "000004294941786");
    VERIFY(uxs::format("{:#015d}", 1452346510) == "000001452346510");
    VERIFY(uxs::format("{:+#015d}", 1452346510) == "+00001452346510");
    VERIFY(uxs::format("{: #015d}", 1452346510) == " 00001452346510");
    VERIFY(uxs::format("{:#015d}", 1452346510u) == "000001452346510");
    VERIFY(uxs::format("{:#015d}", -25510) == "-00000000025510");
    VERIFY(uxs::format("{:#015d}", static_cast<unsigned>(-25510)) == "000004294941786");
    VERIFY(uxs::format("{:07d}", -25510) == "-025510");  // aux

    // longest representation
    struct grouping : std::numpunct<char> {
        char_type do_thousands_sep() const override { return '\''; }
        string_type do_grouping() const override { return "\1"; }
    };
    std::locale loc{std::locale::classic(), new grouping};
    VERIFY(uxs::format("{:#d}", 0xffffffffffffffffull) == "18446744073709551615");
    VERIFY(uxs::format("{:#d}", static_cast<long long>(0x8000000000000000ull)) == "-9223372036854775808");
    VERIFY(uxs::format(loc, "{:#Ld}", 0xffffffffffffffffull) == "1'8'4'4'6'7'4'4'0'7'3'7'0'9'5'5'1'6'1'5");
    VERIFY(uxs::format(loc, "{:#Ld}", static_cast<long long>(0x8000000000000000ull)) ==
           "-9'2'2'3'3'7'2'0'3'6'8'5'4'7'7'5'8'0'8");

    return 0;
}

int test_string_cvt_bin() {  // binary representation
    // no alignment
    VERIFY(uxs::format("{:b}", 25510) == "110001110100110");
    VERIFY(uxs::format("{:+b}", 25510) == "+110001110100110");
    VERIFY(uxs::format("{: b}", 25510) == " 110001110100110");
    VERIFY(uxs::format("{:b}", 25510u) == "110001110100110");
    VERIFY(uxs::format("{:b}", -25510) == "-110001110100110");
    VERIFY(uxs::format("{:b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:#b}", 25510) == "0b110001110100110");
    VERIFY(uxs::format("{:+#b}", 25510) == "+0b110001110100110");
    VERIFY(uxs::format("{: #b}", 25510) == " 0b110001110100110");
    VERIFY(uxs::format("{:#b}", 25510u) == "0b110001110100110");
    VERIFY(uxs::format("{:#b}", -25510) == "-0b110001110100110");
    VERIFY(uxs::format("{:#b}", static_cast<unsigned>(-25510)) == "0b11111111111111111001110001011010");

    // small width
    VERIFY(uxs::format("{:4b}", 25510) == "110001110100110");
    VERIFY(uxs::format("{:+4b}", 25510) == "+110001110100110");
    VERIFY(uxs::format("{: 4b}", 25510) == " 110001110100110");
    VERIFY(uxs::format("{:4b}", 25510u) == "110001110100110");
    VERIFY(uxs::format("{:4b}", -25510) == "-110001110100110");
    VERIFY(uxs::format("{:4b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:#4b}", 25510) == "0b110001110100110");
    VERIFY(uxs::format("{:+#4b}", 25510) == "+0b110001110100110");
    VERIFY(uxs::format("{: #4b}", 25510) == " 0b110001110100110");
    VERIFY(uxs::format("{:#4b}", 25510u) == "0b110001110100110");
    VERIFY(uxs::format("{:#4b}", -25510) == "-0b110001110100110");
    VERIFY(uxs::format("{:#4b}", static_cast<unsigned>(-25510)) == "0b11111111111111111001110001011010");

    // default alignment
    VERIFY(uxs::format("{:20b}", 25510) == "     110001110100110");
    VERIFY(uxs::format("{:+20b}", 25510) == "    +110001110100110");
    VERIFY(uxs::format("{: 20b}", 25510) == "     110001110100110");
    VERIFY(uxs::format("{:20b}", 25510u) == "     110001110100110");
    VERIFY(uxs::format("{:20b}", -25510) == "    -110001110100110");
    VERIFY(uxs::format("{:35b}", static_cast<unsigned>(-25510)) == "   11111111111111111001110001011010");
    VERIFY(uxs::format("{:#20b}", 25510) == "   0b110001110100110");
    VERIFY(uxs::format("{:+#20b}", 25510) == "  +0b110001110100110");
    VERIFY(uxs::format("{: #20b}", 25510) == "   0b110001110100110");
    VERIFY(uxs::format("{:#20b}", 25510u) == "   0b110001110100110");
    VERIFY(uxs::format("{:#20b}", -25510) == "  -0b110001110100110");
    VERIFY(uxs::format("{:#35b}", static_cast<unsigned>(-25510)) == " 0b11111111111111111001110001011010");

    // right alignment
    VERIFY(uxs::format("{:>20b}", 25510) == "     110001110100110");
    VERIFY(uxs::format("{:>+20b}", 25510) == "    +110001110100110");
    VERIFY(uxs::format("{:> 20b}", 25510) == "     110001110100110");
    VERIFY(uxs::format("{:>20b}", 25510u) == "     110001110100110");
    VERIFY(uxs::format("{:>20b}", -25510) == "    -110001110100110");
    VERIFY(uxs::format("{:>35b}", static_cast<unsigned>(-25510)) == "   11111111111111111001110001011010");
    VERIFY(uxs::format("{:>#20b}", 25510) == "   0b110001110100110");
    VERIFY(uxs::format("{:>+#20b}", 25510) == "  +0b110001110100110");
    VERIFY(uxs::format("{:> #20b}", 25510) == "   0b110001110100110");
    VERIFY(uxs::format("{:>#20b}", 25510u) == "   0b110001110100110");
    VERIFY(uxs::format("{:>#20b}", -25510) == "  -0b110001110100110");
    VERIFY(uxs::format("{:>#35b}", static_cast<unsigned>(-25510)) == " 0b11111111111111111001110001011010");

    // mid alignment
    VERIFY(uxs::format("{:^20b}", 25510) == "  110001110100110   ");
    VERIFY(uxs::format("{:^+20b}", 25510) == "  +110001110100110  ");
    VERIFY(uxs::format("{:^ 20b}", 25510) == "   110001110100110  ");
    VERIFY(uxs::format("{:^20b}", 25510u) == "  110001110100110   ");
    VERIFY(uxs::format("{:^20b}", -25510) == "  -110001110100110  ");
    VERIFY(uxs::format("{:^35b}", static_cast<unsigned>(-25510)) == " 11111111111111111001110001011010  ");
    VERIFY(uxs::format("{:^#20b}", 25510) == " 0b110001110100110  ");
    VERIFY(uxs::format("{:^+#20b}", 25510) == " +0b110001110100110 ");
    VERIFY(uxs::format("{:^ #20b}", 25510) == "  0b110001110100110 ");
    VERIFY(uxs::format("{:^#20b}", 25510u) == " 0b110001110100110  ");
    VERIFY(uxs::format("{:^#20b}", -25510) == " -0b110001110100110 ");
    VERIFY(uxs::format("{:^#35b}", static_cast<unsigned>(-25510)) == "0b11111111111111111001110001011010 ");

    // left alignment
    VERIFY(uxs::format("{:<20b}", 25510) == "110001110100110     ");
    VERIFY(uxs::format("{:<+20b}", 25510) == "+110001110100110    ");
    VERIFY(uxs::format("{:< 20b}", 25510) == " 110001110100110    ");
    VERIFY(uxs::format("{:<20b}", 25510u) == "110001110100110     ");
    VERIFY(uxs::format("{:<20b}", -25510) == "-110001110100110    ");
    VERIFY(uxs::format("{:<35b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010   ");
    VERIFY(uxs::format("{:<#20b}", 25510) == "0b110001110100110   ");
    VERIFY(uxs::format("{:<+#20b}", 25510) == "+0b110001110100110  ");
    VERIFY(uxs::format("{:< #20b}", 25510) == " 0b110001110100110  ");
    VERIFY(uxs::format("{:<#20b}", 25510u) == "0b110001110100110   ");
    VERIFY(uxs::format("{:<#20b}", -25510) == "-0b110001110100110  ");
    VERIFY(uxs::format("{:<#35b}", static_cast<unsigned>(-25510)) == "0b11111111111111111001110001011010 ");

    // zero fill
    VERIFY(uxs::format("{:020b}", 25510) == "00000110001110100110");
    VERIFY(uxs::format("{:+020b}", 25510) == "+0000110001110100110");
    VERIFY(uxs::format("{: 020b}", 25510) == " 0000110001110100110");
    VERIFY(uxs::format("{:020b}", 25510u) == "00000110001110100110");
    VERIFY(uxs::format("{:020b}", -25510) == "-0000110001110100110");
    VERIFY(uxs::format("{:035b}", static_cast<unsigned>(-25510)) == "00011111111111111111001110001011010");
    VERIFY(uxs::format("{:#020b}", 25510) == "0b000110001110100110");
    VERIFY(uxs::format("{:+#020b}", 25510) == "+0b00110001110100110");
    VERIFY(uxs::format("{: #020b}", 25510) == " 0b00110001110100110");
    VERIFY(uxs::format("{:#020b}", 25510u) == "0b000110001110100110");
    VERIFY(uxs::format("{:#020b}", -25510) == "-0b00110001110100110");
    VERIFY(uxs::format("{:#035b}", static_cast<unsigned>(-25510)) == "0b011111111111111111001110001011010");
    VERIFY(uxs::format("{:016b}", 25510) == "0110001110100110");       // aux
    VERIFY(uxs::format("{:017b}", -25510) == "-0110001110100110");     // aux
    VERIFY(uxs::format("{:#018b}", 25510) == "0b0110001110100110");    // aux
    VERIFY(uxs::format("{:#019b}", -25510) == "-0b0110001110100110");  // aux

    // longest representation
    struct grouping : std::numpunct<char> {
        char_type do_thousands_sep() const override { return '\''; }
        string_type do_grouping() const override { return "\1"; }
    };
    std::locale loc{std::locale::classic(), new grouping};
    VERIFY(uxs::format("{:#b}", 0xffffffffffffffffull) ==
           "0b1111111111111111111111111111111111111111111111111111111111111111");
    VERIFY(uxs::format("{:#b}", static_cast<long long>(0x8000000000000000ull)) ==
           "-0b1000000000000000000000000000000000000000000000000000000000000000");
    VERIFY(uxs::format(loc, "{:#Lb}", 0xffffffffffffffffull) ==
           "0b1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'1'"
           "1'1'1'1'1'1'1'1'1'1'1'1");
    VERIFY(uxs::format(loc, "{:#Lb}", static_cast<long long>(0x8000000000000000ull)) ==
           "-0b1'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'"
           "0'0'0'0'0'0'0'0'0'0'0'0");

    VERIFY(uxs::format("{:#B}", 25510) == "0B110001110100110");  // capital
    return 0;
}

int test_string_cvt_oct() {  // octal representation
    // no alignment
    VERIFY(uxs::format("{:o}", 1452346510) == "12644206216");
    VERIFY(uxs::format("{:+o}", 1452346510) == "+12644206216");
    VERIFY(uxs::format("{: o}", 1452346510) == " 12644206216");
    VERIFY(uxs::format("{:o}", 1452346510u) == "12644206216");
    VERIFY(uxs::format("{:o}", -25510) == "-61646");
    VERIFY(uxs::format("{:o}", static_cast<unsigned>(-25510)) == "37777716132");
    VERIFY(uxs::format("{:#o}", 1452346510) == "012644206216");
    VERIFY(uxs::format("{:+#o}", 1452346510) == "+012644206216");
    VERIFY(uxs::format("{: #o}", 1452346510) == " 012644206216");
    VERIFY(uxs::format("{:#o}", 1452346510u) == "012644206216");
    VERIFY(uxs::format("{:#o}", -25510) == "-061646");
    VERIFY(uxs::format("{:#o}", static_cast<unsigned>(-25510)) == "037777716132");

    // small width
    VERIFY(uxs::format("{:4o}", 1452346510) == "12644206216");
    VERIFY(uxs::format("{:+4o}", 1452346510) == "+12644206216");
    VERIFY(uxs::format("{: 4o}", 1452346510) == " 12644206216");
    VERIFY(uxs::format("{:4o}", 1452346510u) == "12644206216");
    VERIFY(uxs::format("{:4o}", -25510) == "-61646");
    VERIFY(uxs::format("{:4o}", static_cast<unsigned>(-25510)) == "37777716132");
    VERIFY(uxs::format("{:#4o}", 1452346510) == "012644206216");
    VERIFY(uxs::format("{:+#4o}", 1452346510) == "+012644206216");
    VERIFY(uxs::format("{: #4o}", 1452346510) == " 012644206216");
    VERIFY(uxs::format("{:#4o}", 1452346510u) == "012644206216");
    VERIFY(uxs::format("{:#4o}", -25510) == "-061646");
    VERIFY(uxs::format("{:#4o}", static_cast<unsigned>(-25510)) == "037777716132");

    // default alignment
    VERIFY(uxs::format("{:15o}", 1452346510) == "    12644206216");
    VERIFY(uxs::format("{:+15o}", 1452346510) == "   +12644206216");
    VERIFY(uxs::format("{: 15o}", 1452346510) == "    12644206216");
    VERIFY(uxs::format("{:15o}", 1452346510u) == "    12644206216");
    VERIFY(uxs::format("{:15o}", -25510) == "         -61646");
    VERIFY(uxs::format("{:15o}", static_cast<unsigned>(-25510)) == "    37777716132");
    VERIFY(uxs::format("{:#15o}", 1452346510) == "   012644206216");
    VERIFY(uxs::format("{:+#15o}", 1452346510) == "  +012644206216");
    VERIFY(uxs::format("{: #15o}", 1452346510) == "   012644206216");
    VERIFY(uxs::format("{:#15o}", 1452346510u) == "   012644206216");
    VERIFY(uxs::format("{:#15o}", -25510) == "        -061646");
    VERIFY(uxs::format("{:#15o}", static_cast<unsigned>(-25510)) == "   037777716132");

    // right alignment
    VERIFY(uxs::format("{:>15o}", 1452346510) == "    12644206216");
    VERIFY(uxs::format("{:>+15o}", 1452346510) == "   +12644206216");
    VERIFY(uxs::format("{:> 15o}", 1452346510) == "    12644206216");
    VERIFY(uxs::format("{:>15o}", 1452346510u) == "    12644206216");
    VERIFY(uxs::format("{:>15o}", -25510) == "         -61646");
    VERIFY(uxs::format("{:>15o}", static_cast<unsigned>(-25510)) == "    37777716132");
    VERIFY(uxs::format("{:>#15o}", 1452346510) == "   012644206216");
    VERIFY(uxs::format("{:>+#15o}", 1452346510) == "  +012644206216");
    VERIFY(uxs::format("{:> #15o}", 1452346510) == "   012644206216");
    VERIFY(uxs::format("{:>#15o}", 1452346510u) == "   012644206216");
    VERIFY(uxs::format("{:>#15o}", -25510) == "        -061646");
    VERIFY(uxs::format("{:>#15o}", static_cast<unsigned>(-25510)) == "   037777716132");

    // mid alignment
    VERIFY(uxs::format("{:^15o}", 1452346510) == "  12644206216  ");
    VERIFY(uxs::format("{:^+15o}", 1452346510) == " +12644206216  ");
    VERIFY(uxs::format("{:^ 15o}", 1452346510) == "  12644206216  ");
    VERIFY(uxs::format("{:^15o}", 1452346510u) == "  12644206216  ");
    VERIFY(uxs::format("{:^15o}", -25510) == "    -61646     ");
    VERIFY(uxs::format("{:^15o}", static_cast<unsigned>(-25510)) == "  37777716132  ");
    VERIFY(uxs::format("{:^#15o}", 1452346510) == " 012644206216  ");
    VERIFY(uxs::format("{:^+#15o}", 1452346510) == " +012644206216 ");
    VERIFY(uxs::format("{:^ #15o}", 1452346510) == "  012644206216 ");
    VERIFY(uxs::format("{:^#15o}", 1452346510u) == " 012644206216  ");
    VERIFY(uxs::format("{:^#15o}", -25510) == "    -061646    ");
    VERIFY(uxs::format("{:^#15o}", static_cast<unsigned>(-25510)) == " 037777716132  ");

    // left alignment
    VERIFY(uxs::format("{:<15o}", 1452346510) == "12644206216    ");
    VERIFY(uxs::format("{:<+15o}", 1452346510) == "+12644206216   ");
    VERIFY(uxs::format("{:< 15o}", 1452346510) == " 12644206216   ");
    VERIFY(uxs::format("{:<15o}", 1452346510u) == "12644206216    ");
    VERIFY(uxs::format("{:<15o}", -25510) == "-61646         ");
    VERIFY(uxs::format("{:<15o}", static_cast<unsigned>(-25510)) == "37777716132    ");
    VERIFY(uxs::format("{:<#15o}", 1452346510) == "012644206216   ");
    VERIFY(uxs::format("{:<+#15o}", 1452346510) == "+012644206216  ");
    VERIFY(uxs::format("{:< #15o}", 1452346510) == " 012644206216  ");
    VERIFY(uxs::format("{:<#15o}", 1452346510u) == "012644206216   ");
    VERIFY(uxs::format("{:<#15o}", -25510) == "-061646        ");
    VERIFY(uxs::format("{:<#15o}", static_cast<unsigned>(-25510)) == "037777716132   ");

    // zero fill
    VERIFY(uxs::format("{:015o}", 1452346510) == "000012644206216");
    VERIFY(uxs::format("{:+015o}", 1452346510) == "+00012644206216");
    VERIFY(uxs::format("{: 015o}", 1452346510) == " 00012644206216");
    VERIFY(uxs::format("{:015o}", 1452346510u) == "000012644206216");
    VERIFY(uxs::format("{:015o}", -25510) == "-00000000061646");
    VERIFY(uxs::format("{:015o}", static_cast<unsigned>(-25510)) == "000037777716132");
    VERIFY(uxs::format("{:#015o}", 1452346510) == "000012644206216");
    VERIFY(uxs::format("{:+#015o}", 1452346510) == "+00012644206216");
    VERIFY(uxs::format("{: #015o}", 1452346510) == " 00012644206216");
    VERIFY(uxs::format("{:#015o}", 1452346510u) == "000012644206216");
    VERIFY(uxs::format("{:#015o}", -25510) == "-00000000061646");
    VERIFY(uxs::format("{:#015o}", static_cast<unsigned>(-25510)) == "000037777716132");
    VERIFY(uxs::format("{:06o}", 25510) == "061646");      // aux
    VERIFY(uxs::format("{:07o}", -25510) == "-061646");    // aux
    VERIFY(uxs::format("{:#07o}", 25510) == "0061646");    // aux
    VERIFY(uxs::format("{:#08o}", -25510) == "-0061646");  // aux

    // longest representation
    struct grouping : std::numpunct<char> {
        char_type do_thousands_sep() const override { return '\''; }
        string_type do_grouping() const override { return "\1"; }
    };
    std::locale loc{std::locale::classic(), new grouping};
    VERIFY(uxs::format("{:#o}", 0xffffffffffffffffull) == "01777777777777777777777");
    VERIFY(uxs::format("{:#o}", static_cast<long long>(0x8000000000000000ull)) == "-01000000000000000000000");
    VERIFY(uxs::format(loc, "{:#Lo}", 0xffffffffffffffffull) == "01'7'7'7'7'7'7'7'7'7'7'7'7'7'7'7'7'7'7'7'7'7");
    VERIFY(uxs::format(loc, "{:#Lo}", static_cast<long long>(0x8000000000000000ull)) ==
           "-01'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0");

    return 0;
}

int test_string_cvt_hex() {  // hexadecimal representation
    // no alignment
    VERIFY(uxs::format("{:x}", 1452346510) == "56910c8e");
    VERIFY(uxs::format("{:+x}", 1452346510) == "+56910c8e");
    VERIFY(uxs::format("{: x}", 1452346510) == " 56910c8e");
    VERIFY(uxs::format("{:x}", 1452346510u) == "56910c8e");
    VERIFY(uxs::format("{:x}", -25510) == "-63a6");
    VERIFY(uxs::format("{:x}", static_cast<unsigned>(-25510)) == "ffff9c5a");
    VERIFY(uxs::format("{:#x}", 1452346510) == "0x56910c8e");
    VERIFY(uxs::format("{:+#x}", 1452346510) == "+0x56910c8e");
    VERIFY(uxs::format("{: #x}", 1452346510) == " 0x56910c8e");
    VERIFY(uxs::format("{:#x}", 1452346510u) == "0x56910c8e");
    VERIFY(uxs::format("{:#x}", -25510) == "-0x63a6");
    VERIFY(uxs::format("{:#x}", static_cast<unsigned>(-25510)) == "0xffff9c5a");

    // small width
    VERIFY(uxs::format("{:4x}", 1452346510) == "56910c8e");
    VERIFY(uxs::format("{:+4x}", 1452346510) == "+56910c8e");
    VERIFY(uxs::format("{: 4x}", 1452346510) == " 56910c8e");
    VERIFY(uxs::format("{:4x}", 1452346510u) == "56910c8e");
    VERIFY(uxs::format("{:4x}", -25510) == "-63a6");
    VERIFY(uxs::format("{:4x}", static_cast<unsigned>(-25510)) == "ffff9c5a");
    VERIFY(uxs::format("{:#4x}", 1452346510) == "0x56910c8e");
    VERIFY(uxs::format("{:+#4x}", 1452346510) == "+0x56910c8e");
    VERIFY(uxs::format("{: #4x}", 1452346510) == " 0x56910c8e");
    VERIFY(uxs::format("{:#4x}", 1452346510u) == "0x56910c8e");
    VERIFY(uxs::format("{:#4x}", -25510) == "-0x63a6");
    VERIFY(uxs::format("{:#4x}", static_cast<unsigned>(-25510)) == "0xffff9c5a");

    // default alignment
    VERIFY(uxs::format("{:15x}", 1452346510) == "       56910c8e");
    VERIFY(uxs::format("{:+15x}", 1452346510) == "      +56910c8e");
    VERIFY(uxs::format("{: 15x}", 1452346510) == "       56910c8e");
    VERIFY(uxs::format("{:15x}", 1452346510u) == "       56910c8e");
    VERIFY(uxs::format("{:15x}", -25510) == "          -63a6");
    VERIFY(uxs::format("{:15x}", static_cast<unsigned>(-25510)) == "       ffff9c5a");
    VERIFY(uxs::format("{:#15x}", 1452346510) == "     0x56910c8e");
    VERIFY(uxs::format("{:+#15x}", 1452346510) == "    +0x56910c8e");
    VERIFY(uxs::format("{: #15x}", 1452346510) == "     0x56910c8e");
    VERIFY(uxs::format("{:#15x}", 1452346510u) == "     0x56910c8e");
    VERIFY(uxs::format("{:#15x}", -25510) == "        -0x63a6");
    VERIFY(uxs::format("{:#15x}", static_cast<unsigned>(-25510)) == "     0xffff9c5a");

    // right alignment
    VERIFY(uxs::format("{:>15x}", 1452346510) == "       56910c8e");
    VERIFY(uxs::format("{:>+15x}", 1452346510) == "      +56910c8e");
    VERIFY(uxs::format("{:> 15x}", 1452346510) == "       56910c8e");
    VERIFY(uxs::format("{:>15x}", 1452346510u) == "       56910c8e");
    VERIFY(uxs::format("{:>15x}", -25510) == "          -63a6");
    VERIFY(uxs::format("{:>15x}", static_cast<unsigned>(-25510)) == "       ffff9c5a");
    VERIFY(uxs::format("{:>#15x}", 1452346510) == "     0x56910c8e");
    VERIFY(uxs::format("{:>+#15x}", 1452346510) == "    +0x56910c8e");
    VERIFY(uxs::format("{:> #15x}", 1452346510) == "     0x56910c8e");
    VERIFY(uxs::format("{:>#15x}", 1452346510u) == "     0x56910c8e");
    VERIFY(uxs::format("{:>#15x}", -25510) == "        -0x63a6");
    VERIFY(uxs::format("{:>#15x}", static_cast<unsigned>(-25510)) == "     0xffff9c5a");

    // mid alignment
    VERIFY(uxs::format("{:^15x}", 1452346510) == "   56910c8e    ");
    VERIFY(uxs::format("{:^+15x}", 1452346510) == "   +56910c8e   ");
    VERIFY(uxs::format("{:^ 15x}", 1452346510) == "    56910c8e   ");
    VERIFY(uxs::format("{:^15x}", 1452346510u) == "   56910c8e    ");
    VERIFY(uxs::format("{:^15x}", -25510) == "     -63a6     ");
    VERIFY(uxs::format("{:^15x}", static_cast<unsigned>(-25510)) == "   ffff9c5a    ");
    VERIFY(uxs::format("{:^#15x}", 1452346510) == "  0x56910c8e   ");
    VERIFY(uxs::format("{:^+#15x}", 1452346510) == "  +0x56910c8e  ");
    VERIFY(uxs::format("{:^ #15x}", 1452346510) == "   0x56910c8e  ");
    VERIFY(uxs::format("{:^#15x}", 1452346510u) == "  0x56910c8e   ");
    VERIFY(uxs::format("{:^#15x}", -25510) == "    -0x63a6    ");
    VERIFY(uxs::format("{:^#15x}", static_cast<unsigned>(-25510)) == "  0xffff9c5a   ");

    // left alignment
    VERIFY(uxs::format("{:<15x}", 1452346510) == "56910c8e       ");
    VERIFY(uxs::format("{:<+15x}", 1452346510) == "+56910c8e      ");
    VERIFY(uxs::format("{:< 15x}", 1452346510) == " 56910c8e      ");
    VERIFY(uxs::format("{:<15x}", 1452346510u) == "56910c8e       ");
    VERIFY(uxs::format("{:<15x}", -25510) == "-63a6          ");
    VERIFY(uxs::format("{:<15x}", static_cast<unsigned>(-25510)) == "ffff9c5a       ");
    VERIFY(uxs::format("{:<#15x}", 1452346510) == "0x56910c8e     ");
    VERIFY(uxs::format("{:<+#15x}", 1452346510) == "+0x56910c8e    ");
    VERIFY(uxs::format("{:< #15x}", 1452346510) == " 0x56910c8e    ");
    VERIFY(uxs::format("{:<#15x}", 1452346510u) == "0x56910c8e     ");
    VERIFY(uxs::format("{:<#15x}", -25510) == "-0x63a6        ");
    VERIFY(uxs::format("{:<#15x}", static_cast<unsigned>(-25510)) == "0xffff9c5a     ");

    // zero fill
    VERIFY(uxs::format("{:015x}", 1452346510) == "000000056910c8e");
    VERIFY(uxs::format("{:+015x}", 1452346510) == "+00000056910c8e");
    VERIFY(uxs::format("{: 015x}", 1452346510) == " 00000056910c8e");
    VERIFY(uxs::format("{:015x}", 1452346510u) == "000000056910c8e");
    VERIFY(uxs::format("{:015x}", -25510) == "-000000000063a6");
    VERIFY(uxs::format("{:015x}", static_cast<unsigned>(-25510)) == "0000000ffff9c5a");
    VERIFY(uxs::format("{:#015x}", 1452346510) == "0x0000056910c8e");
    VERIFY(uxs::format("{:+#015x}", 1452346510) == "+0x000056910c8e");
    VERIFY(uxs::format("{: #015x}", 1452346510) == " 0x000056910c8e");
    VERIFY(uxs::format("{:#015x}", 1452346510u) == "0x0000056910c8e");
    VERIFY(uxs::format("{:#015x}", -25510) == "-0x0000000063a6");
    VERIFY(uxs::format("{:#015x}", static_cast<unsigned>(-25510)) == "0x00000ffff9c5a");
    VERIFY(uxs::format("{:09x}", 1452346510) == "056910c8e");      // aux
    VERIFY(uxs::format("{:06x}", -25510) == "-063a6");             // aux
    VERIFY(uxs::format("{:#011x}", 1452346510) == "0x056910c8e");  // aux
    VERIFY(uxs::format("{:#08x}", -25510) == "-0x063a6");          // aux

    // longest representation
    struct grouping : std::numpunct<char> {
        char_type do_thousands_sep() const override { return '\''; }
        string_type do_grouping() const override { return "\1"; }
    };
    std::locale loc{std::locale::classic(), new grouping};
    VERIFY(uxs::format("{:#x}", 0xffffffffffffffffull) == "0xffffffffffffffff");
    VERIFY(uxs::format("{:#x}", static_cast<long long>(0x8000000000000000ull)) == "-0x8000000000000000");
    VERIFY(uxs::format(loc, "{:#Lx}", 0xffffffffffffffffull) == "0xf'f'f'f'f'f'f'f'f'f'f'f'f'f'f'f");
    VERIFY(uxs::format(loc, "{:#Lx}", static_cast<long long>(0x8000000000000000ull)) ==
           "-0x8'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0");

    VERIFY(uxs::format("{:#X}", 1452346510) == "0X56910C8E");  // capital
    return 0;
}

int test_string_cvt_float() {  // float representation
    // no alignment
    VERIFY(uxs::format("{:g}", 3.1415) == "3.1415");
    VERIFY(uxs::format("{:+g}", 3.1415) == "+3.1415");
    VERIFY(uxs::format("{: g}", 3.1415) == " 3.1415");
    VERIFY(uxs::format("{:g}", -3.1415) == "-3.1415");

    // small width
    VERIFY(uxs::format("{:4g}", 3.1415) == "3.1415");
    VERIFY(uxs::format("{:+4g}", 3.1415) == "+3.1415");
    VERIFY(uxs::format("{: 4g}", 3.1415) == " 3.1415");
    VERIFY(uxs::format("{:4g}", -3.1415) == "-3.1415");

    // default alignment
    VERIFY(uxs::format("{:15g}", 3.1415) == "         3.1415");
    VERIFY(uxs::format("{:+15g}", 3.1415) == "        +3.1415");
    VERIFY(uxs::format("{: 15g}", 3.1415) == "         3.1415");
    VERIFY(uxs::format("{:15g}", -3.1415) == "        -3.1415");

    // right alignment
    VERIFY(uxs::format("{:>15g}", 3.1415) == "         3.1415");
    VERIFY(uxs::format("{:>+15g}", 3.1415) == "        +3.1415");
    VERIFY(uxs::format("{:> 15g}", 3.1415) == "         3.1415");
    VERIFY(uxs::format("{:>15g}", -3.1415) == "        -3.1415");

    // mid alignment
    VERIFY(uxs::format("{:^15g}", 3.1415) == "    3.1415     ");
    VERIFY(uxs::format("{:^+15g}", 3.1415) == "    +3.1415    ");
    VERIFY(uxs::format("{:^ 15g}", 3.1415) == "     3.1415    ");
    VERIFY(uxs::format("{:^15g}", -3.1415) == "    -3.1415    ");

    // left alignment
    VERIFY(uxs::format("{:<15g}", 3.1415) == "3.1415         ");
    VERIFY(uxs::format("{:<+15g}", 3.1415) == "+3.1415        ");
    VERIFY(uxs::format("{:< 15g}", 3.1415) == " 3.1415        ");
    VERIFY(uxs::format("{:<15g}", -3.1415) == "-3.1415        ");

    // zero fill
    VERIFY(uxs::format("{:015g}", 3.1415) == "0000000003.1415");
    VERIFY(uxs::format("{:+015g}", 3.1415) == "+000000003.1415");
    VERIFY(uxs::format("{: 015g}", 3.1415) == " 000000003.1415");
    VERIFY(uxs::format("{:015g}", -3.1415) == "-000000003.1415");

    return 0;
}

int test_string_cvt_1() {
    VERIFY(uxs::format("{:f}", 1.2345672222) == "1.234567");
    VERIFY(uxs::format("{:f}", 1.2345677777) == "1.234568");
    VERIFY(uxs::format("{:f}", 1.2345) == "1.234500");
    VERIFY(uxs::format("{:f}", 0.12345) == "0.123450");
    VERIFY(uxs::format("{:f}", 0.00124) == "0.001240");
    VERIFY(uxs::format("{:f}", 23.34563444) == "23.345634");
    VERIFY(uxs::format("{:f}", 23.34563464) == "23.345635");
    VERIFY(uxs::format("{:f}", 23.3456) == "23.345600");
    VERIFY(uxs::format("{:f}", 999.9999997) == "1000.000000");
    VERIFY(uxs::format("{:f}", 0.) == "0.000000");
    VERIFY(uxs::format("{:.0f}", 0.) == "0");

    VERIFY(uxs::format("{:f}", 1000000000.) == "1000000000.000000");
    VERIFY(uxs::format("{:f}", 231000000000000.) == "231000000000000.000000");

    VERIFY(uxs::format("{:f}", 345.) == "345.000000");
    VERIFY(uxs::format("{:f}", 345.234465876) == "345.234466");
    VERIFY(uxs::format("{:f}", 34.5234465876) == "34.523447");
    VERIFY(uxs::format("{:f}", 3.45234465876) == "3.452345");
    VERIFY(uxs::format("{:f}", 0.345234465876) == "0.345234");
    VERIFY(uxs::format("{:f}", 0.0234465876) == "0.023447");
    VERIFY(uxs::format("{:f}", 0.00234465876) == "0.002345");
    VERIFY(uxs::format("{:f}", 0.000234465876) == "0.000234");
    VERIFY(uxs::format("{:f}", 0.0000234465876) == "0.000023");
    VERIFY(uxs::format("{:f}", 0.00000234465876) == "0.000002");
    VERIFY(uxs::format("{:f}", 0.000000234465876) == "0.000000");

    VERIFY(uxs::format("{:.0f}", 345.) == "345");
    VERIFY(uxs::format("{:.0f}", 345.234465876) == "345");
    VERIFY(uxs::format("{:.0f}", 34.5234465876) == "35");
    VERIFY(uxs::format("{:.0f}", 3.45234465876) == "3");
    VERIFY(uxs::format("{:.0f}", 0.345234465876) == "0");
    VERIFY(uxs::format("{:.0f}", 0.0234465876) == "0");

    VERIFY(uxs::format("{:e}", 0.) == "0.000000e+00");
    VERIFY(uxs::format("{:.0e}", 0.) == "0e+00");
    VERIFY(uxs::format("{:g}", 0.) == "0");

    VERIFY(uxs::format("{:g}", 999999.9999996) == "1e+06");
    VERIFY(uxs::format("{:.3g}", 3455.) == "3.46e+03");
    VERIFY(uxs::format("{:.3g}", 3445.) == "3.44e+03");

    VERIFY(uxs::format("{:g}", 0.003) == "0.003");
    VERIFY(uxs::format("{:g}", 0.003000001) == "0.003");
    VERIFY(uxs::format("{:g}", 0.00300001) == "0.00300001");
    VERIFY(uxs::format("{:g}", 0.0030001) == "0.0030001");
    VERIFY(uxs::format("{:g}", 0.003001) == "0.003001");

    VERIFY(uxs::format("{:g}", 0.3) == "0.3");
    VERIFY(uxs::format("{:g}", 0.3000001) == "0.3");
    VERIFY(uxs::format("{:g}", 0.300001) == "0.300001");
    VERIFY(uxs::format("{:g}", 0.30001) == "0.30001");
    VERIFY(uxs::format("{:g}", 0.3001) == "0.3001");

    VERIFY(uxs::format("{:g}", 345.) == "345");
    VERIFY(uxs::format("{:g}", 345.0001) == "345");
    VERIFY(uxs::format("{:g}", 345.001) == "345.001");
    VERIFY(uxs::format("{:g}", 345.01) == "345.01");
    VERIFY(uxs::format("{:g}", 345.1) == "345.1");

    VERIFY(uxs::format("{:g}", 3.e23) == "3e+23");
    VERIFY(uxs::format("{:g}", 3.000001e23) == "3e+23");
    VERIFY(uxs::format("{:g}", 3.00001e23) == "3.00001e+23");
    VERIFY(uxs::format("{:g}", 3.0001e23) == "3.0001e+23");
    VERIFY(uxs::format("{:g}", 3.001e23) == "3.001e+23");

    VERIFY(uxs::format("{:.0g}", 345.) == "3e+02");
    VERIFY(uxs::format("{:.0g}", 345.234465876) == "3e+02");
    VERIFY(uxs::format("{:.0g}", 34.5234465876) == "3e+01");
    VERIFY(uxs::format("{:.0g}", 3.45234465876) == "3");
    VERIFY(uxs::format("{:.0g}", 0.345234465876) == "0.3");
    VERIFY(uxs::format("{:.0g}", 0.0234465876) == "0.02");

    VERIFY(uxs::format("{:f}", 0.0000006) == "0.000001");
    VERIFY(uxs::format("{:f}", 0.000000500001) == "0.000001");
    VERIFY(uxs::format("{:f}", 0.0000004) == "0.000000");
    VERIFY(uxs::format("{:f}", 0.00000049999) == "0.000000");
    VERIFY(uxs::format("{:f}", 0.00000006) == "0.000000");

    double d = 2000000. / 7.;  // 285714.28571428573923185468

    VERIFY(uxs::format("{:.17e}", d) == "2.85714285714285739e+05");
    VERIFY(uxs::format("{:.16e}", d) == "2.8571428571428574e+05");
    VERIFY(uxs::format("{:.15e}", d) == "2.857142857142857e+05");
    VERIFY(uxs::format("{:.13e}", d) == "2.8571428571429e+05");
    VERIFY(uxs::format("{:.2e}", d) == "2.86e+05");
    VERIFY(uxs::format("{:.1e}", d) == "2.9e+05");
    VERIFY(uxs::format("{:.0e}", d) == "3e+05");
    VERIFY(uxs::format("{:.18e}", d) == "2.857142857142857392e+05");
    VERIFY(uxs::format("{:.19e}", d) == "2.8571428571428573923e+05");
    VERIFY(uxs::format("{:.20e}", d) == "2.85714285714285739232e+05");

    VERIFY(uxs::format("{:.12f}", d) == "285714.285714285739");
    VERIFY(uxs::format("{:.11f}", d) == "285714.28571428574");
    VERIFY(uxs::format("{:.10f}", d) == "285714.2857142857");
    VERIFY(uxs::format("{:.8f}", d) == "285714.28571429");
    VERIFY(uxs::format("{:.2f}", d) == "285714.29");
    VERIFY(uxs::format("{:.1f}", d) == "285714.3");
    VERIFY(uxs::format("{:.0f}", d) == "285714");
    VERIFY(uxs::format("{:.13f}", d) == "285714.2857142857392");
    VERIFY(uxs::format("{:.15f}", d) == "285714.285714285739232");

    d = 285714.;

    VERIFY(uxs::format("{:.8f}", d) == "285714.00000000");
    VERIFY(uxs::format("{:.2f}", d) == "285714.00");
    VERIFY(uxs::format("{:.1f}", d) == "285714.0");
    VERIFY(uxs::format("{:.0f}", d) == "285714");

    d = 285714e+20;  // 28571400000000001222639616.00000000

    VERIFY(uxs::format("{:.8f}", d) == "28571400000000001222639616.00000000");
    VERIFY(uxs::format("{:.2f}", d) == "28571400000000001222639616.00");
    VERIFY(uxs::format("{:.1f}", d) == "28571400000000001222639616.0");
    VERIFY(uxs::format("{:.0f}", d) == "28571400000000001222639616");

    VERIFY(uxs::format("{:.21g}", 123412341234123400000.) == "123412341234123407360");
    VERIFY(uxs::format("{:.21g}", 12341234123412340000.) == "12341234123412340736");

    VERIFY(uxs::format("{:.7f}", 0.0006382) == "0.0006382");
    VERIFY(uxs::format("{:.6f}", 0.0006382) == "0.000638");
    VERIFY(uxs::format("{:.5f}", 0.0006382) == "0.00064");
    VERIFY(uxs::format("{:.4f}", 0.0006382) == "0.0006");
    VERIFY(uxs::format("{:.3f}", 0.0006382) == "0.001");
    VERIFY(uxs::format("{:.2f}", 0.0006382) == "0.00");
    VERIFY(uxs::format("{:.1f}", 0.0006382) == "0.0");
    VERIFY(uxs::format("{:.0f}", 0.0006382) == "0");

    VERIFY(uxs::format("{:.4f}", 0.6382) == "0.6382");
    VERIFY(uxs::format("{:.3f}", 0.6382) == "0.638");
    VERIFY(uxs::format("{:.2f}", 0.6382) == "0.64");
    VERIFY(uxs::format("{:.1f}", 0.6382) == "0.6");
    VERIFY(uxs::format("{:.0f}", 0.6382) == "1");

    VERIFY(uxs::format("{:.3f}", 0.0004382) == "0.000");
    VERIFY(uxs::format("{:.0f}", 0.4382) == "0");

    VERIFY(uxs::from_string<double>("9.999999999999999999") == 9.999999999999999999);

    std::array<double, 18> v = {
        uxs::from_string<double>("2.22507385850720139e-308"), uxs::from_string<double>("2.22507385850720138e-308"),
        uxs::from_string<double>("1.11253692925360070e-308"), uxs::from_string<double>("1.11253692925360069e-308"),
        uxs::from_string<double>("5.56268464626800346e-309"), uxs::from_string<double>("5.56268464626800345e-309"),
        uxs::from_string<double>("5.30498947741318079e-315"), uxs::from_string<double>("5.30498947741318078e-315"),
        uxs::from_string<double>("5.05923221341436462e-321"), uxs::from_string<double>("5.05923221341436461e-321"),
        uxs::from_string<double>("1.97626258336498618e-323"), uxs::from_string<double>("1.97626258336498617e-323"),
        uxs::from_string<double>("9.88131291682493089e-324"), uxs::from_string<double>("9.88131291682493088e-324"),
        uxs::from_string<double>("4.94065645841246545e-324"), uxs::from_string<double>("4.94065645841246544e-324"),
        uxs::from_string<double>("2.47032822920623273e-324"), uxs::from_string<double>("2.47032822920623272e-324"),
    };

    VERIFY(uxs::format("{:g}", v[0]) == "2.22507e-308");
    VERIFY(uxs::format("{:g}", v[1]) == "2.22507e-308");
    VERIFY(uxs::format("{:g}", v[2]) == "1.11254e-308");
    VERIFY(uxs::format("{:g}", v[3]) == "1.11254e-308");
    VERIFY(uxs::format("{:g}", v[4]) == "5.56268e-309");
    VERIFY(uxs::format("{:g}", v[5]) == "5.56268e-309");
    VERIFY(uxs::format("{:g}", v[6]) == "5.30499e-315");
    VERIFY(uxs::format("{:g}", v[7]) == "5.30499e-315");
    VERIFY(uxs::format("{:g}", v[8]) == "5.05923e-321");
    VERIFY(uxs::format("{:g}", v[9]) == "5.05923e-321");
    VERIFY(uxs::format("{:g}", v[10]) == "1.97626e-323");
    VERIFY(uxs::format("{:g}", v[11]) == "1.97626e-323");
    VERIFY(uxs::format("{:g}", v[12]) == "9.88131e-324");
    VERIFY(uxs::format("{:g}", v[13]) == "9.88131e-324");
    VERIFY(uxs::format("{:g}", v[14]) == "4.94066e-324");
    VERIFY(uxs::format("{:g}", v[15]) == "4.94066e-324");
    VERIFY(uxs::format("{:g}", v[16]) == "4.94066e-324");
    VERIFY(uxs::format("{:g}", v[17]) == "0");

    for (double d : v) { VERIFY(uxs::from_string<double>(uxs::format("{}", d)) == d); }

    VERIFY(uxs::format("{:#g}", 3.e23) == "3.00000e+23");

    VERIFY(uxs::format("{:G}", 3.e23) == "3E+23");
    VERIFY(uxs::format("{:015g}", 3.e23) == "00000000003e+23");
    VERIFY(uxs::format("{:015G}", 3.e23) == "00000000003E+23");

    VERIFY(uxs::format("{:+g}", 3.e23) == "+3e+23");
    VERIFY(uxs::format("{:+G}", 3.e23) == "+3E+23");
    VERIFY(uxs::format("{:+015g}", 3.e23) == "+0000000003e+23");
    VERIFY(uxs::format("{:+015G}", 3.e23) == "+0000000003E+23");

    VERIFY(uxs::format("{: g}", 3.e23) == " 3e+23");
    VERIFY(uxs::format("{: G}", 3.e23) == " 3E+23");
    VERIFY(uxs::format("{: 015g}", 3.e23) == " 0000000003e+23");
    VERIFY(uxs::format("{: 015G}", 3.e23) == " 0000000003E+23");

    VERIFY(uxs::format("{}", 10.0) == "10");
    VERIFY(uxs::format("{}", 10000.0) == "10000");
    VERIFY(uxs::format("{}", 100000.0) == "1e+05");

    VERIFY(uxs::format("{:#.4f}", 0.00001) == "0.0000");

    VERIFY(fabs(uxs::from_string<float>(uxs::format("{:g}", 0.2355f)) - 0.2355f) < 0.000001);
    VERIFY(fabs(uxs::from_string<double>(uxs::format("{:g}", -123.56e-1)) - -123.56e-1) < 0.000001);

    return 0;
}

int test_string_cvt_2() {
#if defined(has_cpp_lib_format)
    // clang-format off
    double vv[] = {0., 4., 3., 3.5, 3.56, 3.567, 3.5672, 3.56723, 3.567234, 3.5672349, 3.56723498, 3.5672349826212314,
        113153.77877387607, 1.5327672374081723e-06, 16788.647167406853, 1.3835020776380305, 1.0345721104648476e-09,
        1.6711493841707703e-10, 0.010668422372625677, 1588976642.985235, 1.5269287777599061e-09, 14159.993568835396,
        1.7621980399986343e-05, 0.00017360818844845653, 17564104861.32586, 12470388854.199165, 1753.355835205583,
        163163471.79636154, 1436411.4055156393, 1.2377744335463647e-05, 1.1665072001946528, 1.9092081020195376e-09,
        1.504522894934611, 1.9866421121602493, 1090732894.5806298, 0.015007070945925838, 19.138174420710264,
        1.940979954016584e-09, 17702045.470019445, 10158.677008734554, 162954.3418124008, 199945787.9329253,
        0.0013063218308826158, 15911135.82577324, 1.8415106395702357e-05, 0.15373039755458026, 1.1783277036826443e-07,
        1.8024057271780754e-10, 14984801190.585876, 155458384.8387683, 1.8420396127313974e-07, 1.7147099676738363e-08,
        1.274588147476393e-10, 10268.762905467514, 1.2399108048610942e-07, 1886.9906622411315, 0.016813462136839645,
        1.4997410324342583e-07, 184.55756596654362, 1.5561461388448724e-07, 1.4087666938085288e-08, 1.2521264064447728,
        1.961095140961102e-08, 1319.2496736712085, 1651.253741434412, 1.2478417583269796, 1.203250334503043e-10,
        1.4264974131969122e-08, 1.4103130357191493e-08, 1.0921736299386226e-07, 1.365339028556711e-05,
        0.17831531687180588, 1452300.1691578536, 1.6516460553905771e-06, 1.90892188553579e-05, 1.4712623287008073,
        181756.14831203205, 1951.3674505823199, 18246.973869803944, 19871455001.069687, 12.893162031797235,
        0.011034339278569923, 0.18765657191769447, 186926.37381461798, 170653.5474496796, 18.86031117193007,
        10651.938727096238, 16676.79065290979, 191663389.5613636, 1.5439483071574547e-08, 1.989362499876447e-06,
        1.3157324125419783, 0.1439725562818793, 1.3651580603499737e-06, 1153.6044796492556, 1.0005950423039219e-10,
        0.00017273349902723086, 16824.942323298714, 18364.198798677426, 1.094548803296781e-09, 1.6295718364525347e-06,
        0.010810607971510238, 0.01947545062213787, 1.6921689792785573e-05, 0.017838652028444205, 1.1939674726082435e-10,
        1.9832357979185394e-05, 0.01136454938022716, 11768803.684761614, 1.9879370006423467e-05, 1.1016373776071116e-06,
        1696007.304837337, 17529.404131705516, 105.64406624745163, 13187.778801026583, 17.625707218621756,
        0.00015879889133991158, 1.143045260100197e-07, 18602256.957468465, 0.00017389966091303292,
        0.00015285482677595823, 0.15180835871139012, 0.01370226275641143, 0.014758962236338567, 0.001078263208007356,
        16717.841516237197, 172860.8363760241, 1460697338.38837, 1.4604341728810353, 140.1832905348866,
        11503.938894194041, 1.5417101687878547, 1.0149505687221605e-05, 1546136600.7734334, 1.9700866743022516e-05,
        0.1694023666080098, 1.0220837001042759e-07, 0.17078263010469277, 17517100931.54498, 127951.1742878101,
        1.0042539230453253, 116308105.52304977, 0.174667914736846, 1.5847872002869452, 0.015440227124629986,
        1385.7694328703753, 17.09319846740781, 0.0018159321657904206, 1.3101689383883214e-10, 1520960.068863685,
        1.0552787903376122e-09, 0.00017237637306599885, 0.14778043171419816, 1.4464895764426177e-07, 18336.821347257617,
        0.000168116446581674, 19.868829888437137, 1.856192619406638e-10, 148.9651702976252, 16.793321990862594,
        11225068929.049194, 135.13187434947648, 11.259328172819117, 13569249.779302062, 1.2590141922665207e-05,
        181304801.07154846, 17606.17293774067, 1263.6202475890013, 1.2095623056104852e-08, 1.006079369950636e-07,
        1.4319082826670148e-09, 13753100.617034521, 1.973346203132686e-10, 1494244.626289626, 0.0012501552721645409,
        149995.5331333459, 168166.57805048092, 1.2305718844279234e-06, 165.2621920159015, 1.2248011952364203e-06,
        116592.75363128942, 1047.6386022078968, 1.949157147679133, 1.9140249865358212e-10, 1.7218716358939599,
        1.441632603731453, 0.10653562675708238, 0.0001467741361312877, 1.7432739538757492e-06, 1372519766.8729439,
        1.2549574782488712e-09, 1.5263378402108903e-07, 11051848.380491495, 0.00014238904757501292, 157677586.04451537,
        18365.9783398334, 193.52416020164003, 0.153950467493436, 1.24044047616542e-09, 0.00014723799467861369,
        1216386.1494221452, 166882035.12994012, 1.711564447109997e-05, 0.10810055190440421, 1.795352611608604,
        1266.119157679131, 11868247440.079416, 1795855658.0011501, 1.6060012971903932e-09, 1.2924679987749212,
        183433.84570882254, 1802.0843555841611, 1.0414204383865843e-07, 1.0617313798938541, 1.2205155330571257e-06,
        16996.909894460696, 12617.785190316792, 1.2006445293220017e-06, 146949849.38833365, 1.444519203093963e-10,
        103163.8791592349, 0.00018228910324337755, 0.15365282689710777, 19873247404.481426, 0.0001867514226858299,
        1997.5985913274749, 19319.147245641354, 154.15346468326058, 19452502.727740057, 0.00104107476688808,
        1.0965382737587652, 15614837.194538547, 167452.47323373135, 1.0227995467111746e-06, 1.3458717373957556e-09,
        1.921117490149543e-06, 1.586015785835751e-07, 1691126.4557431417, 107.7324317706909, 0.19296977719221725,
        1831255978.9831197, 1.7238364573797726, 11614194152.940325, 12.732511112097942, 1.667049291093208e-08,
        16107.376668463516, 157.1582607756504, 1.15121689007792, 1.031656548750902e-09, 1.3206422953695748e-10,
        0.01107180517846487, 159.71685078675702, 1.7720302941646948, 16501.277442029383, 1.4868495140137323,
        1.5341986463306695e-05, 12801.03728718006, 1.938254025488625e-06, 1597476.2302862697, 16814911.157280367,
        1.2302697496432588e-07, 1251.4221167202877, 1.935935940536059e-05, 12789285673.837286, 0.01122949751444496,
        1.4364912084189495e-08, 0.10998656074357714, 129706.41767320712, 1.3307583509013179e-09, 1.485286040484791e-06,
        197434786.7566706, 1367394.8302057711, 1.1595798011316747e-09, 0.0014681014223422175, 0.014241335306931155,
        1.308327491054288e-09, 1897656898.7638364, 1.770046456922456e-07, 1031403.77608838, 1.8616217767100125e-05,
        0.01586861972717204, 1525670248.951407, 157986.25063988208, 1.3380846998453854e-07, 171891944.8173123,
        171011358.70875308, 1.0898530984013739e-07, 10964.285529263567, 181304683.7313429, 163520536.24763018,
        1.4864707011037295e-08, 13826.195975498871, 1.0768217585400598e-07, 1.708587970535651e-06, 1975.880809753541,
        0.0019086905696888362, 0.17942779126799524, 1.4986305628301446, 149965.73307784504, 1.3001217467398786e-07,
        10803220366.456837, 12384103740.961283, 1.1980799358476111e-08, 133.91191572298044, 0.15856745925786053,
        138.23168995477428, 11487.874215129052, 0.016980664452498472, 1819527.1843524335, 1.0096078880100814,
        1646.4952043872552, 0.00137736405504577, 13343.067523362646, 12915.253675207254, 1.3146988840232428e-07,
        1005588196.9693236, 0.0015165314722876148, 12909998.003608236, 1.6346194261655348e-09, 1.3768165568875515e-07,
        1.6659731977096768e-10, 0.0001560914420018303, 1.320611327913023, 1.8420439043097397e-06,
        1.3733593170585697e-09, 12.024493471154706, 14225524.81143097, 0.001181195782802736, 0.0015894192327339893,
        1627830335.7394037, 0.10504816353818168, 174831107.12340796, 1104.5793599170547, 1581.285095306089,
        1018916015.5224915, 1.9677231554547907, 171.97947556704213, 1834.6914912138436, 0.0016430266872125218,
        0.00016641472856808318, 1442370320.0900595, 1.951753271827555e-08, 18363754707.801895, 15.261817073381003,
        1929.890099297225, 194.14289715355665, 124697368.48119557, 11629580.527181245, 16232426.179800604,
        139155538.0753595, 1.1156065947835153e-10, 0.16287531712714454, 0.0010322693752305976, 1370.8372361011602,
        105995.98063604672, 1.442623693572702e-07, 18972447444.155304, 10330.03565697077, 156933990.15003312,
        1.68859422617205e-06, 1272442858.2129095, 1.9532621498922, 196143613.7114633, 1007007.9412457653,
        1960.0548208553823, 1.1370934158785208e-08, 1501.8537448851087, 1.526454358155824e-08, 1.6689044797052006e-05,
        13914934.905104779, 112344.42971134046, 12012166.772272943, 1.1055624425568064e-07, 177965.8428356184,
        0.0015809693088886805, 1.2920907422627022e-07, 0.016713515061728655, 0.0015793622806878922, 16996390.958486676,
        1.8490677827457517e-05, 11.69960940538246, 199873.56560633774, 0.011645995776408444, 0.12135677948595668,
        1.9293214757422544e-08, 1482048.7165033745, 1.2139157767805068e-05, 1208123313.1684709, 161.65630901219274,
        1.9078477957155692e-06, 17911.40486810537, 1.747968972900039e-08, 0.0016546287793685781, 0.00012150491633510737,
        1914368.1483559876, 17299834.32615836, 0.0011296092952308317, 0.0013621914715815579, 1571891.2590408872,
        14932954.911285976, 0.0014353752790547372, 1281544541.8992293, 1.7445800882369803e-07, 125.19552601875404,
        131.4607511499812, 1.3736617653040429e-08, 1.1260309878335913e-06, 0.10709501346072299, 1.2346758802573174e-06,
        10813264.850744043, 1120176.659614274, 1.0455203345209517e-09, 1141179.3400124093, 139.9214564834683,
        10537268020.038773, 1.4135795704272876e-10, 197446701.49204242, 1.8343689070734277e-05, 1.58970801108347e-06,
        0.10015736186089504, 1.9330054420399372e-10, 137650142.8553137, 0.00016094681862801352, 1653.8730524162536,
        10260002996.35865, 16.423252408822997, 0.11206907302226475, 15.858001188884096, 1.886599726163224e-09,
        0.00011008091394622864, 19130021.97475951, 1813280.0546460128, 1448.7203009134328, 0.18157593986603643,
        1401554747.948769, 1.3784445525259237, 18992673200.677574, 14584.809383628684, 140927.82726965714,
        1.3039324472840504e-06, 127486.86714856543, 178507.3022813896, 1632938.573063787, 0.0016277947538792458,
        14.922073909343553, 1.8579245554443444e-08, 13869129.788004646, 1456879.010409998, 19619206284.248905,
        0.0017416165757376352, 119.90004601863147, 1.2516928431793162e-06, 151375.90112179247, 1989635.179092777,
        1.146671446900871e-08, 124.63004477398403, 1.678709428613905e-09, 0.00018762618003399876, 1744.074605234689,
        15.85027396480012, 1614501388.1489258, 1.2563836295747633e-10, 1.3434892703349201e-10, 0.0161072314176375,
        0.001730372700388163, 1.3762769443533371e-09, 1.4181656932548836e-08, 16.851647518089003, 15318602095.058147,
        1.4753442094712193e-08, 15495.150061748964, 1.4749841937223476e-09, 0.19179770044428582, 19.971767304711342,
        1.846325355962354e-07, 16562725766.454313, 1.1593466521130456e-08, 197.29635246234292, 1062886036.9222146,
        1.4666675929365352e-05, 143082442.6618358, 138476.5823356883, 167273.88343586132, 1.313986343942274e-07,
        1.9209356678323417e-07, 1.1763061906008194e-07, 16682.736033269204, 0.1323404948303612, 109364374.56440702,
        1147.7748582794661, 1882.3502667745026, 1.1703466454262876e-10, 0.0011090831907389594, 1.96383920381256e-05,
        103004023.11067756, 19.132235025580968, 14870479.214688169, 15.830635396310761, 0.00017533368426274636,
        0.001512963671329611, 1.9670378285284343e-10, 145456598.1501025, 125035.60368643349, 14229.611757609428,
        15.263000868533151, 1403687.8756708042, 1.3078589257227753e-07, 1620.672657153556, 1.1542324240486037e-07,
        1.5331803669787756e-07, 1.4090576063580275e-10, 15650613095.133604, 0.00013335698884014966,
        0.00011628679803945414, 164.13757548026433, 1.335103124244087e-09, 0.00014911220248445278, 1439.3204386026407,
        10016643476.341421, 13183765996.888388, 11654064071.737612, 10418849853.799734, 1.2203904004888898e-08,
        1.437510740072273e-05, 154922843.46283647, 16.704681153374658, 19019963.934839822, 12339500380.808475,
        0.00013750401320920154, 1991834.7307151146, 0.16416650087162574, 118188036.86025688, 193097507.2255967,
        16291411554.205456, 1.8306690553228313e-09, 1975293.9376701645, 1.0323388133824594, 1.969666992926398e-06,
        159694749.22970247, 1084772.8316295072, 1.7083984018247051e-09, 10716365003.151995, 14997.731490384152,
        19446.474512283363, 11712271.870366426, 1481877244.658108, 1.3993079334040168e-07, 12.100641617191126,
        1263152.6565640615, 0.0016589120964303886, 145.28831361922886, 10217658.83411577, 15417962574.683632,
        179328.60536188146, 1.4067498199952432e-05, 1213.819311992227, 0.0012763933567143213, 16884009217.276539,
        1.90230390084106e-10, 0.010537520056039766, 1.796038054281672e-10, 1.9214678366877803e-08, 10.730384074575696,
        0.01481429816850573, 1559108768.8351471, 1.5896359341583686e-10, 138738.2982791182, 1.5543088347997087e-05,
        0.0011616204140895996, 1.2657340198474832e-06, 16.481543205262895, 0.0014699457935116935,
        1.4932547635796324e-08, 0.0014382907790564125, 0.001892149101373788, 18016292122.390163, 15.766378247432847,
        1.5065448820387637, 1.0825975522840566e-06, 0.0013852189336639666, 11646847.443625951, 1893365.3333374104,
        1.250745765616041e-05, 1.718897798092854, 135186877.34334835, 1020403146.4890655, 15686319917.891077,
        1587660.2817214297, 10.434072369220853, 15746991859.187824, 12.072091307936951, 0.0001292872378330877,
        198.4921979693616, 1.5215732964575363e-09, 168.16561413794068, 1.2004024473042033e-07, 1197464.9283456232,
        1.7892003778602124e-09, 124702.57294616006, 139179318.0862747, 1596702.5768599005, 19543537.10122144,
        16864307.700423058, 0.017683939226739047, 1420051.264309026, 16907725.128621344, 129.4793061511826,
        0.18986392124889415, 1.4640328456591744e-10, 10.918404417306862, 166762.33275860033, 1.981988439157811e-05,
        1.005181579754284e-09, 1.9267694805342404e-06, 0.00012536613667796736, 0.1295857273195813, 192306133.11548233,
        17524.05777925377, 1.473922101043142e-06, 18977708.73411527, 1973101591.1814334, 0.012628902530644482,
        1770617.9401430704, 1418.6729529368804, 17420021.119206678, 1.7802213606576334e-07, 17877070180.58473,
        14249325.347724931, 0.1944810516910238, 0.001798558766275216, 1.8287048018091366e-10, 109400020.37289558,
        19546512.90063185, 151197.4183792696, 1.417457702491998e-06, 0.0011089047452535093, 0.012039261667545554,
        18992066307.095776, 14168392.599796616, 0.014585241329120526, 16033.597620201414, 1.7390970552645737e-10,
        113017766.96396138, 1.3283296741240153e-06, 1.680743668141285e-05, 170.38495138071906, 107371212.25513475,
        121673.90131561282, 195821.47596879172, 1.195229097796108e-06, 183915.9922999692, 1.4201981732896627e-05,
        1.835845213080077e-09, 0.16336917792866928, 1.7252345758303956e-10, 1.538174704410688e-08,
        1.4357724639176854e-10, 1329.900151073131, 1544640.260033798, 1.7627207867821598e-09, 14703301.289702086,
        13683497.230872288, 12785634.139291195, 189789197.24464026, 11.819809865098339, 135096.8363803284,
        1.235326997676233e-08, 18015602.01142387, 186.78885210011117, 1.755131059116182, 135525.25666025962,
        0.0001878878463023715, 0.00011503711459125978, 102.21688411901913, 18.293311188943953, 103.03791343570224,
        1.1550585031284782e-09, 0.0015743064561188863, 103497.09611758475, 1275347.806800121, 0.0013028710912819717,
        16222.822532112792, 1.1544306542051268, 16964.17503970813, 145884.07290751807, 1.3278403318568496e-10,
        1805493728.747417, 156.70598948828916, 1.7284530957437552e-08, 0.14444859150796185, 1.5851816799097456e-07,
        0.0015254563688191281, 1032957038.6808864, 0.015079674626577211, 11625.330684651974, 1.692757287258368e-07,
        194703.0608144024, 0.0001288709028343442, 0.018350344688285235, 1.4314308945575037e-09, 1.6442861909380766,
        17378.28812415033, 1.3670198308524486, 14155895.299999518, 1.540555479521974e-08, 16.32031277751992,
        1.706819307023855, 19756.47367384398, 1382.199688679945, 0.00014887430319384873, 17002349.878136575,
        1.8005104563624044e-07, 1.2340110353742954e-10, 0.0018717997820448544, 1.6746110333166737e-07,
        13571619.523274995, 10518739.22127694, 19240111.89484368, 10222511397.975674, 1.7359492577367847e-08,
        1.7821748300424758e-10, 1.6053440370634527e-10, 1.6268238862049013e-10, 0.00015770405556382933,
        181428.7834465596, 1.6865668921313197e-08, 1.316177974635076e-10, 156939869.15251684, 1.6161266711745632e-05,
        0.0010886759660465471, 0.01803795428109011, 1.797826088612678e-09, 15918939482.256418, 1.863762164515819e-05,
        115885468.84201647, 1.03974962690578e-09, 1.185877390746918e-10, 1.6826437053285285e-06, 1.3443965857570103e-05,
        10227274430.740936, 1.6621777361969892e-06, 1061747.2622787566, 0.016512396389576463, 1.7406590816855792e-05,
        1.0242923882706622e-05, 19348466514.392834, 0.0018854849577686003, 1.006556306445223e-06, 1.2257696240039104,
        12.204167140921482, 1183.0573368622038, 1.4949294626170324e-05, 1.3517691301380322e-07, 1.082349209048751e-10,
        0.0001244023133930082, 0.0013841465517594345, 1289279392.1964424, 1747.1090884948906, 1.9339272974346238,
        0.00018090859481194926, 1.3229071303381617e-08, 1.6903192123583618e-06, 0.00015158756954898657,
        198.39122013706012, 19.81289166477294, 1.6273690312625653e-06, 0.11716810588857862, 0.011441302663726028,
        119591.57554732065, 18687942.08643452, 157.0928966845399, 1.805742199390998e-08, 0.00017441139760918355,
        1.5038083422311173, 0.11757870555157211, 0.14089043033708895, 1.958914872199386, 1.6850428863169067,
        0.015966204805764387, 0.001980445403467879, 19629219.413297992, 1.1327485215687413e-08, 1794239.3121188378,
        1.351052527819856e-08, 1.0968658497729751e-10, 17038.563264201937, 1579575835.8426201, 15896349934.70701,
        1.0646321488105202e-05, 1.256752451830253e-05, 1.982880624838265e-05, 1044.6624421623715, 16.55651530665387,
        191960964.73328388, 11715863.586499868, 1.9565714292212248e-08, 0.11230697705614448, 1.3966284337983032e-08,
        14215.906232150082, 14.734080993592059, 1.1870787735817103e-06, 163.36407812413105, 1.0971637422591005e-10,
        1784965611.0225377, 1.9312944557870488e-05, 1407080.384930418, 158.6755088677374, 1.6804615008517951,
        16.588654337176116, 1.7547566163786547e-06, 155.96223181332567, 1.6075601824172255e-05, 0.011174738677242075,
        1.1232252778940029e-10, 0.0017939754653502028, 1.841864889005125e-06, 1.5081372359517543e-05,
        1.2845308668298178e-08, 11466639003.735113, 19064.48955477815, 1226.1357953589352, 11987.088115701716,
        1.0823488716387331e-10, 1.1857698645902332e-06, 0.00018576956505713004, 1.4786815663454996e-06,
        1.6896557114294508e-10, 1.2277035248554455e-10, 123559.91310226073, 106306162.25816011, 10.542970293893887,
        193329478.74458116, 13.219218230822749, 1.9653217536891917e-07, 1.9857893133030813e-07, 1660613932.6306179,
        1.777723047243469e-06, 1.496467633841446e-08, 0.013408331196659502, 162870.97845337042, 1.773189820107971e-10,
        0.015055286753350383, 16.55235197722679, 0.13804053782414366, 1769784.3762481064, 1.7775979008613817e-09,
        1566127718.796261, 1.0595074451629769e-08, 1.7196411951039626e-10, 1.6231965571635482e-09, 1.6911104591533332,
        176.76909521834185, 0.000102476984254184, 0.0196182627388742, 0.0012132068064907488, 1.593870731081276e-07,
        1.8174601999178946e-09, 0.017713687419937504, 0.00013258342817825232, 1.301690413390384, 1.1507259346157588e-05,
        1.9839652040554383, 1.4902720497369887e-10, 199987526.71645343, 1.0471861935776494e-09, 11.544317850894835,
        0.0001306139793503792, 10917451333.02179, 0.16595137034376886, 1.9484451098742768, 13719.097828717871,
        1.4196492160567034e-10, 11004318986.109653, 1684.9908791250816, 1.816211128021179e-09, 1.5237203504339194e-07,
        1743.8360791942498, 111523.41123206977, 1.9315293642982783e-06, 0.001579125535776555, 11758085.299840553,
        19.03942649752149, 14807515777.693064, 11180944.475045737, 1.0020347658975224e-06, 1.4172071162168892e-10,
        0.019724772544811787, 0.00019159466183875075, 180.36301800234318, 1.403954703377335e-05, 0.00120231925736849,
        198.61121436664953, 14291.480496056809, 12697062812.52685, 1.3536311992113204, 1.6873457867224924e-06,
        11.368220073872894, 171783.89807782017, 0.012594989623425376, 1.1552446392866048e-06, 11561155.43297764,
        1276.766770805593, 13518744410.430368, 1.4927795824334905e-07, 152.08887906332217, 13042157192.11578,
        1.8600716761928768e-06, 1256259313.04869, 1.9018335067872687e-08, 1.5146675041862258e-10,
        1.3274993114488681e-05, 1826258706.9699252, 1.8723811268250807e-08, 15157883.343976319, 1136.2311571863793,
        1705.2197123664853, 1.8665592421793393e-05, 10.24366506216894, 1367164468.2498543, 0.00019622952409435707,
        0.15775841026961987, 1171462.56404466, 0.0015484818460671352, 1480262.006626667, 14850259.883991053,
        1.1373254736229008e-10, 0.0018581164176958626, 0.00016473690988611845, 1.239789096557367e-08,
        1.3463498351651726e-08, 1.194652386148398, 1.5153331403912986e-06, 1.0660737461016276, 15.024410914564744,
        0.0001364629430734853, 0.16129262673214542, 149621410.90013188, 0.18236119340081225, 12703.461323769305,
        195869972.8859417, 19365036.26166133, 1.9491899217558012e-10, 1.1722063947126856e-05, 12.883645928749868,
        1.001971790210938e-08, 110.90387819566475, 1.151077429530698e-07, 0.015493176806296064, 14528.880820147107,
        1.3058929170210556e-08, 1107.3142481561622, 1.6004124442903637e-08, 1128037553.692641, 102.98451198974419,
        103.88291053298468, 1.8069356381179478e-07, 1.068314404429744e-07, 18128.56741766678, 11.41341650288274,
        1.4347135437544496e-06, 12487541.348525414, 1289.5015737964563, 0.00012256986487936571, 1.3684183912482998e-10,
        1.8617955826216817e-06, 1.4953446577684579e-05, 1.9471712770935922e-08, 1.566858992639465e-06,
        19018772.034521453, 10068916.415908854, 10196259.665600866, 1642.3896030406938, 1.8659032391084666e-06,
        124869764.20467706, 151.86288391643043, 1.7540908842319238e-10, 116635151.09537168, 18257854144.112854,
        1690.1659611085563, 1.0263146989709814e-05, 14208956947.6589, 0.011376463079404122, 0.0011022396066209714,
        0.16802797307219886, 0.016325572466365325, 0.014742918190110713, 1.5661968084870239e-09, 146.55562002975196,
        14751575074.93262, 1.1516341290132464, 19788178095.54145, 18170.20873101227, 0.00011969598042450296,
        1.938370604374926e-06, 0.00011454930786608158, 1.3327475392242898, 0.17647673518013585, 1659.585700997754,
        1.1097309721571954e-05,
        0.15275447144328191
    };
    // clang-format on

    for (double v : vv) {
        VERIFY(uxs::format("{:a}", v) == std::format("{:a}", v));
        VERIFY(uxs::format("{:f}", v) == std::format("{:f}", v));
        VERIFY(uxs::format("{:e}", v) == std::format("{:e}", v));
        VERIFY(uxs::format("{:g}", v) == std::format("{:g}", v));
        VERIFY(uxs::format("{}", v) == std::format("{}", v));
        for (int prec = 0; prec <= 30; ++prec) {
            VERIFY(uxs::format("{:.{}a}", v, prec) == std::format("{:.{}a}", v, prec));
            VERIFY(uxs::format("{:.{}f}", v, prec) == std::format("{:.{}f}", v, prec));
            VERIFY(uxs::format("{:.{}e}", v, prec) == std::format("{:.{}e}", v, prec));
            VERIFY(uxs::format("{:.{}g}", v, prec) == std::format("{:.{}g}", v, prec));
            VERIFY(uxs::format("{:.{}}", v, prec) == std::format("{:.{}}", v, prec));
        }
    }

#    if defined(_MSC_VER) || defined(_LIBCPP_VERSION)
    for (double v : vv) {
        VERIFY(uxs::format("{:#a}", v) == std::format("{:#a}", v));
        VERIFY(uxs::format("{:#f}", v) == std::format("{:#f}", v));
        VERIFY(uxs::format("{:#e}", v) == std::format("{:#e}", v));
#        if !defined(_LIBCPP_VERSION) || _LIBCPP_VERSION >= 180000
        VERIFY(uxs::format("{:#g}", v) == std::format("{:#g}", v));
#        endif
#        if !defined(_MSC_VER)
        VERIFY(uxs::format("{:#}", v) == std::format("{:#}", v));
#        endif
        for (int prec = 0; prec <= 30; ++prec) {
            VERIFY(uxs::format("{:#.{}a}", v, prec) == std::format("{:#.{}a}", v, prec));
            VERIFY(uxs::format("{:#.{}f}", v, prec) == std::format("{:#.{}f}", v, prec));
            VERIFY(uxs::format("{:#.{}e}", v, prec) == std::format("{:#.{}e}", v, prec));
#        if !defined(_LIBCPP_VERSION) || _LIBCPP_VERSION >= 180000
            VERIFY(uxs::format("{:#.{}g}", v, prec) == std::format("{:#.{}g}", v, prec));
#        endif
            VERIFY(uxs::format("{:#.{}}", v, prec) == std::format("{:#.{}}", v, prec));
        }
    }
#    endif
#endif

    VERIFY(uxs::format("{: >+15.3f}", 1230.) == "      +1230.000");
    VERIFY(uxs::format("{: >+15.3e}", 1230.) == "     +1.230e+03");

    VERIFY(uxs::format("{:f}", 0.) == "0.000000");
    VERIFY(uxs::format("{:.0f}", 0.) == "0");
    VERIFY(uxs::format("{:e}", 0.) == "0.000000e+00");
    VERIFY(uxs::format("{:6e}", 0.) == "0.000000e+00");
    VERIFY(uxs::format("{:g}", 0.) == "0");
    VERIFY(uxs::format("{:15g}", 0.) == "              0");
    VERIFY(uxs::format("{: >15g}", 0.) == "              0");
    VERIFY(uxs::format("{: <15g}", 0.) == "0              ");
    VERIFY(uxs::format("{: ^15g}", 0.) == "       0       ");

    VERIFY(uxs::format("{:G}", 0.) == "0");
    VERIFY(uxs::format("{:015g}", 0.) == "000000000000000");
    VERIFY(uxs::format("{:015G}", 0.) == "000000000000000");

    VERIFY(uxs::format("{:+g}", 0.) == "+0");
    VERIFY(uxs::format("{:+15g}", 0.) == "             +0");
    VERIFY(uxs::format("{: >+15g}", 0.) == "             +0");
    VERIFY(uxs::format("{: <+15g}", 0.) == "+0             ");
    VERIFY(uxs::format("{: ^+15g}", 0.) == "      +0       ");
    VERIFY(uxs::format("{:+G}", 0.) == "+0");
    VERIFY(uxs::format("{:+015g}", 0.) == "+00000000000000");
    VERIFY(uxs::format("{:+015G}", 0.) == "+00000000000000");

    VERIFY(uxs::format("{: g}", 0.) == " 0");
    VERIFY(uxs::format("{: G}", 0.) == " 0");
    VERIFY(uxs::format("{: 015g}", 0.) == " 00000000000000");
    VERIFY(uxs::format("{: 015G}", 0.) == " 00000000000000");

    VERIFY(uxs::format("{:g}", -0.) == "-0");
    VERIFY(uxs::format("{:G}", -0.) == "-0");
    VERIFY(uxs::format("{:015g}", -0.) == "-00000000000000");
    VERIFY(uxs::format("{:015G}", -0.) == "-00000000000000");

    VERIFY(uxs::format("{:+g}", -0.) == "-0");
    VERIFY(uxs::format("{:+G}", -0.) == "-0");
    VERIFY(uxs::format("{:+015g}", -0.) == "-00000000000000");
    VERIFY(uxs::format("{:+015G}", -0.) == "-00000000000000");

    VERIFY(uxs::format("{: g}", -0.) == "-0");
    VERIFY(uxs::format("{: G}", -0.) == "-0");
    VERIFY(uxs::format("{: 015g}", -0.) == "-00000000000000");
    VERIFY(uxs::format("{: 015G}", -0.) == "-00000000000000");

    VERIFY(uxs::format("{:g}", std::numeric_limits<double>::infinity()) == "inf");
    VERIFY(uxs::format("{:G}", std::numeric_limits<double>::infinity()) == "INF");
    VERIFY(uxs::format("{:15g}", std::numeric_limits<double>::infinity()) == "inf            ");
    VERIFY(uxs::format("{:15G}", std::numeric_limits<double>::infinity()) == "INF            ");
    VERIFY(uxs::format("{: >15G}", std::numeric_limits<double>::infinity()) == "            INF");
    VERIFY(uxs::format("{: <15G}", std::numeric_limits<double>::infinity()) == "INF            ");
    VERIFY(uxs::format("{: ^15G}", std::numeric_limits<double>::infinity()) == "      INF      ");

    VERIFY(uxs::format("{:+g}", std::numeric_limits<double>::infinity()) == "+inf");
    VERIFY(uxs::format("{:+G}", std::numeric_limits<double>::infinity()) == "+INF");
    VERIFY(uxs::format("{:+15g}", std::numeric_limits<double>::infinity()) == "+inf           ");
    VERIFY(uxs::format("{:+15G}", std::numeric_limits<double>::infinity()) == "+INF           ");
    VERIFY(uxs::format("{: >+15G}", std::numeric_limits<double>::infinity()) == "           +INF");
    VERIFY(uxs::format("{: <+15G}", std::numeric_limits<double>::infinity()) == "+INF           ");
    VERIFY(uxs::format("{: ^+15G}", std::numeric_limits<double>::infinity()) == "     +INF      ");

    VERIFY(uxs::format("{: g}", std::numeric_limits<double>::infinity()) == " inf");
    VERIFY(uxs::format("{: G}", std::numeric_limits<double>::infinity()) == " INF");
    VERIFY(uxs::format("{: 15g}", std::numeric_limits<double>::infinity()) == " inf           ");
    VERIFY(uxs::format("{: 15G}", std::numeric_limits<double>::infinity()) == " INF           ");
    VERIFY(uxs::format("{: > 15G}", std::numeric_limits<double>::infinity()) == "            INF");
    VERIFY(uxs::format("{: < 15G}", std::numeric_limits<double>::infinity()) == " INF           ");
    VERIFY(uxs::format("{: ^ 15G}", std::numeric_limits<double>::infinity()) == "      INF      ");

    VERIFY(uxs::format("{:g}", -std::numeric_limits<double>::infinity()) == "-inf");
    VERIFY(uxs::format("{:G}", -std::numeric_limits<double>::infinity()) == "-INF");
    VERIFY(uxs::format("{:15g}", -std::numeric_limits<double>::infinity()) == "-inf           ");
    VERIFY(uxs::format("{:15G}", -std::numeric_limits<double>::infinity()) == "-INF           ");

    VERIFY(uxs::format("{:+g}", -std::numeric_limits<double>::infinity()) == "-inf");
    VERIFY(uxs::format("{:+G}", -std::numeric_limits<double>::infinity()) == "-INF");
    VERIFY(uxs::format("{:+15g}", -std::numeric_limits<double>::infinity()) == "-inf           ");
    VERIFY(uxs::format("{:+15G}", -std::numeric_limits<double>::infinity()) == "-INF           ");

    VERIFY(uxs::format("{: g}", -std::numeric_limits<double>::infinity()) == "-inf");
    VERIFY(uxs::format("{: G}", -std::numeric_limits<double>::infinity()) == "-INF");
    VERIFY(uxs::format("{: 15g}", -std::numeric_limits<double>::infinity()) == "-inf           ");
    VERIFY(uxs::format("{: 15G}", -std::numeric_limits<double>::infinity()) == "-INF           ");

    VERIFY(uxs::format("{:g}", std::numeric_limits<double>::quiet_NaN()) == "nan");
    VERIFY(uxs::format("{:G}", std::numeric_limits<double>::quiet_NaN()) == "NAN");
    VERIFY(uxs::format("{:15g}", std::numeric_limits<double>::quiet_NaN()) == "nan            ");
    VERIFY(uxs::format("{:15G}", std::numeric_limits<double>::quiet_NaN()) == "NAN            ");

    VERIFY(uxs::format("{:+g}", std::numeric_limits<double>::quiet_NaN()) == "+nan");
    VERIFY(uxs::format("{:+G}", std::numeric_limits<double>::quiet_NaN()) == "+NAN");
    VERIFY(uxs::format("{:+15g}", std::numeric_limits<double>::quiet_NaN()) == "+nan           ");
    VERIFY(uxs::format("{:+15G}", std::numeric_limits<double>::quiet_NaN()) == "+NAN           ");

    VERIFY(uxs::format("{: g}", std::numeric_limits<double>::quiet_NaN()) == " nan");
    VERIFY(uxs::format("{: G}", std::numeric_limits<double>::quiet_NaN()) == " NAN");
    VERIFY(uxs::format("{: 15g}", std::numeric_limits<double>::quiet_NaN()) == " nan           ");
    VERIFY(uxs::format("{: 15G}", std::numeric_limits<double>::quiet_NaN()) == " NAN           ");

    VERIFY(uxs::format("{:g}", -std::numeric_limits<double>::quiet_NaN()) == "-nan");
    VERIFY(uxs::format("{:G}", -std::numeric_limits<double>::quiet_NaN()) == "-NAN");
    VERIFY(uxs::format("{:15g}", -std::numeric_limits<double>::quiet_NaN()) == "-nan           ");
    VERIFY(uxs::format("{:15G}", -std::numeric_limits<double>::quiet_NaN()) == "-NAN           ");

    VERIFY(uxs::format("{:+g}", -std::numeric_limits<double>::quiet_NaN()) == "-nan");
    VERIFY(uxs::format("{:+G}", -std::numeric_limits<double>::quiet_NaN()) == "-NAN");
    VERIFY(uxs::format("{:+15g}", -std::numeric_limits<double>::quiet_NaN()) == "-nan           ");
    VERIFY(uxs::format("{:+15G}", -std::numeric_limits<double>::quiet_NaN()) == "-NAN           ");

    VERIFY(uxs::format("{: g}", -std::numeric_limits<double>::quiet_NaN()) == "-nan");
    VERIFY(uxs::format("{: G}", -std::numeric_limits<double>::quiet_NaN()) == "-NAN");
    VERIFY(uxs::format("{: 15g}", -std::numeric_limits<double>::quiet_NaN()) == "-nan           ");
    VERIFY(uxs::format("{: 15G}", -std::numeric_limits<double>::quiet_NaN()) == "-NAN           ");

    return 0;
}

int test_string_cvt_3() {
    VERIFY(uxs::from_string<int>("10") == 10);
    VERIFY(uxs::from_string<int>("-25510") == -25510);
    VERIFY(uxs::from_string<int>("+2510") == 2510);

    VERIFY(uxs::from_string<unsigned>("10") == 10);
    VERIFY(uxs::from_string<unsigned>("25510") == 25510);

    VERIFY(fabs(uxs::from_string<float>("0.2355") - 0.2355f) < 0.000001);
    VERIFY(fabs(uxs::from_string<float>(".2355") - 0.2355f) < 0.000001);
    VERIFY(fabs(uxs::from_string<float>(".3") - 0.3f) < 0.000001);
    VERIFY(fabs(uxs::from_string<double>("-123.56e-1") - -123.56e-1) < 0.000001);
    VERIFY(fabs(uxs::from_string<double>("-123.56e-0047") - -123.56e-47) < 0.000001);

    VERIFY(uxs::from_string<bool>("true") == true);
    VERIFY(uxs::from_string<bool>("false") == false);
    VERIFY(uxs::from_string<bool>("1") == true);
    VERIFY(uxs::from_string<bool>("001") == true);
    VERIFY(uxs::from_string<bool>("000") == false);

    uxs::vector<std::tuple<std::string_view, size_t, double>> d_tst;
    d_tst.emplace_back("fhjjh", 0, 12345.);
    d_tst.emplace_back("+fhjjh", 0, 12345.);
    d_tst.emplace_back("-fhjjh", 0, 12345.);
    d_tst.emplace_back(".fhjjh", 0, 12345.);
    d_tst.emplace_back("+.fhjjh", 0, 12345.);
    d_tst.emplace_back("-.fhjjh", 0, 12345.);
    d_tst.emplace_back("+", 0, 12345.);
    d_tst.emplace_back("-", 0, 12345.);
    d_tst.emplace_back(".", 0, 12345.);
    d_tst.emplace_back("+.", 0, 12345.);
    d_tst.emplace_back("-.", 0, 12345.);
    d_tst.emplace_back(".123fhjjh", 4, 0.123);
    d_tst.emplace_back("+.123fhjjh", 5, 0.123);
    d_tst.emplace_back("-.123fhjjh", 5, -0.123);
    d_tst.emplace_back("123wert", 3, 123.);
    d_tst.emplace_back("+123wert", 4, 123.);
    d_tst.emplace_back("-123wert", 4, -123.);
    d_tst.emplace_back("123.wert", 4, 123.);
    d_tst.emplace_back("123.0wert", 5, 123.);
    d_tst.emplace_back("123.1wert", 5, 123.1);
    d_tst.emplace_back("123.1esd", 5, 123.1);
    d_tst.emplace_back("123.1e1sd", 7, 1231.);

    for (const auto& el : d_tst) {
        double d = 12345;
        VERIFY(uxs::stoval(std::get<0>(el), d) == std::get<1>(el) && d == std::get<2>(el));
    }

    uxs::vector<std::tuple<std::string_view, size_t, int>> i_tst;
    i_tst.emplace_back("fhjjh", 0, 12345);
    i_tst.emplace_back("+fhjjh", 0, 12345);
    i_tst.emplace_back("-fhjjh", 0, 12345);
    i_tst.emplace_back("+", 0, 12345);
    i_tst.emplace_back("-", 0, 12345);
    i_tst.emplace_back("123wert", 3, 123);
    i_tst.emplace_back("+123wert", 4, 123);
    i_tst.emplace_back("-123wert", 4, -123);

    for (const auto& el : i_tst) {
        double i = 12345;
        VERIFY(uxs::stoval(std::get<0>(el), i) == std::get<1>(el) && i == std::get<2>(el));
    }

    VERIFY(std::isinf(uxs::from_string<double>("inf")));
    VERIFY(std::isinf(uxs::from_string<double>("+inf")));
    VERIFY(std::isinf(-uxs::from_string<double>("-inf")));
    VERIFY(std::isnan(uxs::from_string<double>("nan")));
    VERIFY(std::isnan(uxs::from_string<double>("+nan")));
    VERIFY(std::isnan(uxs::from_string<double>("-nan")));

    VERIFY(uxs::from_string<double>("1125899906842624.1250000") == 1125899906842624.0);
    VERIFY(uxs::from_string<double>("1125899906842624.1250000000000000000000000000000000001") == 1125899906842624.2);

    VERIFY(uxs::from_string<double>("1125899906842624.3750000") == 1125899906842624.5);
    VERIFY(uxs::from_string<double>("1125899906842624.3750000000000000000000000000000000001") == 1125899906842624.5);

    return 0;
}

int test_string_cvt_4() {
    VERIFY(uxs::format("{:a}", 100.5) == "1.92p+6");
    VERIFY(uxs::format("{:A}", 100.5) == "1.92P+6");
    VERIFY(uxs::format("{:015a}", 100.5) == "000000001.92p+6");
    VERIFY(uxs::format("{:015A}", 100.5) == "000000001.92P+6");

    VERIFY(uxs::format("{:+a}", 100.5) == "+1.92p+6");
    VERIFY(uxs::format("{:+A}", 100.5) == "+1.92P+6");
    VERIFY(uxs::format("{:+015a}", 100.5) == "+00000001.92p+6");
    VERIFY(uxs::format("{:+015A}", 100.5) == "+00000001.92P+6");
    return 0;
}

int test_string_cvt_5() {
    VERIFY(uxs::from_string<char>("a") == 'a');
    VERIFY(uxs::from_wstring<wchar_t>(L"a") == L'a');

    std::string_view s("a");
    char ch = '\0';
    VERIFY(uxs::from_chars(s.data(), s.data() + s.size(), ch) == s.data() + 1 && ch == 'a');
    VERIFY(uxs::stoval(s, ch) == 1 && ch == 'a');

    std::wstring_view ws(L"a");
    wchar_t wch = '\0';
    VERIFY(uxs::from_wchars(ws.data(), ws.data() + ws.size(), wch) == ws.data() + 1 && wch == L'a');
    VERIFY(uxs::wstoval(ws, wch) == 1 && wch == L'a');

    VERIFY(uxs::to_string('a') == "a");
    VERIFY(uxs::to_string(100, uxs::fmt_flags::hex | uxs::fmt_flags::alternate) == "0x64");
    VERIFY(uxs::to_wstring(L'a') == L"a");
    VERIFY(uxs::to_wstring(100, uxs::fmt_flags::hex | uxs::fmt_flags::alternate) == L"0x64");

    char buf[7];
    *uxs::to_chars(buf, 123456) = '\0';
    VERIFY(std::string_view(buf) == "123456");
    *uxs::to_chars(buf, 1234, uxs::fmt_flags::hex | uxs::fmt_flags::alternate) = '\0';
    VERIFY(std::string_view(buf) == "0x4d2");
    *uxs::to_chars_n(buf, 6, 12345678) = '\0';
    VERIFY(std::string_view(buf) == "123456");
    *uxs::to_chars_n(buf, 5, 0x12345, uxs::fmt_flags::hex | uxs::fmt_flags::alternate) = '\0';
    VERIFY(std::string_view(buf) == "0x123");

    wchar_t wbuf[7];
    *uxs::to_wchars(wbuf, 123456) = L'\0';
    VERIFY(std::wstring_view(wbuf) == L"123456");
    *uxs::to_wchars(wbuf, 1234, uxs::fmt_flags::hex | uxs::fmt_flags::alternate) = L'\0';
    VERIFY(std::wstring_view(wbuf) == L"0x4d2");
    *uxs::to_wchars_n(wbuf, 6, 12345678) = L'\0';
    VERIFY(std::wstring_view(wbuf) == L"123456");
    *uxs::to_wchars_n(wbuf, 5, 0x12345, uxs::fmt_flags::hex | uxs::fmt_flags::alternate) = L'\0';
    VERIFY(std::wstring_view(wbuf) == L"0x123");

    return 0;
}

int test_string_cvt_6() {
    std::string_view h{"1234abCDz"};
    unsigned n_parsed = 0;
    VERIFY(uxs::from_hex(h.begin(), 8) == 0x1234abcd);
    VERIFY(uxs::from_hex(h.begin(), 8, uxs::nofunc(), &n_parsed) == 0x1234abcd && n_parsed == 8);
    VERIFY(uxs::from_hex(h.begin(), 9, uxs::nofunc(), &n_parsed) == 0x1234abcd && n_parsed == 8);

    char buf[8];
    uxs::to_hex(0x1234abcd, buf, 8);
    VERIFY(std::string_view{buf, 8} == "1234abcd");
    return 0;
}

ADD_TEST_CASE("", "string conversion", test_string_cvt_dec);
ADD_TEST_CASE("", "string conversion", test_string_cvt_bin);
ADD_TEST_CASE("", "string conversion", test_string_cvt_oct);
ADD_TEST_CASE("", "string conversion", test_string_cvt_hex);
ADD_TEST_CASE("", "string conversion", test_string_cvt_float);
ADD_TEST_CASE("", "string conversion", test_string_cvt_1);
ADD_TEST_CASE("", "string conversion", test_string_cvt_2);
ADD_TEST_CASE("", "string conversion", test_string_cvt_3);
ADD_TEST_CASE("", "string conversion", test_string_cvt_4);
ADD_TEST_CASE("", "string conversion", test_string_cvt_5);
ADD_TEST_CASE("", "string conversion", test_string_cvt_6);

//-----------------------------------------------------------------------------
// Bruteforce tests

#if defined(NDEBUG)
const int brute_N = 500000;
#else   // defined(NDEBUG)
const int brute_N = 5000;
#endif  // defined(NDEBUG)

#if defined(_MSC_VER)
#    define INT64_FMT_STRING "%lld"
#else
#    define INT64_FMT_STRING "%ld"
#endif

//------------ int64_t ------------

struct test_context {
    // 0 - success
    // 1 - int->string failure
    // 2 - string->int failure
    int result = 0;
    std::array<char, 32> s_buf, s_buf_ref;
    std::string_view s, s_ref;
    int64_t val = 0;
    int64_t val1 = 0;
    int64_t val2 = 0;
};

void bruteforce_integer(int iter_count, bool use_locale = false) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int64_t> distribution(std::numeric_limits<int64_t>::min(),
                                                        std::numeric_limits<int64_t>::max());

    struct grouping : std::numpunct<char> {
        char_type do_thousands_sep() const override { return '\''; }
        string_type do_grouping() const override { return "\1\2\3"; }
    };

    std::locale loc{std::locale::classic(), new grouping};

    auto test_func = [=](int iter, int64_t val, test_context& ctx) {
        ctx.result = 0;

        for (unsigned n = 0; n < 1000; ++n) {
            ctx.val = val;
            if (use_locale) {
                ctx.s = std::string_view(ctx.s_buf.data(),
                                         uxs::format_to(ctx.s_buf.data(), loc, "{:L}", val) - ctx.s_buf.data());
            } else {
                ctx.s = std::string_view(ctx.s_buf.data(),
                                         uxs::format_to(ctx.s_buf.data(), "{}", val) - ctx.s_buf.data());
            }
            ctx.s_buf[ctx.s.size()] = '\0';

            if (use_locale) {
                ctx.s_ref = std::string_view(
                    ctx.s_buf_ref.data(), fmt::format_to(ctx.s_buf_ref.data(), loc, "{:L}", val) - ctx.s_buf_ref.data());
            } else {
                ctx.s_ref = std::string_view(
                    ctx.s_buf_ref.data(),
                    fmt::format_to(ctx.s_buf_ref.data(), FMT_COMPILE("{}"), val) - ctx.s_buf_ref.data());
            }

            if (ctx.s != ctx.s_ref) {
                ctx.result = 1;
                return;
            }

            if (!use_locale) {
                ctx.val1 = 0, ctx.val2 = 0;
                if (uxs::stoval(ctx.s, ctx.val1) != ctx.s.size()) {
                    ctx.result = 2;
                    return;
                }
#if defined(has_cpp_lib_to_chars)
                std::from_chars(ctx.s.data(), ctx.s.data() + ctx.s.size(), ctx.val2);
#else
                std::sscanf(ctx.s.data(), INT64_FMT_STRING, &ctx.val2);
#endif
                if (ctx.val1 != ctx.val2) {
                    ctx.result = 1;
                    return;
                }
            }
        }
    };

    std::vector<test_context> ctx(g_proc_num);
    std::vector<std::thread> thrd(g_proc_num - 1);

    for (int iter = 0, perc0 = -1; iter < iter_count;) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
            perc0 = perc;
        }

        for (unsigned proc = 0; proc < g_proc_num; ++proc, ++iter) {
            int64_t val = distribution(generator);

            ctx[proc].result = -1;
            if (proc > 0) {
                thrd[proc - 1] = std::thread(std::bind(test_func, iter, val, std::ref(ctx[proc])));
            } else {
                test_func(iter, val, ctx[0]);
            }
        }

        for (unsigned proc = 0; proc < g_proc_num - 1; ++proc) { thrd[proc].join(); }
        for (unsigned proc = 0; proc < g_proc_num; ++proc) {
            if (ctx[proc].result != 0) {
                uxs::stdbuf::out.endl();
                uxs::println("iter = {}", iter);
                uxs::println("result = {}", ctx[proc].s);
                uxs::println("   ref = {}", ctx[proc].s_ref);
                if (ctx[proc].result == 2) {
                    uxs::println("       src = {}", std::to_string(ctx[proc].val));
                    uxs::println("    parsed = {}", std::to_string(ctx[proc].val1));
                    uxs::println("ref parsed = {}", std::to_string(ctx[proc].val2));
                }
                VERIFY(false);
            }
        }
    }
}

ADD_TEST_CASE("1-bruteforce", "int64_t <-> string", []() {
    bruteforce_integer(brute_N);
    return 0;
});
ADD_TEST_CASE("1-bruteforce", "int64_t <-> string (with locale)", []() {
    bruteforce_integer(brute_N, true);
    return 0;
});

//------------ float & double: fixed format ------------

#if defined(NDEBUG)
const bool is_debug = false;
#else   // defined(NDEBUG)
const bool is_debug = true;
#endif  // defined(NDEBUG)

template<typename Ty>
struct test_context_fp {
    // 0 - success
    // 1 - float->string failure
    // 2 - string->float failure
    int result = 0;
    int k = 0, exp = 0, prec = 0;
    uint64_t uval = 0;
    std::array<char, 1024> s_buf, s_buf_ref;
    std::string_view s, s_ref;
    Ty val = 0;
    Ty val1 = 0;
    Ty val2 = 0;
};

template<typename Ty>
void bruteforce_fp_fixed(int iter_count, bool use_locale = false) {
    std::default_random_engine generator;

    const int bits = std::is_same<Ty, double>::value ? 52 : 23;
    const int sign_bit = std::is_same<Ty, double>::value ? 63 : 31;
    const int pow_bias = std::is_same<Ty, double>::value ? 1023 : 127;
    const int default_prec = std::is_same<Ty, double>::value ? 17 : 9;
    const int max_prec = 30;

    std::uniform_int_distribution<uint64_t> distribution(5, (1ull << bits) - 2);

    int N_err = 1;

    struct grouping : std::numpunct<char> {
        char_type do_decimal_point() const override { return ','; }
        char_type do_thousands_sep() const override { return '\''; }
        string_type do_grouping() const override { return "\1\2\3"; }
    };

    std::locale loc{std::locale::classic(), new grouping};

    auto test_func = [=](int iter, uint64_t mantissa, test_context_fp<Ty>& ctx) {
        ctx.result = 0;
        for (ctx.k = 0; ctx.k <= 140; ++ctx.k) {
            ctx.exp = pow_bias - 70 + ctx.k;
            ctx.uval = mantissa | (static_cast<uint64_t>(ctx.exp) << bits) |
                       (static_cast<uint64_t>((iter & 1)) << sign_bit);
            ctx.val = safe_reinterpret<Ty>(
                static_cast<std::conditional_t<std::is_same<Ty, float>::value, uint32_t, uint64_t>>(ctx.uval));
            for (ctx.prec = max_prec; ctx.prec >= 0; --ctx.prec) {
                if (use_locale) {
                    ctx.s = std::string_view(
                        ctx.s_buf.data(),
                        uxs::format_to(ctx.s_buf.data(), loc, "{:.{}Lf}", ctx.val, ctx.prec) - ctx.s_buf.data());
                    ctx.s_buf[ctx.s.size()] = '\0';

                    ctx.s_ref = std::string_view(
                        ctx.s_buf_ref.data(), fmt::format_to(ctx.s_buf_ref.data(), loc, "{:.{}Lf}", ctx.val, ctx.prec) -
                                                  ctx.s_buf_ref.data());
                } else {
                    ctx.s = std::string_view(
                        ctx.s_buf.data(),
                        uxs::format_to(ctx.s_buf.data(), "{:.{}f}", ctx.val, ctx.prec) - ctx.s_buf.data());
                    ctx.s_buf[ctx.s.size()] = '\0';

                    std::string_view s_pos = ctx.s;
                    if (s_pos[0] == '-') { s_pos = s_pos.substr(1); }

                    int n_digs = static_cast<int>(s_pos.size());
                    if (s_pos[0] == '0') {
                        n_digs = ctx.prec;
                        if (n_digs > 1) {
                            auto p = s_pos.begin() + 2;
                            while (p != s_pos.end() && *p == '0') { ++p, --n_digs; }
                        }
                    } else if (std::find(s_pos.begin(), s_pos.end(), '.') != s_pos.end()) {
                        --n_digs;
                    }

#if defined(has_cpp_lib_to_chars)
                    ctx.s_ref = std::string_view(
                        ctx.s_buf_ref.data(),
                        std::to_chars(ctx.s_buf_ref.data(), ctx.s_buf_ref.data() + ctx.s_buf_ref.size(), ctx.val,
                                      std::chars_format::fixed, ctx.prec)
                                .ptr -
                            ctx.s_buf_ref.data());
#else
                    ctx.s_ref = std::string_view(
                        ctx.s_buf_ref.data(),
                        fmt::format_to(ctx.s_buf_ref.data(), FMT_COMPILE("{:.{}f}"), ctx.val, ctx.prec) -
                            ctx.s_buf_ref.data());
#endif

                    ctx.val1 = 0, ctx.val2 = 0;
                    if (uxs::stoval(ctx.s, ctx.val1) != ctx.s.size()) {
                        ctx.result = 2;
                        return;
                    }
#if defined(has_from_chars_implementation_for_floats)
                    auto result = std::from_chars(ctx.s.data(), ctx.s.data() + ctx.s.size(), ctx.val2);
                    if (result.ec == std::errc::result_out_of_range) {
                        ctx.val2 = std::numeric_limits<Ty>::infinity();
                        if (*ctx.s.data() == '-') { ctx.val2 = -ctx.val2; }
                    }
#else
                    std::sscanf(ctx.s.data(), std::is_same<Ty, double>::value ? "%lf" : "%f", &ctx.val2);
#endif
                    if (ctx.val1 != ctx.val2 || (n_digs >= default_prec && ctx.val1 != ctx.val)) {
                        ctx.result = 2;
                        return;
                    }
                }
                if (ctx.s != ctx.s_ref) {
                    ctx.result = 1;
                    return;
                }
            }
        }
    };

    std::vector<test_context_fp<Ty>> ctx(g_proc_num);
    std::vector<std::thread> thrd(g_proc_num - 1);

    for (int iter = 0, perc0 = -1; iter < iter_count;) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
            perc0 = perc;
        }

        for (unsigned proc = 0; proc < g_proc_num; ++proc, ++iter) {
            uint64_t mantissa = 0;
            if (!is_debug && std::is_same<Ty, float>::value) {
                mantissa = iter & ((1ull << bits) - 1);
            } else if (iter > 0) {
                if (iter <= bits) {
                    mantissa = 1ull << (iter - 1);
                } else if (iter <= 2 * bits - 1) {
                    mantissa = ((1ull << bits) - 1) >> (iter - bits - 1);
                } else {
                    mantissa = distribution(generator);
                }
            }

            ctx[proc].result = -1;
            if (proc > 0) {
                thrd[proc - 1] = std::thread(std::bind(test_func, iter, mantissa, std::ref(ctx[proc])));
            } else {
                test_func(iter, mantissa, ctx[0]);
            }
        }

        for (unsigned proc = 0; proc < g_proc_num - 1; ++proc) { thrd[proc].join(); }

        for (unsigned proc = 0; proc < g_proc_num; ++proc) {
            if (ctx[proc].result != 0) {
                uxs::stdbuf::out.endl();
                uxs::println("iter = {} k = {} prec = {}", iter, ctx[proc].k, ctx[proc].prec);
                uxs::println("result = {}", ctx[proc].s);
                uxs::println("   ref = {}", ctx[proc].s_ref);
                if (ctx[proc].result == 2) {
                    uxs::println("       src = {}", fmt::format("{:.{}e}", ctx[proc].val, default_prec - 1));
                    uxs::println("    parsed = {}", fmt::format("{:.{}e}", ctx[proc].val1, default_prec - 1));
                    uxs::println("ref parsed = {}", fmt::format("{:.{}e}", ctx[proc].val2, default_prec - 1));
                }
                uxs::println("-------------------------");
                uxs::println("mantissa = {};", ctx[proc].uval & ((1ull << bits) - 1));
                uxs::println("exp = {} + {};", ctx[proc].exp - pow_bias, pow_bias);
                VERIFY(--N_err > 0);
            }
        }
    }
}

ADD_TEST_CASE("1-bruteforce", "float <-> string conversion (fixed)", []() {
    bruteforce_fp_fixed<float>(is_debug ? brute_N : 1 << 23);
    return 0;
});
ADD_TEST_CASE("1-bruteforce", "double <-> string conversion (fixed)", []() {
    bruteforce_fp_fixed<double>(brute_N);
    return 0;
});
ADD_TEST_CASE("1-bruteforce", "double <-> string conversion (fixed with locale)", []() {
    bruteforce_fp_fixed<double>(brute_N, true);
    return 0;
});

//------------ float & double: scientific & general format ------------

template<typename Ty>
void bruteforce_fp_sci(bool general, int iter_count) {
    std::default_random_engine generator;

    const int bits = std::is_same<Ty, double>::value ? 52 : 23;
    const int sign_bit = std::is_same<Ty, double>::value ? 63 : 31;
    const int pow_max = std::is_same<Ty, double>::value ? 2047 : 255;
    const int pow_bias = std::is_same<Ty, double>::value ? 1023 : 127;
    const int default_prec = std::is_same<Ty, double>::value ? 17 : 9;
    const int max_prec = 30;

    std::uniform_int_distribution<uint64_t> distribution(5, (1ull << bits) - 2);

    int N_err = 1;

    auto test_func = [=](int iter, uint64_t mantissa, test_context_fp<Ty>& ctx) {
        ctx.result = 0;
        for (ctx.k = 0; ctx.k < pow_max; ++ctx.k) {
            ctx.exp = ctx.k;
            ctx.uval = mantissa | (static_cast<uint64_t>(ctx.exp) << bits) |
                       (static_cast<uint64_t>((iter & 1)) << sign_bit);
            ctx.val = safe_reinterpret<Ty>(
                static_cast<std::conditional_t<std::is_same<Ty, float>::value, uint32_t, uint64_t>>(ctx.uval));
            for (int prec = max_prec; prec > 0; --prec) {
                ctx.prec = prec - (general ? 0 : 1);
                ctx.s = std::string_view(ctx.s_buf.data(),
                                         (general ? uxs::format_to(ctx.s_buf.data(), "{:.{}g}", ctx.val, ctx.prec) :
                                                    uxs::format_to(ctx.s_buf.data(), "{:.{}e}", ctx.val, ctx.prec)) -
                                             ctx.s_buf.data());
                ctx.s_buf[ctx.s.size()] = '\0';

#if defined(has_cpp_lib_to_chars)
                ctx.s_ref = std::string_view(
                    ctx.s_buf_ref.data(),
                    std::to_chars(ctx.s_buf_ref.data(), ctx.s_buf_ref.data() + ctx.s_buf_ref.size(), ctx.val,
                                  general ? std::chars_format::general : std::chars_format::scientific, ctx.prec)
                            .ptr -
                        ctx.s_buf_ref.data());
#else
                ctx.s_ref = std::string_view(
                    ctx.s_buf_ref.data(),
                    (general ? fmt::format_to(ctx.s_buf_ref.data(), FMT_COMPILE("{:.{}g}"), ctx.val, ctx.prec) :
                               fmt::format_to(ctx.s_buf_ref.data(), FMT_COMPILE("{:.{}e}"), ctx.val, ctx.prec)) -
                        ctx.s_buf_ref.data());
#endif

                if (ctx.s != ctx.s_ref) {
                    ctx.result = 1;
                    return;
                }

                ctx.val1 = 0, ctx.val2 = 0;
                if (uxs::stoval(ctx.s, ctx.val1) != ctx.s.size()) {
                    ctx.result = 2;
                    return;
                }
#if defined(has_from_chars_implementation_for_floats)
                auto result = std::from_chars(ctx.s.data(), ctx.s.data() + ctx.s.size(), ctx.val2);
                if (result.ec == std::errc::result_out_of_range) {
                    ctx.val2 = std::numeric_limits<Ty>::infinity();
                    if (*ctx.s.data() == '-') { ctx.val2 = -ctx.val2; }
                }
#else
                std::sscanf(ctx.s.data(), std::is_same<Ty, double>::value ? "%lf" : "%f", &ctx.val2);
#endif
                if (ctx.val1 != ctx.val2 || (prec >= default_prec && ctx.val1 != ctx.val)) {
                    ctx.result = 2;
                    return;
                }
            }
        }
    };

    std::vector<test_context_fp<Ty>> ctx(g_proc_num);
    std::vector<std::thread> thrd(g_proc_num - 1);

    for (int iter = 0, perc0 = -1; iter < iter_count;) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
            perc0 = perc;
        }

        for (unsigned proc = 0; proc < g_proc_num; ++proc, ++iter) {
            uint64_t mantissa = 0;
            if (!is_debug && std::is_same<Ty, float>::value) {
                mantissa = iter & ((1ull << bits) - 1);
            } else if (iter > 0) {
                if (iter <= bits) {
                    mantissa = 1ull << (iter - 1);
                } else if (iter <= 2 * bits - 1) {
                    mantissa = ((1ull << bits) - 1) >> (iter - bits - 1);
                } else {
                    mantissa = distribution(generator);
                }
            }

            ctx[proc].result = -1;
            if (proc > 0) {
                thrd[proc - 1] = std::thread(std::bind(test_func, iter, mantissa, std::ref(ctx[proc])));
            } else {
                test_func(iter, mantissa, ctx[0]);
            }
        }

        for (unsigned proc = 0; proc < g_proc_num - 1; ++proc) { thrd[proc].join(); }

        for (unsigned proc = 0; proc < g_proc_num; ++proc) {
            if (ctx[proc].result != 0) {
                uxs::stdbuf::out.endl();
                uxs::println("iter = {} k = {} prec = {}", iter, ctx[proc].k, ctx[proc].prec);
                uxs::println("result = {}", ctx[proc].s);
                uxs::println("   ref = {}", ctx[proc].s_ref);
                if (ctx[proc].result == 2) {
                    uxs::println("       src = {}", fmt::format("{:.{}e}", ctx[proc].val, default_prec - 1));
                    uxs::println("    parsed = {}", fmt::format("{:.{}e}", ctx[proc].val1, default_prec - 1));
                    uxs::println("ref parsed = {}", fmt::format("{:.{}e}", ctx[proc].val2, default_prec - 1));
                }
                uxs::println("-------------------------");
                uxs::println("mantissa = {};", ctx[proc].uval & ((1ull << bits) - 1));
                uxs::println("exp = {} + {};", ctx[proc].exp - pow_bias, pow_bias);
                VERIFY(--N_err > 0);
            }
        }
    }
}

ADD_TEST_CASE("1-bruteforce", "float <-> string conversion (scientific)", []() {
    bruteforce_fp_sci<float>(false, is_debug ? brute_N : 1 << 23);
    return 0;
});
ADD_TEST_CASE("1-bruteforce", "float <-> string conversion (general)", []() {
    bruteforce_fp_sci<float>(true, is_debug ? brute_N : 1 << 23);
    return 0;
});
ADD_TEST_CASE("1-bruteforce", "double <-> string conversion (scientific)", []() {
    bruteforce_fp_sci<double>(false, brute_N);
    return 0;
});
ADD_TEST_CASE("1-bruteforce", "double <-> string conversion (general)", []() {
    bruteforce_fp_sci<double>(true, brute_N);
    return 0;
});

#if defined(has_cpp_lib_to_chars)
//------------ float & double: hex format ------------

template<typename Ty>
void bruteforce_fp_hex(int iter_count) {
    std::default_random_engine generator;

    const int bits = std::is_same<Ty, double>::value ? 52 : 23;
    const int sign_bit = std::is_same<Ty, double>::value ? 63 : 31;
    const int pow_max = std::is_same<Ty, double>::value ? 2047 : 255;
    const int pow_bias = std::is_same<Ty, double>::value ? 1023 : 127;
    const int max_prec = 20;

    std::uniform_int_distribution<uint64_t> distribution(5, (1ull << bits) - 2);

    int N_err = 1;

    auto test_func = [=](int iter, uint64_t mantissa, test_context_fp<Ty>& ctx) {
        ctx.result = 0;
        for (ctx.k = 0; ctx.k < pow_max; ++ctx.k) {
            ctx.exp = ctx.k;
            ctx.uval = mantissa | (static_cast<uint64_t>(ctx.exp) << bits) |
                       (static_cast<uint64_t>((iter & 1)) << sign_bit);
            ctx.val = safe_reinterpret<Ty>(
                static_cast<std::conditional_t<std::is_same<Ty, float>::value, uint32_t, uint64_t>>(ctx.uval));
            for (int prec = max_prec; prec > 0; --prec) {
                ctx.prec = prec - 2;
                ctx.s = std::string_view(
                    ctx.s_buf.data(), (ctx.prec >= 0 ? uxs::format_to(ctx.s_buf.data(), "{:.{}a}", ctx.val, ctx.prec) :
                                                       uxs::format_to(ctx.s_buf.data(), "{:a}", ctx.val, ctx.prec)) -
                                          ctx.s_buf.data());
                ctx.s_buf[ctx.s.size()] = '\0';

                ctx.s_ref = std::string_view(
                    ctx.s_buf_ref.data(),
                    (ctx.prec >= 0 ? std::to_chars(ctx.s_buf_ref.data(), ctx.s_buf_ref.data() + ctx.s_buf_ref.size(),
                                                   ctx.val, std::chars_format::hex, ctx.prec)
                                         .ptr :
                                     std::to_chars(ctx.s_buf_ref.data(), ctx.s_buf_ref.data() + ctx.s_buf_ref.size(),
                                                   ctx.val, std::chars_format::hex)
                                         .ptr) -
                        ctx.s_buf_ref.data());

                if (ctx.s != ctx.s_ref) {
                    ctx.result = 1;
                    return;
                }
            }
        }
    };

    std::vector<test_context_fp<Ty>> ctx(g_proc_num);
    std::vector<std::thread> thrd(g_proc_num - 1);

    for (int iter = 0, perc0 = -1; iter < iter_count;) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
            perc0 = perc;
        }

        for (unsigned proc = 0; proc < g_proc_num; ++proc, ++iter) {
            uint64_t mantissa = 0;
            if (!is_debug && std::is_same<Ty, float>::value) {
                mantissa = iter & ((1ull << bits) - 1);
            } else if (iter > 0) {
                if (iter <= bits) {
                    mantissa = 1ull << (iter - 1);
                } else if (iter <= 2 * bits - 1) {
                    mantissa = ((1ull << bits) - 1) >> (iter - bits - 1);
                } else {
                    mantissa = distribution(generator);
                }
            }

            ctx[proc].result = -1;
            if (proc > 0) {
                thrd[proc - 1] = std::thread(std::bind(test_func, iter, mantissa, std::ref(ctx[proc])));
            } else {
                test_func(iter, mantissa, ctx[0]);
            }
        }

        for (unsigned proc = 0; proc < g_proc_num - 1; ++proc) { thrd[proc].join(); }

        for (unsigned proc = 0; proc < g_proc_num; ++proc) {
            if (ctx[proc].result != 0) {
                uxs::stdbuf::out.endl();
                uxs::println("iter = {} k = {} prec = {}", iter, ctx[proc].k, ctx[proc].prec);
                uxs::println("result = {}", ctx[proc].s);
                uxs::println("   ref = {}", ctx[proc].s_ref);
                uxs::println("-------------------------");
                uxs::println("mantissa = {};", ctx[proc].uval & ((1ull << bits) - 1));
                uxs::println("exp = {} + {};", ctx[proc].exp - pow_bias, pow_bias);
                VERIFY(--N_err > 0);
            }
        }
    }
}

ADD_TEST_CASE("1-bruteforce", "float <-> string conversion (hex)", []() {
    bruteforce_fp_hex<float>(5000);
    return 0;
});
ADD_TEST_CASE("1-bruteforce", "double <-> string conversion (hex)", []() {
    bruteforce_fp_hex<double>(5000);
    return 0;
});

//------------ float & double: default (roundtrip) format ------------

template<typename Ty>
void bruteforce_fp_roundtrip(int iter_count) {
    std::default_random_engine generator;

    const int bits = std::is_same<Ty, double>::value ? 52 : 23;
    const int sign_bit = std::is_same<Ty, double>::value ? 63 : 31;
    const int pow_max = std::is_same<Ty, double>::value ? 2047 : 255;
    const int pow_bias = std::is_same<Ty, double>::value ? 1023 : 127;
    const int default_prec = std::is_same<Ty, double>::value ? 17 : 9;

    std::uniform_int_distribution<uint64_t> distribution(5, (1ull << bits) - 2);

    int N_err = 1;

    auto test_func = [=](int iter, uint64_t mantissa, test_context_fp<Ty>& ctx) {
        ctx.result = 0;

        for (ctx.k = 0; ctx.k < pow_max; ++ctx.k) {
            ctx.exp = ctx.k;
            ctx.uval = mantissa | (static_cast<uint64_t>(ctx.exp) << bits) |
                       (static_cast<uint64_t>((iter & 1)) << sign_bit);
            ctx.val = safe_reinterpret<Ty>(
                static_cast<std::conditional_t<std::is_same<Ty, float>::value, uint32_t, uint64_t>>(ctx.uval));
            ctx.s = std::string_view(ctx.s_buf.data(),
                                     uxs::format_to(ctx.s_buf.data(), "{}", ctx.val) - ctx.s_buf.data());
            ctx.s_buf[ctx.s.size()] = '\0';

            ctx.val1 = 0, ctx.val2 = 0;
            if (uxs::stoval(ctx.s, ctx.val1) != ctx.s.size()) {
                ctx.result = 2;
                return;
            }
#    if defined(has_from_chars_implementation_for_floats)
            auto result = std::from_chars(ctx.s.data(), ctx.s.data() + ctx.s.size(), ctx.val2);
            if (result.ec == std::errc::result_out_of_range) {
                ctx.val2 = std::numeric_limits<Ty>::infinity();
                if (*ctx.s.data() == '-') { ctx.val2 = -ctx.val2; }
            }
#    else
            std::sscanf(ctx.s.data(), std::is_same<Ty, double>::value ? "%lf" : "%f", &ctx.val2);
#    endif
            if (ctx.val1 != ctx.val2 || ctx.val1 != ctx.val) {
                ctx.result = 2;
                return;
            }
        }
    };

    std::vector<test_context_fp<Ty>> ctx(g_proc_num);
    std::vector<std::thread> thrd(g_proc_num - 1);

    for (int iter = 0, perc0 = -1; iter < iter_count;) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
            perc0 = perc;
        }

        for (unsigned proc = 0; proc < g_proc_num; ++proc, ++iter) {
            uint64_t mantissa = 0;
            if (!is_debug && std::is_same<Ty, float>::value) {
                mantissa = iter & ((1ull << bits) - 1);
            } else if (iter > 0) {
                if (iter <= bits) {
                    mantissa = 1ull << (iter - 1);
                } else if (iter <= 2 * bits - 1) {
                    mantissa = ((1ull << bits) - 1) >> (iter - bits - 1);
                } else {
                    mantissa = distribution(generator);
                }
            }

            ctx[proc].result = -1;
            if (proc > 0) {
                thrd[proc - 1] = std::thread(std::bind(test_func, iter, mantissa, std::ref(ctx[proc])));
            } else {
                test_func(iter, mantissa, ctx[0]);
            }
        }

        for (unsigned proc = 0; proc < g_proc_num - 1; ++proc) { thrd[proc].join(); }

        for (unsigned proc = 0; proc < g_proc_num; ++proc) {
            if (ctx[proc].result != 0) {
                uxs::stdbuf::out.endl();
                uxs::println("iter = {} k = {}", iter, ctx[proc].k);
                uxs::println("result = {}", ctx[proc].s);
                uxs::println("   ref = {}", fmt::format("{}", ctx[proc].val));
                if (ctx[proc].result == 2) {
                    uxs::println("       src = {}", fmt::format("{:.{}e}", ctx[proc].val, default_prec - 1));
                    uxs::println("    parsed = {}", fmt::format("{:.{}e}", ctx[proc].val1, default_prec - 1));
                    uxs::println("ref parsed = {}", fmt::format("{:.{}e}", ctx[proc].val2, default_prec - 1));
                }
                uxs::println("-------------------------");
                uxs::println("mantissa = {};", ctx[proc].uval & ((1ull << bits) - 1));
                uxs::println("exp = {} + {};", ctx[proc].exp - pow_bias, pow_bias);
                VERIFY(--N_err > 0);
            }
        }
    }
}

ADD_TEST_CASE("1-bruteforce", "float <-> string conversion (roundtrip)", []() {
    bruteforce_fp_roundtrip<float>(is_debug ? 10 * brute_N : 1 << 23);
    return 0;
});
ADD_TEST_CASE("1-bruteforce", "double <-> string conversion (roundtrip)", []() {
    bruteforce_fp_roundtrip<double>(10 * brute_N);
    return 0;
});

//------------ float & double: general format with big precision ------------

template<typename Ty>
void bruteforce_fp_big_prec(int iter_count) {
    std::default_random_engine generator;

    const int bits = std::is_same<Ty, double>::value ? 52 : 23;
    const int sign_bit = std::is_same<Ty, double>::value ? 63 : 31;
    const int pow_max = std::is_same<Ty, double>::value ? 2047 : 255;
    const int pow_bias = std::is_same<Ty, double>::value ? 1023 : 127;
    const int default_prec = std::is_same<Ty, double>::value ? 17 : 9;

    std::uniform_int_distribution<uint64_t> distribution(5, (1ull << bits) - 2);
    std::uniform_int_distribution<int> prec_distrib(18, 800);

    int N_err = 1;

    auto test_func = [=](int iter, uint64_t mantissa, int prec, test_context_fp<Ty>& ctx) {
        ctx.result = 0;
        for (ctx.k = 0; ctx.k < pow_max; ++ctx.k) {
            ctx.exp = ctx.k;
            ctx.uval = mantissa | (static_cast<uint64_t>(ctx.exp) << bits) |
                       (static_cast<uint64_t>((iter & 1)) << sign_bit);
            ctx.val = safe_reinterpret<Ty>(
                static_cast<std::conditional_t<std::is_same<Ty, float>::value, uint32_t, uint64_t>>(ctx.uval));
            ctx.prec = prec;
            ctx.s = std::string_view(ctx.s_buf.data(),
                                     uxs::format_to(ctx.s_buf.data(), "{:.{}g}", ctx.val, ctx.prec) - ctx.s_buf.data());
            ctx.s_buf[ctx.s.size()] = '\0';

#    if defined(has_cpp_lib_to_chars)
            ctx.s_ref = std::string_view(
                ctx.s_buf_ref.data(), std::to_chars(ctx.s_buf_ref.data(), ctx.s_buf_ref.data() + ctx.s_buf_ref.size(),
                                                    ctx.val, std::chars_format::general, ctx.prec)
                                              .ptr -
                                          ctx.s_buf_ref.data());
#    else
            ctx.s_ref = std::string_view(
                ctx.s_buf_ref.data(),
                fmt::format_to(ctx.s_buf_ref.data(), FMT_COMPILE("{:.{}g}"), ctx.val, ctx.prec) - ctx.s_buf_ref.data());
#    endif

            if (ctx.s != ctx.s_ref) {
                ctx.result = 1;
                return;
            }

            ctx.val1 = 0, ctx.val2 = 0;
            if (uxs::stoval(ctx.s, ctx.val1) != ctx.s.size()) {
                ctx.result = 2;
                return;
            }
#    if defined(has_from_chars_implementation_for_floats)
            auto result = std::from_chars(ctx.s.data(), ctx.s.data() + ctx.s.size(), ctx.val2);
            if (result.ec == std::errc::result_out_of_range) {
                ctx.val2 = std::numeric_limits<Ty>::infinity();
                if (*ctx.s.data() == '-') { ctx.val2 = -ctx.val2; }
            }
#    else
            std::sscanf(ctx.s.data(), std::is_same<Ty, double>::value ? "%lf" : "%f", &ctx.val2);
#    endif
            if (ctx.val1 != ctx.val2 || (prec >= default_prec && ctx.val1 != ctx.val)) {
                ctx.result = 2;
                return;
            }
        }
    };

    std::vector<test_context_fp<Ty>> ctx(g_proc_num);
    std::vector<std::thread> thrd(g_proc_num - 1);

    for (int iter = 0, perc0 = -1; iter < iter_count;) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
            perc0 = perc;
        }

        for (unsigned proc = 0; proc < g_proc_num; ++proc, ++iter) {
            uint64_t mantissa = 0;
            if (iter > 0) {
                if (iter <= bits) {
                    mantissa = 1ull << (iter - 1);
                } else if (iter <= 2 * bits - 1) {
                    mantissa = ((1ull << bits) - 1) >> (iter - bits - 1);
                } else {
                    mantissa = distribution(generator);
                }
            }

            const int prec = prec_distrib(generator);

            ctx[proc].result = -1;
            if (proc > 0) {
                thrd[proc - 1] = std::thread(std::bind(test_func, iter, mantissa, prec, std::ref(ctx[proc])));
            } else {
                test_func(iter, mantissa, prec, ctx[0]);
            }
        }

        for (unsigned proc = 0; proc < g_proc_num - 1; ++proc) { thrd[proc].join(); }

        for (unsigned proc = 0; proc < g_proc_num; ++proc) {
            if (ctx[proc].result != 0) {
                uxs::stdbuf::out.endl();
                uxs::println("iter = {} k = {} prec = {}", iter, ctx[proc].k, ctx[proc].prec);
                uxs::println("result = {}", ctx[proc].s);
                uxs::println("   ref = {}", ctx[proc].s_ref);
                if (ctx[proc].result == 2) {
                    uxs::println("       src = {}", fmt::format("{:.{}e}", ctx[proc].val, default_prec - 1));
                    uxs::println("    parsed = {}", fmt::format("{:.{}e}", ctx[proc].val1, default_prec - 1));
                    uxs::println("ref parsed = {}", fmt::format("{:.{}e}", ctx[proc].val2, default_prec - 1));
                }
                uxs::println("-------------------------");
                uxs::println("mantissa = {};", ctx[proc].uval & ((1ull << bits) - 1));
                uxs::println("exp = {} + {};", ctx[proc].exp - pow_bias, pow_bias);
                VERIFY(--N_err > 0);
            }
        }
    }
}

ADD_TEST_CASE("1-bruteforce", "float <-> string conversion (general, 18-800 prec)", []() {
    bruteforce_fp_big_prec<float>(10 * brute_N);
    return 0;
});
ADD_TEST_CASE("1-bruteforce", "double <-> string conversion (general, 18-800 prec)", []() {
    bruteforce_fp_big_prec<double>(10 * brute_N);
    return 0;
});

#endif

//-----------------------------------------------------------------------------
// Performance tests

const int perf_N_secs = 5;
const size_t perf_data_set_size = 10000;

//------------ int64_t -> string ------------

template<typename Func>
int perf_int64_to_string(const Func& fn, int n_secs) {
    std::array<char, 128> buf;
    std::default_random_engine generator;
    std::uniform_int_distribution<int64_t> distribution(std::numeric_limits<int64_t>::min(),
                                                        std::numeric_limits<int64_t>::max());

    std::vector<int64_t> v;
    v.resize(perf_data_set_size);
    for (int64_t& val : v) {
        val = distribution(generator);
        const size_t len = fn(buf.data(), buf.data() + buf.size(), val);
        VERIFY(uxs::from_string<int64_t>(std::string_view{buf.data(), len}) == val);
    }

    size_t len = 0;
    int64_t duration = 0;
    int subloops_count = 0;
    const auto start0 = curr_clock_t::now();
    do {
        for (int64_t val : v) { len = std::max<size_t>(len, fn(buf.data(), buf.data() + buf.size(), val)); }
        const auto start = curr_clock_t::now();
        ++subloops_count, duration = as_ns_duration(start - start0);
    } while (duration < 100000000);
    const int loop_count = static_cast<int>(std::ceil((n_secs * subloops_count * 1000000000.0) / duration));
    const auto start = curr_clock_t::now();
    for (int i = 0; i < loop_count; ++i) {
        for (int64_t val : v) { len = std::max<size_t>(len, fn(buf.data(), buf.data() + buf.size(), val)); }
    }
    return len ? static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) /
                                  (loop_count * perf_data_set_size)) :
                 0;
}

ADD_TEST_CASE("2-perf", "int64_t -> string", ([]() {
                  return perf_int64_to_string(
                      [](char* first, char* last, int64_t val) {
                          return static_cast<size_t>(uxs::to_chars(first, val) - first);
                      },
                      perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<libc> int64_t -> string", ([]() {
                  return perf_int64_to_string(
                      [](char* first, char* last, int64_t val) {
                          return static_cast<size_t>(std::sprintf(first, INT64_FMT_STRING, val));
                      },
                      perf_N_secs);
              }));
#if defined(has_cpp_lib_to_chars)
ADD_TEST_CASE("2-perf", "<std::to_chars> int64_t -> string", ([]() {
                  return perf_int64_to_string(
                      [](char* first, char* last, int64_t val) {
                          return static_cast<size_t>(std::to_chars(first, last, val).ptr - first);
                      },
                      perf_N_secs);
              }));
#endif
ADD_TEST_CASE("2-perf", "<{fmt}> int64_t -> string", ([]() {
                  return perf_int64_to_string(
                      [](char* first, char* last, int64_t val) {
                          return static_cast<size_t>(fmt::format_to(first, FMT_COMPILE("{}"), val) - first);
                      },
                      perf_N_secs);
              }));
#if defined(has_cpp_lib_format)
ADD_TEST_CASE("2-perf", "<std::format_to> int64_t -> string", ([]() {
                  return perf_int64_to_string(
                      [](char* first, char* last, int64_t val) {
                          return static_cast<size_t>(std::format_to(first, "{}", val) - first);
                      },
                      perf_N_secs);
              }));
#endif

//------------ string -> int64_t ------------

template<typename Func>
int perf_string_to_int64(const Func& fn, int n_secs) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int64_t> distribution(std::numeric_limits<int64_t>::min(),
                                                        std::numeric_limits<int64_t>::max());

    std::vector<std::string> v;
    v.resize(perf_data_set_size);
    for (auto& s : v) { s = uxs::to_string(distribution(generator)); }

    size_t len = 0;
    int64_t duration = 0;
    int subloops_count = 0;
    const auto start0 = curr_clock_t::now();
    do {
        for (const auto& s : v) {
            int64_t val = 0;
            len = std::max<size_t>(len, fn(s, val));
        }
        const auto start = curr_clock_t::now();
        ++subloops_count, duration = as_ns_duration(start - start0);
    } while (duration < 100000000);
    const int loop_count = static_cast<int>(std::ceil((n_secs * subloops_count * 1000000000.0) / duration));
    const auto start = curr_clock_t::now();
    for (int i = 0; i < loop_count; ++i) {
        for (const auto& s : v) {
            int64_t val = 0;
            len = std::max<size_t>(len, fn(s, val));
        }
    }
    return len ? static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) /
                                  (loop_count * perf_data_set_size)) :
                 0;
}

ADD_TEST_CASE("2-perf", "string -> int64_t", ([]() {
                  return perf_string_to_int64([](std::string_view s, int64_t& val) { return uxs::stoval(s, val); },
                                              perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<libc> string -> int64_t", ([]() {
                  return perf_string_to_int64(
                      [](std::string_view s, int64_t& val) { return std::sscanf(s.data(), INT64_FMT_STRING, &val); },
                      perf_N_secs);
              }));
#if defined(has_cpp_lib_to_chars)
ADD_TEST_CASE("2-perf", "<std::from_chars> string -> int64_t", ([]() {
                  return perf_string_to_int64(
                      [](std::string_view s, int64_t& val) {
                          return static_cast<size_t>(std::from_chars(s.data(), s.data() + s.size(), val).ptr - s.data());
                      },
                      perf_N_secs);
              }));
#endif

//------------ double -> string ------------

template<typename Func, typename... Ts>
int perf_double_to_string(const Func& fn, int n_secs, Ts&&... params) {
    std::array<char, 4096> buf;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> pow_distr(0, 2046);
    std::uniform_int_distribution<uint64_t> mantissa_distr(0, (1ull << 52) - 1);

    uint64_t sign = 0;
    std::vector<double> v;
    v.resize(perf_data_set_size);
    for (double& val : v) {
        val = safe_reinterpret<double>(mantissa_distr(generator) | (static_cast<uint64_t>(pow_distr(generator)) << 52) |
                                       ((sign ^= 1) << 63));
        const size_t len = fn(buf.data(), buf.data() + buf.size(), val, std::forward<Ts>(params)...);
        VERIFY(uxs::from_string<double>(std::string_view{buf.data(), len}) == val);
    }

    size_t len = 0;
    int64_t duration = 0;
    int subloops_count = 0;
    const auto start0 = curr_clock_t::now();
    do {
        for (double val : v) {
            len = std::max<size_t>(len, fn(buf.data(), buf.data() + buf.size(), val, std::forward<Ts>(params)...));
        }
        const auto start = curr_clock_t::now();
        ++subloops_count, duration = as_ns_duration(start - start0);
    } while (duration < 100000000);
    const int loop_count = static_cast<int>(std::ceil((n_secs * subloops_count * 1000000000.0) / duration));
    const auto start = curr_clock_t::now();
    for (int i = 0; i < loop_count; ++i) {
        for (double val : v) {
            len = std::max<size_t>(len, fn(buf.data(), buf.data() + buf.size(), val, std::forward<Ts>(params)...));
        }
    }
    return len ? static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) /
                                  (loop_count * perf_data_set_size)) :
                 0;
}

ADD_TEST_CASE("2-perf", "double -> string (optimal)", ([]() {
                  return perf_double_to_string(
                      [](char* first, char* last, double val) {
                          return static_cast<size_t>(uxs::to_chars(first, val) - first);
                      },
                      perf_N_secs);
              }));
static auto perf_double_to_string_prec = [](char* first, char* last, double val, int prec) {
    return static_cast<size_t>(uxs::to_chars(first, val, uxs::fmt_flags::none, prec) - first);
};
ADD_TEST_CASE("2-perf", "double -> string (    17 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec, perf_N_secs, 17); }));
ADD_TEST_CASE("2-perf", "double -> string (    18 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec, perf_N_secs, 18); }));
ADD_TEST_CASE("2-perf", "double -> string (    19 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec, perf_N_secs, 19); }));
ADD_TEST_CASE("2-perf", "double -> string (    50 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec, perf_N_secs, 50); }));
ADD_TEST_CASE("2-perf", "double -> string (   100 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec, perf_N_secs, 100); }));
ADD_TEST_CASE("2-perf", "double -> string (   240 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec, perf_N_secs, 240); }));
ADD_TEST_CASE("2-perf", "double -> string (   500 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec, perf_N_secs, 500); }));
ADD_TEST_CASE("2-perf", "double -> string (  1000 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec, perf_N_secs, 1000); }));
ADD_TEST_CASE("2-perf", "double -> string (  4000 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec, perf_N_secs, 4000); }));

ADD_TEST_CASE("2-perf", "<libc> double -> string (optimal)", ([]() {
                  return perf_double_to_string(
                      [](char* first, char* last, double val) { return std::sprintf(first, "%.17lg", val); },
                      perf_N_secs);
              }));
static auto perf_double_to_string_prec_libc = [](char* first, char* last, double val, int prec) {
    return std::sprintf(first, "%.*lg", prec, val);
};
ADD_TEST_CASE("2-perf", "<libc> double -> string (    17 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_libc, perf_N_secs, 17); }));
ADD_TEST_CASE("2-perf", "<libc> double -> string (    18 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_libc, perf_N_secs, 18); }));
ADD_TEST_CASE("2-perf", "<libc> double -> string (    19 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_libc, perf_N_secs, 19); }));
ADD_TEST_CASE("2-perf", "<libc> double -> string (    50 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_libc, perf_N_secs, 50); }));
ADD_TEST_CASE("2-perf", "<libc> double -> string (   100 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_libc, perf_N_secs, 100); }));
ADD_TEST_CASE("2-perf", "<libc> double -> string (   240 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_libc, perf_N_secs, 240); }));
ADD_TEST_CASE("2-perf", "<libc> double -> string (   500 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_libc, perf_N_secs, 500); }));
ADD_TEST_CASE("2-perf", "<libc> double -> string (  1000 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_libc, perf_N_secs, 1000); }));
ADD_TEST_CASE("2-perf", "<libc> double -> string (  4000 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_libc, perf_N_secs, 4000); }));

#if defined(has_cpp_lib_to_chars)
ADD_TEST_CASE("2-perf", "<std::to_chars> double -> string (optimal)", ([]() {
                  return perf_double_to_string(
                      [](char* first, char* last, double val) {
                          return static_cast<size_t>(std::to_chars(first, last, val).ptr - first);
                      },
                      perf_N_secs);
              }));
static auto perf_double_to_string_prec_cxx = [](char* first, char* last, double val, int prec) {
    return static_cast<size_t>(std::to_chars(first, last, val, std::chars_format::general, prec).ptr - first);
};
ADD_TEST_CASE("2-perf", "<std::to_chars> double -> string (    17 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 17); }));
ADD_TEST_CASE("2-perf", "<std::to_chars> double -> string (    18 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 18); }));
ADD_TEST_CASE("2-perf", "<std::to_chars> double -> string (    19 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 19); }));
ADD_TEST_CASE("2-perf", "<std::to_chars> double -> string (    50 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 50); }));
ADD_TEST_CASE("2-perf", "<std::to_chars> double -> string (   100 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 100); }));
ADD_TEST_CASE("2-perf", "<std::to_chars> double -> string (   240 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 240); }));
ADD_TEST_CASE("2-perf", "<std::to_chars> double -> string (   500 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 500); }));
ADD_TEST_CASE("2-perf", "<std::to_chars> double -> string (  1000 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 1000); }));
ADD_TEST_CASE("2-perf", "<std::to_chars> double -> string (  4000 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 4000); }));
#endif

ADD_TEST_CASE("2-perf", "<{fmt}> double -> string (optimal)", ([]() {
                  return perf_double_to_string(
                      [](char* first, char* last, double val) {
                          return static_cast<size_t>(fmt::format_to(first, FMT_COMPILE("{}"), val) - first);
                      },
                      perf_N_secs);
              }));
static auto perf_double_to_string_prec_fmt = [](char* first, char* last, double val, int prec) {
    return static_cast<size_t>(fmt::format_to(first, FMT_COMPILE("{:.{}}"), val, prec) - first);
};
ADD_TEST_CASE("2-perf", "<{fmt}> double -> string (    17 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_fmt, perf_N_secs, 17); }));
ADD_TEST_CASE("2-perf", "<{fmt}> double -> string (    18 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_fmt, perf_N_secs, 18); }));
ADD_TEST_CASE("2-perf", "<{fmt}> double -> string (    19 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_fmt, perf_N_secs, 19); }));
ADD_TEST_CASE("2-perf", "<{fmt}> double -> string (    50 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_fmt, perf_N_secs, 50); }));
ADD_TEST_CASE("2-perf", "<{fmt}> double -> string (   100 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_fmt, perf_N_secs, 100); }));
ADD_TEST_CASE("2-perf", "<{fmt}> double -> string (   240 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_fmt, perf_N_secs, 240); }));
ADD_TEST_CASE("2-perf", "<{fmt}> double -> string (   500 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_fmt, perf_N_secs, 500); }));
ADD_TEST_CASE("2-perf", "<{fmt}> double -> string (  1000 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_fmt, perf_N_secs, 1000); }));
ADD_TEST_CASE("2-perf", "<{fmt}> double -> string (  4000 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_fmt, perf_N_secs, 4000); }));
#if defined(has_cpp_lib_format)
ADD_TEST_CASE("2-perf", "<std::format_to> double -> string (optimal)", ([]() {
                  return perf_double_to_string(
                      [](char* first, char* last, double val) {
                          return static_cast<size_t>(std::format_to(first, "{}", val) - first);
                      },
                      perf_N_secs);
              }));
static auto perf_double_to_string_prec_std_format = [](char* first, char* last, double val, int prec) {
    return static_cast<size_t>(std::format_to(first, "{:.{}}", val, prec) - first);
};
ADD_TEST_CASE("2-perf", "<std::format_to> double -> string (    17 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_std_format, perf_N_secs, 17); }));
ADD_TEST_CASE("2-perf", "<std::format_to> double -> string (    18 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_std_format, perf_N_secs, 18); }));
ADD_TEST_CASE("2-perf", "<std::format_to> double -> string (    19 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_std_format, perf_N_secs, 19); }));
ADD_TEST_CASE("2-perf", "<std::format_to> double -> string (    50 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_std_format, perf_N_secs, 50); }));
ADD_TEST_CASE("2-perf", "<std::format_to> double -> string (   100 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_std_format, perf_N_secs, 100); }));
ADD_TEST_CASE("2-perf", "<std::format_to> double -> string (   240 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_std_format, perf_N_secs, 240); }));
ADD_TEST_CASE("2-perf", "<std::format_to> double -> string (   500 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_std_format, perf_N_secs, 500); }));
ADD_TEST_CASE("2-perf", "<std::format_to> double -> string (  1000 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_std_format, perf_N_secs, 1000); }));
ADD_TEST_CASE("2-perf", "<std::format_to> double -> string (  4000 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_std_format, perf_N_secs, 4000); }));
#endif

//------------ string -> double ------------

template<typename Func>
int perf_string_to_double(const Func& fn, int n_secs) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> pow_distr(0, 2046);
    std::uniform_int_distribution<uint64_t> mantissa_distr(0, (1ull << 52) - 1);

    uint64_t sign = 0;
    std::vector<std::string> v;
    v.resize(perf_data_set_size);
    for (auto& s : v) {
        s = uxs::to_string(safe_reinterpret<double>(
            mantissa_distr(generator) | (static_cast<uint64_t>(pow_distr(generator)) << 52) | ((sign ^= 1) << 63)));
    }

    size_t len = 0;
    int64_t duration = 0;
    int subloops_count = 0;
    const auto start0 = curr_clock_t::now();
    do {
        for (const auto& s : v) {
            double val = 0;
            len = std::max<size_t>(len, fn(s, val));
        }
        const auto start = curr_clock_t::now();
        ++subloops_count, duration = as_ns_duration(start - start0);
    } while (duration < 100000000);
    const int loop_count = static_cast<int>(std::ceil((n_secs * subloops_count * 1000000000.0) / duration));
    const auto start = curr_clock_t::now();
    for (int i = 0; i < loop_count; ++i) {
        for (const auto& s : v) {
            double val = 0;
            len = std::max<size_t>(len, fn(s, val));
        }
    }
    return len ? static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) /
                                  (loop_count * perf_data_set_size)) :
                 0;
}

ADD_TEST_CASE("2-perf", "string -> double", ([]() {
                  return perf_string_to_double([](std::string_view s, double& val) { return uxs::stoval(s, val); },
                                               perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<libc> string -> double", ([]() {
                  return perf_string_to_double(
                      [](std::string_view s, double& val) { return std::sscanf(s.data(), "%lf", &val); }, perf_N_secs);
              }));
#if defined(has_from_chars_implementation_for_floats)
ADD_TEST_CASE("2-perf", "<std::from_chars> string -> double", ([]() {
                  return perf_string_to_double(
                      [](std::string_view s, double& val) {
                          return static_cast<size_t>(std::from_chars(s.data(), s.data() + s.size(), val).ptr - s.data());
                      },
                      perf_N_secs);
              }));
#endif

}  // namespace
