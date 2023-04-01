#if defined(_MSC_VER)
#    define _CRT_SECURE_NO_WARNINGS
#endif

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

#if !defined(_MSC_VER) || _MSC_VER > 1800
#    include "fmt/compile.h"
#    include "fmt/format.h"
#    include "milo/dtoa_milo.h"
#endif

#if __cplusplus >= 201703L && \
    ((defined(_MSC_VER) && _MSC_VER >= 1920) || (defined(__GNUC__) && (__GNUC__ >= 11 || __clang_major__ >= 14)))
#    include <charconv>
#endif

using namespace uxs_test_suite;

extern unsigned g_proc_num;

static_assert(uxs::has_string_parser<unsigned char, char>::value, "");
static_assert(uxs::has_string_parser<unsigned short, char>::value, "");
static_assert(uxs::has_string_parser<unsigned, char>::value, "");
static_assert(uxs::has_string_parser<unsigned long, char>::value, "");
static_assert(uxs::has_string_parser<unsigned long long, char>::value, "");
static_assert(uxs::has_string_parser<signed char, char>::value, "");
static_assert(uxs::has_string_parser<signed short, char>::value, "");
static_assert(uxs::has_string_parser<signed, char>::value, "");
static_assert(uxs::has_string_parser<signed long, char>::value, "");
static_assert(uxs::has_string_parser<signed long long, char>::value, "");
static_assert(uxs::has_string_parser<char, char>::value, "");
static_assert(!uxs::has_string_parser<wchar_t, char>::value, "");
static_assert(uxs::has_string_parser<bool, char>::value, "");
static_assert(uxs::has_string_parser<float, char>::value, "");
static_assert(uxs::has_string_parser<double, char>::value, "");
static_assert(uxs::has_string_parser<long double, char>::value, "");

static_assert(uxs::has_string_parser<unsigned char, wchar_t>::value, "");
static_assert(uxs::has_string_parser<unsigned short, wchar_t>::value, "");
static_assert(uxs::has_string_parser<unsigned, wchar_t>::value, "");
static_assert(uxs::has_string_parser<unsigned long, wchar_t>::value, "");
static_assert(uxs::has_string_parser<unsigned long long, wchar_t>::value, "");
static_assert(uxs::has_string_parser<signed char, wchar_t>::value, "");
static_assert(uxs::has_string_parser<signed short, wchar_t>::value, "");
static_assert(uxs::has_string_parser<signed, wchar_t>::value, "");
static_assert(uxs::has_string_parser<signed long, wchar_t>::value, "");
static_assert(uxs::has_string_parser<signed long long, wchar_t>::value, "");
static_assert(!uxs::has_string_parser<char, wchar_t>::value, "");
static_assert(uxs::has_string_parser<wchar_t, wchar_t>::value, "");
static_assert(uxs::has_string_parser<bool, wchar_t>::value, "");
static_assert(uxs::has_string_parser<float, wchar_t>::value, "");
static_assert(uxs::has_string_parser<double, wchar_t>::value, "");
static_assert(uxs::has_string_parser<long double, wchar_t>::value, "");

static_assert(uxs::has_formatter<unsigned char, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<unsigned short, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<unsigned, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<unsigned long, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<unsigned long long, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<signed char, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<signed short, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<signed, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<signed long, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<signed long long, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<char, uxs::membuffer>::value, "");
static_assert(!uxs::has_formatter<wchar_t, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<bool, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<float, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<double, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<long double, uxs::membuffer>::value, "");

static_assert(uxs::has_formatter<unsigned char, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<unsigned short, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<unsigned, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<unsigned long, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<unsigned long long, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<signed char, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<signed short, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<signed, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<signed long, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<signed long long, uxs::wmembuffer>::value, "");
static_assert(!uxs::has_formatter<char, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<wchar_t, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<bool, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<float, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<double, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<long double, uxs::wmembuffer>::value, "");

#if defined(_MSC_VER)
static_assert(uxs::has_string_parser<unsigned __int64, char>::value, "");
static_assert(uxs::has_string_parser<signed __int64, char>::value, "");
static_assert(uxs::has_string_parser<unsigned __int64, wchar_t>::value, "");
static_assert(uxs::has_string_parser<signed __int64, wchar_t>::value, "");
static_assert(uxs::has_formatter<unsigned __int64, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<signed __int64, uxs::membuffer>::value, "");
static_assert(uxs::has_formatter<unsigned __int64, uxs::wmembuffer>::value, "");
static_assert(uxs::has_formatter<signed __int64, uxs::wmembuffer>::value, "");
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
#if !defined(_MSC_VER) || _MSC_VER > 1800
    double vv[] = {4., 3., 3.5, 3.56, 3.567, 3.5672, 3.56723, 3.567234, 3.5672349, 3.56723498, 3.5672349826212314, 0.};

    for (double v : vv) {
        if (v != 0.) { VERIFY("0x" + uxs::format("{:a}", v) == fmt::format("{:a}", v)); }
        VERIFY(uxs::format("{:f}", v) == fmt::format("{:f}", v));
        VERIFY(uxs::format("{:e}", v) == fmt::format("{:e}", v));
        VERIFY(uxs::format("{:g}", v) == fmt::format("{:g}", v));
        VERIFY(uxs::format("{}", v) == fmt::format("{}", v));
        for (int prec = 0; prec <= 30; ++prec) {
            if (v != 0.) { VERIFY("0x" + uxs::format("{:.{}a}", v, prec) == fmt::format("{:.{}a}", v, prec)); }
            VERIFY(uxs::format("{:.{}f}", v, prec) == fmt::format("{:.{}f}", v, prec));
            VERIFY(uxs::format("{:.{}e}", v, prec) == fmt::format("{:.{}e}", v, prec));
            VERIFY(uxs::format("{:.{}g}", v, prec) == fmt::format("{:.{}g}", v, prec));
            VERIFY(uxs::format("{:.{}}", v, prec) == fmt::format("{:.{}}", v, prec));
        }
    }

    for (double v : vv) {
        if (v != 0.) { VERIFY(uxs::format("{:#a}", v) == fmt::format("{:#a}", v)); }
        VERIFY(uxs::format("{:#f}", v) == fmt::format("{:#f}", v));
        VERIFY(uxs::format("{:#e}", v) == fmt::format("{:#e}", v));
        VERIFY(uxs::format("{:#g}", v) == fmt::format("{:#g}", v));
        VERIFY(uxs::format("{:#}", v) == fmt::format("{:#}", v));
        for (int prec = 0; prec <= 30; ++prec) {
            if (v != 0.) { VERIFY(uxs::format("{:#.{}a}", v, prec) == fmt::format("{:#.{}a}", v, prec)); }
            VERIFY(uxs::format("{:#.{}f}", v, prec) == fmt::format("{:#.{}f}", v, prec));
            VERIFY(uxs::format("{:#.{}e}", v, prec) == fmt::format("{:#.{}e}", v, prec));
            std::string s;
            s = uxs::format("{:#.{}g}", v, prec);
            if (s.back() == '.') { s += '0'; }
            VERIFY(s == fmt::format("{:#.{}g}", v, prec));
            s = uxs::format("{:#.{}}", v, prec);
            if (s.back() == '.') { s += '0'; }
            VERIFY(s == fmt::format("{:#.{}}", v, prec));
        }
    }
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
    VERIFY(uxs::format("{:#a}", 100.5) == "0x1.92p+6");
    VERIFY(uxs::format("{:#A}", 100.5) == "0X1.92P+6");
    VERIFY(uxs::format("{:#015a}", 100.5) == "0x0000001.92p+6");
    VERIFY(uxs::format("{:#015A}", 100.5) == "0X0000001.92P+6");

    VERIFY(uxs::format("{:+a}", 100.5) == "+1.92p+6");
    VERIFY(uxs::format("{:+A}", 100.5) == "+1.92P+6");
    VERIFY(uxs::format("{:+015a}", 100.5) == "+00000001.92p+6");
    VERIFY(uxs::format("{:+015A}", 100.5) == "+00000001.92P+6");
    VERIFY(uxs::format("{:+#a}", 100.5) == "+0x1.92p+6");
    VERIFY(uxs::format("{:+#A}", 100.5) == "+0X1.92P+6");
    VERIFY(uxs::format("{:+#015a}", 100.5) == "+0x000001.92p+6");
    VERIFY(uxs::format("{:+#015A}", 100.5) == "+0X000001.92P+6");
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
    VERIFY(uxs::to_string(100, uxs::fmt_flags::kHex | uxs::fmt_flags::kAlternate) == "0x64");
    VERIFY(uxs::to_wstring(L'a') == L"a");
    VERIFY(uxs::to_wstring(100, uxs::fmt_flags::kHex | uxs::fmt_flags::kAlternate) == L"0x64");

    char buf[7];
    *uxs::to_chars(buf, 123456) = '\0';
    VERIFY(std::string_view(buf) == "123456");
    *uxs::to_chars(buf, 1234, uxs::fmt_flags::kHex | uxs::fmt_flags::kAlternate) = '\0';
    VERIFY(std::string_view(buf) == "0x4d2");
    *uxs::to_chars_n(buf, 6, 12345678) = '\0';
    VERIFY(std::string_view(buf) == "123456");
    *uxs::to_chars_n(buf, 5, 0x12345, uxs::fmt_flags::kHex | uxs::fmt_flags::kAlternate) = '\0';
    VERIFY(std::string_view(buf) == "0x123");

    wchar_t wbuf[7];
    *uxs::to_wchars(wbuf, 123456) = L'\0';
    VERIFY(std::wstring_view(wbuf) == L"123456");
    *uxs::to_wchars(wbuf, 1234, uxs::fmt_flags::kHex | uxs::fmt_flags::kAlternate) = L'\0';
    VERIFY(std::wstring_view(wbuf) == L"0x4d2");
    *uxs::to_wchars_n(wbuf, 6, 12345678) = L'\0';
    VERIFY(std::wstring_view(wbuf) == L"123456");
    *uxs::to_wchars_n(wbuf, 5, 0x12345, uxs::fmt_flags::kHex | uxs::fmt_flags::kAlternate) = L'\0';
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

#if !defined(_MSC_VER) || (_MSC_VER > 1800 && __cplusplus >= 201703L)
            if (use_locale) {
                ctx.s_ref = std::string_view(
                    ctx.s_buf_ref.data(), fmt::format_to(ctx.s_buf_ref.data(), loc, "{:L}", val) - ctx.s_buf_ref.data());
            } else {
                ctx.s_ref = std::string_view(
                    ctx.s_buf_ref.data(),
                    fmt::format_to(ctx.s_buf_ref.data(), FMT_COMPILE("{}"), val) - ctx.s_buf_ref.data());
            }
#else
            size_t len = std::sprintf(ctx.s_buf_ref.data(), INT64_FMT_STRING, val);
            ctx.s_ref = std::string_view(ctx.s_buf_ref.data(), len);
#endif

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
#if __cplusplus >= 201703L && \
    ((defined(_MSC_VER) && _MSC_VER >= 1920) || (defined(__GNUC__) && (__GNUC__ >= 11 || __clang_major__ >= 14)))
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
#if !defined(_MSC_VER) || (_MSC_VER > 1800 && __cplusplus >= 201703L)
ADD_TEST_CASE("1-bruteforce", "int64_t <-> string (with locale)", []() {
    bruteforce_integer(brute_N, true);
    return 0;
});
#endif

#if !defined(_MSC_VER) || (_MSC_VER > 1800 && __cplusplus >= 201703L)

//------------ float & double: fixed format ------------

#    if defined(NDEBUG)
const bool is_debug = false;
#    else   // defined(NDEBUG)
const bool is_debug = true;
#    endif  // defined(NDEBUG)

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

#    if __cplusplus >= 201703L && \
        ((defined(_MSC_VER) && _MSC_VER >= 1920) || (defined(__GNUC__) && (__GNUC__ >= 11 || __clang_major__ >= 14)))
                    ctx.s_ref = std::string_view(
                        ctx.s_buf_ref.data(),
                        std::to_chars(ctx.s_buf_ref.data(), ctx.s_buf_ref.data() + ctx.s_buf_ref.size(), ctx.val,
                                      std::chars_format::fixed, ctx.prec)
                                .ptr -
                            ctx.s_buf_ref.data());
#    else
                    ctx.s_ref = std::string_view(
                        ctx.s_buf_ref.data(),
                        fmt::format_to(ctx.s_buf_ref.data(), FMT_COMPILE("{:.{}f}"), ctx.val, ctx.prec) -
                            ctx.s_buf_ref.data());
#    endif

                    ctx.val1 = 0, ctx.val2 = 0;
                    if (uxs::stoval(ctx.s, ctx.val1) != ctx.s.size()) {
                        ctx.result = 2;
                        return;
                    }
#    if __cplusplus >= 201703L && \
        ((defined(_MSC_VER) && _MSC_VER >= 1920) || (defined(__GNUC__) && (__GNUC__ >= 11 || __clang_major__ >= 14)))
                    auto result = std::from_chars(ctx.s.data(), ctx.s.data() + ctx.s.size(), ctx.val2);
                    if (result.ec == std::errc::result_out_of_range) {
                        ctx.val2 = std::numeric_limits<Ty>::infinity();
                        if (*ctx.s.data() == '-') { ctx.val2 = -ctx.val2; }
                    }
#    else
                    std::sscanf(ctx.s.data(), std::is_same<Ty, double>::value ? "%lf" : "%f", &ctx.val2);
#    endif
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

#    if __cplusplus >= 201703L && \
        ((defined(_MSC_VER) && _MSC_VER >= 1920) || (defined(__GNUC__) && (__GNUC__ >= 11 || __clang_major__ >= 14)))
                ctx.s_ref = std::string_view(
                    ctx.s_buf_ref.data(),
                    std::to_chars(ctx.s_buf_ref.data(), ctx.s_buf_ref.data() + ctx.s_buf_ref.size(), ctx.val,
                                  general ? std::chars_format::general : std::chars_format::scientific, ctx.prec)
                            .ptr -
                        ctx.s_buf_ref.data());
#    else
                ctx.s_ref = std::string_view(
                    ctx.s_buf_ref.data(),
                    (general ? fmt::format_to(ctx.s_buf_ref.data(), FMT_COMPILE("{:.{}g}"), ctx.val, ctx.prec) :
                               fmt::format_to(ctx.s_buf_ref.data(), FMT_COMPILE("{:.{}e}"), ctx.val, ctx.prec)) -
                        ctx.s_buf_ref.data());
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
#    if __cplusplus >= 201703L && \
        ((defined(_MSC_VER) && _MSC_VER >= 1920) || (defined(__GNUC__) && (__GNUC__ >= 11 || __clang_major__ >= 14)))
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

#    if __cplusplus >= 201703L && \
        ((defined(_MSC_VER) && _MSC_VER >= 1920) || (defined(__GNUC__) && (__GNUC__ >= 11 || __clang_major__ >= 14)))
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
#    endif

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
#    if __cplusplus >= 201703L && \
        ((defined(_MSC_VER) && _MSC_VER >= 1920) || (defined(__GNUC__) && (__GNUC__ >= 11 || __clang_major__ >= 14)))
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

#    if __cplusplus >= 201703L && \
        ((defined(_MSC_VER) && _MSC_VER >= 1920) || (defined(__GNUC__) && (__GNUC__ >= 11 || __clang_major__ >= 14)))
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
#    if __cplusplus >= 201703L && \
        ((defined(_MSC_VER) && _MSC_VER >= 1920) || (defined(__GNUC__) && (__GNUC__ >= 11 || __clang_major__ >= 14)))
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
#if __cplusplus >= 201703L && \
    ((defined(_MSC_VER) && _MSC_VER >= 1920) || (defined(__GNUC__) && (__GNUC__ >= 11 || __clang_major__ >= 14)))
ADD_TEST_CASE("2-perf", "<c++17/20> int64_t -> string", ([]() {
                  return perf_int64_to_string(
                      [](char* first, char* last, int64_t val) {
                          return static_cast<size_t>(std::to_chars(first, last, val).ptr - first);
                      },
                      perf_N_secs);
              }));
#endif
#if !defined(_MSC_VER) || _MSC_VER > 1800
ADD_TEST_CASE("2-perf", "<{fmt}> int64_t -> string", ([]() {
                  return perf_int64_to_string(
                      [](char* first, char* last, int64_t val) {
                          return static_cast<size_t>(fmt::format_to(first, FMT_COMPILE("{}"), val) - first);
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
#if __cplusplus >= 201703L && \
    ((defined(_MSC_VER) && _MSC_VER >= 1920) || (defined(__GNUC__) && (__GNUC__ >= 11 || __clang_major__ >= 14)))
ADD_TEST_CASE("2-perf", "<c++17/20> string -> int64_t", ([]() {
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
    return static_cast<size_t>(uxs::to_chars(first, val, uxs::fmt_flags::kDefault, prec) - first);
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

#if __cplusplus >= 201703L && \
    ((defined(_MSC_VER) && _MSC_VER >= 1920) || (defined(__GNUC__) && (__GNUC__ >= 11 || __clang_major__ >= 14)))
ADD_TEST_CASE("2-perf", "<c++17/20> double -> string (optimal)", ([]() {
                  return perf_double_to_string(
                      [](char* first, char* last, double val) {
                          return static_cast<size_t>(std::to_chars(first, last, val).ptr - first);
                      },
                      perf_N_secs);
              }));
static auto perf_double_to_string_prec_cxx = [](char* first, char* last, double val, int prec) {
    return static_cast<size_t>(std::to_chars(first, last, val, std::chars_format::general, prec).ptr - first);
};
ADD_TEST_CASE("2-perf", "<c++17/20> double -> string (    17 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 17); }));
ADD_TEST_CASE("2-perf", "<c++17/20> double -> string (    18 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 18); }));
ADD_TEST_CASE("2-perf", "<c++17/20> double -> string (    19 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 19); }));
ADD_TEST_CASE("2-perf", "<c++17/20> double -> string (    50 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 50); }));
ADD_TEST_CASE("2-perf", "<c++17/20> double -> string (   100 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 100); }));
ADD_TEST_CASE("2-perf", "<c++17/20> double -> string (   240 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 240); }));
ADD_TEST_CASE("2-perf", "<c++17/20> double -> string (   500 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 500); }));
ADD_TEST_CASE("2-perf", "<c++17/20> double -> string (  1000 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 1000); }));
ADD_TEST_CASE("2-perf", "<c++17/20> double -> string (  4000 prec)",
              ([]() { return perf_double_to_string(perf_double_to_string_prec_cxx, perf_N_secs, 4000); }));
#endif

#if !defined(_MSC_VER) || _MSC_VER > 1800
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
#if __cplusplus >= 201703L && \
    ((defined(_MSC_VER) && _MSC_VER >= 1920) || (defined(__GNUC__) && (__GNUC__ >= 11 || __clang_major__ >= 14)))
ADD_TEST_CASE("2-perf", "<c++17/20> string -> double", ([]() {
                  return perf_string_to_double(
                      [](std::string_view s, double& val) {
                          return static_cast<size_t>(std::from_chars(s.data(), s.data() + s.size(), val).ptr - s.data());
                      },
                      perf_N_secs);
              }));
#endif

}  // namespace
