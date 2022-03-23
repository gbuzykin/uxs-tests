#include "test_suite.h"
#include "util/utf.h"

namespace {

int test_string_utf_0() {
    char ch[4];
    uint32_t code = 0;

    VERIFY(util::to_utf8(0x7E, ch) == 1);
    VERIFY(ch[0] == '\x7E');
    VERIFY(util::from_utf8(ch, (char*)ch, &code) == (char*)ch);
    VERIFY(util::from_utf8(ch, (char*)ch + 1, &code) == (char*)ch + 1);
    VERIFY(code == 0x7E);

    VERIFY(util::to_utf8(0x7BC, ch) == 2);
    VERIFY(ch[0] == '\xDE');
    VERIFY(ch[1] == '\xBC');
    VERIFY(util::from_utf8(ch, (char*)ch + 1, &code) == (char*)ch);
    VERIFY(util::from_utf8(ch, (char*)ch + 2, &code) == (char*)ch + 2);
    VERIFY(code == 0x7BC);

    VERIFY(util::to_utf8(0xEF38, ch) == 3);
    VERIFY(ch[0] == '\xEE');
    VERIFY(ch[1] == '\xBC');
    VERIFY(ch[2] == '\xB8');
    VERIFY(util::from_utf8(ch, (char*)ch + 2, &code) == (char*)ch);
    VERIFY(util::from_utf8(ch, (char*)ch + 3, &code) == (char*)ch + 3);
    VERIFY(code == 0xEF38);

    VERIFY(util::to_utf8(0x10EF38, ch) == 4);
    VERIFY(ch[0] == '\xF4');
    VERIFY(ch[1] == '\x8E');
    VERIFY(ch[2] == '\xBC');
    VERIFY(ch[3] == '\xB8');
    VERIFY(util::from_utf8(ch, (char*)ch + 3, &code) == (char*)ch);
    VERIFY(util::from_utf8(ch, (char*)ch + 4, &code) == (char*)ch + 4);
    VERIFY(code == 0x10EF38);

    VERIFY(util::to_utf8(0x110000, ch) == 3);
    VERIFY(ch[0] == '\xEF');
    VERIFY(ch[1] == '\xBF');
    VERIFY(ch[2] == '\xBD');
    VERIFY(util::from_utf8(ch, (char*)ch + 3, &code) == (char*)ch + 3);
    VERIFY(code == 0xFFFD);
    return 0;
}

int test_string_utf_1() {
    wchar_t ch[2];
    uint32_t code = 0;

    VERIFY(util::to_utf16(0xD7FE, ch) == 1);
    VERIFY(ch[0] == static_cast<wchar_t>(0xD7FE));
    VERIFY(util::from_utf16(ch, (wchar_t*)ch, &code) == (wchar_t*)ch);
    VERIFY(util::from_utf16(ch, (wchar_t*)ch + 1, &code) == (wchar_t*)ch + 1);
    VERIFY(code == 0xD7FE);

    VERIFY(util::to_utf16(0xEEEE, ch) == 1);
    VERIFY(ch[0] == static_cast<wchar_t>(0xEEEE));
    VERIFY(util::from_utf16(ch, (wchar_t*)ch + 1, &code) == (wchar_t*)ch + 1);
    VERIFY(code == 0xEEEE);

    VERIFY(util::to_utf16(0xDCFE, ch) == 1);
    VERIFY(ch[0] == static_cast<wchar_t>(0xFFFD));
    VERIFY(util::from_utf16(ch, (wchar_t*)ch + 1, &code) == (wchar_t*)ch + 1);
    VERIFY(code == 0xFFFD);

    VERIFY(util::to_utf16(0x110000, ch) == 1);
    VERIFY(ch[0] == static_cast<wchar_t>(0xFFFD));
    VERIFY(util::from_utf16(ch, (wchar_t*)ch + 1, &code) == (wchar_t*)ch + 1);
    VERIFY(code == 0xFFFD);

    VERIFY(util::to_utf16(0x10FBFC, ch) == 2);
    VERIFY(ch[0] == static_cast<wchar_t>(0xDBFE));
    VERIFY(ch[1] == static_cast<wchar_t>(0xDFFC));
    VERIFY(util::from_utf16(ch, (wchar_t*)ch + 1, &code) == (wchar_t*)ch);
    VERIFY(util::from_utf16(ch, (wchar_t*)ch + 2, &code) == (wchar_t*)ch + 2);
    VERIFY(code == 0x10FBFC);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "string utf conversion", test_string_utf_0);
ADD_TEST_CASE("", "string utf conversion", test_string_utf_1);
