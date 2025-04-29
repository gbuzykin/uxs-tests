#include "uxs/guid.h"

#include "test_suite.h"

namespace {

int test_guid_1() {
    for (int i = 0; i < 1000; ++i) {
        const uxs::guid id = uxs::guid::generate();

        VERIFY((id.data.w[1] >> 12) == 4 && (id.data.b[0] >> 4) >= 8 && (id.data.b[0] >> 4) <= 11);
        {
            std::string s_id = uxs::to_string(id, uxs::fmt_opts{uxs::fmt_flags::uppercase});
            VERIFY(s_id[15] == '4' && (s_id[20] == '8' || s_id[20] == '9' || s_id[20] == 'A' || s_id[20] == 'B'));
            VERIFY(uxs::from_string<uxs::guid>(s_id) == id);
        }
        {
            std::string s_id = id.to_per_byte_string();
            VERIFY(s_id[14] == '4' && (s_id[16] == '8' || s_id[16] == '9' || s_id[16] == 'A' || s_id[16] == 'B'));
            VERIFY(uxs::guid::from_per_byte_string(s_id) == id);
        }

        const auto data8 = id.data8();
        const auto data64 = id.data64();

        VERIFY(uxs::guid(data8) == id);
        VERIFY(uxs::guid(data64) == id);
    }

    const uxs::guid id(0x17364152, 0x36b4, 0x4b3e, 0x81, 0xba, 0x5e, 0x79, 0xa6, 0x81, 0xba, 0xee);
    VERIFY(uxs::to_string(id, uxs::fmt_opts{uxs::fmt_flags::uppercase}) == "{17364152-36B4-4B3E-81BA-5E79A681BAEE}");
    VERIFY(id.to_per_byte_string() == "52413617B4363E4B81BA5E79A681BAEE");
    VERIFY(id.data.l == 0x17364152 && id.data.w[0] == 0x36b4 && id.data.w[1] == 0x4b3e);
    VERIFY(id.data.b[0] == 0x81 && id.data.b[1] == 0xba && id.data.b[2] == 0x5e && id.data.b[3] == 0x79);
    VERIFY(id.data.b[4] == 0xa6 && id.data.b[5] == 0x81 && id.data.b[6] == 0xba && id.data.b[7] == 0xee);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "guid", test_guid_1);
