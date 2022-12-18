#if defined(_MSC_VER)
#    define _CRT_SECURE_NO_WARNINGS
#endif

#include "test_suite.h"

#include "uxs/format.h"
#include "uxs/vector.h"

#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <random>
#include <thread>

#if !defined(_MSC_VER) || _MSC_VER >= 1920
#    include "fmt/compile.h"
#    include "fmt/format.h"
#    include "milo/dtoa_milo.h"
#endif

#if defined(_MSC_VER) && __cplusplus >= 201703L
#    include <charconv>
#endif

#define MUST_THROW(x) \
    try { \
        x; \
        VERIFY(false); \
    } catch (const uxs::format_error&) {}

using namespace uxs_test_suite;

extern unsigned g_proc_num;

template<typename TyTo, typename TyFrom>
TyTo safe_reinterpret(const TyFrom& v) {
    static_assert(sizeof(TyTo) == sizeof(TyFrom), "bad reinterpret");
    return *reinterpret_cast<const TyTo*>(&v);
}

namespace {

//-----------------------------------------------------------------------------
// Sanity tests

int test_string_cvt_0() {
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{"}, 123));

    VERIFY(uxs::format(uxs::runtime_string{"{}"}, 123) == "123");
    VERIFY(uxs::format(uxs::runtime_string{"{}"}, 'A') == "A");
    VERIFY(uxs::format(uxs::runtime_string{"{}"}, true) == "true");
    VERIFY(uxs::format(uxs::runtime_string{"{}"}, 123.) == "123");
    VERIFY(uxs::format(uxs::runtime_string{"{}"}, 123.f) == "123");
    VERIFY(uxs::format(uxs::runtime_string{"{}"}, reinterpret_cast<void*>(0x123)) == "0x123");
    VERIFY(uxs::format(uxs::runtime_string{"{}"}, "hello") == "hello");

    VERIFY(uxs::format(uxs::runtime_string{"{:d}"}, 123) == "123");
    VERIFY(uxs::format(uxs::runtime_string{"{:d}"}, 'A') == "65");
    VERIFY(uxs::format(uxs::runtime_string{"{:d}"}, true) == "1");
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:d}"}, 123.));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:d}"}, 123.f));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:d}"}, reinterpret_cast<void*>(0x123)));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:d}"}, "hello"));

    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:f}"}, 123));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:f}"}, 'A'));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:f}"}, true));
    VERIFY(uxs::format(uxs::runtime_string{"{:f}"}, 123.) == "123.000000");
    VERIFY(uxs::format(uxs::runtime_string{"{:f}"}, 123.f) == "123.000000");
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:f}"}, reinterpret_cast<void*>(0x123)));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:f}"}, "hello"));

    VERIFY(uxs::format(uxs::runtime_string{"{:c}"}, 123) == "{");
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:c}"}, 1230));
    VERIFY(uxs::format(uxs::runtime_string{"{:c}"}, 'A') == "A");
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:c}"}, true));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:c}"}, 123.));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:c}"}, 123.f));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:c}"}, reinterpret_cast<void*>(0x123)));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:c}"}, "hello"));

    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:p}"}, 123));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:p}"}, 'A'));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:p}"}, true));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:p}"}, 123.));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:p}"}, 123.f));
    VERIFY(uxs::format(uxs::runtime_string{"{:p}"}, reinterpret_cast<void*>(0x123)) == "0x123");
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:p}"}, "hello"));

    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:s}"}, 123));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:s}"}, 'A'));
    VERIFY(uxs::format(uxs::runtime_string{"{:s}"}, true) == "true");
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:s}"}, 123.));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:s}"}, 123.f));
    MUST_THROW((void)uxs::format(uxs::runtime_string{"{:s}"}, reinterpret_cast<void*>(0x123)));
    VERIFY(uxs::format(uxs::runtime_string{"{:s}"}, "hello") == "hello");

    VERIFY(uxs::format(uxs::runtime_wstring{L"{}"}, 'A') == L"A");
    VERIFY(uxs::format(uxs::runtime_wstring{L"{}"}, L'A') == L"A");
    VERIFY(uxs::format(uxs::runtime_wstring{L"{}"}, L"hello") == L"hello");
    VERIFY(uxs::format(uxs::runtime_wstring{L"{:c}"}, 123) == L"{");
#if defined(_MSC_VER)
    MUST_THROW((void)uxs::format(uxs::runtime_wstring{L"{:c}"}, 123000));
#endif
    VERIFY(uxs::format(uxs::runtime_wstring{L"{:c}"}, 'A') == L"A");
    VERIFY(uxs::format(uxs::runtime_wstring{L"{:c}"}, L'A') == L"A");
    VERIFY(uxs::format(uxs::runtime_wstring{L"{:s}"}, L"hello") == L"hello");

    VERIFY(uxs::to_wstring(123.4556) == L"123.4556");
    VERIFY(uxs::format(L"{} {} {}", 123.4556, L"aaa", 567) == L"123.4556 aaa 567");

    return 0;
}

int test_string_cvt_1() {
    VERIFY(uxs::format("{}", 1234) == "1234");
    VERIFY(uxs::format("{:b}", 1234) == "10011010010");
    VERIFY(uxs::format("{:B}", 1234) == "10011010010");
    VERIFY(uxs::format("{:o}", 1234) == "2322");
    VERIFY(uxs::format("{:x}", 1234) == "4d2");
    VERIFY(uxs::format("{:X}", 1234) == "4D2");

    VERIFY(uxs::format("{:#}", 1234) == "1234");
    VERIFY(uxs::format("{:#b}", 1234) == "10011010010b");
    VERIFY(uxs::format("{:#B}", 1234) == "10011010010B");
    VERIFY(uxs::format("{:#o}", 1234) == "02322");
    VERIFY(uxs::format("{:#x}", 1234) == "0x4d2");
    VERIFY(uxs::format("{:#X}", 1234) == "0X4D2");

    VERIFY(uxs::format("{:015}", 1234) == "000000000001234");
    VERIFY(uxs::format("{:015b}", 1234) == "000010011010010");
    VERIFY(uxs::format("{:015B}", 1234) == "000010011010010");
    VERIFY(uxs::format("{:015o}", 1234) == "000000000002322");
    VERIFY(uxs::format("{:015x}", 1234) == "0000000000004d2");
    VERIFY(uxs::format("{:015X}", 1234) == "0000000000004D2");

    VERIFY(uxs::format("{:#015}", 1234) == "000000000001234");
    VERIFY(uxs::format("{:#015b}", 1234) == "00010011010010b");
    VERIFY(uxs::format("{:#015B}", 1234) == "00010011010010B");
    VERIFY(uxs::format("{:#015o}", 1234) == "000000000002322");
    VERIFY(uxs::format("{:#015x}", 1234) == "0x00000000004d2");
    VERIFY(uxs::format("{:#015X}", 1234) == "0X00000000004D2");

    VERIFY(uxs::format("{}", -25510) == "-25510");
    VERIFY(uxs::format("{:b}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:B}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:o}", -25510) == "37777716132");
    VERIFY(uxs::format("{:x}", -25510) == "ffff9c5a");
    VERIFY(uxs::format("{:X}", -25510) == "FFFF9C5A");

    VERIFY(uxs::format("{:#}", -25510) == "-25510");
    VERIFY(uxs::format("{:#b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(uxs::format("{:#B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(uxs::format("{:#o}", -25510) == "037777716132");
    VERIFY(uxs::format("{:#x}", -25510) == "0xffff9c5a");
    VERIFY(uxs::format("{:#X}", -25510) == "0XFFFF9C5A");

    VERIFY(uxs::format("{:015}", -25510) == "-00000000025510");
    VERIFY(uxs::format("{:015b}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:015B}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:015o}", -25510) == "000037777716132");
    VERIFY(uxs::format("{:015x}", -25510) == "0000000ffff9c5a");
    VERIFY(uxs::format("{:015X}", -25510) == "0000000FFFF9C5A");

    VERIFY(uxs::format("{:#015}", -25510) == "-00000000025510");
    VERIFY(uxs::format("{:#015b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(uxs::format("{:#015B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(uxs::format("{:#015o}", -25510) == "000037777716132");
    VERIFY(uxs::format("{:#015x}", -25510) == "0x00000ffff9c5a");
    VERIFY(uxs::format("{:#015X}", -25510) == "0X00000FFFF9C5A");

    VERIFY(uxs::format("{:+}", 1234) == "+1234");
    VERIFY(uxs::format("{: }", 1234) == " 1234");
    VERIFY(uxs::format("{:+b}", 1234) == "10011010010");
    VERIFY(uxs::format("{:+B}", 1234) == "10011010010");
    VERIFY(uxs::format("{:+o}", 1234) == "2322");
    VERIFY(uxs::format("{:+x}", 1234) == "4d2");
    VERIFY(uxs::format("{:+X}", 1234) == "4D2");

    VERIFY(uxs::format("{:+15}", 1234) == "          +1234");
    VERIFY(uxs::format("{: 15}", 1234) == "           1234");
    VERIFY(uxs::format("{:+15b}", 1234) == "    10011010010");
    VERIFY(uxs::format("{:+15B}", 1234) == "    10011010010");
    VERIFY(uxs::format("{:+15o}", 1234) == "           2322");
    VERIFY(uxs::format("{:+15x}", 1234) == "            4d2");
    VERIFY(uxs::format("{:+15X}", 1234) == "            4D2");

    VERIFY(uxs::format("{: >+15}", 1234) == "          +1234");
    VERIFY(uxs::format("{: > 15}", 1234) == "           1234");
    VERIFY(uxs::format("{: >+15b}", 1234) == "    10011010010");
    VERIFY(uxs::format("{: >+15B}", 1234) == "    10011010010");
    VERIFY(uxs::format("{: >+15o}", 1234) == "           2322");
    VERIFY(uxs::format("{: >+15x}", 1234) == "            4d2");
    VERIFY(uxs::format("{: >+15X}", 1234) == "            4D2");

    VERIFY(uxs::format("{: <+15}", 1234) == "+1234          ");
    VERIFY(uxs::format("{: < 15}", 1234) == " 1234          ");
    VERIFY(uxs::format("{: <+15b}", 1234) == "10011010010    ");
    VERIFY(uxs::format("{: <+15B}", 1234) == "10011010010    ");
    VERIFY(uxs::format("{: <+15o}", 1234) == "2322           ");
    VERIFY(uxs::format("{: <+15x}", 1234) == "4d2            ");
    VERIFY(uxs::format("{: <+15X}", 1234) == "4D2            ");

    VERIFY(uxs::format("{: ^+15}", 1234) == "     +1234     ");
    VERIFY(uxs::format("{: ^ 15}", 1234) == "      1234     ");
    VERIFY(uxs::format("{: ^+15b}", 1234) == "  10011010010  ");
    VERIFY(uxs::format("{: ^+15B}", 1234) == "  10011010010  ");
    VERIFY(uxs::format("{: ^+15o}", 1234) == "     2322      ");
    VERIFY(uxs::format("{: ^+15x}", 1234) == "      4d2      ");
    VERIFY(uxs::format("{: ^+15X}", 1234) == "      4D2      ");

    VERIFY(uxs::format("{:+#}", 1234) == "+1234");
    VERIFY(uxs::format("{: #}", 1234) == " 1234");
    VERIFY(uxs::format("{:+#b}", 1234) == "10011010010b");
    VERIFY(uxs::format("{:+#B}", 1234) == "10011010010B");
    VERIFY(uxs::format("{:+#o}", 1234) == "02322");
    VERIFY(uxs::format("{:+#x}", 1234) == "0x4d2");
    VERIFY(uxs::format("{:+#X}", 1234) == "0X4D2");

    VERIFY(uxs::format("{:+#15}", 1234) == "          +1234");
    VERIFY(uxs::format("{: #15}", 1234) == "           1234");
    VERIFY(uxs::format("{:+#15b}", 1234) == "   10011010010b");
    VERIFY(uxs::format("{:+#15B}", 1234) == "   10011010010B");
    VERIFY(uxs::format("{:+#15o}", 1234) == "          02322");
    VERIFY(uxs::format("{:+#15x}", 1234) == "          0x4d2");
    VERIFY(uxs::format("{:+#15X}", 1234) == "          0X4D2");

    VERIFY(uxs::format("{: >+#15}", 1234) == "          +1234");
    VERIFY(uxs::format("{: > #15}", 1234) == "           1234");
    VERIFY(uxs::format("{: >+#15b}", 1234) == "   10011010010b");
    VERIFY(uxs::format("{: >+#15B}", 1234) == "   10011010010B");
    VERIFY(uxs::format("{: >+#15o}", 1234) == "          02322");
    VERIFY(uxs::format("{: >+#15x}", 1234) == "          0x4d2");
    VERIFY(uxs::format("{: >+#15X}", 1234) == "          0X4D2");

    VERIFY(uxs::format("{: <+#15}", 1234) == "+1234          ");
    VERIFY(uxs::format("{: < #15}", 1234) == " 1234          ");
    VERIFY(uxs::format("{: <+#15b}", 1234) == "10011010010b   ");
    VERIFY(uxs::format("{: <+#15B}", 1234) == "10011010010B   ");
    VERIFY(uxs::format("{: <+#15o}", 1234) == "02322          ");
    VERIFY(uxs::format("{: <+#15x}", 1234) == "0x4d2          ");
    VERIFY(uxs::format("{: <+#15X}", 1234) == "0X4D2          ");

    VERIFY(uxs::format("{: ^+#15}", 1234) == "     +1234     ");
    VERIFY(uxs::format("{: ^ #15}", 1234) == "      1234     ");
    VERIFY(uxs::format("{: ^+#15b}", 1234) == " 10011010010b  ");
    VERIFY(uxs::format("{: ^+#15B}", 1234) == " 10011010010B  ");
    VERIFY(uxs::format("{: ^+#15o}", 1234) == "     02322     ");
    VERIFY(uxs::format("{: ^+#15x}", 1234) == "     0x4d2     ");
    VERIFY(uxs::format("{: ^+#15X}", 1234) == "     0X4D2     ");

    VERIFY(uxs::format("{:+015}", 1234) == "+00000000001234");
    VERIFY(uxs::format("{: 015}", 1234) == " 00000000001234");
    VERIFY(uxs::format("{:+015b}", 1234) == "000010011010010");
    VERIFY(uxs::format("{:+015B}", 1234) == "000010011010010");
    VERIFY(uxs::format("{:+015o}", 1234) == "000000000002322");
    VERIFY(uxs::format("{:+015x}", 1234) == "0000000000004d2");
    VERIFY(uxs::format("{:+015X}", 1234) == "0000000000004D2");

    VERIFY(uxs::format("{:+#015}", 1234) == "+00000000001234");
    VERIFY(uxs::format("{: #015}", 1234) == " 00000000001234");
    VERIFY(uxs::format("{:+#015b}", 1234) == "00010011010010b");
    VERIFY(uxs::format("{:+#015B}", 1234) == "00010011010010B");
    VERIFY(uxs::format("{:+#015o}", 1234) == "000000000002322");
    VERIFY(uxs::format("{:+#015x}", 1234) == "0x00000000004d2");
    VERIFY(uxs::format("{:+#015X}", 1234) == "0X00000000004D2");

    VERIFY(uxs::format("{:+}", -25510) == "-25510");
    VERIFY(uxs::format("{: }", -25510) == "-25510");
    VERIFY(uxs::format("{:+b}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:+B}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:+o}", -25510) == "37777716132");
    VERIFY(uxs::format("{:+x}", -25510) == "ffff9c5a");
    VERIFY(uxs::format("{:+X}", -25510) == "FFFF9C5A");

    VERIFY(uxs::format("{:+15}", -25510) == "         -25510");
    VERIFY(uxs::format("{: 15}", -25510) == "         -25510");
    VERIFY(uxs::format("{:+15b}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:+15B}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:+15o}", -25510) == "    37777716132");
    VERIFY(uxs::format("{:+15x}", -25510) == "       ffff9c5a");
    VERIFY(uxs::format("{:+15X}", -25510) == "       FFFF9C5A");

    VERIFY(uxs::format("{: >+15}", -25510) == "         -25510");
    VERIFY(uxs::format("{: > 15}", -25510) == "         -25510");
    VERIFY(uxs::format("{: >+15b}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{: >+15B}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{: >+15o}", -25510) == "    37777716132");
    VERIFY(uxs::format("{: >+15x}", -25510) == "       ffff9c5a");
    VERIFY(uxs::format("{: >+15X}", -25510) == "       FFFF9C5A");

    VERIFY(uxs::format("{: <+15}", -25510) == "-25510         ");
    VERIFY(uxs::format("{: < 15}", -25510) == "-25510         ");
    VERIFY(uxs::format("{: <+15b}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{: <+15B}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{: <+15o}", -25510) == "37777716132    ");
    VERIFY(uxs::format("{: <+15x}", -25510) == "ffff9c5a       ");
    VERIFY(uxs::format("{: <+15X}", -25510) == "FFFF9C5A       ");

    VERIFY(uxs::format("{: ^+15}", -25510) == "    -25510     ");
    VERIFY(uxs::format("{: ^ 15}", -25510) == "    -25510     ");
    VERIFY(uxs::format("{: ^+15b}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{: ^+15B}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{: ^+15o}", -25510) == "  37777716132  ");
    VERIFY(uxs::format("{: ^+15x}", -25510) == "   ffff9c5a    ");
    VERIFY(uxs::format("{: ^+15X}", -25510) == "   FFFF9C5A    ");

    VERIFY(uxs::format("{:+#}", -25510) == "-25510");
    VERIFY(uxs::format("{: #}", -25510) == "-25510");
    VERIFY(uxs::format("{:+#b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(uxs::format("{:+#B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(uxs::format("{:+#o}", -25510) == "037777716132");
    VERIFY(uxs::format("{:+#x}", -25510) == "0xffff9c5a");
    VERIFY(uxs::format("{:+#X}", -25510) == "0XFFFF9C5A");

    VERIFY(uxs::format("{:+#15}", -25510) == "         -25510");
    VERIFY(uxs::format("{: #15}", -25510) == "         -25510");
    VERIFY(uxs::format("{:+#15b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(uxs::format("{:+#15B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(uxs::format("{:+#15o}", -25510) == "   037777716132");
    VERIFY(uxs::format("{:+#15x}", -25510) == "     0xffff9c5a");
    VERIFY(uxs::format("{:+#15X}", -25510) == "     0XFFFF9C5A");

    VERIFY(uxs::format("{: >+#15}", -25510) == "         -25510");
    VERIFY(uxs::format("{: > #15}", -25510) == "         -25510");
    VERIFY(uxs::format("{: >+#15b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(uxs::format("{: >+#15B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(uxs::format("{: >+#15o}", -25510) == "   037777716132");
    VERIFY(uxs::format("{: >+#15x}", -25510) == "     0xffff9c5a");
    VERIFY(uxs::format("{: >+#15X}", -25510) == "     0XFFFF9C5A");

    VERIFY(uxs::format("{: <+#15}", -25510) == "-25510         ");
    VERIFY(uxs::format("{: < #15}", -25510) == "-25510         ");
    VERIFY(uxs::format("{: <+#15b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(uxs::format("{: <+#15B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(uxs::format("{: <+#15o}", -25510) == "037777716132   ");
    VERIFY(uxs::format("{: <+#15x}", -25510) == "0xffff9c5a     ");
    VERIFY(uxs::format("{: <+#15X}", -25510) == "0XFFFF9C5A     ");

    VERIFY(uxs::format("{: ^+#15}", -25510) == "    -25510     ");
    VERIFY(uxs::format("{: ^ #15}", -25510) == "    -25510     ");
    VERIFY(uxs::format("{: ^+#15b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(uxs::format("{: ^+#15B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(uxs::format("{: ^+#15o}", -25510) == " 037777716132  ");
    VERIFY(uxs::format("{: ^+#15x}", -25510) == "  0xffff9c5a   ");
    VERIFY(uxs::format("{: ^+#15X}", -25510) == "  0XFFFF9C5A   ");

    VERIFY(uxs::format("{:+015}", -25510) == "-00000000025510");
    VERIFY(uxs::format("{: 015}", -25510) == "-00000000025510");
    VERIFY(uxs::format("{:+015b}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:+015B}", -25510) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:+015o}", -25510) == "000037777716132");
    VERIFY(uxs::format("{:+015x}", -25510) == "0000000ffff9c5a");
    VERIFY(uxs::format("{:+015X}", -25510) == "0000000FFFF9C5A");

    VERIFY(uxs::format("{:+#015}", -25510) == "-00000000025510");
    VERIFY(uxs::format("{: #015}", -25510) == "-00000000025510");
    VERIFY(uxs::format("{:+#015b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(uxs::format("{:+#015B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(uxs::format("{:+#015o}", -25510) == "000037777716132");
    VERIFY(uxs::format("{:+#015x}", -25510) == "0x00000ffff9c5a");
    VERIFY(uxs::format("{:+#015X}", -25510) == "0X00000FFFF9C5A");

    return 0;
}

int test_string_cvt_1u() {
    VERIFY(uxs::format("{}", 1234u) == "1234");
    VERIFY(uxs::format("{:b}", 1234u) == "10011010010");
    VERIFY(uxs::format("{:B}", 1234u) == "10011010010");
    VERIFY(uxs::format("{:o}", 1234u) == "2322");
    VERIFY(uxs::format("{:x}", 1234u) == "4d2");
    VERIFY(uxs::format("{:X}", 1234u) == "4D2");

    VERIFY(uxs::format("{:#}", 1234u) == "1234");
    VERIFY(uxs::format("{:#b}", 1234u) == "10011010010b");
    VERIFY(uxs::format("{:#B}", 1234u) == "10011010010B");
    VERIFY(uxs::format("{:#o}", 1234u) == "02322");
    VERIFY(uxs::format("{:#x}", 1234u) == "0x4d2");
    VERIFY(uxs::format("{:#X}", 1234u) == "0X4D2");

    VERIFY(uxs::format("{:015}", 1234u) == "000000000001234");
    VERIFY(uxs::format("{:015b}", 1234u) == "000010011010010");
    VERIFY(uxs::format("{:015B}", 1234u) == "000010011010010");
    VERIFY(uxs::format("{:015o}", 1234u) == "000000000002322");
    VERIFY(uxs::format("{:015x}", 1234u) == "0000000000004d2");
    VERIFY(uxs::format("{:015X}", 1234u) == "0000000000004D2");

    VERIFY(uxs::format("{:#015}", 1234u) == "000000000001234");
    VERIFY(uxs::format("{:#015b}", 1234u) == "00010011010010b");
    VERIFY(uxs::format("{:#015B}", 1234u) == "00010011010010B");
    VERIFY(uxs::format("{:#015o}", 1234u) == "000000000002322");
    VERIFY(uxs::format("{:#015x}", 1234u) == "0x00000000004d2");
    VERIFY(uxs::format("{:#015X}", 1234u) == "0X00000000004D2");

    VERIFY(uxs::format("{}", static_cast<unsigned>(-25510)) == "4294941786");
    VERIFY(uxs::format("{:b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:o}", static_cast<unsigned>(-25510)) == "37777716132");
    VERIFY(uxs::format("{:x}", static_cast<unsigned>(-25510)) == "ffff9c5a");
    VERIFY(uxs::format("{:X}", static_cast<unsigned>(-25510)) == "FFFF9C5A");

    VERIFY(uxs::format("{:#}", static_cast<unsigned>(-25510)) == "4294941786");
    VERIFY(uxs::format("{:#b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010b");
    VERIFY(uxs::format("{:#B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010B");
    VERIFY(uxs::format("{:#o}", static_cast<unsigned>(-25510)) == "037777716132");
    VERIFY(uxs::format("{:#x}", static_cast<unsigned>(-25510)) == "0xffff9c5a");
    VERIFY(uxs::format("{:#X}", static_cast<unsigned>(-25510)) == "0XFFFF9C5A");

    VERIFY(uxs::format("{:015}", static_cast<unsigned>(-25510)) == "000004294941786");
    VERIFY(uxs::format("{:015b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:015B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:015o}", static_cast<unsigned>(-25510)) == "000037777716132");
    VERIFY(uxs::format("{:015x}", static_cast<unsigned>(-25510)) == "0000000ffff9c5a");
    VERIFY(uxs::format("{:015X}", static_cast<unsigned>(-25510)) == "0000000FFFF9C5A");

    VERIFY(uxs::format("{:#015}", static_cast<unsigned>(-25510)) == "000004294941786");
    VERIFY(uxs::format("{:#015b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010b");
    VERIFY(uxs::format("{:#015B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010B");
    VERIFY(uxs::format("{:#015o}", static_cast<unsigned>(-25510)) == "000037777716132");
    VERIFY(uxs::format("{:#015x}", static_cast<unsigned>(-25510)) == "0x00000ffff9c5a");
    VERIFY(uxs::format("{:#015X}", static_cast<unsigned>(-25510)) == "0X00000FFFF9C5A");

    VERIFY(uxs::format("{:+}", 1234u) == "1234");
    VERIFY(uxs::format("{: }", 1234u) == "1234");
    VERIFY(uxs::format("{:+b}", 1234u) == "10011010010");
    VERIFY(uxs::format("{:+B}", 1234u) == "10011010010");
    VERIFY(uxs::format("{:+o}", 1234u) == "2322");
    VERIFY(uxs::format("{:+x}", 1234u) == "4d2");
    VERIFY(uxs::format("{:+X}", 1234u) == "4D2");

    VERIFY(uxs::format("{:+#}", 1234u) == "1234");
    VERIFY(uxs::format("{: #}", 1234u) == "1234");
    VERIFY(uxs::format("{:+#b}", 1234u) == "10011010010b");
    VERIFY(uxs::format("{:+#B}", 1234u) == "10011010010B");
    VERIFY(uxs::format("{:+#o}", 1234u) == "02322");
    VERIFY(uxs::format("{:+#x}", 1234u) == "0x4d2");
    VERIFY(uxs::format("{:+#X}", 1234u) == "0X4D2");

    VERIFY(uxs::format("{:+015}", 1234u) == "000000000001234");
    VERIFY(uxs::format("{: 015}", 1234u) == "000000000001234");
    VERIFY(uxs::format("{:+015b}", 1234u) == "000010011010010");
    VERIFY(uxs::format("{:+015B}", 1234u) == "000010011010010");
    VERIFY(uxs::format("{:+015o}", 1234u) == "000000000002322");
    VERIFY(uxs::format("{:+015x}", 1234u) == "0000000000004d2");
    VERIFY(uxs::format("{:+015X}", 1234u) == "0000000000004D2");

    VERIFY(uxs::format("{:+#015}", 1234u) == "000000000001234");
    VERIFY(uxs::format("{: #015}", 1234u) == "000000000001234");
    VERIFY(uxs::format("{:+#015b}", 1234u) == "00010011010010b");
    VERIFY(uxs::format("{:+#015B}", 1234u) == "00010011010010B");
    VERIFY(uxs::format("{:+#015o}", 1234u) == "000000000002322");
    VERIFY(uxs::format("{:+#015x}", 1234u) == "0x00000000004d2");
    VERIFY(uxs::format("{:+#015X}", 1234u) == "0X00000000004D2");

    VERIFY(uxs::format("{:+}", static_cast<unsigned>(-25510)) == "4294941786");
    VERIFY(uxs::format("{: }", static_cast<unsigned>(-25510)) == "4294941786");
    VERIFY(uxs::format("{:+b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:+B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:+o}", static_cast<unsigned>(-25510)) == "37777716132");
    VERIFY(uxs::format("{:+x}", static_cast<unsigned>(-25510)) == "ffff9c5a");
    VERIFY(uxs::format("{:+X}", static_cast<unsigned>(-25510)) == "FFFF9C5A");

    VERIFY(uxs::format("{:+#}", static_cast<unsigned>(-25510)) == "4294941786");
    VERIFY(uxs::format("{: #}", static_cast<unsigned>(-25510)) == "4294941786");
    VERIFY(uxs::format("{:+#b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010b");
    VERIFY(uxs::format("{:+#B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010B");
    VERIFY(uxs::format("{:+#o}", static_cast<unsigned>(-25510)) == "037777716132");
    VERIFY(uxs::format("{:+#x}", static_cast<unsigned>(-25510)) == "0xffff9c5a");
    VERIFY(uxs::format("{:+#X}", static_cast<unsigned>(-25510)) == "0XFFFF9C5A");

    VERIFY(uxs::format("{:+015}", static_cast<unsigned>(-25510)) == "000004294941786");
    VERIFY(uxs::format("{: 015}", static_cast<unsigned>(-25510)) == "000004294941786");
    VERIFY(uxs::format("{:+015b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:+015B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(uxs::format("{:+015o}", static_cast<unsigned>(-25510)) == "000037777716132");
    VERIFY(uxs::format("{:+015x}", static_cast<unsigned>(-25510)) == "0000000ffff9c5a");
    VERIFY(uxs::format("{:+015X}", static_cast<unsigned>(-25510)) == "0000000FFFF9C5A");

    VERIFY(uxs::format("{:+#015}", static_cast<unsigned>(-25510)) == "000004294941786");
    VERIFY(uxs::format("{: #015}", static_cast<unsigned>(-25510)) == "000004294941786");
    VERIFY(uxs::format("{:+#015b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010b");
    VERIFY(uxs::format("{:+#015B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010B");
    VERIFY(uxs::format("{:+#015o}", static_cast<unsigned>(-25510)) == "000037777716132");
    VERIFY(uxs::format("{:+#015x}", static_cast<unsigned>(-25510)) == "0x00000ffff9c5a");
    VERIFY(uxs::format("{:+#015X}", static_cast<unsigned>(-25510)) == "0X00000FFFF9C5A");

    return 0;
}

int test_string_cvt_2() {
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

int test_string_cvt_3() {
#if !defined(_MSC_VER) || _MSC_VER >= 1920
    double vv[] = {3., 3.5, 3.56, 3.567, 3.5672, 3.56723, 3.567234, 0.};

    for (double v : vv) {
        VERIFY(fmt::format("{:f}", v) == uxs::format("{:f}", v));
        VERIFY(fmt::format("{:e}", v) == uxs::format("{:e}", v));
        VERIFY(fmt::format("{:g}", v) == uxs::format("{:g}", v));
        VERIFY(fmt::format("{}", v) == uxs::format("{}", v));
        for (int prec = 0; prec <= 10; ++prec) {
            VERIFY(fmt::format("{:.{}f}", v, prec) == uxs::format("{:.{}f}", v, prec));
            VERIFY(fmt::format("{:.{}e}", v, prec) == uxs::format("{:.{}e}", v, prec));
            VERIFY(fmt::format("{:.{}g}", v, prec) == uxs::format("{:.{}g}", v, prec));
            VERIFY(fmt::format("{:.{}}", v, prec) == uxs::format("{:.{}}", v, prec));
        }
    }

    for (double v : vv) {
        VERIFY(fmt::format("{:#f}", v) == uxs::format("{:#f}", v));
        VERIFY(fmt::format("{:#e}", v) == uxs::format("{:#e}", v));
        VERIFY(fmt::format("{:#g}", v) == uxs::format("{:#g}", v));
        VERIFY(fmt::format("{:#}", v) == uxs::format("{:#}", v));
        for (int prec = 0; prec <= 10; ++prec) {
            VERIFY(fmt::format("{:#.{}f}", v, prec) == uxs::format("{:#.{}f}", v, prec));
            VERIFY(fmt::format("{:#.{}e}", v, prec) == uxs::format("{:#.{}e}", v, prec));
            VERIFY(fmt::format("{:#.{}g}", v, prec) == uxs::format("{:#.{}g}", v, prec));
            VERIFY(fmt::format("{:#.{}}", v, prec) == uxs::format("{:#.{}}", v, prec));
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
    VERIFY(uxs::format("{:015g}", std::numeric_limits<double>::infinity()) == "            inf");
    VERIFY(uxs::format("{:015G}", std::numeric_limits<double>::infinity()) == "            INF");
    VERIFY(uxs::format("{: >015G}", std::numeric_limits<double>::infinity()) == "            INF");
    VERIFY(uxs::format("{: <015G}", std::numeric_limits<double>::infinity()) == "INF            ");
    VERIFY(uxs::format("{: ^015G}", std::numeric_limits<double>::infinity()) == "      INF      ");

    VERIFY(uxs::format("{:+g}", std::numeric_limits<double>::infinity()) == "+inf");
    VERIFY(uxs::format("{:+G}", std::numeric_limits<double>::infinity()) == "+INF");
    VERIFY(uxs::format("{:+015g}", std::numeric_limits<double>::infinity()) == "           +inf");
    VERIFY(uxs::format("{:+015G}", std::numeric_limits<double>::infinity()) == "           +INF");
    VERIFY(uxs::format("{: >+015G}", std::numeric_limits<double>::infinity()) == "           +INF");
    VERIFY(uxs::format("{: <+015G}", std::numeric_limits<double>::infinity()) == "+INF           ");
    VERIFY(uxs::format("{: ^+015G}", std::numeric_limits<double>::infinity()) == "     +INF      ");

    VERIFY(uxs::format("{: g}", std::numeric_limits<double>::infinity()) == " inf");
    VERIFY(uxs::format("{: G}", std::numeric_limits<double>::infinity()) == " INF");
    VERIFY(uxs::format("{: 015g}", std::numeric_limits<double>::infinity()) == "            inf");
    VERIFY(uxs::format("{: 015G}", std::numeric_limits<double>::infinity()) == "            INF");
    VERIFY(uxs::format("{: > 015G}", std::numeric_limits<double>::infinity()) == "            INF");
    VERIFY(uxs::format("{: < 015G}", std::numeric_limits<double>::infinity()) == " INF           ");
    VERIFY(uxs::format("{: ^ 015G}", std::numeric_limits<double>::infinity()) == "      INF      ");

    VERIFY(uxs::format("{:g}", -std::numeric_limits<double>::infinity()) == "-inf");
    VERIFY(uxs::format("{:G}", -std::numeric_limits<double>::infinity()) == "-INF");
    VERIFY(uxs::format("{:015g}", -std::numeric_limits<double>::infinity()) == "           -inf");
    VERIFY(uxs::format("{:015G}", -std::numeric_limits<double>::infinity()) == "           -INF");

    VERIFY(uxs::format("{:+g}", -std::numeric_limits<double>::infinity()) == "-inf");
    VERIFY(uxs::format("{:+G}", -std::numeric_limits<double>::infinity()) == "-INF");
    VERIFY(uxs::format("{:+015g}", -std::numeric_limits<double>::infinity()) == "           -inf");
    VERIFY(uxs::format("{:+015G}", -std::numeric_limits<double>::infinity()) == "           -INF");

    VERIFY(uxs::format("{: g}", -std::numeric_limits<double>::infinity()) == "-inf");
    VERIFY(uxs::format("{: G}", -std::numeric_limits<double>::infinity()) == "-INF");
    VERIFY(uxs::format("{: 015g}", -std::numeric_limits<double>::infinity()) == "           -inf");
    VERIFY(uxs::format("{: 015G}", -std::numeric_limits<double>::infinity()) == "           -INF");

    VERIFY(uxs::format("{:g}", std::numeric_limits<double>::quiet_NaN()) == "nan");
    VERIFY(uxs::format("{:G}", std::numeric_limits<double>::quiet_NaN()) == "NAN");
    VERIFY(uxs::format("{:015g}", std::numeric_limits<double>::quiet_NaN()) == "            nan");
    VERIFY(uxs::format("{:015G}", std::numeric_limits<double>::quiet_NaN()) == "            NAN");

    VERIFY(uxs::format("{:+g}", std::numeric_limits<double>::quiet_NaN()) == "+nan");
    VERIFY(uxs::format("{:+G}", std::numeric_limits<double>::quiet_NaN()) == "+NAN");
    VERIFY(uxs::format("{:+015g}", std::numeric_limits<double>::quiet_NaN()) == "           +nan");
    VERIFY(uxs::format("{:+015G}", std::numeric_limits<double>::quiet_NaN()) == "           +NAN");

    VERIFY(uxs::format("{: g}", std::numeric_limits<double>::quiet_NaN()) == " nan");
    VERIFY(uxs::format("{: G}", std::numeric_limits<double>::quiet_NaN()) == " NAN");
    VERIFY(uxs::format("{: 015g}", std::numeric_limits<double>::quiet_NaN()) == "            nan");
    VERIFY(uxs::format("{: 015G}", std::numeric_limits<double>::quiet_NaN()) == "            NAN");

    VERIFY(uxs::format("{:g}", -std::numeric_limits<double>::quiet_NaN()) == "-nan");
    VERIFY(uxs::format("{:G}", -std::numeric_limits<double>::quiet_NaN()) == "-NAN");
    VERIFY(uxs::format("{:015g}", -std::numeric_limits<double>::quiet_NaN()) == "           -nan");
    VERIFY(uxs::format("{:015G}", -std::numeric_limits<double>::quiet_NaN()) == "           -NAN");

    VERIFY(uxs::format("{:+g}", -std::numeric_limits<double>::quiet_NaN()) == "-nan");
    VERIFY(uxs::format("{:+G}", -std::numeric_limits<double>::quiet_NaN()) == "-NAN");
    VERIFY(uxs::format("{:+015g}", -std::numeric_limits<double>::quiet_NaN()) == "           -nan");
    VERIFY(uxs::format("{:+015G}", -std::numeric_limits<double>::quiet_NaN()) == "           -NAN");

    VERIFY(uxs::format("{: g}", -std::numeric_limits<double>::quiet_NaN()) == "-nan");
    VERIFY(uxs::format("{: G}", -std::numeric_limits<double>::quiet_NaN()) == "-NAN");
    VERIFY(uxs::format("{: 015g}", -std::numeric_limits<double>::quiet_NaN()) == "           -nan");
    VERIFY(uxs::format("{: 015G}", -std::numeric_limits<double>::quiet_NaN()) == "           -NAN");

    return 0;
}

int test_string_cvt_4() {
    VERIFY(uxs::from_string<int>(" \t 10") == 10);
    VERIFY(uxs::from_string<int>("  \t  -25510") == -25510);
    VERIFY(uxs::from_string<int>("  \t  +2510") == 2510);

    VERIFY(uxs::from_string<unsigned>(" \t  10") == 10);
    VERIFY(uxs::from_string<unsigned>("\t    25510") == 25510);

    VERIFY(fabs(uxs::from_string<float>(" \t 0.2355") - 0.2355f) < 0.000001);
    VERIFY(fabs(uxs::from_string<float>(" \t .2355") - 0.2355f) < 0.000001);
    VERIFY(fabs(uxs::from_string<float>(" \t .3") - 0.3f) < 0.000001);
    VERIFY(fabs(uxs::from_string<double>("   \t  -123.56e-1") - -123.56e-1) < 0.000001);
    VERIFY(fabs(uxs::from_string<double>("   \t  -123.56e-0047") - -123.56e-47) < 0.000001);

    VERIFY(uxs::from_string<bool>(" \t   true") == true);
    VERIFY(uxs::from_string<bool>(" \t   false") == false);
    VERIFY(uxs::from_string<bool>(" \t   1") == true);
    VERIFY(uxs::from_string<bool>(" \t   001") == true);
    VERIFY(uxs::from_string<bool>(" \t   000") == false);

    uxs::vector<std::tuple<std::string_view, size_t, double>> d_tst;
    d_tst.emplace_back(" \t   fhjjh", 0, 12345.);
    d_tst.emplace_back(" \t   +fhjjh", 0, 12345.);
    d_tst.emplace_back(" \t   -fhjjh", 0, 12345.);
    d_tst.emplace_back(" \t   .fhjjh", 0, 12345.);
    d_tst.emplace_back(" \t   +.fhjjh", 0, 12345.);
    d_tst.emplace_back(" \t   -.fhjjh", 0, 12345.);
    d_tst.emplace_back(" \t   +", 0, 12345.);
    d_tst.emplace_back(" \t   -", 0, 12345.);
    d_tst.emplace_back(" \t   .", 0, 12345.);
    d_tst.emplace_back(" \t   +.", 0, 12345.);
    d_tst.emplace_back(" \t   -.", 0, 12345.);
    d_tst.emplace_back(" \t   .123fhjjh", 9, 0.123);
    d_tst.emplace_back(" \t   +.123fhjjh", 10, 0.123);
    d_tst.emplace_back(" \t   -.123fhjjh", 10, -0.123);
    d_tst.emplace_back(" \t   123wert", 8, 123.);
    d_tst.emplace_back(" \t   +123wert", 9, 123.);
    d_tst.emplace_back(" \t   -123wert", 9, -123.);
    d_tst.emplace_back(" \t   123.wert", 9, 123.);
    d_tst.emplace_back(" \t   123.0wert", 10, 123.);
    d_tst.emplace_back(" \t   123.1wert", 10, 123.1);
    d_tst.emplace_back(" \t   123.1esd", 10, 123.1);
    d_tst.emplace_back(" \t   123.1e1sd", 12, 1231.);

    for (const auto& el : d_tst) {
        double d = 12345;
        VERIFY(uxs::stoval(std::get<0>(el), d) == std::get<1>(el) && d == std::get<2>(el));
    }

    uxs::vector<std::tuple<std::string_view, size_t, int>> i_tst;
    i_tst.emplace_back(" \t   fhjjh", 0, 12345);
    i_tst.emplace_back(" \t   +fhjjh", 0, 12345);
    i_tst.emplace_back(" \t   -fhjjh", 0, 12345);
    i_tst.emplace_back(" \t   +", 0, 12345);
    i_tst.emplace_back(" \t   -", 0, 12345);
    i_tst.emplace_back(" \t   123wert", 8, 123);
    i_tst.emplace_back(" \t   +123wert", 9, 123);
    i_tst.emplace_back(" \t   -123wert", 9, -123);

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

    return 0;
}

int test_string_cvt_5() {
    std::string_view h{"1234abCDz"};
    unsigned n_parsed = 0;
    VERIFY(uxs::from_hex(h.begin(), 8) == 0x1234abcd);
    VERIFY(uxs::from_hex(h.begin(), 8, uxs::nofunc(), &n_parsed) == 0x1234abcd && n_parsed == 8);
    VERIFY(uxs::from_hex(h.begin(), 9, uxs::nofunc(), &n_parsed) == 0x1234abcd && n_parsed == 8);

    char buf[8];
    uxs::to_hex(0x1234abcd, buf, 8);
    VERIFY(std::string_view{buf, 8} == "1234ABCD");
    return 0;
}

ADD_TEST_CASE("", "string conversion", test_string_cvt_0);
ADD_TEST_CASE("", "string conversion", test_string_cvt_1);
ADD_TEST_CASE("", "string conversion", test_string_cvt_1u);
ADD_TEST_CASE("", "string conversion", test_string_cvt_2);
ADD_TEST_CASE("", "string conversion", test_string_cvt_3);
ADD_TEST_CASE("", "string conversion", test_string_cvt_4);
ADD_TEST_CASE("", "string conversion", test_string_cvt_5);

//-----------------------------------------------------------------------------
// Bruteforce tests

#if defined(NDEBUG)
const int brute_N = 5000000;
#else   // defined(NDEBUG)
const int brute_N = 5000;
#endif  // defined(NDEBUG)

#if defined(_MSC_VER)
#    define UINT64_FMT_STRING "%llu"
#else
#    define UINT64_FMT_STRING "%lu"
#endif

//------------ uint64_t ------------

struct test_context {
    // 0 - success
    // 1 - int->string failure
    // 2 - string->int failure
    int result = 0;
    std::array<char, 32> s_buf, s_buf_ref;
    std::string_view s, s_ref;
    uint64_t val = 0;
    uint64_t val1 = 0;
    uint64_t val2 = 0;
};

void bruteforce_integer(int iter_count) {
    std::default_random_engine generator;
    std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());

    auto test_func = [=](int iter, uint64_t val, test_context& ctx) {
        ctx.result = 0;

        for (unsigned n = 0; n < 1000; ++n) {
            ctx.val = val;
            ctx.s = std::string_view(ctx.s_buf.data(), uxs::format_to(ctx.s_buf.data(), "{}", val) - ctx.s_buf.data());
            ctx.s_buf[ctx.s.size()] = '\0';

#if defined(_MSC_VER) && __cplusplus >= 201703L
            auto result = std::to_chars(ctx.s_buf_ref.data(), ctx.s_buf_ref.data() + ctx.s_buf_ref.size(), val);
            ctx.s_ref = std::string_view(ctx.s_buf_ref.data(), result.ptr - ctx.s_buf_ref.data());
#else
            size_t len = std::sprintf(ctx.s_buf_ref.data(), UINT64_FMT_STRING, val);
            ctx.s_ref = std::string_view(ctx.s_buf_ref.data(), len);
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
#if defined(_MSC_VER) && __cplusplus >= 201703L
            std::from_chars(ctx.s.data(), ctx.s.data() + ctx.s.size(), ctx.val2);
#else
            std::sscanf(ctx.s.data(), UINT64_FMT_STRING, &ctx.val2);
#endif
            if (ctx.val1 != ctx.val2) {
                ctx.result = 1;
                return;
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
            uint64_t val = distribution(generator);

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

ADD_TEST_CASE("1-bruteforce", "uin64_t <-> string", []() {
    bruteforce_integer(brute_N);
    return 0;
});

#if !defined(_MSC_VER) || (_MSC_VER >= 1920 && __cplusplus >= 201703L)

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
void bruteforce_fp_fixed(int iter_count) {
    std::default_random_engine generator;

    const int bits = std::is_same<Ty, double>::value ? 52 : 23;
    const int pow_bias = std::is_same<Ty, double>::value ? 1023 : 127;
    const int default_prec = std::is_same<Ty, double>::value ? 17 : 9;
    const int max_prec = 30;

    std::uniform_int_distribution<uint64_t> distribution(5, (1ull << bits) - 2);

    int N_err = 1;

    auto test_func = [=](int iter, uint64_t mantissa, test_context_fp<Ty>& ctx) {
        ctx.result = 0;
        for (ctx.k = 0; ctx.k <= 140; ++ctx.k) {
            ctx.exp = pow_bias - 70 + ctx.k;
            ctx.uval = mantissa | (static_cast<uint64_t>(ctx.exp) << bits);
            ctx.val = safe_reinterpret<Ty>(
                static_cast<std::conditional_t<std::is_same<Ty, float>::value, uint32_t, uint64_t>>(ctx.uval));
            for (ctx.prec = max_prec; ctx.prec >= 0; --ctx.prec) {
                ctx.s = std::string_view(
                    ctx.s_buf.data(), uxs::format_to(ctx.s_buf.data(), "{:.{}f}", ctx.val, ctx.prec) - ctx.s_buf.data());
                ctx.s_buf[ctx.s.size()] = '\0';

                int n_digs = static_cast<int>(ctx.s.size());
                if (ctx.s[0] == '0') {
                    n_digs = ctx.prec;
                    if (n_digs > 1) {
                        auto p = ctx.s.begin() + 2;
                        while (p != ctx.s.end() && *p == '0') { ++p, --n_digs; }
                    }
                } else if (std::find(ctx.s.begin(), ctx.s.end(), '.') != ctx.s.end()) {
                    --n_digs;
                }

                ctx.s_ref = std::string_view(
                    ctx.s_buf_ref.data(),
                    fmt::format_to(ctx.s_buf_ref.data(), FMT_COMPILE("{:.{}f}"), ctx.val, ctx.prec) -
                        ctx.s_buf_ref.data());
                if (ctx.s != ctx.s_ref) {
                    ctx.result = 1;
                    return;
                }

                ctx.val1 = 0, ctx.val2 = 0;
                if (uxs::stoval(ctx.s, ctx.val1) != ctx.s.size()) {
                    ctx.result = 2;
                    return;
                }
#    if defined(_MSC_VER)
                std::from_chars(ctx.s.data(), ctx.s.data() + ctx.s.size(), ctx.val2);
#    else
                std::sscanf(ctx.s.data(), std::is_same<Ty, double>::value ? "%lf" : "%f", &ctx.val2);
#    endif
                if (ctx.val1 != ctx.val2 || (n_digs >= default_prec && ctx.val1 != ctx.val)) {
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
                mantissa = iter;
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
                uxs::println("mantissa = {};", ctx[proc].uval);
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

//------------ float & double: scientific & general format ------------

template<typename Ty>
void bruteforce_fp_sci(bool general, int iter_count) {
    std::default_random_engine generator;

    const int bits = std::is_same<Ty, double>::value ? 52 : 23;
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
            ctx.uval = mantissa | (static_cast<uint64_t>(ctx.exp) << bits);
            ctx.val = safe_reinterpret<Ty>(
                static_cast<std::conditional_t<std::is_same<Ty, float>::value, uint32_t, uint64_t>>(ctx.uval));
            for (int prec = max_prec; prec > 0; --prec) {
                ctx.prec = prec - (general ? 0 : 1);
                ctx.s = std::string_view(ctx.s_buf.data(),
                                         (general ? uxs::format_to(ctx.s_buf.data(), "{:.{}g}", ctx.val, ctx.prec) :
                                                    uxs::format_to(ctx.s_buf.data(), "{:.{}e}", ctx.val, ctx.prec)) -
                                             ctx.s_buf.data());
                ctx.s_buf[ctx.s.size()] = '\0';

                ctx.s_ref = std::string_view(
                    ctx.s_buf_ref.data(),
                    (general ? fmt::format_to(ctx.s_buf_ref.data(), FMT_COMPILE("{:.{}g}"), ctx.val, ctx.prec) :
                               fmt::format_to(ctx.s_buf_ref.data(), FMT_COMPILE("{:.{}e}"), ctx.val, ctx.prec)) -
                        ctx.s_buf_ref.data());

                if (ctx.s != ctx.s_ref) {
                    ctx.result = 1;
                    return;
                }

                ctx.val1 = 0, ctx.val2 = 0;
                if (uxs::stoval(ctx.s, ctx.val1) != ctx.s.size()) {
                    ctx.result = 2;
                    return;
                }
#    if defined(_MSC_VER)
                std::from_chars(ctx.s.data(), ctx.s.data() + ctx.s.size(), ctx.val2);
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
                mantissa = iter;
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
                uxs::println("mantissa = {};", ctx[proc].uval);
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

//------------ float & double: default (roundtrip) format ------------

template<typename Ty>
void bruteforce_fp_roundtrip(int iter_count) {
    std::default_random_engine generator;

    const int bits = std::is_same<Ty, double>::value ? 52 : 23;
    const int pow_max = std::is_same<Ty, double>::value ? 2047 : 255;
    const int pow_bias = std::is_same<Ty, double>::value ? 1023 : 127;
    const int default_prec = std::is_same<Ty, double>::value ? 17 : 9;

    std::uniform_int_distribution<uint64_t> distribution(5, (1ull << bits) - 2);

    int N_err = 1;

    auto test_func = [=](int iter, uint64_t mantissa, test_context_fp<Ty>& ctx) {
        ctx.result = 0;

        for (ctx.k = 0; ctx.k < pow_max; ++ctx.k) {
            ctx.exp = ctx.k;
            ctx.uval = mantissa | (static_cast<uint64_t>(ctx.exp) << bits);
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
#    if defined(_MSC_VER)
            std::from_chars(ctx.s.data(), ctx.s.data() + ctx.s.size(), ctx.val2);
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
                mantissa = iter;
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
                uxs::println("mantissa = {};", ctx[proc].uval);
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
    const int pow_max = std::is_same<Ty, double>::value ? 2047 : 255;
    const int pow_bias = std::is_same<Ty, double>::value ? 1023 : 127;
    const int default_prec = std::is_same<Ty, double>::value ? 17 : 9;

    std::uniform_int_distribution<uint64_t> distribution(5, (1ull << bits) - 2);
    std::uniform_int_distribution<int> prec_distrib(19, 250);

    int N_err = 1;

    auto test_func = [=](int iter, uint64_t mantissa, int prec, test_context_fp<Ty>& ctx) {
        ctx.result = 0;
        for (ctx.k = 0; ctx.k < pow_max; ++ctx.k) {
            ctx.exp = ctx.k;
            ctx.uval = mantissa | (static_cast<uint64_t>(ctx.exp) << bits);
            ctx.val = safe_reinterpret<Ty>(
                static_cast<std::conditional_t<std::is_same<Ty, float>::value, uint32_t, uint64_t>>(ctx.uval));
            ctx.prec = prec;
            ctx.s = std::string_view(ctx.s_buf.data(),
                                     uxs::format_to(ctx.s_buf.data(), "{:.{}g}", ctx.val, ctx.prec) - ctx.s_buf.data());
            ctx.s_buf[ctx.s.size()] = '\0';

            ctx.s_ref = std::string_view(
                ctx.s_buf_ref.data(),
                fmt::format_to(ctx.s_buf_ref.data(), FMT_COMPILE("{:.{}g}"), ctx.val, ctx.prec) - ctx.s_buf_ref.data());

            if (ctx.s != ctx.s_ref) {
                ctx.result = 1;
                return;
            }

            ctx.val1 = 0, ctx.val2 = 0;
            if (uxs::stoval(ctx.s, ctx.val1) != ctx.s.size()) {
                ctx.result = 2;
                return;
            }
#    if defined(_MSC_VER)
            std::from_chars(ctx.s.data(), ctx.s.data() + ctx.s.size(), ctx.val2);
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
                uxs::println("mantissa = {};", ctx[proc].uval);
                uxs::println("exp = {} + {};", ctx[proc].exp - pow_bias, pow_bias);
                VERIFY(--N_err > 0);
            }
        }
    }
}

ADD_TEST_CASE("1-bruteforce", "float <-> string conversion (general, 19-250 prec)", []() {
    bruteforce_fp_big_prec<float>(10 * brute_N);
    return 0;
});
ADD_TEST_CASE("1-bruteforce", "double <-> string conversion (general, 19-250 prec)", []() {
    bruteforce_fp_big_prec<double>(10 * brute_N);
    return 0;
});

#endif

//-----------------------------------------------------------------------------
// Performance tests

const int perf_N_secs = 5;
const size_t perf_data_set_size = 10000;

//------------ uint64_t -> string ------------

template<typename Func>
int perf_int64_to_string(const Func& fn, int n_secs) {
    std::array<char, 128> buf;
    std::default_random_engine generator;
    std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());

    std::vector<uint64_t> v;
    v.resize(perf_data_set_size);
    for (uint64_t& val : v) {
        val = distribution(generator);
        const size_t len = fn(buf.data(), buf.data() + buf.size(), val);
        VERIFY(uxs::from_string<uint64_t>(std::string_view{buf.data(), len}) == val);
    }

    size_t len = 0;
    const auto start0 = curr_clock_t::now();
    for (uint64_t val : v) { len = std::max<size_t>(len, fn(buf.data(), buf.data() + buf.size(), val)); }
    const auto start = curr_clock_t::now();
    const int loop_count = static_cast<int>(std::ceil((n_secs * 1000000000.0) / as_ns_duration(start - start0)));
    for (int i = 0; i < loop_count; ++i) {
        for (uint64_t val : v) { len = std::max<size_t>(len, fn(buf.data(), buf.data() + buf.size(), val)); }
    }
    return len ? static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) /
                                  (loop_count * perf_data_set_size)) :
                 0;
}

ADD_TEST_CASE("2-perf", "uint64_t -> string", ([]() {
                  return perf_int64_to_string(
                      [](char* first, char* last, uint64_t val) {
                          return static_cast<size_t>(uxs::to_chars(first, val) - first);
                      },
                      perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<libc> uint64_t -> string", ([]() {
                  return perf_int64_to_string(
                      [](char* first, char* last, uint64_t val) {
                          return static_cast<size_t>(std::sprintf(first, UINT64_FMT_STRING, val));
                      },
                      perf_N_secs);
              }));
#if defined(_MSC_VER) && __cplusplus >= 201703L
ADD_TEST_CASE("2-perf", "<c++17/20> uint64_t -> string", ([]() {
                  return perf_int64_to_string(
                      [](char* first, char* last, uint64_t val) {
                          return static_cast<size_t>(std::to_chars(first, last, val).ptr - first);
                      },
                      perf_N_secs);
              }));
#endif
#if !defined(_MSC_VER) || _MSC_VER >= 1920
ADD_TEST_CASE("2-perf", "<{fmt}> uint64_t -> string", ([]() {
                  return perf_int64_to_string(
                      [](char* first, char* last, uint64_t val) {
                          return static_cast<size_t>(fmt::format_to(first, FMT_COMPILE("{}"), val) - first);
                      },
                      perf_N_secs);
              }));
#endif

//------------ string -> uint64_t ------------

template<typename Func>
int perf_string_to_int64(const Func& fn, int n_secs) {
    std::default_random_engine generator;
    std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());

    std::vector<std::string> v;
    v.resize(perf_data_set_size);
    for (auto& s : v) { s = uxs::to_string(distribution(generator)); }

    size_t len = 0;
    const auto start0 = curr_clock_t::now();
    for (const auto& s : v) {
        uint64_t val = 0;
        len = std::max<size_t>(len, fn(s, val));
    }
    const auto start = curr_clock_t::now();
    const int loop_count = static_cast<int>(std::ceil((n_secs * 1000000000.0) / as_ns_duration(start - start0)));
    for (int i = 0; i < loop_count; ++i) {
        for (const auto& s : v) {
            uint64_t val = 0;
            len = std::max<size_t>(len, fn(s, val));
        }
    }
    return len ? static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) /
                                  (loop_count * perf_data_set_size)) :
                 0;
}

ADD_TEST_CASE("2-perf", "string -> uint64_t", ([]() {
                  return perf_string_to_int64([](std::string_view s, uint64_t& val) { return uxs::stoval(s, val); },
                                              perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<libc> string -> uint64_t", ([]() {
                  return perf_string_to_int64(
                      [](std::string_view s, uint64_t& val) { return std::sscanf(s.data(), UINT64_FMT_STRING, &val); },
                      perf_N_secs);
              }));
#if defined(_MSC_VER) && __cplusplus >= 201703L
ADD_TEST_CASE("2-perf", "<c++17/20> string -> uint64_t", ([]() {
                  return perf_string_to_int64(
                      [](std::string_view s, uint64_t& val) {
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

    std::vector<double> v;
    v.resize(perf_data_set_size);
    for (double& val : v) {
        val = safe_reinterpret<double>(mantissa_distr(generator) | (static_cast<uint64_t>(pow_distr(generator)) << 52));
        const size_t len = fn(buf.data(), buf.data() + buf.size(), val, std::forward<Ts>(params)...);
        VERIFY(uxs::from_string<double>(std::string_view{buf.data(), len}) == val);
    }

    size_t len = 0;
    const auto start0 = curr_clock_t::now();
    for (double val : v) {
        len = std::max<size_t>(len, fn(buf.data(), buf.data() + buf.size(), val, std::forward<Ts>(params)...));
    }
    const auto start = curr_clock_t::now();
    const int loop_count = static_cast<int>(std::ceil((n_secs * 1000000000.0) / as_ns_duration(start - start0)));
    for (int i = 0; i < loop_count; ++i) {
        for (double val : v) {
            len = std::max<size_t>(len, fn(buf.data(), buf.data() + buf.size(), val, std::forward<Ts>(params)...));
        }
    }
    return len ? static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) /
                                  (loop_count * perf_data_set_size)) :
                 0;
}

ADD_TEST_CASE("2-perf", "double -> string", ([]() {
                  return perf_double_to_string(
                      [](char* first, char* last, double val) {
                          return static_cast<size_t>(uxs::to_chars(first, val) - first);
                      },
                      perf_N_secs);
              }));
static auto perf_double_to_string_prec = [](char* first, char* last, double val, int prec) {
    return static_cast<size_t>(uxs::format_to(first, "{:.{}}", val, prec) - first);
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

ADD_TEST_CASE("2-perf", "<libc> double -> string", ([]() {
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

#if defined(_MSC_VER) && __cplusplus >= 201703L
ADD_TEST_CASE("2-perf", "<c++17/20> double -> string", ([]() {
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

#if !defined(_MSC_VER) || _MSC_VER >= 1920
ADD_TEST_CASE("2-perf", "<{fmt}> double -> string", ([]() {
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

    std::vector<std::string> v;
    v.resize(perf_data_set_size);
    for (auto& s : v) {
        s = uxs::to_string(
            safe_reinterpret<double>(mantissa_distr(generator) | (static_cast<uint64_t>(pow_distr(generator)) << 52)));
    }

    size_t len = 0;
    const auto start0 = curr_clock_t::now();
    for (const auto& s : v) {
        double val = 0;
        len = std::max<size_t>(len, fn(s, val));
    }
    const auto start = curr_clock_t::now();
    const int loop_count = static_cast<int>(std::ceil((n_secs * 1000000000.0) / as_ns_duration(start - start0)));
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
#if defined(_MSC_VER) && __cplusplus >= 201703L
ADD_TEST_CASE("2-perf", "<c++17/20> string -> double", ([]() {
                  return perf_string_to_double(
                      [](std::string_view s, double& val) {
                          return static_cast<size_t>(std::from_chars(s.data(), s.data() + s.size(), val).ptr - s.data());
                      },
                      perf_N_secs);
              }));
#endif

}  // namespace
