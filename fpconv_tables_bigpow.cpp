#include <cassert>
#include <cstdint>
#include <cstdio>

inline uint64_t lo32(uint64_t x) { return x & 0xffffffff; }
inline uint64_t hi32(uint64_t x) { return x >> 32; }

template<typename TyH, typename TyL>
uint64_t make64(TyH hi, TyL lo) {
    return (static_cast<uint64_t>(hi) << 32) | static_cast<uint64_t>(lo);
}

inline uint64_t umul128(uint64_t x, uint64_t y, uint64_t bias, uint64_t& result_hi) {
    const uint64_t lower = lo32(x) * lo32(y) + lo32(bias), higher = hi32(x) * hi32(y);
    const uint64_t mid1 = lo32(x) * hi32(y) + hi32(bias), mid2 = hi32(x) * lo32(y) + hi32(lower);
    const uint64_t t = lo32(mid1) + lo32(mid2);
    result_hi = higher + hi32(mid1) + hi32(mid2) + hi32(t);
    return make64(lo32(t), lo32(lower));
}

struct ulog2_table_t {
    unsigned n_bit[256];
    constexpr ulog2_table_t() : n_bit() {
        for (uint32_t n = 0; n < 256; ++n) {
            uint32_t u8 = n;
            n_bit[n] = 0;
            while (u8 >>= 1) { ++n_bit[n]; }
        }
    }
};
constexpr ulog2_table_t g_ulog2_tbl{};
inline unsigned ulog2(uint32_t x) {
    unsigned bias = 0;
    if (x >= 1u << 16) { x >>= 16, bias += 16; }
    if (x >= 1u << 8) { x >>= 8, bias += 8; }
    return bias + g_ulog2_tbl.n_bit[x];
}
inline unsigned ulog2(uint64_t x) {
    if (x >= 1ull << 32) { return 32 + ulog2(static_cast<uint32_t>(hi32(x))); }
    return ulog2(static_cast<uint32_t>(lo32(x)));
}

// --------------------------

inline uint64_t bignum_mul(uint64_t* x, const uint64_t* y, unsigned sz, uint64_t mul) {
    assert(sz > 0);
    uint64_t higher;
    uint64_t* x0 = x;
    x += sz - 1, y += sz - 1;
    *x = umul128(*y, mul, 0, higher);
    while (x != x0) { --x, --y, *x = umul128(*y, mul, higher, higher); }
    return higher;
}

inline uint64_t bignum_shift_left(uint64_t* x, unsigned sz, unsigned shift) {
    uint64_t higher = x[0] >> (64 - shift);
    for (unsigned n = 0; n < sz - 1; ++n) { x[n] = (x[n] << shift) | (x[n + 1] >> (64 - shift)); }
    x[sz - 1] <<= shift;
    return higher;
}

inline uint64_t bignum_shift_right(uint64_t* x, unsigned sz, unsigned shift, uint64_t higher) {
    uint64_t lower = x[sz - 1] << (64 - shift);
    for (unsigned k = sz; k > 1; --k) { x[k - 1] = (x[k - 1] >> shift) | (x[k - 2] << (64 - shift)); }
    x[0] = (x[0] >> shift) | (higher << (64 - shift));
    return lower;
}

// --------------------------

const int kBigPowStep = 18;
const int kBigPowTblSize = (324 + kBigPowStep) / 18;

uint64_t g_bigpow10[200];
unsigned g_bigpow10_offset[kBigPowTblSize + 1];

int main() {
    // 10^N big numbers and its offsets
    const uint64_t mul = 0x3782dace9d9;  // 5^18
    unsigned sz = 1, prev_sz = 0, total_sz = 1;
    g_bigpow10[0] = mul << (63 - ulog2(mul));  // normalized num
    g_bigpow10_offset[0] = 0;
    for (unsigned n = 1; n < kBigPowTblSize; ++n) {
        uint64_t* x = &g_bigpow10[total_sz];
        g_bigpow10_offset[n] = total_sz;
        x[0] = bignum_mul(x + 1, &g_bigpow10[prev_sz], sz, mul);
        bignum_shift_left(x, sz + 1, 63 - ulog2(x[0]));
        if (x[sz]) { ++sz; }
        prev_sz = total_sz, total_sz += sz;
    }
    g_bigpow10_offset[kBigPowTblSize] = total_sz;

    std::printf("{\n");
    for (unsigned n = 0; n < g_bigpow10_offset[kBigPowTblSize]; ++n) { std::printf("0x%016lx,\n", g_bigpow10[n]); }
    std::printf("}\n");
    std::printf("{\n");
    for (unsigned n = 0; n < kBigPowTblSize + 1; ++n) { std::printf("%d,\n", g_bigpow10_offset[n]); }
    std::printf("}\n");
    std::printf("{\n");
    const int64_t ln10_ln2 = 0x35269e12f;  // 2^32 * ln(10) / ln(2)
    for (unsigned n = 0; n < kBigPowTblSize; ++n) {
        int exp10to2 = hi32(ln10_ln2 * 18 * (1 + n));
        std::printf("%d,\n", exp10to2);
    }
    std::printf("}\n");

    return 0;
}
