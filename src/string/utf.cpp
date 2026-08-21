#include "test_suite.h"

#include <uxs/utf.h>

namespace {

template<typename InputIt>
bool check_result(const uxs::from_utf_result<InputIt>& result, InputIt iter, uxs::utf_errc ec) {
    return result.iter == iter && result.ec == ec;
}

int test_string_utf_0() {
    char ch[4];
    uint32_t code = 0;

    VERIFY(uxs::to_utf8(0x7e, ch).count == 1);
    VERIFY(ch[0] == '\x7E');
    VERIFY(check_result(uxs::from_utf8(ch, (char*)ch, code), (char*)ch, uxs::utf_errc::empty));
    VERIFY(check_result(uxs::from_utf8(ch, (char*)ch + 1, code), (char*)ch + 1, uxs::utf_errc::wellformed));
    VERIFY(code == 0x7e);

    ch[0] = '\xBF';
    ch[1] = '\x80';
    VERIFY(check_result(uxs::from_utf8(ch, (char*)ch + 2, code), (char*)ch + 1, uxs::utf_errc::invalid));

    ch[0] = '\xF8';
    ch[1] = '\x80';
    VERIFY(check_result(uxs::from_utf8(ch, (char*)ch + 2, code), (char*)ch + 1, uxs::utf_errc::invalid));

    ch[0] = '\xDE';
    ch[1] = '\x7F';
    VERIFY(check_result(uxs::from_utf8(ch, (char*)ch + 2, code), (char*)ch + 1, uxs::utf_errc::invalid));

    ch[0] = '\xDE';
    ch[1] = '\xC0';
    VERIFY(check_result(uxs::from_utf8(ch, (char*)ch + 2, code), (char*)ch + 1, uxs::utf_errc::invalid));

    ch[0] = '\xF7';
    ch[1] = '\xBF';
    ch[2] = '\xBF';
    ch[3] = '\xBF';
    VERIFY(check_result(uxs::from_utf8(ch, (char*)ch + 4, code), (char*)ch + 1, uxs::utf_errc::invalid));

    ch[0] = '\xED';
    ch[1] = '\xA0';
    ch[2] = '\x80';
    VERIFY(check_result(uxs::from_utf8(ch, (char*)ch + 3, code), (char*)ch + 1, uxs::utf_errc::invalid));

    VERIFY(uxs::to_utf8(0x7bc, ch).count == 2);
    VERIFY(ch[0] == '\xDE');
    VERIFY(ch[1] == '\xBC');
    VERIFY(check_result(uxs::from_utf8(ch, (char*)ch + 1, code), (char*)ch + 1, uxs::utf_errc::invalid));
    VERIFY(check_result(uxs::from_utf8(ch, (char*)ch + 2, code), (char*)ch + 2, uxs::utf_errc::wellformed));
    VERIFY(code == 0x7bc);

    VERIFY(uxs::to_utf8(0xef38, ch).count == 3);
    VERIFY(ch[0] == '\xEE');
    VERIFY(ch[1] == '\xBC');
    VERIFY(ch[2] == '\xB8');
    VERIFY(check_result(uxs::from_utf8(ch, (char*)ch + 2, code), (char*)ch + 1, uxs::utf_errc::invalid));
    VERIFY(check_result(uxs::from_utf8(ch, (char*)ch + 3, code), (char*)ch + 3, uxs::utf_errc::wellformed));
    VERIFY(code == 0xef38);

    VERIFY(uxs::to_utf8(0x10ef38, ch).count == 4);
    VERIFY(ch[0] == '\xF4');
    VERIFY(ch[1] == '\x8E');
    VERIFY(ch[2] == '\xBC');
    VERIFY(ch[3] == '\xB8');
    VERIFY(check_result(uxs::from_utf8(ch, (char*)ch + 3, code), (char*)ch + 1, uxs::utf_errc::invalid));
    VERIFY(check_result(uxs::from_utf8(ch, (char*)ch + 4, code), (char*)ch + 4, uxs::utf_errc::wellformed));
    VERIFY(code == 0x10ef38);

    VERIFY(uxs::to_utf8(0x110000, ch).count == 3);
    VERIFY(ch[0] == '\xEF');
    VERIFY(ch[1] == '\xBF');
    VERIFY(ch[2] == '\xBD');
    VERIFY(check_result(uxs::from_utf8(ch, (char*)ch + 3, code), (char*)ch + 3, uxs::utf_errc::wellformed));
    VERIFY(code == 0xfffd);
    return 0;
}

int test_string_utf_1() {
    wchar_t ch[2];
    uint32_t code = 0;

    VERIFY(uxs::to_utf16(0xD7FE, ch).count == 1);
    VERIFY(ch[0] == static_cast<wchar_t>(0xd7fe));
    VERIFY(check_result(uxs::from_utf16(ch, (wchar_t*)ch, code), (wchar_t*)ch, uxs::utf_errc::empty));
    VERIFY(check_result(uxs::from_utf16(ch, (wchar_t*)ch + 1, code), (wchar_t*)ch + 1, uxs::utf_errc::wellformed));
    VERIFY(code == 0xd7fe);

    ch[0] = 0xDC00;
    ch[1] = 0xD800;
    VERIFY(check_result(uxs::from_utf16(ch, (wchar_t*)ch + 2, code), (wchar_t*)ch + 1, uxs::utf_errc::invalid));

    ch[0] = 0xD800;
    ch[1] = 0xE800;
    VERIFY(check_result(uxs::from_utf16(ch, (wchar_t*)ch + 2, code), (wchar_t*)ch + 1, uxs::utf_errc::invalid));

    VERIFY(uxs::to_utf16(0xeeee, ch).count == 1);
    VERIFY(ch[0] == static_cast<wchar_t>(0xeeee));
    VERIFY(check_result(uxs::from_utf16(ch, (wchar_t*)ch + 1, code), (wchar_t*)ch + 1, uxs::utf_errc::wellformed));
    VERIFY(code == 0xeeee);

    VERIFY(uxs::to_utf16(0xdcfe, ch).count == 1);
    VERIFY(ch[0] == static_cast<wchar_t>(0xfffd));
    VERIFY(check_result(uxs::from_utf16(ch, (wchar_t*)ch + 1, code), (wchar_t*)ch + 1, uxs::utf_errc::wellformed));
    VERIFY(code == 0xfffd);

    VERIFY(uxs::to_utf16(0x110000, ch).count == 1);
    VERIFY(ch[0] == static_cast<wchar_t>(0xfffd));
    VERIFY(check_result(uxs::from_utf16(ch, (wchar_t*)ch + 1, code), (wchar_t*)ch + 1, uxs::utf_errc::wellformed));
    VERIFY(code == 0xfffd);

    VERIFY(uxs::to_utf16(0x10fbfc, ch).count == 2);
    VERIFY(ch[0] == static_cast<wchar_t>(0xdbfe));
    VERIFY(ch[1] == static_cast<wchar_t>(0xdffc));
    VERIFY(check_result(uxs::from_utf16(ch, (wchar_t*)ch + 1, code), (wchar_t*)ch + 1, uxs::utf_errc::invalid));
    VERIFY(check_result(uxs::from_utf16(ch, (wchar_t*)ch + 2, code), (wchar_t*)ch + 2, uxs::utf_errc::wellformed));
    VERIFY(code == 0x10fbfc);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "string utf conversion", test_string_utf_0);
ADD_TEST_CASE("", "string utf conversion", test_string_utf_1);
