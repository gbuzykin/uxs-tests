#include <algorithm>
#include <cstdint>
#include <cstdio>

const uint64_t msb64 = 1ull << 63;

inline uint64_t lo32(uint64_t x) { return x & 0xffffffff; }
inline uint64_t hi32(uint64_t x) { return x >> 32; }

template<typename TyH, typename TyL>
uint64_t make64(TyH hi, TyL lo) {
    return (static_cast<uint64_t>(hi) << 32) | static_cast<uint64_t>(lo);
}

inline uint64_t umul128(uint64_t x, uint64_t y, uint64_t bias, uint64_t& result_hi) {
    uint64_t lower = lo32(x) * lo32(y) + lo32(bias), higher = hi32(x) * hi32(y);
    uint64_t mid1 = lo32(x) * hi32(y) + hi32(bias), mid2 = hi32(x) * lo32(y) + hi32(lower);
    uint64_t t = lo32(mid1) + lo32(mid2);
    result_hi = higher + hi32(mid1) + hi32(mid2) + hi32(t);
    return make64(lo32(t), lo32(lower));
}

inline uint64_t umul96(uint64_t x, uint32_t y, uint32_t bias, uint64_t& result_hi) {
    const uint64_t lower = lo32(x) * y + bias;
    const uint64_t higher = hi32(x) * y + hi32(lower);
    result_hi = hi32(higher);
    return make64(lo32(higher), lo32(lower));
}

using one_bit_t = uint8_t;
inline one_bit_t add64_carry(uint64_t a, uint64_t b, uint64_t& c, one_bit_t carry = 0) {
    b += carry;
    c = a + b;
    return c < b || b < carry;
}

inline uint64_t shl128(uint64_t x_hi, uint64_t x_lo, unsigned shift) {
    return (x_hi << shift) | (x_lo >> (64 - shift));
}

// --------------------------

struct uint96_t {
    uint64_t hi;
    uint32_t lo;
};

inline uint64_t umul96x32(uint96_t x, uint32_t y, uint64_t& result_hi) {
    const uint64_t lower = static_cast<uint64_t>(x.lo) * y;
    const uint64_t mid = lo32(x.hi) * y + hi32(lower);
    result_hi = hi32(x.hi) * y + hi32(mid);
    return make64(lo32(mid), lo32(lower));
}

inline uint64_t umul96x64_higher128(uint96_t x, uint64_t y, uint64_t& result_hi, uint32_t& result_lo) {
    const uint64_t lower = lo32(y) * x.lo;
    result_lo = static_cast<uint32_t>(lo32(lower));
    return umul128(x.hi, y, hi32(y) * x.lo + hi32(lower), result_hi);
}

// --------------------------

struct uint128_t {
    uint64_t hi;
    uint64_t lo;
};

inline uint128_t umul128_2(uint64_t x, uint64_t y, uint64_t bias) {
    uint128_t result;
    result.lo = umul128(x, y, bias, result.hi);
    return result;
}

inline uint128_t umul96_2(uint64_t x, uint32_t y, uint32_t bias) {
    uint128_t result;
    result.lo = umul96(x, y, bias, result.hi);
    return result;
}

static uint128_t uint128_multiply_by_10(const uint128_t& v, int& exp) {
    const uint32_t mul = 10;
    uint128_t t = umul96_2(v.lo, mul, 0);
    uint128_t result{0, t.lo};
    t = umul96_2(v.hi, mul, static_cast<uint32_t>(lo32(t.hi)));
    const uint64_t higher = lo32(t.hi);
    result.hi = t.lo;
    // re-normalize
    const unsigned shift = higher >= 8 ? 4 : 3;
    exp += shift;
    result.lo = (result.lo >> shift) | (result.hi << (64 - shift));
    result.hi = (result.hi >> shift) | (higher << (64 - shift));
    return result;
}

static uint128_t uint128_multiply_by_0_1(const uint128_t& v, int& exp) {
    uint128_t lower = umul128_2(v.lo, 0xcccccccccccccccd, 0);
    uint128_t t = umul128_2(v.hi, 0xcccccccccccccccc, lower.hi);
    uint128_t higher{0, t.hi};
    lower.hi = t.lo;
    one_bit_t carry = add64_carry(lower.hi, lower.lo, lower.hi);
    carry = add64_carry(higher.lo, t.lo, higher.lo, carry);
    add64_carry(higher.hi, t.hi, higher.hi, carry);
    exp -= 3;
    if (!(higher.hi & msb64)) {  // re-normalize
        higher.hi = (higher.hi << 1) | (higher.lo >> 63);
        higher.lo = (higher.lo << 1) | (lower.hi >> 63);
        --exp;
    }
    return higher;
}

static uint96_t uint128_to_fp_m96(const uint128_t& v) {
    uint64_t lower;
    one_bit_t carry = add64_carry(v.lo, 0x80000000, lower);
    return uint96_t{v.hi + carry, static_cast<uint32_t>(hi32(lower))};
}

// --------------------------

const int pow10_max = 344;
uint96_t g_coef10to2_m[2 * pow10_max + 1];
int g_coef10to2_exp[2 * pow10_max + 1];

const int pow2_bias = 1074, pow2_max = 1023;
int g_exp2to10[pow2_bias + pow2_max + 1];

int main() {
    // 10^N -> 2^M power conversion table
    int exp = 0;
    uint128_t v{msb64, 0};
    g_coef10to2_m[pow10_max] = uint96_t{msb64, 0}, g_coef10to2_exp[pow10_max] = 0;
    for (unsigned n = 0; n < pow10_max; ++n) {
        v = uint128_multiply_by_10(v, exp);
        g_coef10to2_m[pow10_max + n + 1] = uint128_to_fp_m96(v);
        g_coef10to2_exp[pow10_max + n + 1] = exp;
    }

    exp = 0, v = uint128_t{msb64, 0};
    for (unsigned n = 0; n < pow10_max; ++n) {
        v = uint128_multiply_by_0_1(v, exp);
        g_coef10to2_m[pow10_max - n - 1] = uint128_to_fp_m96(v);
        g_coef10to2_exp[pow10_max - n - 1] = exp;
    }

    for (int n = -pow10_max; n <= pow10_max; ++n) {
        static const uint32_t mul10[] = {0xa0000000, 0xc8000000, 0xfa000000, 0x9c400000,
                                         0xc3500000, 0xf4240000, 0x98968000};

        uint96_t ref = g_coef10to2_m[pow10_max + n];
        if (n & 7) {
            uint96_t base = g_coef10to2_m[pow10_max + (n & ~7)];
            uint32_t mul = mul10[(n & 7) - 1];

            uint96_t result;
            uint64_t t = umul96x32(base, mul, result.hi);
            if (!(result.hi & msb64)) {
                result.hi = shl128(result.hi, t, 1);
                t <<= 1;
            }

            result.lo = static_cast<uint32_t>(hi32(t + (1ull << 31)));

            if (result.hi != ref.hi) {
                std::printf("error: n = %d, hi: %lx != %lx\n", n, result.hi, ref.hi);
                return -1;
            }
            if (std::abs(static_cast<int>(result.lo - ref.lo)) > 1) {
                std::printf("error: n = %d, lo: %x != %x\n", n, result.lo, ref.lo);
                return -1;
            }
        }
    }

    for (int n = 0; n <= pow10_max; ++n) {
        // const uint64_t m = 0x8000000000000800;
        const uint64_t m = 0x8000000000000001;
        uint64_t result_hi;
        uint32_t result_lo;
        uint64_t result_mid = umul96x64_higher128(g_coef10to2_m[pow10_max + n], m, result_hi, result_lo);
        std::printf("n = %02d: 0x%016lx|%016lx|%08x\n", n, result_hi, result_mid, result_lo);
        if (result_lo) {
            std::printf("max pow = %d\n", n - 1);
            break;
        }
    }

    // 2^N -> 10^M power conversion index table
    for (int exp = -pow2_bias; exp <= pow2_max; ++exp) {
        auto it = std::lower_bound(g_coef10to2_exp, g_coef10to2_exp + 2 * pow10_max + 1, -exp,
                                   [](int el, int exp) { return el < exp; });
        g_exp2to10[pow2_bias + exp] = pow10_max - static_cast<int>(it - g_coef10to2_exp);
    }

    const int64_t ln2_ln10 = 0x4d104d42;  // 2^32 * ln(2) / ln(10)
    for (int exp = -pow2_bias; exp <= pow2_max; ++exp) {
        int exp2to10 = hi32(ln2_ln10 * exp);
        if (g_exp2to10[pow2_bias + exp] != exp2to10) {
            std::printf("error: %d != %d\n", g_exp2to10[pow2_bias + exp], exp2to10);
            return -1;
        }
    }

    const int64_t ln10_ln2 = 0x35269e12f;  // 2^32 * ln(10) / ln(2)
    for (int exp = -pow10_max; exp <= pow10_max; ++exp) {
        int exp10to2 = hi32(ln10_ln2 * exp);
        if (g_coef10to2_exp[pow10_max + exp] != exp10to2) {
            std::printf("error: %d != %d\n", g_coef10to2_exp[pow10_max + exp], exp10to2);
            return -1;
        }
    }

    for (int exp = 1; exp <= pow10_max; ++exp) {
        if (g_coef10to2_exp[pow10_max + exp] + g_coef10to2_exp[pow10_max - exp] != -1) {
            std::printf("error: %d, %d\n", g_coef10to2_exp[pow10_max + exp], g_coef10to2_exp[pow10_max - exp]);
            return -1;
        }
    }

    std::printf("{\n");
    for (unsigned n = 0; n < 2 * pow10_max + 1; ++n) {
        if (!(n & 7)) { std::printf("{0x%016lx, 0x%08x},\n", g_coef10to2_m[n].hi, g_coef10to2_m[n].lo); }
    }
    std::printf("}\n");

    return 0;
}
