#include <cassert>
#include <cstdint>
#include <cstdio>

enum char_bits : std::uint8_t {
    is_space = 1 << 0,
    is_number = 1 << 1,
    is_lower = 1 << 2,
    is_upper = 1 << 3,
    is_string_special = 1 << 4,
    is_json_ws = 1 << 5,
    is_xml_special = 1 << 6,
    is_xml_string_special = 1 << 7,
};

int main() {
    std::uint8_t flags[256];
    std::uint8_t digs[256];
    for (unsigned ch = 0; ch < 256; ++ch) {
        if (ch >= '0' && ch <= '9') {
            digs[ch] = ch - '0', flags[ch] = is_number;
        } else if (ch >= 'a' && ch <= 'z') {
            digs[ch] = ch - 'a' + 10, flags[ch] = is_lower;
        } else if (ch >= 'A' && ch <= 'Z') {
            digs[ch] = ch - 'A' + 10, flags[ch] = is_upper;
        } else {
            digs[ch] = 255, flags[ch] = ch == ' ' || (ch >= '\t' && ch <= '\r') ? is_space : 0;
        }
        if (ch == '\0' || ch == '\n' || ch == '\"' || ch == '\\') { flags[ch] |= is_string_special; }
        if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') { flags[ch] |= is_json_ws; }
        if (ch == '\0' || ch == '<' || ch == '&') { flags[ch] |= is_xml_special; }
        if (ch == '\0' || ch == '\n' || ch == '\"' || ch == '\'' || ch == '<' || ch == '&') {
            flags[ch] |= is_xml_string_special;
        }
    }
    std::printf("{\n");
    for (unsigned n = 0; n < 256; ++n) { std::printf("0x%02x,\n", flags[n]); }
    std::printf("}\n");
    std::printf("{\n");
    for (unsigned n = 0; n < 256; ++n) { std::printf("0x%02x,\n", digs[n]); }
    std::printf("}\n");
    return 0;
}
