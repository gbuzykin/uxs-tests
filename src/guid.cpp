#include "test_suite.h"

#include <uxs/guid.h>

namespace {

template<typename StrLikeTy, typename = std::enable_if_t<uxs::is_string_like<StrLikeTy>::value>>
UXS_CONSTEXPR uxs::guid guid_from_per_byte_string(const StrLikeTy& s) {
    uxs::guid id;
    if (id.from_per_byte_string(s).ec != uxs::sconv_errc::ok) { throw std::runtime_error("invalid guid string"); }
    return id;
}

template<typename StrLikeTy, typename = std::enable_if_t<uxs::is_string_like<StrLikeTy>::value>>
UXS_CONSTEXPR uxs::guid guid_from_string(const StrLikeTy& s) {
    uxs::guid id;
    if (uxs::from_string_v(s, id).ec != uxs::sconv_errc::ok) { throw std::runtime_error("invalid guid string"); }
    return id;
}

int test_guid_1() {
    for (int i = 0; i < 1000; ++i) {
        const uxs::guid id = uxs::guid::generate();

        VERIFY((id.layout.w[1] >> 12) == 4 && (id.layout.b[0] >> 6) == 2);
        {
            std::string s_id = uxs::to_string(id, uxs::fmt_opts{uxs::fmt_flags::uppercase});
            VERIFY(s_id[15] == '4' && (s_id[20] == '8' || s_id[20] == '9' || s_id[20] == 'A' || s_id[20] == 'B'));
            VERIFY(guid_from_string(s_id) == id);
        }
        {
            std::string s_id = id.to_per_byte_string();
            VERIFY(s_id[14] == '4' && (s_id[16] == '8' || s_id[16] == '9' || s_id[16] == 'A' || s_id[16] == 'B'));
            VERIFY(guid_from_per_byte_string(s_id) == id);
        }

        const auto data8 = id.data8;
        const auto data64 = id.data64;

        VERIFY(uxs::guid(data8) == id);
        VERIFY(uxs::guid(data64) == id);
    }

    const uxs::guid id(0x17364152, 0x36b4, 0x4b3e, 0x81, 0xba, 0x5e, 0x79, 0xa6, 0x81, 0xba, 0xee);
    VERIFY(uxs::to_string(id, uxs::fmt_opts{uxs::fmt_flags::uppercase}) == "{17364152-36B4-4B3E-81BA-5E79A681BAEE}");
    VERIFY(id.to_per_byte_string() == "52413617B4363E4B81BA5E79A681BAEE");
    VERIFY(id.layout.l == 0x17364152 && id.layout.w[0] == 0x36b4 && id.layout.w[1] == 0x4b3e);
    VERIFY(id.layout.b[0] == 0x81 && id.layout.b[1] == 0xba && id.layout.b[2] == 0x5e && id.layout.b[3] == 0x79);
    VERIFY(id.layout.b[4] == 0xa6 && id.layout.b[5] == 0x81 && id.layout.b[6] == 0xba && id.layout.b[7] == 0xee);
    return 0;
}

int test_guid_2() {
#if __cplusplus >= 201703L
    constexpr uxs::guid id0;
    constexpr bool is_valid = id0.valid() && id0 == uxs::guid();
    VERIFY(!is_valid);

    constexpr uxs::guid id1(uxs::guid::data64_t{0, 0});
    VERIFY(id1 == uxs::guid());

    constexpr uxs::guid id2(0x17364152, 0x36b4, 0x4b3e, 0x81, 0xba, 0x5e, 0x79, 0xa6, 0x81, 0xba, 0xee);
    VERIFY(id2.to_per_byte_string() == "52413617B4363E4B81BA5E79A681BAEE");

    constexpr uxs::guid id3 = guid_from_per_byte_string("52413617B4363E4B81BA5E79A681BAEE");
    VERIFY(id3.to_per_byte_string() == "52413617B4363E4B81BA5E79A681BAEE");

    constexpr uxs::guid id4 = guid_from_string("{17364152-36B4-4B3E-81BA-5E79A681BAEE}");
    VERIFY(uxs::to_string(id4) == "{17364152-36b4-4b3e-81ba-5e79a681baee}");
#endif

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "guid", test_guid_1);
ADD_TEST_CASE("", "guid", test_guid_2);
