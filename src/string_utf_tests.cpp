#include "uxs/utf.h"

#include "test_suite.h"

namespace {

int test_string_utf_0() {
    char ch[4];
    uint32_t code = 0;
    char* p;

    VERIFY(uxs::to_utf8(0x7e, ch) == 1);
    VERIFY(ch[0] == '\x7E');
    VERIFY(uxs::from_utf8(ch, (char*)ch, p, code) == 0 && p == (char*)ch);
    VERIFY(uxs::from_utf8(ch, (char*)ch + 1, p, code) == 1 && p == (char*)ch + 1);
    VERIFY(code == 0x7e);

    VERIFY(uxs::to_utf8(0x7bc, ch) == 2);
    VERIFY(ch[0] == '\xDE');
    VERIFY(ch[1] == '\xBC');
    VERIFY(uxs::from_utf8(ch, (char*)ch + 1, p, code) == 0 && p == (char*)ch);
    VERIFY(uxs::from_utf8(ch, (char*)ch + 2, p, code) == 2 && p == (char*)ch + 2);
    VERIFY(code == 0x7bc);

    VERIFY(uxs::to_utf8(0xef38, ch) == 3);
    VERIFY(ch[0] == '\xEE');
    VERIFY(ch[1] == '\xBC');
    VERIFY(ch[2] == '\xB8');
    VERIFY(uxs::from_utf8(ch, (char*)ch + 2, p, code) == 0 && p == (char*)ch);
    VERIFY(uxs::from_utf8(ch, (char*)ch + 3, p, code) == 3 && p == (char*)ch + 3);
    VERIFY(code == 0xef38);

    VERIFY(uxs::to_utf8(0x10ef38, ch) == 4);
    VERIFY(ch[0] == '\xF4');
    VERIFY(ch[1] == '\x8E');
    VERIFY(ch[2] == '\xBC');
    VERIFY(ch[3] == '\xB8');
    VERIFY(uxs::from_utf8(ch, (char*)ch + 3, p, code) == 0 && p == (char*)ch);
    VERIFY(uxs::from_utf8(ch, (char*)ch + 4, p, code) == 4 && p == (char*)ch + 4);
    VERIFY(code == 0x10ef38);

    VERIFY(uxs::to_utf8(0x110000, ch) == 3);
    VERIFY(ch[0] == '\xEF');
    VERIFY(ch[1] == '\xBF');
    VERIFY(ch[2] == '\xBD');
    VERIFY(uxs::from_utf8(ch, (char*)ch + 3, p, code) == 3 && p == (char*)ch + 3);
    VERIFY(code == 0xfffd);
    return 0;
}

int test_string_utf_1() {
    wchar_t ch[2];
    uint32_t code = 0;
    wchar_t* p;

    VERIFY(uxs::to_utf16(0xD7FE, ch) == 1);
    VERIFY(ch[0] == static_cast<wchar_t>(0xd7fe));
    VERIFY(uxs::from_utf16(ch, (wchar_t*)ch, p, code) == 0 && p == (wchar_t*)ch);
    VERIFY(uxs::from_utf16(ch, (wchar_t*)ch + 1, p, code) == 1 && p == (wchar_t*)ch + 1);
    VERIFY(code == 0xd7fe);

    VERIFY(uxs::to_utf16(0xeeee, ch) == 1);
    VERIFY(ch[0] == static_cast<wchar_t>(0xeeee));
    VERIFY(uxs::from_utf16(ch, (wchar_t*)ch + 1, p, code) == 1 && p == (wchar_t*)ch + 1);
    VERIFY(code == 0xeeee);

    VERIFY(uxs::to_utf16(0xdcfe, ch) == 1);
    VERIFY(ch[0] == static_cast<wchar_t>(0xfffd));
    VERIFY(uxs::from_utf16(ch, (wchar_t*)ch + 1, p, code) == 1 && p == (wchar_t*)ch + 1);
    VERIFY(code == 0xfffd);

    VERIFY(uxs::to_utf16(0x110000, ch) == 1);
    VERIFY(ch[0] == static_cast<wchar_t>(0xfffd));
    VERIFY(uxs::from_utf16(ch, (wchar_t*)ch + 1, p, code) == 1 && p == (wchar_t*)ch + 1);
    VERIFY(code == 0xfffd);

    VERIFY(uxs::to_utf16(0x10fbfc, ch) == 2);
    VERIFY(ch[0] == static_cast<wchar_t>(0xdbfe));
    VERIFY(ch[1] == static_cast<wchar_t>(0xdffc));
    VERIFY(uxs::from_utf16(ch, (wchar_t*)ch + 1, p, code) == 0 && p == (wchar_t*)ch);
    VERIFY(uxs::from_utf16(ch, (wchar_t*)ch + 2, p, code) == 2 && p == (wchar_t*)ch + 2);
    VERIFY(code == 0x10fbfc);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "string utf conversion", test_string_utf_0);
ADD_TEST_CASE("", "string utf conversion", test_string_utf_1);
