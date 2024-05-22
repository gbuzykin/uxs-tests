#include <cassert>
#include <cstdint>
#include <cstdio>

std::uint32_t reflect(std::uint32_t ref, unsigned ch) {
    std::uint32_t value = 0;
    for (unsigned i = 1; i < ch + 1; ++i) {  // Swap bit 0 for bit 7 bit 1 for bit 6, etc.
        if (ref & 1) { value |= 1 << (ch - i); }
        ref >>= 1;
    }
    return value;
}

int main() {
    std::uint32_t v[256];
    // The official polynomial used by CRC-32 in PKZip, WinZip and Ethernet
    const std::uint32_t poly = 0x04c11db7;
    const std::uint32_t msbit = 0x80000000;
    for (unsigned i = 0; i < 256; ++i) {
        std::uint32_t r = reflect(i, 8) << 24;
        for (int j = 0; j < 8; ++j) { r = (r << 1) ^ (r & msbit ? poly : 0); }
        v[i] = reflect(r, 32);
    }
    std::printf("{\n");
    for (unsigned n = 0; n < 256; ++n) { std::printf("0x%08x,\n", v[n]); }
    std::printf("}\n");
    return 0;
}
