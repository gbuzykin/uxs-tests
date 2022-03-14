#if defined(_MSC_VER)
#    define _CRT_SECURE_NO_WARNINGS
#endif

#include "test_suite.h"
#include "util/format.h"
#include "util/vector.h"

#include <array>
#include <cstdio>
#include <cstring>
#include <random>

#if !defined(_MSC_VER) || _MSC_VER >= 1920
#    include "fmt/format.h"
#    include "milo/dtoa_milo.h"
#endif

#if defined(_MSC_VER) && __cplusplus >= 201703L
#    include <charconv>
#endif

namespace {

int test_string_cvt_0() {
    VERIFY(util::format("{}", 123456) == "123456");
    VERIFY(util::format("{}", true) == "true");

    return 0;
}

int test_string_cvt_1() {
    VERIFY(util::format("{}", 1234) == "1234");
    VERIFY(util::format("{:b}", 1234) == "10011010010");
    VERIFY(util::format("{:B}", 1234) == "10011010010");
    VERIFY(util::format("{:o}", 1234) == "2322");
    VERIFY(util::format("{:x}", 1234) == "4d2");
    VERIFY(util::format("{:X}", 1234) == "4D2");

    VERIFY(util::format("{:#}", 1234) == "1234");
    VERIFY(util::format("{:#b}", 1234) == "10011010010b");
    VERIFY(util::format("{:#B}", 1234) == "10011010010B");
    VERIFY(util::format("{:#o}", 1234) == "02322");
    VERIFY(util::format("{:#x}", 1234) == "0x4d2");
    VERIFY(util::format("{:#X}", 1234) == "0X4D2");

    VERIFY(util::format("{:015}", 1234) == "000000000001234");
    VERIFY(util::format("{:015b}", 1234) == "000010011010010");
    VERIFY(util::format("{:015B}", 1234) == "000010011010010");
    VERIFY(util::format("{:015o}", 1234) == "000000000002322");
    VERIFY(util::format("{:015x}", 1234) == "0000000000004d2");
    VERIFY(util::format("{:015X}", 1234) == "0000000000004D2");

    VERIFY(util::format("{:#015}", 1234) == "000000000001234");
    VERIFY(util::format("{:#015b}", 1234) == "00010011010010b");
    VERIFY(util::format("{:#015B}", 1234) == "00010011010010B");
    VERIFY(util::format("{:#015o}", 1234) == "000000000002322");
    VERIFY(util::format("{:#015x}", 1234) == "0x00000000004d2");
    VERIFY(util::format("{:#015X}", 1234) == "0X00000000004D2");

    VERIFY(util::format("{}", -25510) == "-25510");
    VERIFY(util::format("{:b}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{:B}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{:o}", -25510) == "37777716132");
    VERIFY(util::format("{:x}", -25510) == "ffff9c5a");
    VERIFY(util::format("{:X}", -25510) == "FFFF9C5A");

    VERIFY(util::format("{:#}", -25510) == "-25510");
    VERIFY(util::format("{:#b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(util::format("{:#B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(util::format("{:#o}", -25510) == "037777716132");
    VERIFY(util::format("{:#x}", -25510) == "0xffff9c5a");
    VERIFY(util::format("{:#X}", -25510) == "0XFFFF9C5A");

    VERIFY(util::format("{:015}", -25510) == "-00000000025510");
    VERIFY(util::format("{:015b}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{:015B}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{:015o}", -25510) == "000037777716132");
    VERIFY(util::format("{:015x}", -25510) == "0000000ffff9c5a");
    VERIFY(util::format("{:015X}", -25510) == "0000000FFFF9C5A");

    VERIFY(util::format("{:#015}", -25510) == "-00000000025510");
    VERIFY(util::format("{:#015b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(util::format("{:#015B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(util::format("{:#015o}", -25510) == "000037777716132");
    VERIFY(util::format("{:#015x}", -25510) == "0x00000ffff9c5a");
    VERIFY(util::format("{:#015X}", -25510) == "0X00000FFFF9C5A");

    VERIFY(util::format("{:+}", 1234) == "+1234");
    VERIFY(util::format("{: }", 1234) == " 1234");
    VERIFY(util::format("{:+b}", 1234) == "10011010010");
    VERIFY(util::format("{:+B}", 1234) == "10011010010");
    VERIFY(util::format("{:+o}", 1234) == "2322");
    VERIFY(util::format("{:+x}", 1234) == "4d2");
    VERIFY(util::format("{:+X}", 1234) == "4D2");

    VERIFY(util::format("{:+15}", 1234) == "          +1234");
    VERIFY(util::format("{: 15}", 1234) == "           1234");
    VERIFY(util::format("{:+15b}", 1234) == "    10011010010");
    VERIFY(util::format("{:+15B}", 1234) == "    10011010010");
    VERIFY(util::format("{:+15o}", 1234) == "           2322");
    VERIFY(util::format("{:+15x}", 1234) == "            4d2");
    VERIFY(util::format("{:+15X}", 1234) == "            4D2");

    VERIFY(util::format("{: >+15}", 1234) == "          +1234");
    VERIFY(util::format("{: > 15}", 1234) == "           1234");
    VERIFY(util::format("{: >+15b}", 1234) == "    10011010010");
    VERIFY(util::format("{: >+15B}", 1234) == "    10011010010");
    VERIFY(util::format("{: >+15o}", 1234) == "           2322");
    VERIFY(util::format("{: >+15x}", 1234) == "            4d2");
    VERIFY(util::format("{: >+15X}", 1234) == "            4D2");

    VERIFY(util::format("{: <+15}", 1234) == "+1234          ");
    VERIFY(util::format("{: < 15}", 1234) == " 1234          ");
    VERIFY(util::format("{: <+15b}", 1234) == "10011010010    ");
    VERIFY(util::format("{: <+15B}", 1234) == "10011010010    ");
    VERIFY(util::format("{: <+15o}", 1234) == "2322           ");
    VERIFY(util::format("{: <+15x}", 1234) == "4d2            ");
    VERIFY(util::format("{: <+15X}", 1234) == "4D2            ");

    VERIFY(util::format("{: ^+15}", 1234) == "     +1234     ");
    VERIFY(util::format("{: ^ 15}", 1234) == "      1234     ");
    VERIFY(util::format("{: ^+15b}", 1234) == "  10011010010  ");
    VERIFY(util::format("{: ^+15B}", 1234) == "  10011010010  ");
    VERIFY(util::format("{: ^+15o}", 1234) == "     2322      ");
    VERIFY(util::format("{: ^+15x}", 1234) == "      4d2      ");
    VERIFY(util::format("{: ^+15X}", 1234) == "      4D2      ");

    VERIFY(util::format("{:+#}", 1234) == "+1234");
    VERIFY(util::format("{: #}", 1234) == " 1234");
    VERIFY(util::format("{:+#b}", 1234) == "10011010010b");
    VERIFY(util::format("{:+#B}", 1234) == "10011010010B");
    VERIFY(util::format("{:+#o}", 1234) == "02322");
    VERIFY(util::format("{:+#x}", 1234) == "0x4d2");
    VERIFY(util::format("{:+#X}", 1234) == "0X4D2");

    VERIFY(util::format("{:+#15}", 1234) == "          +1234");
    VERIFY(util::format("{: #15}", 1234) == "           1234");
    VERIFY(util::format("{:+#15b}", 1234) == "   10011010010b");
    VERIFY(util::format("{:+#15B}", 1234) == "   10011010010B");
    VERIFY(util::format("{:+#15o}", 1234) == "          02322");
    VERIFY(util::format("{:+#15x}", 1234) == "          0x4d2");
    VERIFY(util::format("{:+#15X}", 1234) == "          0X4D2");

    VERIFY(util::format("{: >+#15}", 1234) == "          +1234");
    VERIFY(util::format("{: > #15}", 1234) == "           1234");
    VERIFY(util::format("{: >+#15b}", 1234) == "   10011010010b");
    VERIFY(util::format("{: >+#15B}", 1234) == "   10011010010B");
    VERIFY(util::format("{: >+#15o}", 1234) == "          02322");
    VERIFY(util::format("{: >+#15x}", 1234) == "          0x4d2");
    VERIFY(util::format("{: >+#15X}", 1234) == "          0X4D2");

    VERIFY(util::format("{: <+#15}", 1234) == "+1234          ");
    VERIFY(util::format("{: < #15}", 1234) == " 1234          ");
    VERIFY(util::format("{: <+#15b}", 1234) == "10011010010b   ");
    VERIFY(util::format("{: <+#15B}", 1234) == "10011010010B   ");
    VERIFY(util::format("{: <+#15o}", 1234) == "02322          ");
    VERIFY(util::format("{: <+#15x}", 1234) == "0x4d2          ");
    VERIFY(util::format("{: <+#15X}", 1234) == "0X4D2          ");

    VERIFY(util::format("{: ^+#15}", 1234) == "     +1234     ");
    VERIFY(util::format("{: ^ #15}", 1234) == "      1234     ");
    VERIFY(util::format("{: ^+#15b}", 1234) == " 10011010010b  ");
    VERIFY(util::format("{: ^+#15B}", 1234) == " 10011010010B  ");
    VERIFY(util::format("{: ^+#15o}", 1234) == "     02322     ");
    VERIFY(util::format("{: ^+#15x}", 1234) == "     0x4d2     ");
    VERIFY(util::format("{: ^+#15X}", 1234) == "     0X4D2     ");

    VERIFY(util::format("{:+015}", 1234) == "+00000000001234");
    VERIFY(util::format("{: 015}", 1234) == " 00000000001234");
    VERIFY(util::format("{:+015b}", 1234) == "000010011010010");
    VERIFY(util::format("{:+015B}", 1234) == "000010011010010");
    VERIFY(util::format("{:+015o}", 1234) == "000000000002322");
    VERIFY(util::format("{:+015x}", 1234) == "0000000000004d2");
    VERIFY(util::format("{:+015X}", 1234) == "0000000000004D2");

    VERIFY(util::format("{:+#015}", 1234) == "+00000000001234");
    VERIFY(util::format("{: #015}", 1234) == " 00000000001234");
    VERIFY(util::format("{:+#015b}", 1234) == "00010011010010b");
    VERIFY(util::format("{:+#015B}", 1234) == "00010011010010B");
    VERIFY(util::format("{:+#015o}", 1234) == "000000000002322");
    VERIFY(util::format("{:+#015x}", 1234) == "0x00000000004d2");
    VERIFY(util::format("{:+#015X}", 1234) == "0X00000000004D2");

    VERIFY(util::format("{:+}", -25510) == "-25510");
    VERIFY(util::format("{: }", -25510) == "-25510");
    VERIFY(util::format("{:+b}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{:+B}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{:+o}", -25510) == "37777716132");
    VERIFY(util::format("{:+x}", -25510) == "ffff9c5a");
    VERIFY(util::format("{:+X}", -25510) == "FFFF9C5A");

    VERIFY(util::format("{:+15}", -25510) == "         -25510");
    VERIFY(util::format("{: 15}", -25510) == "         -25510");
    VERIFY(util::format("{:+15b}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{:+15B}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{:+15o}", -25510) == "    37777716132");
    VERIFY(util::format("{:+15x}", -25510) == "       ffff9c5a");
    VERIFY(util::format("{:+15X}", -25510) == "       FFFF9C5A");

    VERIFY(util::format("{: >+15}", -25510) == "         -25510");
    VERIFY(util::format("{: > 15}", -25510) == "         -25510");
    VERIFY(util::format("{: >+15b}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{: >+15B}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{: >+15o}", -25510) == "    37777716132");
    VERIFY(util::format("{: >+15x}", -25510) == "       ffff9c5a");
    VERIFY(util::format("{: >+15X}", -25510) == "       FFFF9C5A");

    VERIFY(util::format("{: <+15}", -25510) == "-25510         ");
    VERIFY(util::format("{: < 15}", -25510) == "-25510         ");
    VERIFY(util::format("{: <+15b}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{: <+15B}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{: <+15o}", -25510) == "37777716132    ");
    VERIFY(util::format("{: <+15x}", -25510) == "ffff9c5a       ");
    VERIFY(util::format("{: <+15X}", -25510) == "FFFF9C5A       ");

    VERIFY(util::format("{: ^+15}", -25510) == "    -25510     ");
    VERIFY(util::format("{: ^ 15}", -25510) == "    -25510     ");
    VERIFY(util::format("{: ^+15b}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{: ^+15B}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{: ^+15o}", -25510) == "  37777716132  ");
    VERIFY(util::format("{: ^+15x}", -25510) == "   ffff9c5a    ");
    VERIFY(util::format("{: ^+15X}", -25510) == "   FFFF9C5A    ");

    VERIFY(util::format("{:+#}", -25510) == "-25510");
    VERIFY(util::format("{: #}", -25510) == "-25510");
    VERIFY(util::format("{:+#b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(util::format("{:+#B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(util::format("{:+#o}", -25510) == "037777716132");
    VERIFY(util::format("{:+#x}", -25510) == "0xffff9c5a");
    VERIFY(util::format("{:+#X}", -25510) == "0XFFFF9C5A");

    VERIFY(util::format("{:+#15}", -25510) == "         -25510");
    VERIFY(util::format("{: #15}", -25510) == "         -25510");
    VERIFY(util::format("{:+#15b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(util::format("{:+#15B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(util::format("{:+#15o}", -25510) == "   037777716132");
    VERIFY(util::format("{:+#15x}", -25510) == "     0xffff9c5a");
    VERIFY(util::format("{:+#15X}", -25510) == "     0XFFFF9C5A");

    VERIFY(util::format("{: >+#15}", -25510) == "         -25510");
    VERIFY(util::format("{: > #15}", -25510) == "         -25510");
    VERIFY(util::format("{: >+#15b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(util::format("{: >+#15B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(util::format("{: >+#15o}", -25510) == "   037777716132");
    VERIFY(util::format("{: >+#15x}", -25510) == "     0xffff9c5a");
    VERIFY(util::format("{: >+#15X}", -25510) == "     0XFFFF9C5A");

    VERIFY(util::format("{: <+#15}", -25510) == "-25510         ");
    VERIFY(util::format("{: < #15}", -25510) == "-25510         ");
    VERIFY(util::format("{: <+#15b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(util::format("{: <+#15B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(util::format("{: <+#15o}", -25510) == "037777716132   ");
    VERIFY(util::format("{: <+#15x}", -25510) == "0xffff9c5a     ");
    VERIFY(util::format("{: <+#15X}", -25510) == "0XFFFF9C5A     ");

    VERIFY(util::format("{: ^+#15}", -25510) == "    -25510     ");
    VERIFY(util::format("{: ^ #15}", -25510) == "    -25510     ");
    VERIFY(util::format("{: ^+#15b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(util::format("{: ^+#15B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(util::format("{: ^+#15o}", -25510) == " 037777716132  ");
    VERIFY(util::format("{: ^+#15x}", -25510) == "  0xffff9c5a   ");
    VERIFY(util::format("{: ^+#15X}", -25510) == "  0XFFFF9C5A   ");

    VERIFY(util::format("{:+015}", -25510) == "-00000000025510");
    VERIFY(util::format("{: 015}", -25510) == "-00000000025510");
    VERIFY(util::format("{:+015b}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{:+015B}", -25510) == "11111111111111111001110001011010");
    VERIFY(util::format("{:+015o}", -25510) == "000037777716132");
    VERIFY(util::format("{:+015x}", -25510) == "0000000ffff9c5a");
    VERIFY(util::format("{:+015X}", -25510) == "0000000FFFF9C5A");

    VERIFY(util::format("{:+#015}", -25510) == "-00000000025510");
    VERIFY(util::format("{: #015}", -25510) == "-00000000025510");
    VERIFY(util::format("{:+#015b}", -25510) == "11111111111111111001110001011010b");
    VERIFY(util::format("{:+#015B}", -25510) == "11111111111111111001110001011010B");
    VERIFY(util::format("{:+#015o}", -25510) == "000037777716132");
    VERIFY(util::format("{:+#015x}", -25510) == "0x00000ffff9c5a");
    VERIFY(util::format("{:+#015X}", -25510) == "0X00000FFFF9C5A");

    return 0;
}

int test_string_cvt_1u() {
    VERIFY(util::format("{}", 1234u) == "1234");
    VERIFY(util::format("{:b}", 1234u) == "10011010010");
    VERIFY(util::format("{:B}", 1234u) == "10011010010");
    VERIFY(util::format("{:o}", 1234u) == "2322");
    VERIFY(util::format("{:x}", 1234u) == "4d2");
    VERIFY(util::format("{:X}", 1234u) == "4D2");

    VERIFY(util::format("{:#}", 1234u) == "1234");
    VERIFY(util::format("{:#b}", 1234u) == "10011010010b");
    VERIFY(util::format("{:#B}", 1234u) == "10011010010B");
    VERIFY(util::format("{:#o}", 1234u) == "02322");
    VERIFY(util::format("{:#x}", 1234u) == "0x4d2");
    VERIFY(util::format("{:#X}", 1234u) == "0X4D2");

    VERIFY(util::format("{:015}", 1234u) == "000000000001234");
    VERIFY(util::format("{:015b}", 1234u) == "000010011010010");
    VERIFY(util::format("{:015B}", 1234u) == "000010011010010");
    VERIFY(util::format("{:015o}", 1234u) == "000000000002322");
    VERIFY(util::format("{:015x}", 1234u) == "0000000000004d2");
    VERIFY(util::format("{:015X}", 1234u) == "0000000000004D2");

    VERIFY(util::format("{:#015}", 1234u) == "000000000001234");
    VERIFY(util::format("{:#015b}", 1234u) == "00010011010010b");
    VERIFY(util::format("{:#015B}", 1234u) == "00010011010010B");
    VERIFY(util::format("{:#015o}", 1234u) == "000000000002322");
    VERIFY(util::format("{:#015x}", 1234u) == "0x00000000004d2");
    VERIFY(util::format("{:#015X}", 1234u) == "0X00000000004D2");

    VERIFY(util::format("{}", static_cast<unsigned>(-25510)) == "4294941786");
    VERIFY(util::format("{:b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(util::format("{:B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(util::format("{:o}", static_cast<unsigned>(-25510)) == "37777716132");
    VERIFY(util::format("{:x}", static_cast<unsigned>(-25510)) == "ffff9c5a");
    VERIFY(util::format("{:X}", static_cast<unsigned>(-25510)) == "FFFF9C5A");

    VERIFY(util::format("{:#}", static_cast<unsigned>(-25510)) == "4294941786");
    VERIFY(util::format("{:#b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010b");
    VERIFY(util::format("{:#B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010B");
    VERIFY(util::format("{:#o}", static_cast<unsigned>(-25510)) == "037777716132");
    VERIFY(util::format("{:#x}", static_cast<unsigned>(-25510)) == "0xffff9c5a");
    VERIFY(util::format("{:#X}", static_cast<unsigned>(-25510)) == "0XFFFF9C5A");

    VERIFY(util::format("{:015}", static_cast<unsigned>(-25510)) == "000004294941786");
    VERIFY(util::format("{:015b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(util::format("{:015B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(util::format("{:015o}", static_cast<unsigned>(-25510)) == "000037777716132");
    VERIFY(util::format("{:015x}", static_cast<unsigned>(-25510)) == "0000000ffff9c5a");
    VERIFY(util::format("{:015X}", static_cast<unsigned>(-25510)) == "0000000FFFF9C5A");

    VERIFY(util::format("{:#015}", static_cast<unsigned>(-25510)) == "000004294941786");
    VERIFY(util::format("{:#015b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010b");
    VERIFY(util::format("{:#015B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010B");
    VERIFY(util::format("{:#015o}", static_cast<unsigned>(-25510)) == "000037777716132");
    VERIFY(util::format("{:#015x}", static_cast<unsigned>(-25510)) == "0x00000ffff9c5a");
    VERIFY(util::format("{:#015X}", static_cast<unsigned>(-25510)) == "0X00000FFFF9C5A");

    VERIFY(util::format("{:+}", 1234u) == "1234");
    VERIFY(util::format("{: }", 1234u) == "1234");
    VERIFY(util::format("{:+b}", 1234u) == "10011010010");
    VERIFY(util::format("{:+B}", 1234u) == "10011010010");
    VERIFY(util::format("{:+o}", 1234u) == "2322");
    VERIFY(util::format("{:+x}", 1234u) == "4d2");
    VERIFY(util::format("{:+X}", 1234u) == "4D2");

    VERIFY(util::format("{:+#}", 1234u) == "1234");
    VERIFY(util::format("{: #}", 1234u) == "1234");
    VERIFY(util::format("{:+#b}", 1234u) == "10011010010b");
    VERIFY(util::format("{:+#B}", 1234u) == "10011010010B");
    VERIFY(util::format("{:+#o}", 1234u) == "02322");
    VERIFY(util::format("{:+#x}", 1234u) == "0x4d2");
    VERIFY(util::format("{:+#X}", 1234u) == "0X4D2");

    VERIFY(util::format("{:+015}", 1234u) == "000000000001234");
    VERIFY(util::format("{: 015}", 1234u) == "000000000001234");
    VERIFY(util::format("{:+015b}", 1234u) == "000010011010010");
    VERIFY(util::format("{:+015B}", 1234u) == "000010011010010");
    VERIFY(util::format("{:+015o}", 1234u) == "000000000002322");
    VERIFY(util::format("{:+015x}", 1234u) == "0000000000004d2");
    VERIFY(util::format("{:+015X}", 1234u) == "0000000000004D2");

    VERIFY(util::format("{:+#015}", 1234u) == "000000000001234");
    VERIFY(util::format("{: #015}", 1234u) == "000000000001234");
    VERIFY(util::format("{:+#015b}", 1234u) == "00010011010010b");
    VERIFY(util::format("{:+#015B}", 1234u) == "00010011010010B");
    VERIFY(util::format("{:+#015o}", 1234u) == "000000000002322");
    VERIFY(util::format("{:+#015x}", 1234u) == "0x00000000004d2");
    VERIFY(util::format("{:+#015X}", 1234u) == "0X00000000004D2");

    VERIFY(util::format("{:+}", static_cast<unsigned>(-25510)) == "4294941786");
    VERIFY(util::format("{: }", static_cast<unsigned>(-25510)) == "4294941786");
    VERIFY(util::format("{:+b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(util::format("{:+B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(util::format("{:+o}", static_cast<unsigned>(-25510)) == "37777716132");
    VERIFY(util::format("{:+x}", static_cast<unsigned>(-25510)) == "ffff9c5a");
    VERIFY(util::format("{:+X}", static_cast<unsigned>(-25510)) == "FFFF9C5A");

    VERIFY(util::format("{:+#}", static_cast<unsigned>(-25510)) == "4294941786");
    VERIFY(util::format("{: #}", static_cast<unsigned>(-25510)) == "4294941786");
    VERIFY(util::format("{:+#b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010b");
    VERIFY(util::format("{:+#B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010B");
    VERIFY(util::format("{:+#o}", static_cast<unsigned>(-25510)) == "037777716132");
    VERIFY(util::format("{:+#x}", static_cast<unsigned>(-25510)) == "0xffff9c5a");
    VERIFY(util::format("{:+#X}", static_cast<unsigned>(-25510)) == "0XFFFF9C5A");

    VERIFY(util::format("{:+015}", static_cast<unsigned>(-25510)) == "000004294941786");
    VERIFY(util::format("{: 015}", static_cast<unsigned>(-25510)) == "000004294941786");
    VERIFY(util::format("{:+015b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(util::format("{:+015B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010");
    VERIFY(util::format("{:+015o}", static_cast<unsigned>(-25510)) == "000037777716132");
    VERIFY(util::format("{:+015x}", static_cast<unsigned>(-25510)) == "0000000ffff9c5a");
    VERIFY(util::format("{:+015X}", static_cast<unsigned>(-25510)) == "0000000FFFF9C5A");

    VERIFY(util::format("{:+#015}", static_cast<unsigned>(-25510)) == "000004294941786");
    VERIFY(util::format("{: #015}", static_cast<unsigned>(-25510)) == "000004294941786");
    VERIFY(util::format("{:+#015b}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010b");
    VERIFY(util::format("{:+#015B}", static_cast<unsigned>(-25510)) == "11111111111111111001110001011010B");
    VERIFY(util::format("{:+#015o}", static_cast<unsigned>(-25510)) == "000037777716132");
    VERIFY(util::format("{:+#015x}", static_cast<unsigned>(-25510)) == "0x00000ffff9c5a");
    VERIFY(util::format("{:+#015X}", static_cast<unsigned>(-25510)) == "0X00000FFFF9C5A");

    return 0;
}

int test_string_cvt_2() {
    VERIFY(util::format("{:f}", 1.2345672222) == "1.234567");
    VERIFY(util::format("{:f}", 1.2345677777) == "1.234568");
    VERIFY(util::format("{:f}", 1.2345) == "1.234500");
    VERIFY(util::format("{:f}", 0.12345) == "0.123450");
    VERIFY(util::format("{:f}", 0.00124) == "0.001240");
    VERIFY(util::format("{:f}", 23.34563444) == "23.345634");
    VERIFY(util::format("{:f}", 23.34563464) == "23.345635");
    VERIFY(util::format("{:f}", 23.3456) == "23.345600");
    VERIFY(util::format("{:f}", 999.9999997) == "1000.000000");
    VERIFY(util::format("{:f}", 0.) == "0.000000");
    VERIFY(util::format("{:.0f}", 0.) == "0");

    VERIFY(util::format("{:f}", 1000000000.) == "1000000000.000000");
    VERIFY(util::format("{:f}", 231000000000000.) == "231000000000000.000000");

    VERIFY(util::format("{:f}", 345.) == "345.000000");
    VERIFY(util::format("{:f}", 345.234465876) == "345.234466");
    VERIFY(util::format("{:f}", 34.5234465876) == "34.523447");
    VERIFY(util::format("{:f}", 3.45234465876) == "3.452345");
    VERIFY(util::format("{:f}", 0.345234465876) == "0.345234");
    VERIFY(util::format("{:f}", 0.0234465876) == "0.023447");
    VERIFY(util::format("{:f}", 0.00234465876) == "0.002345");
    VERIFY(util::format("{:f}", 0.000234465876) == "0.000234");
    VERIFY(util::format("{:f}", 0.0000234465876) == "0.000023");
    VERIFY(util::format("{:f}", 0.00000234465876) == "0.000002");
    VERIFY(util::format("{:f}", 0.000000234465876) == "0.000000");

    VERIFY(util::format("{:.0f}", 345.) == "345");
    VERIFY(util::format("{:.0f}", 345.234465876) == "345");
    VERIFY(util::format("{:.0f}", 34.5234465876) == "35");
    VERIFY(util::format("{:.0f}", 3.45234465876) == "3");
    VERIFY(util::format("{:.0f}", 0.345234465876) == "0");
    VERIFY(util::format("{:.0f}", 0.0234465876) == "0");

    VERIFY(util::format("{:e}", 0.) == "0.000000e+00");
    VERIFY(util::format("{:.0e}", 0.) == "0e+00");
    VERIFY(util::format("{:g}", 0.) == "0");

    VERIFY(util::format("{:g}", 999999.9999996) == "1e+06");
    VERIFY(util::format("{:.3g}", 3455.) == "3.46e+03");
    VERIFY(util::format("{:.3g}", 3445.) == "3.44e+03");

    VERIFY(util::format("{:g}", 0.003) == "0.003");
    VERIFY(util::format("{:g}", 0.003000001) == "0.003");
    VERIFY(util::format("{:g}", 0.00300001) == "0.00300001");
    VERIFY(util::format("{:g}", 0.0030001) == "0.0030001");
    VERIFY(util::format("{:g}", 0.003001) == "0.003001");

    VERIFY(util::format("{:g}", 0.3) == "0.3");
    VERIFY(util::format("{:g}", 0.3000001) == "0.3");
    VERIFY(util::format("{:g}", 0.300001) == "0.300001");
    VERIFY(util::format("{:g}", 0.30001) == "0.30001");
    VERIFY(util::format("{:g}", 0.3001) == "0.3001");

    VERIFY(util::format("{:g}", 345.) == "345");
    VERIFY(util::format("{:g}", 345.0001) == "345");
    VERIFY(util::format("{:g}", 345.001) == "345.001");
    VERIFY(util::format("{:g}", 345.01) == "345.01");
    VERIFY(util::format("{:g}", 345.1) == "345.1");

    VERIFY(util::format("{:g}", 3.e23) == "3e+23");
    VERIFY(util::format("{:g}", 3.000001e23) == "3e+23");
    VERIFY(util::format("{:g}", 3.00001e23) == "3.00001e+23");
    VERIFY(util::format("{:g}", 3.0001e23) == "3.0001e+23");
    VERIFY(util::format("{:g}", 3.001e23) == "3.001e+23");

    VERIFY(util::format("{:.0g}", 345.) == "3e+02");
    VERIFY(util::format("{:.0g}", 345.234465876) == "3e+02");
    VERIFY(util::format("{:.0g}", 34.5234465876) == "3e+01");
    VERIFY(util::format("{:.0g}", 3.45234465876) == "3");
    VERIFY(util::format("{:.0g}", 0.345234465876) == "0.3");
    VERIFY(util::format("{:.0g}", 0.0234465876) == "0.02");

    VERIFY(util::format("{:f}", 0.0000006) == "0.000001");
    VERIFY(util::format("{:f}", 0.000000500001) == "0.000001");
    VERIFY(util::format("{:f}", 0.0000004) == "0.000000");
    VERIFY(util::format("{:f}", 0.00000049999) == "0.000000");
    VERIFY(util::format("{:f}", 0.00000006) == "0.000000");

    double d = 2000000. / 7.;  // 285714.28571428573923185468

    VERIFY(util::format("{:.17e}", d) == "2.85714285714285739e+05");
    VERIFY(util::format("{:.16e}", d) == "2.8571428571428574e+05");
    VERIFY(util::format("{:.15e}", d) == "2.857142857142857e+05");
    VERIFY(util::format("{:.13e}", d) == "2.8571428571429e+05");
    VERIFY(util::format("{:.2e}", d) == "2.86e+05");
    VERIFY(util::format("{:.1e}", d) == "2.9e+05");
    VERIFY(util::format("{:.0e}", d) == "3e+05");
    VERIFY(util::format("{:.18e}", d) == "2.857142857142857392e+05");
    VERIFY(util::format("{:.19e}", d) == "2.8571428571428573920e+05");
    VERIFY(util::format("{:.20e}", d) == "2.85714285714285739200e+05");

    VERIFY(util::format("{:.12f}", d) == "285714.285714285739");
    VERIFY(util::format("{:.11f}", d) == "285714.28571428574");
    VERIFY(util::format("{:.10f}", d) == "285714.2857142857");
    VERIFY(util::format("{:.8f}", d) == "285714.28571429");
    VERIFY(util::format("{:.2f}", d) == "285714.29");
    VERIFY(util::format("{:.1f}", d) == "285714.3");
    VERIFY(util::format("{:.0f}", d) == "285714");
    VERIFY(util::format("{:.13f}", d) == "285714.2857142857392");
    VERIFY(util::format("{:.15f}", d) == "285714.285714285739200");

    d = 285714.;

    VERIFY(util::format("{:.8f}", d) == "285714.00000000");
    VERIFY(util::format("{:.2f}", d) == "285714.00");
    VERIFY(util::format("{:.1f}", d) == "285714.0");
    VERIFY(util::format("{:.0f}", d) == "285714");

    d = 285714e+20;  // 28571400000000001222639616.00000000

    VERIFY(util::format("{:.8f}", d) == "28571400000000001220000000.00000000");
    VERIFY(util::format("{:.2f}", d) == "28571400000000001220000000.00");
    VERIFY(util::format("{:.1f}", d) == "28571400000000001220000000.0");
    VERIFY(util::format("{:.0f}", d) == "28571400000000001220000000");

    VERIFY(util::format("{:.21g}", 123412341234123400000.) == "123412341234123407400");
    VERIFY(util::format("{:.21g}", 12341234123412340000.) == "12341234123412340740");

    VERIFY(util::format("{:.7f}", 0.0006382) == "0.0006382");
    VERIFY(util::format("{:.6f}", 0.0006382) == "0.000638");
    VERIFY(util::format("{:.5f}", 0.0006382) == "0.00064");
    VERIFY(util::format("{:.4f}", 0.0006382) == "0.0006");
    VERIFY(util::format("{:.3f}", 0.0006382) == "0.001");
    VERIFY(util::format("{:.2f}", 0.0006382) == "0.00");
    VERIFY(util::format("{:.1f}", 0.0006382) == "0.0");
    VERIFY(util::format("{:.0f}", 0.0006382) == "0");

    VERIFY(util::format("{:.4f}", 0.6382) == "0.6382");
    VERIFY(util::format("{:.3f}", 0.6382) == "0.638");
    VERIFY(util::format("{:.2f}", 0.6382) == "0.64");
    VERIFY(util::format("{:.1f}", 0.6382) == "0.6");
    VERIFY(util::format("{:.0f}", 0.6382) == "1");

    VERIFY(util::format("{:.3f}", 0.0004382) == "0.000");
    VERIFY(util::format("{:.0f}", 0.4382) == "0");

    VERIFY(util::from_string<double>("9.999999999999999999") == 9.999999999999999999);

    std::array<double, 18> v = {
        util::from_string<double>("2.22507385850720139e-308"), util::from_string<double>("2.22507385850720138e-308"),
        util::from_string<double>("1.11253692925360070e-308"), util::from_string<double>("1.11253692925360069e-308"),
        util::from_string<double>("5.56268464626800346e-309"), util::from_string<double>("5.56268464626800345e-309"),
        util::from_string<double>("5.30498947741318079e-315"), util::from_string<double>("5.30498947741318078e-315"),
        util::from_string<double>("5.05923221341436462e-321"), util::from_string<double>("5.05923221341436461e-321"),
        util::from_string<double>("1.97626258336498618e-323"), util::from_string<double>("1.97626258336498617e-323"),
        util::from_string<double>("9.88131291682493089e-324"), util::from_string<double>("9.88131291682493088e-324"),
        util::from_string<double>("4.94065645841246545e-324"), util::from_string<double>("4.94065645841246544e-324"),
        util::from_string<double>("2.47032822920623273e-324"), util::from_string<double>("2.47032822920623272e-324"),
    };

    VERIFY(util::format("{:g}", v[0]) == "2.22507e-308");
    VERIFY(util::format("{:g}", v[1]) == "2.22507e-308");
    VERIFY(util::format("{:g}", v[2]) == "1.11254e-308");
    VERIFY(util::format("{:g}", v[3]) == "1.11254e-308");
    VERIFY(util::format("{:g}", v[4]) == "5.56268e-309");
    VERIFY(util::format("{:g}", v[5]) == "5.56268e-309");
    VERIFY(util::format("{:g}", v[6]) == "5.30499e-315");
    VERIFY(util::format("{:g}", v[7]) == "5.30499e-315");
    VERIFY(util::format("{:g}", v[8]) == "5.05923e-321");
    VERIFY(util::format("{:g}", v[9]) == "5.05923e-321");
    VERIFY(util::format("{:g}", v[10]) == "1.97626e-323");
    VERIFY(util::format("{:g}", v[11]) == "1.97626e-323");
    VERIFY(util::format("{:g}", v[12]) == "9.88131e-324");
    VERIFY(util::format("{:g}", v[13]) == "9.88131e-324");
    VERIFY(util::format("{:g}", v[14]) == "4.94066e-324");
    VERIFY(util::format("{:g}", v[15]) == "4.94066e-324");
    VERIFY(util::format("{:g}", v[16]) == "4.94066e-324");
    VERIFY(util::format("{:g}", v[17]) == "0");

    for (double d : v) { VERIFY(util::from_string<double>(util::format("{}", d)) == d); }

    VERIFY(util::format("{:#g}", 3.e23) == "3.e+23");

    VERIFY(util::format("{:G}", 3.e23) == "3E+23");
    VERIFY(util::format("{:015g}", 3.e23) == "00000000003e+23");
    VERIFY(util::format("{:015G}", 3.e23) == "00000000003E+23");

    VERIFY(util::format("{:+g}", 3.e23) == "+3e+23");
    VERIFY(util::format("{:+G}", 3.e23) == "+3E+23");
    VERIFY(util::format("{:+015g}", 3.e23) == "+0000000003e+23");
    VERIFY(util::format("{:+015G}", 3.e23) == "+0000000003E+23");

    VERIFY(util::format("{: g}", 3.e23) == " 3e+23");
    VERIFY(util::format("{: G}", 3.e23) == " 3E+23");
    VERIFY(util::format("{: 015g}", 3.e23) == " 0000000003e+23");
    VERIFY(util::format("{: 015G}", 3.e23) == " 0000000003E+23");

    VERIFY(fabs(util::from_string<float>(util::format("{:g}", 0.2355f)) - 0.2355f) < 0.000001);
    VERIFY(fabs(util::from_string<double>(util::format("{:g}", -123.56e-1)) - -123.56e-1) < 0.000001);

    return 0;
}

int test_string_cvt_3() {
    VERIFY(util::format("{: >+15.3f}", 1230.) == "      +1230.000");
    VERIFY(util::format("{: >+15.3e}", 1230.) == "     +1.230e+03");

    VERIFY(util::format("{:f}", 0.) == "0.000000");
    VERIFY(util::format("{:.0f}", 0.) == "0");
    VERIFY(util::format("{:e}", 0.) == "0.000000e+00");
    VERIFY(util::format("{:6e}", 0.) == "0.000000e+00");
    VERIFY(util::format("{:g}", 0.) == "0");
    VERIFY(util::format("{:15g}", 0.) == "              0");
    VERIFY(util::format("{: >15g}", 0.) == "              0");
    VERIFY(util::format("{: <15g}", 0.) == "0              ");
    VERIFY(util::format("{: ^15g}", 0.) == "       0       ");

    VERIFY(util::format("{:G}", 0.) == "0");
    VERIFY(util::format("{:015g}", 0.) == "000000000000000");
    VERIFY(util::format("{:015G}", 0.) == "000000000000000");

    VERIFY(util::format("{:+g}", 0.) == "+0");
    VERIFY(util::format("{:+15g}", 0.) == "             +0");
    VERIFY(util::format("{: >+15g}", 0.) == "             +0");
    VERIFY(util::format("{: <+15g}", 0.) == "+0             ");
    VERIFY(util::format("{: ^+15g}", 0.) == "      +0       ");
    VERIFY(util::format("{:+G}", 0.) == "+0");
    VERIFY(util::format("{:+015g}", 0.) == "+00000000000000");
    VERIFY(util::format("{:+015G}", 0.) == "+00000000000000");

    VERIFY(util::format("{: g}", 0.) == " 0");
    VERIFY(util::format("{: G}", 0.) == " 0");
    VERIFY(util::format("{: 015g}", 0.) == " 00000000000000");
    VERIFY(util::format("{: 015G}", 0.) == " 00000000000000");

    VERIFY(util::format("{:g}", -0.) == "-0");
    VERIFY(util::format("{:G}", -0.) == "-0");
    VERIFY(util::format("{:015g}", -0.) == "-00000000000000");
    VERIFY(util::format("{:015G}", -0.) == "-00000000000000");

    VERIFY(util::format("{:+g}", -0.) == "-0");
    VERIFY(util::format("{:+G}", -0.) == "-0");
    VERIFY(util::format("{:+015g}", -0.) == "-00000000000000");
    VERIFY(util::format("{:+015G}", -0.) == "-00000000000000");

    VERIFY(util::format("{: g}", -0.) == "-0");
    VERIFY(util::format("{: G}", -0.) == "-0");
    VERIFY(util::format("{: 015g}", -0.) == "-00000000000000");
    VERIFY(util::format("{: 015G}", -0.) == "-00000000000000");

    VERIFY(util::format("{:g}", std::numeric_limits<double>::infinity()) == "inf");
    VERIFY(util::format("{:G}", std::numeric_limits<double>::infinity()) == "INF");
    VERIFY(util::format("{:015g}", std::numeric_limits<double>::infinity()) == "            inf");
    VERIFY(util::format("{:015G}", std::numeric_limits<double>::infinity()) == "            INF");
    VERIFY(util::format("{: >015G}", std::numeric_limits<double>::infinity()) == "            INF");
    VERIFY(util::format("{: <015G}", std::numeric_limits<double>::infinity()) == "INF            ");
    VERIFY(util::format("{: ^015G}", std::numeric_limits<double>::infinity()) == "      INF      ");

    VERIFY(util::format("{:+g}", std::numeric_limits<double>::infinity()) == "+inf");
    VERIFY(util::format("{:+G}", std::numeric_limits<double>::infinity()) == "+INF");
    VERIFY(util::format("{:+015g}", std::numeric_limits<double>::infinity()) == "           +inf");
    VERIFY(util::format("{:+015G}", std::numeric_limits<double>::infinity()) == "           +INF");
    VERIFY(util::format("{: >+015G}", std::numeric_limits<double>::infinity()) == "           +INF");
    VERIFY(util::format("{: <+015G}", std::numeric_limits<double>::infinity()) == "+INF           ");
    VERIFY(util::format("{: ^+015G}", std::numeric_limits<double>::infinity()) == "     +INF      ");

    VERIFY(util::format("{: g}", std::numeric_limits<double>::infinity()) == " inf");
    VERIFY(util::format("{: G}", std::numeric_limits<double>::infinity()) == " INF");
    VERIFY(util::format("{: 015g}", std::numeric_limits<double>::infinity()) == "            inf");
    VERIFY(util::format("{: 015G}", std::numeric_limits<double>::infinity()) == "            INF");
    VERIFY(util::format("{: > 015G}", std::numeric_limits<double>::infinity()) == "            INF");
    VERIFY(util::format("{: < 015G}", std::numeric_limits<double>::infinity()) == " INF           ");
    VERIFY(util::format("{: ^ 015G}", std::numeric_limits<double>::infinity()) == "      INF      ");

    VERIFY(util::format("{:g}", -std::numeric_limits<double>::infinity()) == "-inf");
    VERIFY(util::format("{:G}", -std::numeric_limits<double>::infinity()) == "-INF");
    VERIFY(util::format("{:015g}", -std::numeric_limits<double>::infinity()) == "           -inf");
    VERIFY(util::format("{:015G}", -std::numeric_limits<double>::infinity()) == "           -INF");

    VERIFY(util::format("{:+g}", -std::numeric_limits<double>::infinity()) == "-inf");
    VERIFY(util::format("{:+G}", -std::numeric_limits<double>::infinity()) == "-INF");
    VERIFY(util::format("{:+015g}", -std::numeric_limits<double>::infinity()) == "           -inf");
    VERIFY(util::format("{:+015G}", -std::numeric_limits<double>::infinity()) == "           -INF");

    VERIFY(util::format("{: g}", -std::numeric_limits<double>::infinity()) == "-inf");
    VERIFY(util::format("{: G}", -std::numeric_limits<double>::infinity()) == "-INF");
    VERIFY(util::format("{: 015g}", -std::numeric_limits<double>::infinity()) == "           -inf");
    VERIFY(util::format("{: 015G}", -std::numeric_limits<double>::infinity()) == "           -INF");

    VERIFY(util::format("{:g}", std::numeric_limits<double>::quiet_NaN()) == "nan");
    VERIFY(util::format("{:G}", std::numeric_limits<double>::quiet_NaN()) == "NAN");
    VERIFY(util::format("{:015g}", std::numeric_limits<double>::quiet_NaN()) == "            nan");
    VERIFY(util::format("{:015G}", std::numeric_limits<double>::quiet_NaN()) == "            NAN");

    VERIFY(util::format("{:+g}", std::numeric_limits<double>::quiet_NaN()) == "+nan");
    VERIFY(util::format("{:+G}", std::numeric_limits<double>::quiet_NaN()) == "+NAN");
    VERIFY(util::format("{:+015g}", std::numeric_limits<double>::quiet_NaN()) == "           +nan");
    VERIFY(util::format("{:+015G}", std::numeric_limits<double>::quiet_NaN()) == "           +NAN");

    VERIFY(util::format("{: g}", std::numeric_limits<double>::quiet_NaN()) == " nan");
    VERIFY(util::format("{: G}", std::numeric_limits<double>::quiet_NaN()) == " NAN");
    VERIFY(util::format("{: 015g}", std::numeric_limits<double>::quiet_NaN()) == "            nan");
    VERIFY(util::format("{: 015G}", std::numeric_limits<double>::quiet_NaN()) == "            NAN");

    VERIFY(util::format("{:g}", -std::numeric_limits<double>::quiet_NaN()) == "-nan");
    VERIFY(util::format("{:G}", -std::numeric_limits<double>::quiet_NaN()) == "-NAN");
    VERIFY(util::format("{:015g}", -std::numeric_limits<double>::quiet_NaN()) == "           -nan");
    VERIFY(util::format("{:015G}", -std::numeric_limits<double>::quiet_NaN()) == "           -NAN");

    VERIFY(util::format("{:+g}", -std::numeric_limits<double>::quiet_NaN()) == "-nan");
    VERIFY(util::format("{:+G}", -std::numeric_limits<double>::quiet_NaN()) == "-NAN");
    VERIFY(util::format("{:+015g}", -std::numeric_limits<double>::quiet_NaN()) == "           -nan");
    VERIFY(util::format("{:+015G}", -std::numeric_limits<double>::quiet_NaN()) == "           -NAN");

    VERIFY(util::format("{: g}", -std::numeric_limits<double>::quiet_NaN()) == "-nan");
    VERIFY(util::format("{: G}", -std::numeric_limits<double>::quiet_NaN()) == "-NAN");
    VERIFY(util::format("{: 015g}", -std::numeric_limits<double>::quiet_NaN()) == "           -nan");
    VERIFY(util::format("{: 015G}", -std::numeric_limits<double>::quiet_NaN()) == "           -NAN");

    return 0;
}

int test_string_cvt_4() {
    VERIFY(util::from_string<int>(" \t 10") == 10);
    VERIFY(util::from_string<int>("  \t  -25510") == -25510);
    VERIFY(util::from_string<int>("  \t  +2510") == 2510);

    VERIFY(util::from_string<unsigned>(" \t  10") == 10);
    VERIFY(util::from_string<unsigned>("\t    25510") == 25510);

    VERIFY(fabs(util::from_string<float>(" \t 0.2355") - 0.2355f) < 0.000001);
    VERIFY(fabs(util::from_string<float>(" \t .2355") - 0.2355f) < 0.000001);
    VERIFY(fabs(util::from_string<float>(" \t .3") - 0.3f) < 0.000001);
    VERIFY(fabs(util::from_string<double>("   \t  -123.56e-1") - -123.56e-1) < 0.000001);
    VERIFY(fabs(util::from_string<double>("   \t  -123.56e-0047") - -123.56e-47) < 0.000001);

    VERIFY(util::from_string<bool>(" \t   true") == true);
    VERIFY(util::from_string<bool>(" \t   false") == false);
    VERIFY(util::from_string<bool>(" \t   1") == true);
    VERIFY(util::from_string<bool>(" \t   001") == true);
    VERIFY(util::from_string<bool>(" \t   000") == false);

    util::vector<std::tuple<std::string_view, size_t, double>> d_tst;
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
        std::string_view s = std::get<0>(el);
        VERIFY(util::string_converter<double>::from_string(s.data(), s.data() + s.size(), d) ==
                   s.data() + std::get<1>(el) &&
               d == std::get<2>(el));
    }

    util::vector<std::tuple<std::string_view, size_t, int>> i_tst;
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
        std::string_view s = std::get<0>(el);
        VERIFY(util::string_converter<double>::from_string(s.data(), s.data() + s.size(), i) ==
                   s.data() + std::get<1>(el) &&
               i == std::get<2>(el));
    }

    VERIFY(std::isinf(util::from_string<double>("inf")));
    VERIFY(std::isinf(util::from_string<double>("+inf")));
    VERIFY(std::isinf(-util::from_string<double>("-inf")));
    VERIFY(std::isnan(util::from_string<double>("nan")));
    VERIFY(std::isnan(util::from_string<double>("+nan")));
    VERIFY(std::isnan(util::from_string<double>("-nan")));

    return 0;
}

// --------------------------------------------

void string_test_0(int iter_count) {
    std::array<char, 128> s;
    std::array<char, 128> s_ref;
    std::default_random_engine generator;
    std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());

    for (int iter = 0, perc0 = -1; iter < iter_count; ++iter) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            std::cout << std::setw(3) << (perc / 10) << "." << std::setw(0) << (perc % 10) << "%\b\b\b\b\b\b"
                      << std::flush;
            perc0 = perc;
        }

        uint64_t val = distribution(generator);

        char* last = util::format_to(s.data(), "{}", val);
        *last = '\0';

#if defined(_MSC_VER) && __cplusplus >= 201703L
        *std::to_chars(s_ref.data(), s_ref.data() + s_ref.size(), val).ptr = '\0';
#else
        std::sprintf(s_ref.data(), "%.lu", val);
#endif

        VERIFY(std::strcmp(s.data(), s_ref.data()) == 0);

        uint64_t val1 = 0, val2 = 0;
        VERIFY(util::string_converter<uint64_t>::from_string(s.data(), last, val1) == last);
#if defined(_MSC_VER) && __cplusplus >= 201703L
        std::from_chars(s.data(), last, val2);
#else
        std::sscanf(s.data(), "%lu", &val2);
#endif
        VERIFY(val1 == val2);
    }
}

#if !defined(_MSC_VER) || _MSC_VER >= 1920
template<typename Ty>
void string_test_1(int iter_count) {
    std::array<char, 128> s;
    std::array<char, 128> s_ref;
    std::default_random_engine generator;

    const int bits = std::is_same<Ty, double>::value ? 52 : 23;
    const int pow_max = std::is_same<Ty, double>::value ? 2047 : 255;
    const int pow_bias = std::is_same<Ty, double>::value ? 1023 : 127;
    const int default_prec = std::is_same<Ty, double>::value ? 17 : 9;
    const int max_prec = 19;

    std::uniform_int_distribution<uint64_t> distribution(5, (1ull << bits) - 2);

    int N_err = 1;

    for (int iter = 0, perc0 = -1; iter < iter_count; ++iter) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            std::cout << std::setw(3) << (perc / 10) << "." << std::setw(0) << (perc % 10) << "%\b\b\b\b\b\b"
                      << std::flush;
            perc0 = perc;
        }

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

        for (int k = 0; k <= 140; ++k) {
            int exp = pow_bias - 70 + k;
            uint64_t uval = mantissa | (static_cast<uint64_t>(exp) << bits);
            auto val = *reinterpret_cast<Ty*>(&uval);

            for (int prec = max_prec; prec >= 0; --prec) {
                char* last = util::format_to(s.data(), "{:.{}f}", val, prec);
                *last = '\0';
                int n_digs = static_cast<int>(last - s.data());
                if (s[0] == '0') {
                    n_digs = prec;
                    if (n_digs > 1) {
                        const char* p = s.data() + 2;
                        while (p != last && *p == '0') { ++p, --n_digs; }
                    }
                } else if (std::find(s.data(), last, '.') != last) {
                    --n_digs;
                }

                *fmt::format_to(s_ref.data(), "{:.{}f}", val, prec) = '\0';

                if (n_digs <= max_prec) {
                    if (std::strcmp(s.data(), s_ref.data()) != 0) {
                        std::cout << std::endl << "k = " << k << " iter = " << iter << " prec = " << prec << std::endl;
                        std::cout << "result = " << s.data() << std::endl;
                        std::cout << "   ref = " << s_ref.data() << std::endl;
                        std::cout << "mantissa = " << uval << std::endl;
                        std::cout << "exp = " << (exp - pow_bias) << " (+ " << pow_bias << ")" << std::endl;
                        VERIFY(--N_err > 0);
                    }
                }

                Ty val1 = 0, val2 = 0;
                VERIFY(util::string_converter<Ty>::from_string(s.data(), last, val1) == last);
#    if defined(_MSC_VER) && __cplusplus >= 201703L
                std::from_chars(s.data(), last, val2);
#    else
                std::sscanf(s.data(), std::is_same<Ty, double>::value ? "%lf" : "%f", &val2);
#    endif
                if (val1 != val2 || (n_digs >= default_prec && val1 != val)) {
                    std::cout << std::endl << "k = " << k << " iter = " << iter << " prec = " << prec << std::endl;
                    std::cout << "result = " << s.data() << std::endl;
                    std::cout << "   ref = " << s_ref.data() << std::endl;
                    std::cout << "       src = " << fmt::format("{:.{}e}", val, default_prec - 1) << std::endl;
                    std::cout << "    parsed = " << fmt::format("{:.{}e}", val1, default_prec - 1) << std::endl;
                    std::cout << "ref parsed = " << fmt::format("{:.{}e}", val2, default_prec - 1) << std::endl;
                    std::cout << "mantissa = " << uval << std::endl;
                    std::cout << "exp = " << (exp - pow_bias) << " (+ " << pow_bias << ")" << std::endl;
                    VERIFY(--N_err > 0);
                }
            }
        }
    }
}

template<typename Ty>
void string_test_2(bool general, int iter_count) {
    std::array<char, 128> s;
    std::array<char, 128> s_ref;
    std::default_random_engine generator;

    const int bits = std::is_same<Ty, double>::value ? 52 : 23;
    const int pow_max = std::is_same<Ty, double>::value ? 2047 : 255;
    const int pow_bias = std::is_same<Ty, double>::value ? 1023 : 127;
    const int default_prec = std::is_same<Ty, double>::value ? 17 : 9;
    const int max_prec = 19;

    std::uniform_int_distribution<uint64_t> distribution(5, (1ull << bits) - 2);

    int N_err = 1;

    for (int iter = 0, perc0 = -1; iter < iter_count; ++iter) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            std::cout << std::setw(3) << (perc / 10) << "." << std::setw(0) << (perc % 10) << "%\b\b\b\b\b\b"
                      << std::flush;
            perc0 = perc;
        }

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

        for (int k = 0; k < pow_max; ++k) {
            int exp = k;
            uint64_t uval = mantissa | (static_cast<uint64_t>(exp) << bits);
            auto val = *reinterpret_cast<Ty*>(&uval);

            for (int prec0 = max_prec; prec0 > 0; --prec0) {
                int prec = prec0 - (general ? 0 : 1);
                char* last = util::format_to(s.data(), general ? "{:.{}g}" : "{:.{}e}", val, prec);
                *last = '\0';

                *fmt::format_to(s_ref.data(), general ? "{:.{}g}" : "{:.{}e}", val, prec) = '\0';

                if (std::strcmp(s.data(), s_ref.data()) != 0) {
                    std::cout << std::endl << "k = " << k << " iter = " << iter << " prec = " << prec << std::endl;
                    std::cout << "result = " << s.data() << std::endl;
                    std::cout << "   ref = " << s_ref.data() << std::endl;
                    std::cout << "mantissa = " << uval << std::endl;
                    std::cout << "exp = " << (exp - pow_bias) << " (+ " << pow_bias << ")" << std::endl;
                    VERIFY(--N_err > 0);
                }

                Ty val1 = 0, val2 = 0;
                VERIFY(util::string_converter<Ty>::from_string(s.data(), last, val1) == last);
#    if defined(_MSC_VER) && __cplusplus >= 201703L
                std::from_chars(s.data(), last, val2);
#    else
                std::sscanf(s.data(), std::is_same<Ty, double>::value ? "%lf" : "%f", &val2);
#    endif
                if (val1 != val2 || (prec0 >= default_prec && val1 != val)) {
                    std::cout << std::endl << "k = " << k << " iter = " << iter << " prec = " << prec << std::endl;
                    std::cout << "result = " << s.data() << std::endl;
                    std::cout << "   ref = " << s_ref.data() << std::endl;
                    std::cout << "       src = " << fmt::format("{:.{}e}", val, default_prec - 1) << std::endl;
                    std::cout << "    parsed = " << fmt::format("{:.{}e}", val1, default_prec - 1) << std::endl;
                    std::cout << "ref parsed = " << fmt::format("{:.{}e}", val2, default_prec - 1) << std::endl;
                    std::cout << "mantissa = " << uval << std::endl;
                    std::cout << "exp = " << (exp - pow_bias) << " (+ " << pow_bias << ")" << std::endl;
                    VERIFY(--N_err > 0);
                }
            }
        }
    }
}

template<typename Ty>
void string_test_3(int iter_count) {
    std::array<char, 128> s;
    std::array<char, 128> s_ref;
    std::default_random_engine generator;

    const int bits = std::is_same<Ty, double>::value ? 52 : 23;
    const int pow_max = std::is_same<Ty, double>::value ? 2047 : 255;
    const int pow_bias = std::is_same<Ty, double>::value ? 1023 : 127;
    const int default_prec = std::is_same<Ty, double>::value ? 17 : 9;
    const int max_prec = 19;

    std::uniform_int_distribution<uint64_t> distribution(5, (1ull << bits) - 2);

    int N_err = 100;

    uint64_t tot_length = 0, tot_length_fmt = 0;
    unsigned tot_length_count = 0;

    auto count_digs = [](const char* p, const char* last) {
        bool first = true;
        int count = 0, z_count = 0;
        while (p != last) {
            char ch = *p++;
            if (ch == '0') {
                ++z_count;
            } else if (std::isdigit(static_cast<unsigned char>(ch))) {
                count += (first ? 0 : z_count) + 1;
                z_count = 0, first = false;
            } else if (ch == 'e') {
                break;
            }
        }
        return count;
    };

    for (int iter = 0, perc0 = -1; iter < iter_count; ++iter) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            std::cout << std::setw(3) << (perc / 10) << "." << std::setw(0) << (perc % 10) << "%\b\b\b\b\b\b"
                      << std::flush;
            perc0 = perc;
        }

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

        for (int k = 0; k < pow_max; ++k) {
            int exp = k;
            uint64_t uval = mantissa | (static_cast<uint64_t>(exp) << bits);
            auto val = *reinterpret_cast<Ty*>(&uval);

            char* last = util::format_to(s.data(), "{}", val);
            *last = '\0';

            dtoa_milo(val, s_ref.data());
            char* last_fmt = s_ref.data() + std::strlen(s_ref.data());
            // char* last_fmt = fmt::format_to(s_ref.data(), "{}", val);
            // *last_fmt = '\0';

            if (std::is_same<Ty, double>::value) {
                if (tot_length_count < (1ull << 32) - 1) {
                    tot_length_fmt += last_fmt - s_ref.data();
                    tot_length += last - s.data();
                    ++tot_length_count;
                }
                if (count_digs(s.data(), last) > count_digs(s_ref.data(), last_fmt) ||
                    last - s.data() > last_fmt - s_ref.data()) {
                    std::cout << std::endl << "k = " << k << " iter = " << iter << std::endl;
                    std::cout << "result = " << s.data() << std::endl;
                    std::cout << "   ref = " << s_ref.data() << std::endl;
                    std::cout << "mantissa = " << uval << std::endl;
                    std::cout << "exp = " << (exp - pow_bias) << " (+ " << pow_bias << ")" << std::endl;
                    VERIFY(--N_err > 0);
                }
            }

            Ty val1 = 0, val2 = 0;
            VERIFY(util::string_converter<Ty>::from_string(s.data(), last, val1) == last);
#    if defined(_MSC_VER) && __cplusplus >= 201703L
            std::from_chars(s.data(), last, val2);
#    else
            std::sscanf(s.data(), std::is_same<Ty, double>::value ? "%lf" : "%f", &val2);
#    endif
            if (val1 != val2 || val1 != val) {
                std::cout << std::endl << "k = " << k << " iter = " << iter << std::endl;
                std::cout << "result = " << s.data() << std::endl;
                std::cout << "   ref = " << s_ref.data() << std::endl;
                std::cout << "       src = " << fmt::format("{:.{}e}", val, default_prec - 1) << std::endl;
                std::cout << "    parsed = " << fmt::format("{:.{}e}", val1, default_prec - 1) << std::endl;
                std::cout << "ref parsed = " << fmt::format("{:.{}e}", val2, default_prec - 1) << std::endl;
                std::cout << "mantissa = " << uval << std::endl;
                std::cout << "exp = " << (exp - pow_bias) << " (+ " << pow_bias << ")" << std::endl;
                VERIFY(--N_err > 0);
            }
        }
    }

    if (std::is_same<Ty, double>::value) {
        std::cout << "               " << std::endl
                  << "avg length = " << static_cast<double>(tot_length) / tot_length_count
                  << " (ref = " << static_cast<double>(tot_length_fmt) / tot_length_count << ')' << std::endl;
    }
}
#endif

#if defined(NDEBUG)
const int brute_N = 200000;
#else   // defined(NDEBUG)
const int brute_N = 200;
#endif  // defined(NDEBUG)

int test_bruteforce0() {
    string_test_0(2500 * brute_N);
    return 0;
}
#if !defined(_MSC_VER) || _MSC_VER >= 1920
int test_bruteforce1() {
    string_test_1<double>(25 * brute_N);
    return 0;
}
int test_bruteforce2() {
    string_test_2<double>(false, brute_N);
    return 0;
}
int test_bruteforce3() {
    string_test_2<double>(true, brute_N);
    return 0;
}
int test_bruteforce4() {
    string_test_1<float>(25 * brute_N);
    return 0;
}
int test_bruteforce5() {
    string_test_2<float>(false, brute_N);
    return 0;
}
int test_bruteforce6() {
    string_test_2<float>(true, brute_N);
    return 0;
}
int test_bruteforce7() {
    string_test_3<double>(250 * brute_N);
    return 0;
}
int test_bruteforce8() {
    string_test_3<float>(250 * brute_N);
    return 0;
}
#endif

// --------------------------------------------

int perf_integer(int iter_count) {
    std::array<char, 128> s;
    std::default_random_engine generator;
    std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());

    std::vector<uint64_t> v;
    v.resize(iter_count);
    for (uint64_t& val : v) { val = distribution(generator); }

    double eps = 0;

    auto start = std::clock();
    for (uint64_t val : v) {
        char* last = util::to_string_to(s.data(), val);
        uint64_t val1 = util::from_string<uint64_t>(std::string_view(s.data(), last - s.data()));
        eps += val - val1;
    }

    return eps == 0 ? static_cast<int>(std::clock() - start) : 0;
}

int perf_integer_libc(int iter_count) {
    std::array<char, 128> s;
    std::default_random_engine generator;
    std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());

    std::vector<uint64_t> v;
    v.resize(iter_count);
    for (uint64_t& val : v) { val = distribution(generator); }

    double eps = 0;

    auto start = std::clock();
    for (uint64_t val : v) {
#if defined(_MSC_VER) && __cplusplus >= 201703L
        auto result = std::to_chars(s.data(), s.data() + s.size(), val);
        *result.ptr = '\0';
#else
        std::sprintf(s.data(), "%.lu", val);
#endif

        uint64_t val1 = 0;
#if defined(_MSC_VER) && __cplusplus >= 201703L
        std::from_chars(s.data(), result.ptr, val1);
#else
        std::sscanf(s.data(), "%lu", &val1);
#endif
        eps += val - val1;
    }

    return eps == 0 ? static_cast<int>(std::clock() - start) : 0;
}

int perf_float(int iter_count) {
    std::array<char, 128> s;
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(1., 10.);
    std::uniform_int_distribution<int> exp_distribution(-324, 308);

    std::vector<double> v;
    v.resize(iter_count);
    for (double& val : v) { val = distribution(generator) * std::pow(10, exp_distribution(generator)); }

    double eps = 0;

    auto start = std::clock();
    for (double val : v) {
        char* last = util::format_to(s.data(), "{}", val);
        double val1 = util::from_string<double>(std::string_view(s.data(), last - s.data()));
        eps = std::max(std::fabs((val - val1) / val), eps);
    }

    return eps == 0 ? static_cast<int>(std::clock() - start) : 0;
}

int perf_float_libc(int iter_count) {
    std::array<char, 128> s;
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(1., 10.);
    std::uniform_int_distribution<int> exp_distribution(-324, 308);

    std::vector<double> v;
    v.resize(iter_count);
    for (double& val : v) { val = distribution(generator) * std::pow(10, exp_distribution(generator)); }

    double eps = 0;

    auto start = std::clock();
    for (double val : v) {
#if defined(_MSC_VER) && __cplusplus >= 201703L
        auto result = std::to_chars(s.data(), s.data() + s.size(), val, std::chars_format::general, 17);
        *result.ptr = '\0';
#else
        std::sprintf(s.data(), "%.17lg", val);
#endif

        double val1 = 0;
#if defined(_MSC_VER) && __cplusplus >= 201703L
        std::from_chars(s.data(), result.ptr, val1);
#else
        std::sscanf(s.data(), "%lf", &val1);
#endif
        eps = std::max(std::fabs((val - val1) / val), eps);
    }

    return eps == 0 ? static_cast<int>(std::clock() - start) : 0;
}

#if !defined(_MSC_VER) || _MSC_VER >= 1920
int perf_float_fmt(int iter_count) {
    std::array<char, 128> s;
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(1., 10.);
    std::uniform_int_distribution<int> exp_distribution(-324, 308);

    std::vector<double> v;
    v.resize(iter_count);
    for (double& val : v) { val = distribution(generator) * std::pow(10, exp_distribution(generator)); }

    double eps = 0;

    auto start = std::clock();
    for (double val : v) {
        char* last = fmt::format_to(s.data(), "{}", val);
        double val1 = util::from_string<double>(std::string_view(s.data(), last - s.data()));
        eps = std::max(std::fabs((val - val1) / val), eps);
    }

    return eps == 0 ? static_cast<int>(std::clock() - start) : 0;
}
#endif

const int perf_N = 2000000;
int test_integer_perf() { return perf_integer(2 * perf_N); }
int test_integer_perf_libc() { return perf_integer_libc(2 * perf_N); }
int test_float_perf() { return perf_float(perf_N); }
int test_float_perf_libc() { return perf_float_libc(perf_N); }
#if !defined(_MSC_VER) || _MSC_VER >= 1920
int test_float_perf_fmt() { return perf_float_fmt(perf_N); }
#endif

}  // namespace

ADD_TEST_CASE("", "string convertion", test_string_cvt_0);
ADD_TEST_CASE("", "string convertion", test_string_cvt_1);
ADD_TEST_CASE("", "string convertion", test_string_cvt_1u);
ADD_TEST_CASE("", "string convertion", test_string_cvt_2);
ADD_TEST_CASE("", "string convertion", test_string_cvt_3);
ADD_TEST_CASE("", "string convertion", test_string_cvt_4);

ADD_TEST_CASE("1-bruteforce", "string integer convertion", test_bruteforce0);
#if !defined(_MSC_VER) || _MSC_VER >= 1920
ADD_TEST_CASE("1-bruteforce", "string double fixed convertion", test_bruteforce1);
ADD_TEST_CASE("1-bruteforce", "string double scientific convertion", test_bruteforce2);
ADD_TEST_CASE("1-bruteforce", "string double general convertion", test_bruteforce3);
ADD_TEST_CASE("1-bruteforce", "string float fixed convertion", test_bruteforce4);
ADD_TEST_CASE("1-bruteforce", "string float scientific convertion", test_bruteforce5);
ADD_TEST_CASE("1-bruteforce", "string float general convertion", test_bruteforce6);
ADD_TEST_CASE("1-bruteforce", "string double default convertion", test_bruteforce7);
ADD_TEST_CASE("1-bruteforce", "string float default convertion", test_bruteforce8);
#endif

ADD_TEST_CASE("2-perf", "string uint64_t convertion", test_integer_perf);
ADD_TEST_CASE("2-perf", "<libc> string uint64_t convertion", test_integer_perf_libc);
ADD_TEST_CASE("2-perf", "string double convertion", test_float_perf);
ADD_TEST_CASE("2-perf", "<libc> string double convertion", test_float_perf_libc);
#if !defined(_MSC_VER) || _MSC_VER >= 1920
ADD_TEST_CASE("2-perf", "<fmt> string double convertion", test_float_perf_fmt);
#endif
