#include "test_suite.h"
#include "util/format.h"
#include "util/vector.h"

#include <cstring>
#include <random>

#if defined(_MSC_VER) && __cplusplus >= 201703L
#    include <charconv>
#endif

namespace {

int test_string_cvt_0() {
    VERIFY(util::to_string<int>(123456) == "123456");
    VERIFY(util::to_string<bool>(true) == "true");

    return 0;
}

int test_string_cvt_1() {
    VERIFY(util::to_string<int>(1234) == "1234");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kBin) == "10011010010");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kOct) == "2322");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kHex) == "4d2");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kHex | util::fmt_flags::kUpperCase) == "4D2");

    VERIFY(util::to_string<int>(1234, util::fmt_flags::kShowBase) == "1234");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kBin | util::fmt_flags::kShowBase) == "10011010010b");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kBin | util::fmt_flags::kShowBase |
                                          util::fmt_flags::kUpperCase) == "10011010010B");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kOct | util::fmt_flags::kShowBase) == "02322");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kHex | util::fmt_flags::kShowBase) == "0x4d2");
    VERIFY(util::to_string<int>(
               1234, util::fmt_flags::kHex | util::fmt_flags::kShowBase | util::fmt_flags::kUpperCase) == "0X4D2");

    VERIFY(util::to_string<int>(1234, util::fmt_flags::kLeadingZeroes, 0, 15, ' ') == "000000000001234");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kBin | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "000010011010010");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kOct | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "000000000002322");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "0000000000004d2");
    VERIFY(util::to_string<int>(1234,
                                util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase,
                                0, 15, ' ') == "0000000000004D2");

    VERIFY(util::to_string<int>(1234, util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15, ' ') ==
           "000000000001234");
    VERIFY(util::to_string<int>(1234,
                                util::fmt_flags::kBin | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0,
                                15, ' ') == "00010011010010b");
    VERIFY(util::to_string<int>(1234,
                                util::fmt_flags::kBin | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase |
                                    util::fmt_flags::kUpperCase,
                                0, 15, ' ') == "00010011010010B");
    VERIFY(util::to_string<int>(1234,
                                util::fmt_flags::kOct | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0,
                                15, ' ') == "000000000002322");
    VERIFY(util::to_string<int>(1234,
                                util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0,
                                15, ' ') == "0x00000000004d2");
    VERIFY(util::to_string<int>(1234,
                                util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase |
                                    util::fmt_flags::kUpperCase,
                                0, 15, ' ') == "0X00000000004D2");

    VERIFY(util::to_string<int>(-25510) == "-25510");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kBin) == "11111111111111111001110001011010");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kOct) == "37777716132");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kHex) == "ffff9c5a");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kHex | util::fmt_flags::kUpperCase) == "FFFF9C5A");

    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kShowBase) == "-25510");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kBin | util::fmt_flags::kShowBase) ==
           "11111111111111111001110001011010b");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kBin | util::fmt_flags::kShowBase |
                                            util::fmt_flags::kUpperCase) == "11111111111111111001110001011010B");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kOct | util::fmt_flags::kShowBase) == "037777716132");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kHex | util::fmt_flags::kShowBase) == "0xffff9c5a");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kHex | util::fmt_flags::kShowBase |
                                            util::fmt_flags::kUpperCase) == "0XFFFF9C5A");

    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kLeadingZeroes, 0, 15, ' ') == "-00000000025510");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kBin | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "11111111111111111001110001011010");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kOct | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "000037777716132");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "0000000ffff9c5a");
    VERIFY(util::to_string<int>(-25510,
                                util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase,
                                0, 15, ' ') == "0000000FFFF9C5A");

    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15, ' ') ==
           "-00000000025510");
    VERIFY(util::to_string<int>(-25510,
                                util::fmt_flags::kBin | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0,
                                15, ' ') == "11111111111111111001110001011010b");
    VERIFY(util::to_string<int>(-25510,
                                util::fmt_flags::kBin | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase |
                                    util::fmt_flags::kUpperCase,
                                0, 15, ' ') == "11111111111111111001110001011010B");
    VERIFY(util::to_string<int>(-25510,
                                util::fmt_flags::kOct | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0,
                                15, ' ') == "000037777716132");
    VERIFY(util::to_string<int>(-25510,
                                util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0,
                                15, ' ') == "0x00000ffff9c5a");
    VERIFY(util::to_string<int>(-25510,
                                util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase |
                                    util::fmt_flags::kUpperCase,
                                0, 15, ' ') == "0X00000FFFF9C5A");

    VERIFY(util::to_string<int>(1234, util::fmt_flags::kSignPos) == "+1234");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kSignAlign) == " 1234");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kBin | util::fmt_flags::kSignPos) == "10011010010");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kOct | util::fmt_flags::kSignPos) == "2322");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kHex | util::fmt_flags::kSignPos) == "4d2");
    VERIFY(util::to_string<int>(
               1234, util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kUpperCase) == "4D2");

    VERIFY(util::to_string<int>(1234, util::fmt_flags::kSignPos | util::fmt_flags::kShowBase) == "+1234");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kSignAlign | util::fmt_flags::kShowBase) == " 1234");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kBin | util::fmt_flags::kSignPos | util::fmt_flags::kShowBase) ==
           "10011010010b");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kBin | util::fmt_flags::kSignPos | util::fmt_flags::kShowBase |
                                          util::fmt_flags::kUpperCase) == "10011010010B");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kOct | util::fmt_flags::kSignPos | util::fmt_flags::kShowBase) ==
           "02322");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kShowBase) ==
           "0x4d2");
    VERIFY(util::to_string<int>(
               1234, util::fmt_flags::kHex | util::fmt_flags::kShowBase | util::fmt_flags::kUpperCase) == "0X4D2");

    VERIFY(util::to_string<int>(1234, util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "+00000000001234");
    VERIFY(util::to_string<int>(1234, util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           " 00000000001234");
    VERIFY(util::to_string<int>(1234,
                                util::fmt_flags::kBin | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0,
                                15, ' ') == "000010011010010");
    VERIFY(util::to_string<int>(1234,
                                util::fmt_flags::kOct | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0,
                                15, ' ') == "000000000002322");
    VERIFY(util::to_string<int>(1234,
                                util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0,
                                15, ' ') == "0000000000004d2");
    VERIFY(util::to_string<int>(1234,
                                util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes |
                                    util::fmt_flags::kUpperCase,
                                0, 15, ' ') == "0000000000004D2");

    VERIFY(util::to_string<int>(
               1234, util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15,
               ' ') == "+00000000001234");
    VERIFY(util::to_string<int>(
               1234, util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15,
               ' ') == " 00000000001234");
    VERIFY(util::to_string<int>(1234,
                                util::fmt_flags::kBin | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes |
                                    util::fmt_flags::kShowBase,
                                0, 15, ' ') == "00010011010010b");
    VERIFY(util::to_string<int>(1234,
                                util::fmt_flags::kBin | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes |
                                    util::fmt_flags::kShowBase | util::fmt_flags::kUpperCase,
                                0, 15, ' ') == "00010011010010B");
    VERIFY(util::to_string<int>(1234,
                                util::fmt_flags::kOct | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes |
                                    util::fmt_flags::kShowBase,
                                0, 15, ' ') == "000000000002322");
    VERIFY(util::to_string<int>(1234,
                                util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes |
                                    util::fmt_flags::kShowBase,
                                0, 15, ' ') == "0x00000000004d2");
    VERIFY(util::to_string<int>(1234,
                                util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes |
                                    util::fmt_flags::kShowBase | util::fmt_flags::kUpperCase,
                                0, 15, ' ') == "0X00000000004D2");

    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kSignPos) == "-25510");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kSignAlign) == "-25510");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kBin | util::fmt_flags::kSignPos) ==
           "11111111111111111001110001011010");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kOct | util::fmt_flags::kSignPos) == "37777716132");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kHex | util::fmt_flags::kSignPos) == "ffff9c5a");
    VERIFY(util::to_string<int>(
               -25510, util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kUpperCase) == "FFFF9C5A");

    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kSignPos | util::fmt_flags::kShowBase) == "-25510");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kSignAlign | util::fmt_flags::kShowBase) == "-25510");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kBin | util::fmt_flags::kSignPos |
                                            util::fmt_flags::kShowBase) == "11111111111111111001110001011010b");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kBin | util::fmt_flags::kSignPos | util::fmt_flags::kShowBase |
                                            util::fmt_flags::kUpperCase) == "11111111111111111001110001011010B");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kOct | util::fmt_flags::kSignPos |
                                            util::fmt_flags::kShowBase) == "037777716132");
    VERIFY(util::to_string<int>(
               -25510, util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kShowBase) == "0xffff9c5a");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kShowBase |
                                            util::fmt_flags::kUpperCase) == "0XFFFF9C5A");

    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "-00000000025510");
    VERIFY(util::to_string<int>(-25510, util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "-00000000025510");
    VERIFY(util::to_string<int>(-25510,
                                util::fmt_flags::kBin | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0,
                                15, ' ') == "11111111111111111001110001011010");
    VERIFY(util::to_string<int>(-25510,
                                util::fmt_flags::kOct | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0,
                                15, ' ') == "000037777716132");
    VERIFY(util::to_string<int>(-25510,
                                util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0,
                                15, ' ') == "0000000ffff9c5a");
    VERIFY(util::to_string<int>(-25510,
                                util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes |
                                    util::fmt_flags::kUpperCase,
                                0, 15, ' ') == "0000000FFFF9C5A");

    VERIFY(util::to_string<int>(
               -25510, util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15,
               ' ') == "-00000000025510");
    VERIFY(util::to_string<int>(
               -25510, util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0,
               15, ' ') == "-00000000025510");
    VERIFY(util::to_string<int>(-25510,
                                util::fmt_flags::kBin | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes |
                                    util::fmt_flags::kShowBase,
                                0, 15, ' ') == "11111111111111111001110001011010b");
    VERIFY(util::to_string<int>(-25510,
                                util::fmt_flags::kBin | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes |
                                    util::fmt_flags::kShowBase | util::fmt_flags::kUpperCase,
                                0, 15, ' ') == "11111111111111111001110001011010B");
    VERIFY(util::to_string<int>(-25510,
                                util::fmt_flags::kOct | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes |
                                    util::fmt_flags::kShowBase,
                                0, 15, ' ') == "000037777716132");
    VERIFY(util::to_string<int>(-25510,
                                util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes |
                                    util::fmt_flags::kShowBase,
                                0, 15, ' ') == "0x00000ffff9c5a");
    VERIFY(util::to_string<int>(-25510,
                                util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes |
                                    util::fmt_flags::kShowBase | util::fmt_flags::kUpperCase,
                                0, 15, ' ') == "0X00000FFFF9C5A");

    return 0;
}

int test_string_cvt_1u() {
    VERIFY(util::to_string<unsigned>(1234) == "1234");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kBin) == "10011010010");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kOct) == "2322");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kHex) == "4d2");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kHex | util::fmt_flags::kUpperCase) == "4D2");

    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kShowBase) == "1234");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kBin | util::fmt_flags::kShowBase) == "10011010010b");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kBin | util::fmt_flags::kShowBase |
                                               util::fmt_flags::kUpperCase) == "10011010010B");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kOct | util::fmt_flags::kShowBase) == "02322");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kHex | util::fmt_flags::kShowBase) == "0x4d2");
    VERIFY(util::to_string<unsigned>(
               1234, util::fmt_flags::kHex | util::fmt_flags::kShowBase | util::fmt_flags::kUpperCase) == "0X4D2");

    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kLeadingZeroes, 0, 15, ' ') == "000000000001234");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kBin | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "000010011010010");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kOct | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "000000000002322");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "0000000000004d2");
    VERIFY(util::to_string<unsigned>(
               1234, util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, 0, 15,
               ' ') == "0000000000004D2");

    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15, ' ') ==
           "000000000001234");
    VERIFY(util::to_string<unsigned>(
               1234, util::fmt_flags::kBin | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15,
               ' ') == "00010011010010b");
    VERIFY(util::to_string<unsigned>(1234,
                                     util::fmt_flags::kBin | util::fmt_flags::kLeadingZeroes |
                                         util::fmt_flags::kShowBase | util::fmt_flags::kUpperCase,
                                     0, 15, ' ') == "00010011010010B");
    VERIFY(util::to_string<unsigned>(
               1234, util::fmt_flags::kOct | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15,
               ' ') == "000000000002322");
    VERIFY(util::to_string<unsigned>(
               1234, util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15,
               ' ') == "0x00000000004d2");
    VERIFY(util::to_string<unsigned>(1234,
                                     util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes |
                                         util::fmt_flags::kShowBase | util::fmt_flags::kUpperCase,
                                     0, 15, ' ') == "0X00000000004D2");

    VERIFY(util::to_string<unsigned>(-25510) == "4294941786");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kBin) == "11111111111111111001110001011010");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kOct) == "37777716132");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kHex) == "ffff9c5a");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kHex | util::fmt_flags::kUpperCase) == "FFFF9C5A");

    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kShowBase) == "4294941786");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kBin | util::fmt_flags::kShowBase) ==
           "11111111111111111001110001011010b");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kBin | util::fmt_flags::kShowBase |
                                                 util::fmt_flags::kUpperCase) == "11111111111111111001110001011010B");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kOct | util::fmt_flags::kShowBase) == "037777716132");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kHex | util::fmt_flags::kShowBase) == "0xffff9c5a");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kHex | util::fmt_flags::kShowBase |
                                                 util::fmt_flags::kUpperCase) == "0XFFFF9C5A");

    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kLeadingZeroes, 0, 15, ' ') == "000004294941786");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kBin | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "11111111111111111001110001011010");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kOct | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "000037777716132");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "0000000ffff9c5a");
    VERIFY(util::to_string<unsigned>(
               -25510, util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, 0, 15,
               ' ') == "0000000FFFF9C5A");

    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15,
                                     ' ') == "000004294941786");
    VERIFY(util::to_string<unsigned>(
               -25510, util::fmt_flags::kBin | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15,
               ' ') == "11111111111111111001110001011010b");
    VERIFY(util::to_string<unsigned>(-25510,
                                     util::fmt_flags::kBin | util::fmt_flags::kLeadingZeroes |
                                         util::fmt_flags::kShowBase | util::fmt_flags::kUpperCase,
                                     0, 15, ' ') == "11111111111111111001110001011010B");
    VERIFY(util::to_string<unsigned>(
               -25510, util::fmt_flags::kOct | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15,
               ' ') == "000037777716132");
    VERIFY(util::to_string<unsigned>(
               -25510, util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15,
               ' ') == "0x00000ffff9c5a");
    VERIFY(util::to_string<unsigned>(-25510,
                                     util::fmt_flags::kHex | util::fmt_flags::kLeadingZeroes |
                                         util::fmt_flags::kShowBase | util::fmt_flags::kUpperCase,
                                     0, 15, ' ') == "0X00000FFFF9C5A");

    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kSignPos) == "1234");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kSignAlign) == "1234");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kBin | util::fmt_flags::kSignPos) == "10011010010");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kOct | util::fmt_flags::kSignPos) == "2322");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kHex | util::fmt_flags::kSignPos) == "4d2");
    VERIFY(util::to_string<unsigned>(
               1234, util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kUpperCase) == "4D2");

    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kSignPos | util::fmt_flags::kShowBase) == "1234");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kSignAlign | util::fmt_flags::kShowBase) == "1234");
    VERIFY(util::to_string<unsigned>(
               1234, util::fmt_flags::kBin | util::fmt_flags::kSignPos | util::fmt_flags::kShowBase) == "10011010010b");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kBin | util::fmt_flags::kSignPos |
                                               util::fmt_flags::kShowBase | util::fmt_flags::kUpperCase) ==
           "10011010010B");
    VERIFY(util::to_string<unsigned>(
               1234, util::fmt_flags::kOct | util::fmt_flags::kSignPos | util::fmt_flags::kShowBase) == "02322");
    VERIFY(util::to_string<unsigned>(
               1234, util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kShowBase) == "0x4d2");
    VERIFY(util::to_string<unsigned>(
               1234, util::fmt_flags::kHex | util::fmt_flags::kShowBase | util::fmt_flags::kUpperCase) == "0X4D2");

    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "000000000001234");
    VERIFY(util::to_string<unsigned>(1234, util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "000000000001234");
    VERIFY(util::to_string<unsigned>(
               1234, util::fmt_flags::kBin | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "000010011010010");
    VERIFY(util::to_string<unsigned>(
               1234, util::fmt_flags::kOct | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "000000000002322");
    VERIFY(util::to_string<unsigned>(
               1234, util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "0000000000004d2");
    VERIFY(util::to_string<unsigned>(1234,
                                     util::fmt_flags::kHex | util::fmt_flags::kSignPos |
                                         util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase,
                                     0, 15, ' ') == "0000000000004D2");

    VERIFY(util::to_string<unsigned>(
               1234, util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15,
               ' ') == "000000000001234");
    VERIFY(util::to_string<unsigned>(
               1234, util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15,
               ' ') == "000000000001234");
    VERIFY(util::to_string<unsigned>(1234,
                                     util::fmt_flags::kBin | util::fmt_flags::kSignPos |
                                         util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase,
                                     0, 15, ' ') == "00010011010010b");
    VERIFY(util::to_string<unsigned>(1234,
                                     util::fmt_flags::kBin | util::fmt_flags::kSignPos |
                                         util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase |
                                         util::fmt_flags::kUpperCase,
                                     0, 15, ' ') == "00010011010010B");
    VERIFY(util::to_string<unsigned>(1234,
                                     util::fmt_flags::kOct | util::fmt_flags::kSignPos |
                                         util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase,
                                     0, 15, ' ') == "000000000002322");
    VERIFY(util::to_string<unsigned>(1234,
                                     util::fmt_flags::kHex | util::fmt_flags::kSignPos |
                                         util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase,
                                     0, 15, ' ') == "0x00000000004d2");
    VERIFY(util::to_string<unsigned>(1234,
                                     util::fmt_flags::kHex | util::fmt_flags::kSignPos |
                                         util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase |
                                         util::fmt_flags::kUpperCase,
                                     0, 15, ' ') == "0X00000000004D2");

    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kSignPos) == "4294941786");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kSignAlign) == "4294941786");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kBin | util::fmt_flags::kSignPos) ==
           "11111111111111111001110001011010");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kOct | util::fmt_flags::kSignPos) == "37777716132");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kHex | util::fmt_flags::kSignPos) == "ffff9c5a");
    VERIFY(util::to_string<unsigned>(
               -25510, util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kUpperCase) == "FFFF9C5A");

    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kSignPos | util::fmt_flags::kShowBase) == "4294941786");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kSignAlign | util::fmt_flags::kShowBase) == "4294941786");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kBin | util::fmt_flags::kSignPos |
                                                 util::fmt_flags::kShowBase) == "11111111111111111001110001011010b");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kBin | util::fmt_flags::kSignPos |
                                                 util::fmt_flags::kShowBase | util::fmt_flags::kUpperCase) ==
           "11111111111111111001110001011010B");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kOct | util::fmt_flags::kSignPos |
                                                 util::fmt_flags::kShowBase) == "037777716132");
    VERIFY(util::to_string<unsigned>(
               -25510, util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kShowBase) == "0xffff9c5a");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kHex | util::fmt_flags::kSignPos |
                                                 util::fmt_flags::kShowBase | util::fmt_flags::kUpperCase) ==
           "0XFFFF9C5A");

    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0, 15, ' ') ==
           "000004294941786");
    VERIFY(util::to_string<unsigned>(-25510, util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes, 0, 15,
                                     ' ') == "000004294941786");
    VERIFY(util::to_string<unsigned>(
               -25510, util::fmt_flags::kBin | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0, 15,
               ' ') == "11111111111111111001110001011010");
    VERIFY(util::to_string<unsigned>(
               -25510, util::fmt_flags::kOct | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0, 15,
               ' ') == "000037777716132");
    VERIFY(util::to_string<unsigned>(
               -25510, util::fmt_flags::kHex | util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, 0, 15,
               ' ') == "0000000ffff9c5a");
    VERIFY(util::to_string<unsigned>(-25510,
                                     util::fmt_flags::kHex | util::fmt_flags::kSignPos |
                                         util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase,
                                     0, 15, ' ') == "0000000FFFF9C5A");

    VERIFY(util::to_string<unsigned>(
               -25510, util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0, 15,
               ' ') == "000004294941786");
    VERIFY(util::to_string<unsigned>(
               -25510, util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase, 0,
               15, ' ') == "000004294941786");
    VERIFY(util::to_string<unsigned>(-25510,
                                     util::fmt_flags::kBin | util::fmt_flags::kSignPos |
                                         util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase,
                                     0, 15, ' ') == "11111111111111111001110001011010b");
    VERIFY(util::to_string<unsigned>(-25510,
                                     util::fmt_flags::kBin | util::fmt_flags::kSignPos |
                                         util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase |
                                         util::fmt_flags::kUpperCase,
                                     0, 15, ' ') == "11111111111111111001110001011010B");
    VERIFY(util::to_string<unsigned>(-25510,
                                     util::fmt_flags::kOct | util::fmt_flags::kSignPos |
                                         util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase,
                                     0, 15, ' ') == "000037777716132");
    VERIFY(util::to_string<unsigned>(-25510,
                                     util::fmt_flags::kHex | util::fmt_flags::kSignPos |
                                         util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase,
                                     0, 15, ' ') == "0x00000ffff9c5a");
    VERIFY(util::to_string<unsigned>(-25510,
                                     util::fmt_flags::kHex | util::fmt_flags::kSignPos |
                                         util::fmt_flags::kLeadingZeroes | util::fmt_flags::kShowBase |
                                         util::fmt_flags::kUpperCase,
                                     0, 15, ' ') == "0X00000FFFF9C5A");

    return 0;
}

int test_string_cvt_2() {
    VERIFY(util::to_string<double>(1.2345672222, util::fmt_flags::kFixed) == "1.234567");
    VERIFY(util::to_string<double>(1.2345677777, util::fmt_flags::kFixed) == "1.234568");
    VERIFY(util::to_string<double>(1.2345, util::fmt_flags::kFixed) == "1.234500");
    VERIFY(util::to_string<double>(0.12345, util::fmt_flags::kFixed) == "0.123450");
    VERIFY(util::to_string<double>(0.00124, util::fmt_flags::kFixed) == "0.001240");
    VERIFY(util::to_string<double>(23.34563444, util::fmt_flags::kFixed) == "23.345634");
    VERIFY(util::to_string<double>(23.34563464, util::fmt_flags::kFixed) == "23.345635");
    VERIFY(util::to_string<double>(23.3456, util::fmt_flags::kFixed) == "23.345600");
    VERIFY(util::to_string<double>(999.9999997, util::fmt_flags::kFixed) == "1000.000000");
    VERIFY(util::to_string<double>(0, util::fmt_flags::kFixed) == "0.000000");
    VERIFY(util::to_string<double>(0, util::fmt_flags::kFixed, 0) == "0");

    VERIFY(util::to_string<double>(1000000000, util::fmt_flags::kFixed) == "1000000000.000000");
    VERIFY(util::to_string<double>(231000000000000, util::fmt_flags::kFixed) == "231000000000000.000000");

    VERIFY(util::to_string<double>(345., util::fmt_flags::kFixed) == "345.000000");
    VERIFY(util::to_string<double>(345.234465876, util::fmt_flags::kFixed) == "345.234466");
    VERIFY(util::to_string<double>(34.5234465876, util::fmt_flags::kFixed) == "34.523447");
    VERIFY(util::to_string<double>(3.45234465876, util::fmt_flags::kFixed) == "3.452345");
    VERIFY(util::to_string<double>(0.345234465876, util::fmt_flags::kFixed) == "0.345234");
    VERIFY(util::to_string<double>(0.0234465876, util::fmt_flags::kFixed) == "0.023447");
    VERIFY(util::to_string<double>(0.00234465876, util::fmt_flags::kFixed) == "0.002345");
    VERIFY(util::to_string<double>(0.000234465876, util::fmt_flags::kFixed) == "0.000234");
    VERIFY(util::to_string<double>(0.0000234465876, util::fmt_flags::kFixed) == "0.000023");
    VERIFY(util::to_string<double>(0.00000234465876, util::fmt_flags::kFixed) == "0.000002");
    VERIFY(util::to_string<double>(0.000000234465876, util::fmt_flags::kFixed) == "0.000000");

    VERIFY(util::to_string<double>(345., util::fmt_flags::kFixed, 0) == "345");
    VERIFY(util::to_string<double>(345.234465876, util::fmt_flags::kFixed, 0) == "345");
    VERIFY(util::to_string<double>(34.5234465876, util::fmt_flags::kFixed, 0) == "35");
    VERIFY(util::to_string<double>(3.45234465876, util::fmt_flags::kFixed, 0) == "3");
    VERIFY(util::to_string<double>(0.345234465876, util::fmt_flags::kFixed, 0) == "0");
    VERIFY(util::to_string<double>(0.0234465876, util::fmt_flags::kFixed, 0) == "0");

    VERIFY(util::to_string<double>(0, util::fmt_flags::kScientific) == "0.000000e+00");
    VERIFY(util::to_string<double>(0, util::fmt_flags::kScientific, 0) == "0e+00");
    VERIFY(util::to_string<double>(0) == "0");

    VERIFY(util::to_string<double>(999999.9999996) == "1e+06");
    VERIFY(util::to_string<double>(3455, util::fmt_flags::kGeneral, 3) == "3.46e+03");
    VERIFY(util::to_string<double>(3445, util::fmt_flags::kGeneral, 3) == "3.44e+03");

    VERIFY(util::to_string<double>(0.003) == "0.003");
    VERIFY(util::to_string<double>(0.003000001) == "0.003");
    VERIFY(util::to_string<double>(0.00300001) == "0.00300001");
    VERIFY(util::to_string<double>(0.0030001) == "0.0030001");
    VERIFY(util::to_string<double>(0.003001) == "0.003001");

    VERIFY(util::to_string<double>(0.3) == "0.3");
    VERIFY(util::to_string<double>(0.3000001) == "0.3");
    VERIFY(util::to_string<double>(0.300001) == "0.300001");
    VERIFY(util::to_string<double>(0.30001) == "0.30001");
    VERIFY(util::to_string<double>(0.3001) == "0.3001");

    VERIFY(util::to_string<double>(345.) == "345");
    VERIFY(util::to_string<double>(345.0001) == "345");
    VERIFY(util::to_string<double>(345.001) == "345.001");
    VERIFY(util::to_string<double>(345.01) == "345.01");
    VERIFY(util::to_string<double>(345.1) == "345.1");

    VERIFY(util::to_string<double>(3.e23) == "3e+23");
    VERIFY(util::to_string<double>(3.000001e23) == "3e+23");
    VERIFY(util::to_string<double>(3.00001e23) == "3.00001e+23");
    VERIFY(util::to_string<double>(3.0001e23) == "3.0001e+23");
    VERIFY(util::to_string<double>(3.001e23) == "3.001e+23");

    VERIFY(util::to_string<double>(345., util::fmt_flags::kGeneral, 0) == "3e+02");
    VERIFY(util::to_string<double>(345.234465876, util::fmt_flags::kGeneral, 0) == "3e+02");
    VERIFY(util::to_string<double>(34.5234465876, util::fmt_flags::kGeneral, 0) == "3e+01");
    VERIFY(util::to_string<double>(3.45234465876, util::fmt_flags::kGeneral, 0) == "3");
    VERIFY(util::to_string<double>(0.345234465876, util::fmt_flags::kGeneral, 0) == "0.3");
    VERIFY(util::to_string<double>(0.0234465876, util::fmt_flags::kGeneral, 0) == "0.02");

    VERIFY(util::to_string<double>(0.0000006, util::fmt_flags::kFixed) == "0.000001");
    VERIFY(util::to_string<double>(0.000000500001, util::fmt_flags::kFixed) == "0.000001");
    VERIFY(util::to_string<double>(0.0000004, util::fmt_flags::kFixed) == "0.000000");
    VERIFY(util::to_string<double>(0.00000049999, util::fmt_flags::kFixed) == "0.000000");
    VERIFY(util::to_string<double>(0.00000006, util::fmt_flags::kFixed) == "0.000000");

    double d = 2000000. / 7.;  // 285714.28571428573923185468

    VERIFY(util::to_string<double>(d, util::fmt_flags::kScientific, 17) == "2.85714285714285739e+05");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kScientific, 16) == "2.8571428571428574e+05");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kScientific, 15) == "2.857142857142857e+05");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kScientific, 13) == "2.8571428571429e+05");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kScientific, 2) == "2.86e+05");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kScientific, 1) == "2.9e+05");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kScientific, 0) == "3e+05");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kScientific, 18) == "2.857142857142857392e+05");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kScientific, 20) == "2.85714285714285739200e+05");

    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 12) == "285714.285714285739");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 11) == "285714.28571428574");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 10) == "285714.2857142857");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 8) == "285714.28571429");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 2) == "285714.29");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 1) == "285714.3");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 0) == "285714");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 13) == "285714.2857142857392");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 15) == "285714.285714285739200");

    d = 285714.;

    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 8) == "285714.00000000");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 2) == "285714.00");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 1) == "285714.0");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 0) == "285714");

    d = 285714e+20;  // 28571400000000001222639616.00000000

    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 8) == "28571400000000001220000000.00000000");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 2) == "28571400000000001220000000.00");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 1) == "28571400000000001220000000.0");
    VERIFY(util::to_string<double>(d, util::fmt_flags::kFixed, 0) == "28571400000000001220000000");

    VERIFY(util::to_string<double>(0.0006382, util::fmt_flags::kFixed, 7) == "0.0006382");
    VERIFY(util::to_string<double>(0.0006382, util::fmt_flags::kFixed, 6) == "0.000638");
    VERIFY(util::to_string<double>(0.0006382, util::fmt_flags::kFixed, 5) == "0.00064");
    VERIFY(util::to_string<double>(0.0006382, util::fmt_flags::kFixed, 4) == "0.0006");
    VERIFY(util::to_string<double>(0.0006382, util::fmt_flags::kFixed, 3) == "0.001");
    VERIFY(util::to_string<double>(0.0006382, util::fmt_flags::kFixed, 2) == "0.00");
    VERIFY(util::to_string<double>(0.0006382, util::fmt_flags::kFixed, 1) == "0.0");
    VERIFY(util::to_string<double>(0.0006382, util::fmt_flags::kFixed, 0) == "0");

    VERIFY(util::to_string<double>(0.6382, util::fmt_flags::kFixed, 4) == "0.6382");
    VERIFY(util::to_string<double>(0.6382, util::fmt_flags::kFixed, 3) == "0.638");
    VERIFY(util::to_string<double>(0.6382, util::fmt_flags::kFixed, 2) == "0.64");
    VERIFY(util::to_string<double>(0.6382, util::fmt_flags::kFixed, 1) == "0.6");
    VERIFY(util::to_string<double>(0.6382, util::fmt_flags::kFixed, 0) == "1");

    VERIFY(util::to_string<double>(0.0004382, util::fmt_flags::kFixed, 3) == "0.000");
    VERIFY(util::to_string<double>(0.4382, util::fmt_flags::kFixed, 0) == "0");

    VERIFY(util::from_string<double>("9.999999999999999999") == 9.999999999999999999);

    VERIFY(util::to_string(util::from_string<double>("2.22507385850720139e-308")) == "2.22507e-308");
    VERIFY(util::to_string(util::from_string<double>("2.22507385850720138e-308")) == "2.22507e-308");
    VERIFY(util::to_string(util::from_string<double>("1.11253692925360070e-308")) == "1.11254e-308");
    VERIFY(util::to_string(util::from_string<double>("1.11253692925360069e-308")) == "1.11254e-308");
    VERIFY(util::to_string(util::from_string<double>("5.56268464626800346e-309")) == "5.56268e-309");
    VERIFY(util::to_string(util::from_string<double>("5.56268464626800345e-309")) == "5.56268e-309");
    VERIFY(util::to_string(util::from_string<double>("5.30498947741318079e-315")) == "5.30499e-315");
    VERIFY(util::to_string(util::from_string<double>("5.30498947741318078e-315")) == "5.30499e-315");
    VERIFY(util::to_string(util::from_string<double>("5.05923221341436462e-321")) == "5.05923e-321");
    VERIFY(util::to_string(util::from_string<double>("5.05923221341436461e-321")) == "5.05923e-321");
    VERIFY(util::to_string(util::from_string<double>("1.97626258336498618e-323")) == "1.97626e-323");
    VERIFY(util::to_string(util::from_string<double>("1.97626258336498617e-323")) == "1.97626e-323");
    VERIFY(util::to_string(util::from_string<double>("9.88131291682493089e-324")) == "9.88131e-324");
    VERIFY(util::to_string(util::from_string<double>("9.88131291682493088e-324")) == "9.88131e-324");
    VERIFY(util::to_string(util::from_string<double>("4.94065645841246545e-324")) == "4.94066e-324");
    VERIFY(util::to_string(util::from_string<double>("4.94065645841246544e-324")) == "4.94066e-324");
    VERIFY(util::to_string(util::from_string<double>("2.47032822920623273e-324")) == "4.94066e-324");
    VERIFY(util::to_string(util::from_string<double>("2.47032822920623272e-324")) == "0");

    VERIFY(util::to_string<double>(3.e23, util::fmt_flags::kShowPoint) == "3.e+23");

    VERIFY(util::to_string<double>(3.e23, util::fmt_flags::kUpperCase) == "3E+23");
    VERIFY(util::to_string<double>(3.e23, util::fmt_flags::kLeadingZeroes, -1, 15, ' ') == "00000000003e+23");
    VERIFY(util::to_string<double>(3.e23, util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15, ' ') ==
           "00000000003E+23");

    VERIFY(util::to_string<double>(3.e23, util::fmt_flags::kSignPos) == "+3e+23");
    VERIFY(util::to_string<double>(3.e23, util::fmt_flags::kSignPos | util::fmt_flags::kUpperCase) == "+3E+23");
    VERIFY(util::to_string<double>(3.e23, util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, -1, 15, ' ') ==
           "+0000000003e+23");
    VERIFY(util::to_string<double>(
               3.e23, util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
               ' ') == "+0000000003E+23");

    VERIFY(util::to_string<double>(3.e23, util::fmt_flags::kSignAlign) == " 3e+23");
    VERIFY(util::to_string<double>(3.e23, util::fmt_flags::kSignAlign | util::fmt_flags::kUpperCase) == " 3E+23");
    VERIFY(util::to_string<double>(3.e23, util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes, -1, 15, ' ') ==
           " 0000000003e+23");
    VERIFY(util::to_string<double>(
               3.e23, util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1,
               15, ' ') == " 0000000003E+23");

    VERIFY(fabs(util::from_string<float>(util::to_string<float>(0.2355f)) - 0.2355f) < 0.000001);
    VERIFY(fabs(util::from_string<double>(util::to_string<double>(-123.56e-1)) - -123.56e-1) < 0.000001);

    return 0;
}

int test_string_cvt_3() {
    VERIFY(util::to_string<double>(0, util::fmt_flags::kUpperCase) == "0");
    VERIFY(util::to_string<double>(0, util::fmt_flags::kLeadingZeroes, -1, 15, ' ') == "000000000000000");
    VERIFY(util::to_string<double>(0, util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15, ' ') ==
           "000000000000000");

    VERIFY(util::to_string<double>(0, util::fmt_flags::kSignPos) == "+0");
    VERIFY(util::to_string<double>(0, util::fmt_flags::kSignPos | util::fmt_flags::kUpperCase) == "+0");
    VERIFY(util::to_string<double>(0, util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, -1, 15, ' ') ==
           "+00000000000000");
    VERIFY(util::to_string<double>(
               0, util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
               ' ') == "+00000000000000");

    VERIFY(util::to_string<double>(0, util::fmt_flags::kSignAlign) == " 0");
    VERIFY(util::to_string<double>(0, util::fmt_flags::kSignAlign | util::fmt_flags::kUpperCase) == " 0");
    VERIFY(util::to_string<double>(0, util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes, -1, 15, ' ') ==
           " 00000000000000");
    VERIFY(util::to_string<double>(
               0, util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
               ' ') == " 00000000000000");

    VERIFY(util::to_string<double>(-0.) == "-0");
    VERIFY(util::to_string<double>(-0., util::fmt_flags::kUpperCase) == "-0");
    VERIFY(util::to_string<double>(-0., util::fmt_flags::kLeadingZeroes, -1, 15, ' ') == "-00000000000000");
    VERIFY(util::to_string<double>(-0., util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15, ' ') ==
           "-00000000000000");

    VERIFY(util::to_string<double>(-0., util::fmt_flags::kSignPos) == "-0");
    VERIFY(util::to_string<double>(-0., util::fmt_flags::kSignPos | util::fmt_flags::kUpperCase) == "-0");
    VERIFY(util::to_string<double>(-0., util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, -1, 15, ' ') ==
           "-00000000000000");
    VERIFY(util::to_string<double>(
               -0., util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
               ' ') == "-00000000000000");

    VERIFY(util::to_string<double>(-0., util::fmt_flags::kSignAlign) == "-0");
    VERIFY(util::to_string<double>(-0., util::fmt_flags::kSignAlign | util::fmt_flags::kUpperCase) == "-0");
    VERIFY(util::to_string<double>(-0., util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes, -1, 15, ' ') ==
           "-00000000000000");
    VERIFY(util::to_string<double>(
               -0., util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
               ' ') == "-00000000000000");

    VERIFY(util::to_string<double>(std::numeric_limits<double>::infinity()) == "inf");
    VERIFY(util::to_string<double>(std::numeric_limits<double>::infinity(), util::fmt_flags::kUpperCase) == "INF");
    VERIFY(util::to_string<double>(std::numeric_limits<double>::infinity(), util::fmt_flags::kLeadingZeroes, -1, 15,
                                   ' ') == "            inf");
    VERIFY(util::to_string<double>(std::numeric_limits<double>::infinity(),
                                   util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
                                   ' ') == "            INF");

    VERIFY(util::to_string<double>(std::numeric_limits<double>::infinity(), util::fmt_flags::kSignPos) == "+inf");
    VERIFY(util::to_string<double>(std::numeric_limits<double>::infinity(),
                                   util::fmt_flags::kSignPos | util::fmt_flags::kUpperCase) == "+INF");
    VERIFY(util::to_string<double>(std::numeric_limits<double>::infinity(),
                                   util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, -1, 15,
                                   ' ') == "           +inf");
    VERIFY(util::to_string<double>(
               std::numeric_limits<double>::infinity(),
               util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
               ' ') == "           +INF");

    VERIFY(util::to_string<double>(std::numeric_limits<double>::infinity(), util::fmt_flags::kSignAlign) == " inf");
    VERIFY(util::to_string<double>(std::numeric_limits<double>::infinity(),
                                   util::fmt_flags::kSignAlign | util::fmt_flags::kUpperCase) == " INF");
    VERIFY(util::to_string<double>(std::numeric_limits<double>::infinity(),
                                   util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes, -1, 15,
                                   ' ') == "            inf");
    VERIFY(util::to_string<double>(
               std::numeric_limits<double>::infinity(),
               util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
               ' ') == "            INF");

    VERIFY(util::to_string<double>(-std::numeric_limits<double>::infinity()) == "-inf");
    VERIFY(util::to_string<double>(-std::numeric_limits<double>::infinity(), util::fmt_flags::kUpperCase) == "-INF");
    VERIFY(util::to_string<double>(-std::numeric_limits<double>::infinity(), util::fmt_flags::kLeadingZeroes, -1, 15,
                                   ' ') == "           -inf");
    VERIFY(util::to_string<double>(-std::numeric_limits<double>::infinity(),
                                   util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
                                   ' ') == "           -INF");

    VERIFY(util::to_string<double>(-std::numeric_limits<double>::infinity(), util::fmt_flags::kSignPos) == "-inf");
    VERIFY(util::to_string<double>(-std::numeric_limits<double>::infinity(),
                                   util::fmt_flags::kSignPos | util::fmt_flags::kUpperCase) == "-INF");
    VERIFY(util::to_string<double>(-std::numeric_limits<double>::infinity(),
                                   util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, -1, 15,
                                   ' ') == "           -inf");
    VERIFY(util::to_string<double>(
               -std::numeric_limits<double>::infinity(),
               util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
               ' ') == "           -INF");

    VERIFY(util::to_string<double>(-std::numeric_limits<double>::infinity(), util::fmt_flags::kSignAlign) == "-inf");
    VERIFY(util::to_string<double>(-std::numeric_limits<double>::infinity(),
                                   util::fmt_flags::kSignAlign | util::fmt_flags::kUpperCase) == "-INF");
    VERIFY(util::to_string<double>(-std::numeric_limits<double>::infinity(),
                                   util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes, -1, 15,
                                   ' ') == "           -inf");
    VERIFY(util::to_string<double>(
               -std::numeric_limits<double>::infinity(),
               util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
               ' ') == "           -INF");

    VERIFY(util::to_string<double>(std::numeric_limits<double>::quiet_NaN()) == "nan");
    VERIFY(util::to_string<double>(std::numeric_limits<double>::quiet_NaN(), util::fmt_flags::kUpperCase) == "NAN");
    VERIFY(util::to_string<double>(std::numeric_limits<double>::quiet_NaN(), util::fmt_flags::kLeadingZeroes, -1, 15,
                                   ' ') == "            nan");
    VERIFY(util::to_string<double>(std::numeric_limits<double>::quiet_NaN(),
                                   util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
                                   ' ') == "            NAN");

    VERIFY(util::to_string<double>(std::numeric_limits<double>::quiet_NaN(), util::fmt_flags::kSignPos) == "+nan");
    VERIFY(util::to_string<double>(std::numeric_limits<double>::quiet_NaN(),
                                   util::fmt_flags::kSignPos | util::fmt_flags::kUpperCase) == "+NAN");
    VERIFY(util::to_string<double>(std::numeric_limits<double>::quiet_NaN(),
                                   util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, -1, 15,
                                   ' ') == "           +nan");
    VERIFY(util::to_string<double>(
               std::numeric_limits<double>::quiet_NaN(),
               util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
               ' ') == "           +NAN");

    VERIFY(util::to_string<double>(std::numeric_limits<double>::quiet_NaN(), util::fmt_flags::kSignAlign) == " nan");
    VERIFY(util::to_string<double>(std::numeric_limits<double>::quiet_NaN(),
                                   util::fmt_flags::kSignAlign | util::fmt_flags::kUpperCase) == " NAN");
    VERIFY(util::to_string<double>(std::numeric_limits<double>::quiet_NaN(),
                                   util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes, -1, 15,
                                   ' ') == "            nan");
    VERIFY(util::to_string<double>(
               std::numeric_limits<double>::quiet_NaN(),
               util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
               ' ') == "            NAN");

    VERIFY(util::to_string<double>(-std::numeric_limits<double>::quiet_NaN()) == "-nan");
    VERIFY(util::to_string<double>(-std::numeric_limits<double>::quiet_NaN(), util::fmt_flags::kUpperCase) == "-NAN");
    VERIFY(util::to_string<double>(-std::numeric_limits<double>::quiet_NaN(), util::fmt_flags::kLeadingZeroes, -1, 15,
                                   ' ') == "           -nan");
    VERIFY(util::to_string<double>(-std::numeric_limits<double>::quiet_NaN(),
                                   util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
                                   ' ') == "           -NAN");

    VERIFY(util::to_string<double>(-std::numeric_limits<double>::quiet_NaN(), util::fmt_flags::kSignPos) == "-nan");
    VERIFY(util::to_string<double>(-std::numeric_limits<double>::quiet_NaN(),
                                   util::fmt_flags::kSignPos | util::fmt_flags::kUpperCase) == "-NAN");
    VERIFY(util::to_string<double>(-std::numeric_limits<double>::quiet_NaN(),
                                   util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes, -1, 15,
                                   ' ') == "           -nan");
    VERIFY(util::to_string<double>(
               -std::numeric_limits<double>::quiet_NaN(),
               util::fmt_flags::kSignPos | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
               ' ') == "           -NAN");

    VERIFY(util::to_string<double>(-std::numeric_limits<double>::quiet_NaN(), util::fmt_flags::kSignAlign) == "-nan");
    VERIFY(util::to_string<double>(-std::numeric_limits<double>::quiet_NaN(),
                                   util::fmt_flags::kSignAlign | util::fmt_flags::kUpperCase) == "-NAN");
    VERIFY(util::to_string<double>(-std::numeric_limits<double>::quiet_NaN(),
                                   util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes, -1, 15,
                                   ' ') == "           -nan");
    VERIFY(util::to_string<double>(
               -std::numeric_limits<double>::quiet_NaN(),
               util::fmt_flags::kSignAlign | util::fmt_flags::kLeadingZeroes | util::fmt_flags::kUpperCase, -1, 15,
               ' ') == "           -NAN");

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
        auto result = std::to_chars(s_ref.data(), s_ref.data() + s_ref.size(), val);
        *result.ptr = '\0';
#else
        int result = sprintf(s_ref.data(), "%.lu", val);
        s_ref[result] = '\0';
#endif

        VERIFY(std::strcmp(s.data(), s_ref.data()) == 0);

        uint64_t val1 = 0, val2 = 0;
        VERIFY(util::string_converter<uint64_t>::from_string(s.data(), last, val1) == last);
#if defined(_MSC_VER) && __cplusplus >= 201703L
        std::from_chars(s.data(), last, val2);
#else
        sscanf(s.data(), "%lu", &val2);
#endif
        VERIFY(val1 == val2);
    }
}

void string_test_1(int iter_count) {
    std::array<char, 128> s;
    std::array<char, 128> s_ref;
    std::default_random_engine generator;
    std::uniform_int_distribution<uint64_t> distribution(4, (1ull << 52) - 2);

    int N_err = 1;

    for (int iter = 0, perc0 = -1; iter < iter_count; ++iter) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            std::cout << std::setw(3) << (perc / 10) << "." << std::setw(0) << (perc % 10) << "%\b\b\b\b\b\b"
                      << std::flush;
            perc0 = perc;
        }

        for (int k = 0; k <= 140; ++k) {
            int exp = 1023 - 70 + k;
            uint64_t mantissa = 0;
            if (iter > 0) {
                if (iter <= 52) {
                    mantissa = 1ull << (iter - 1);
                } else if (iter <= 103) {
                    mantissa = ((1ull << 52) - 1) >> (iter - 53);
                } else {
                    mantissa = distribution(generator);
                }
            }
            uint64_t uval = mantissa | (static_cast<uint64_t>(exp) << 52);
            auto val = *reinterpret_cast<double*>(&uval);

            for (int prec = 19; prec >= 0; --prec) {
                char* last = util::format_to(s.data(), "{:f}", util::setprec(prec), val);
                *last = '\0';
                int n_digs = static_cast<int>(last - s.data());
                if (s[0] == '0') {
                    n_digs = 0;
                } else {
                    char* pos = std::find(s.data(), last, '.');
                    if (pos != last) { n_digs = static_cast<int>(pos - s.data()); }
                }

                n_digs += prec;

                if (n_digs <= 19) {
#if defined(_MSC_VER) && __cplusplus >= 201703L
                    auto result = std::to_chars(s_ref.data(), s_ref.data() + s_ref.size(), val,
                                                std::chars_format::fixed, prec);
                    *result.ptr = '\0';
#else
                    int result = sprintf(s_ref.data(), "%.*lf", prec, val);
                    s_ref[result] = '\0';
#endif

                    if (std::strcmp(s.data(), s_ref.data()) != 0) {
                        auto u64 = *reinterpret_cast<uint64_t*>(&val);
                        int exp = static_cast<int>((u64 >> 52) & 2047) - 1023;
                        u64 &= (1ull << 52) - 1;
                        std::cout << std::endl << "k = " << k << " iter = " << iter << " prec = " << prec << std::endl;
                        std::cout << s.data() << std::endl << s_ref.data() << std::endl;
                        std::cout << "mantissa = " << u64 << std::endl;
                        std::cout << "exp = " << exp << std::endl;
                        VERIFY(--N_err > 0);
                    }

                    double val1 = 0, val2 = 0;
                    VERIFY(util::string_converter<double>::from_string(s.data(), last, val1) == last);
#if defined(_MSC_VER) && __cplusplus >= 201703L
                    std::from_chars(s.data(), last, val2);
#else
                    sscanf(s.data(), "%lf", &val2);
#endif
                    if (val1 != val2) {
                        std::cout << std::endl << s.data() << std::endl;
                        printf("%.*lf\n", prec, val1);
                        printf("%.*lf\n", prec, val2);
                        VERIFY(--N_err > 0);
                    }
                }
            }
        }
    }
}

void string_test_2(bool general, int iter_count) {
    std::array<char, 128> s;
    std::array<char, 128> s_ref;
    std::default_random_engine generator;
    std::uniform_int_distribution<uint64_t> distribution(4, (1ull << 52) - 2);

    int N_err = 1;

    for (int iter = 0, perc0 = -1; iter < iter_count; ++iter) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            std::cout << std::setw(3) << (perc / 10) << "." << std::setw(0) << (perc % 10) << "%\b\b\b\b\b\b"
                      << std::flush;
            perc0 = perc;
        }

        for (int k = 0; k <= 2046; ++k) {
            int exp = k;
            uint64_t mantissa = 0;
            if (iter > 0) {
                if (iter <= 52) {
                    mantissa = 1ull << (iter - 1);
                } else if (iter <= 103) {
                    mantissa = ((1ull << 52) - 1) >> (iter - 53);
                } else {
                    mantissa = distribution(generator);
                }
            }

            mantissa = 0;
            exp = 1023 + -1000;

            uint64_t uval = mantissa | (static_cast<uint64_t>(exp) << 52);
            auto val = *reinterpret_cast<double*>(&uval);

            for (int prec = general ? 19 : 18; prec >= 1; --prec) {
                char* last = util::format_to(s.data(), general ? "{}" : "{:e}", util::setprec(prec), val);
                *last = '\0';
#if defined(_MSC_VER) && __cplusplus >= 201703L
                auto result = std::to_chars(s_ref.data(), s_ref.data() + s_ref.size(), val,
                                            general ? std::chars_format::general : std::chars_format::scientific, prec);
                *result.ptr = '\0';
#else
                int result = sprintf(s_ref.data(), general ? "%.*lg" : "%.*le", prec, val);
                s_ref[result] = '\0';
#endif
                if (std::strcmp(s.data(), s_ref.data()) != 0) {
                    auto u64 = *reinterpret_cast<uint64_t*>(&val);
                    int exp = static_cast<int>((u64 >> 52) & 2047) - 1023;
                    u64 &= (1ull << 52) - 1;
                    std::cout << std::endl << "k = " << k << " iter = " << iter << " prec = " << prec << std::endl;
                    std::cout << s.data() << std::endl << s_ref.data() << std::endl;
                    std::cout << "mantissa = " << u64 << std::endl;
                    std::cout << "exp = " << exp << std::endl;
                    VERIFY(--N_err > 0);
                }

                double val1 = 0, val2 = 0;
                VERIFY(util::string_converter<double>::from_string(s.data(), last, val1) == last);
#if defined(_MSC_VER) && __cplusplus >= 201703L
                std::from_chars(s.data(), last, val2);
#else
                sscanf(s.data(), "%lf", &val2);
#endif
                if (val1 != val2 || (prec >= 18 && val1 != val)) {
                    std::cout << std::endl << s.data() << std::endl;
                    printf(general ? "%.*lg\n" : "%.*le\n", prec, val1);
                    printf(general ? "%.*lg\n" : "%.*le\n", prec, val2);
                    VERIFY(--N_err > 0);
                }
            }
        }
    }
}

#if defined(NDEBUG)
const int brute_N = 20000;
#else   // defined(NDEBUG)
const int brute_N = 200;
#endif  // defined(NDEBUG)

int test_bruteforce0() {
    string_test_0(25000 * brute_N);
    return 0;
}
int test_bruteforce1() {
    string_test_1(25 * brute_N);
    return 0;
}
int test_bruteforce2() {
    string_test_2(false, brute_N);
    return 0;
}
int test_bruteforce3() {
    string_test_2(true, brute_N);
    return 0;
}

// --------------------------------------------

int perf_integer(int iter_count) {
    std::array<char, 128> s;
    std::default_random_engine generator;
    std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());

    uint64_t eps = 0;

    auto start = std::clock();
    for (int iter = 0; iter < iter_count; ++iter) {
        auto val = distribution(generator);
        char* last = util::format_to(s.data(), "{}", val);
        auto val1 = util::from_string<uint64_t>(std::string_view(s.data(), last - s.data()));
        eps += val - val1;
    }

    return eps == 0 ? static_cast<int>(std::clock() - start) : 0;
}

int perf_integer_libc(int iter_count) {
    std::array<char, 128> s;
    std::default_random_engine generator;
    std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());

    uint64_t eps = 0;

    auto start = std::clock();
    for (int iter = 0; iter < iter_count; ++iter) {
        auto val = distribution(generator);

#if defined(_MSC_VER) && __cplusplus >= 201703L
        auto result = std::to_chars(s.data(), s.data() + s.size(), val);
        *result.ptr = '\0';
#else
        int result = sprintf(s.data(), "%.lu", val);
        s[result] = '\0';
#endif

        uint64_t val1 = 0;
#if defined(_MSC_VER) && __cplusplus >= 201703L
        std::from_chars(s.data(), result.ptr, val1);
#else
        sscanf(s.data(), "%lu", &val1);
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

    double eps = 0;

    auto start = std::clock();
    for (int iter = 0; iter < iter_count; ++iter) {
        auto val = distribution(generator) * std::pow(10, exp_distribution(generator));
        char* last = util::format_to(s.data(), "{:.18}", val);
        auto val1 = util::from_string<double>(std::string_view(s.data(), last - s.data()));
        eps = std::max(std::fabs((val - val1) / val), eps);
    }

    return eps == 0 ? static_cast<int>(std::clock() - start) : 0;
}

int perf_float_libc(int iter_count) {
    std::array<char, 128> s;
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(1., 10.);
    std::uniform_int_distribution<int> exp_distribution(-324, 308);

    double eps = 0;
    const int prec = 18;

    auto start = std::clock();
    for (int iter = 0; iter < iter_count; ++iter) {
        auto val = distribution(generator) * std::pow(10, exp_distribution(generator));

#if defined(_MSC_VER) && __cplusplus >= 201703L
        auto result = std::to_chars(s.data(), s.data() + s.size(), val, std::chars_format::general, prec);
        *result.ptr = '\0';
#else
        int result = sprintf(s.data(), "%.*lg", prec, val);
        s[result] = '\0';
#endif

        double val1 = 0;
#if defined(_MSC_VER) && __cplusplus >= 201703L
        std::from_chars(s.data(), result.ptr, val1);
#else
        sscanf(s.data(), "%lf", &val1);
#endif
        eps = std::max(std::fabs((val - val1) / val), eps);
    }

    return eps == 0 ? static_cast<int>(std::clock() - start) : 0;
}

const int perf_N = 2000000;
int test_integer_perf() { return perf_integer(2 * perf_N); }
int test_integer_perf_libc() { return perf_integer_libc(2 * perf_N); }
int test_float_perf() { return perf_float(perf_N); }
int test_float_perf_libc() { return perf_float_libc(perf_N); }

}  // namespace

ADD_TEST_CASE("", "string convertion", test_string_cvt_0);
ADD_TEST_CASE("", "string convertion", test_string_cvt_1);
ADD_TEST_CASE("", "string convertion", test_string_cvt_1u);
ADD_TEST_CASE("", "string convertion", test_string_cvt_2);
ADD_TEST_CASE("", "string convertion", test_string_cvt_3);
ADD_TEST_CASE("", "string convertion", test_string_cvt_4);

// ADD_TEST_CASE("1-bruteforce", "string convertion", test_bruteforce0);
ADD_TEST_CASE("1-bruteforce", "string convertion", test_bruteforce1);
ADD_TEST_CASE("1-bruteforce", "string convertion", test_bruteforce2);
ADD_TEST_CASE("1-bruteforce", "string convertion", test_bruteforce3);

ADD_TEST_CASE("2-perf", "string integer convertion", test_integer_perf);
ADD_TEST_CASE("2-perf", "<libc> string integer convertion", test_integer_perf_libc);
ADD_TEST_CASE("2-perf", "string float convertion", test_float_perf);
ADD_TEST_CASE("2-perf", "<libc> string float convertion", test_float_perf_libc);
