#include <cassert>
#include <cstdint>
#include <cstdio>

int main() {
    std::uint8_t n_bit[256];
    for (std::uint32_t n = 0; n < 256; ++n) {
        std::uint8_t u8 = n;
        n_bit[n] = 0;
        while (u8 >>= 1) { ++n_bit[n]; }
    }
    std::printf("{\n");
    for (unsigned n = 0; n < 256; ++n) { std::printf("%d,\n", n_bit[n]); }
    std::printf("}\n");
    return 0;
}
