#include "uxs/guid.h"

#include "test_suite.h"

namespace {

int test_guid_1() {
    for (int i = 0; i < 1000; ++i) {
        uxs::guid id = uxs::guid::generate();
        VERIFY((id.data16(3) >> 12) == 4 && (id.data8(8) >> 4) >= 8 && (id.data8(8) >> 4) <= 11);
        {
            std::string s_id = uxs::to_string(id);
            VERIFY(s_id[15] == '4' && (s_id[20] == '8' || s_id[20] == '9' || s_id[20] == 'A' || s_id[20] == 'B'));
            VERIFY(uxs::from_string<uxs::guid>(s_id) == id);
        }
        {
            std::string s_id = id.to_per_byte_string();
            VERIFY(s_id[14] == '4' && (s_id[16] == '8' || s_id[16] == '9' || s_id[16] == 'A' || s_id[16] == 'B'));
            VERIFY(uxs::guid::from_per_byte_string(s_id) == id);
        }

        uint32_t data[4];
        id.get_data(data);
        VERIFY(uxs::guid(data) == id);

        uxs::guid xor_guid = id.make_xor(0xdeadbeef);
        VERIFY(data[0] == (xor_guid.data32(0) ^ 0xdeadbeef));
        VERIFY(data[1] == (xor_guid.data32(1) ^ 0xdeadbeef));
        VERIFY(data[2] == (xor_guid.data32(2) ^ 0xdeadbeef));
        VERIFY(data[3] == (xor_guid.data32(3) ^ 0xdeadbeef));
    }

    uxs::guid id(0x17364152, 0x36b4, 0x4b3e, 0x81, 0xba, 0x5e, 0x79, 0xa6, 0x81, 0xba, 0xee);
    VERIFY(uxs::to_string(id) == "{17364152-36B4-4B3E-81BA-5E79A681BAEE}");
    VERIFY(id.to_per_byte_string() == "52413617B4363E4B81BA5E79A681BAEE");
    VERIFY(id.data32(0) == 0x17364152 && id.data16(2) == 0x36b4 && id.data16(3) == 0x4b3e);
    VERIFY(id.data8(8) == 0x81 && id.data8(9) == 0xba && id.data8(10) == 0x5e && id.data8(11) == 0x79);
    VERIFY(id.data8(12) == 0xa6 && id.data8(13) == 0x81 && id.data8(14) == 0xba && id.data8(15) == 0xee);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "guid", test_guid_1);
