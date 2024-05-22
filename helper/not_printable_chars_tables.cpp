#include <cassert>
#include <cstdint>
#include <cstdio>
#include <format>
#include <vector>

template<typename OutputIt>
unsigned to_utf8(std::uint32_t code, OutputIt out) {
    if (code < 0x80) {
        *out++ = static_cast<std::uint8_t>(code);
        return 1;
    }
    std::uint8_t ch[4];
    const std::uint8_t mask[] = {0, 0x1f, 0xf, 0x7};
    const std::uint8_t hdr[] = {0, 0xc0, 0xe0, 0xf0};
    unsigned count = 0;
    do { ch[count] = 0x80 | (code & 0x3f); } while ((code >>= 6) > mask[++count]);
    *out++ = static_cast<std::uint8_t>(hdr[count] | code);
    const unsigned n_written = count + 1;
    do { *out++ = ch[--count]; } while (count > 0);
    return n_written;
}

enum class utf_type_t { normal = 0, double_width, special, ill_formed };

template<typename Func>
void classify_utf(const Func& fn, utf_type_t tgt_type, std::vector<std::pair<std::uint32_t, std::uint32_t>>& v) {
    utf_type_t last_type = utf_type_t::normal;
    const std::uint32_t max_code = 0x110000;
    std::uint32_t code_first = 0;
    for (std::uint32_t code = 0; code < max_code; ++code) {
        auto s = fn(code);
        utf_type_t curr_type = utf_type_t::normal;
        if (code < 0x20 || s.starts_with("\"\\u")) {
            curr_type = utf_type_t::special;
        } else if (s.starts_with("\"\\x")) {
            curr_type = utf_type_t::ill_formed;
        } else if (code >= 0x100 && s.back() != ' ') {
            curr_type = utf_type_t::double_width;
        }
        if (curr_type != last_type) {
            if (last_type == tgt_type) {
                std::uint32_t code_last = code - 1;
                std::uint32_t h1 = code_first >> 16, h2 = code_last >> 16;
                if (h1 == h2) {
                    v.emplace_back(code_first, code_last);
                } else {
                    v.emplace_back(code_first, (h1 << 16) | 0xffff);
                    for (std::uint32_t h = h1 + 1; h < h2; ++h) { v.emplace_back(h << 16, (h << 16) | 0xffff); }
                    v.emplace_back(h2 << 16, code_last);
                }
            }
            last_type = curr_type;
            code_first = code;
        }
    }
    if (last_type == tgt_type) {
        std::uint32_t code_last = max_code - 1;
        std::uint32_t h1 = code_first >> 16, h2 = code_last >> 16;
        if (h1 == h2) {
            v.emplace_back(code_first, code_last);
        } else {
            v.emplace_back(code_first, (h1 << 16) | 0xffff);
            for (std::uint32_t h = h1 + 1; h < h2; ++h) { v.emplace_back(h << 16, (h << 16) | 0xffff); }
            v.emplace_back(h2 << 16, code_last);
        }
    }
}

template<typename Func>
void print_ranges(const std::vector<std::pair<std::uint32_t, std::uint32_t>>& v, const Func& fn) {
    std::vector<std::uint32_t> idx, compressed;
    std::uint32_t h_last = ~(std::uint32_t)0;
    for (auto [from, to] : v) {
        printf("%s-%s\n", fn(from).c_str(), fn(to).c_str());
        std::uint32_t h = from >> 16;
        while (h_last != h) {
            idx.push_back(static_cast<std::uint32_t>(compressed.size()));
            ++h_last;
        }
        compressed.push_back((from & 0xffff) | ((to - from) << 16));
    }
    while (h_last != 0x11) {
        idx.push_back(static_cast<std::uint32_t>(compressed.size()));
        ++h_last;
    }
    std::printf("size = %lu\n", sizeof(std::uint32_t) * (idx.size() + compressed.size()));
    std::printf("{\n");
    for (std::uint32_t i : idx) { std::printf("0x%x,\n", i + static_cast<std::uint32_t>(idx.size())); }
    for (std::uint32_t c : compressed) { std::printf("0x%x,\n", c); }
    std::printf("}\n");
}

int main() {
    auto fn = [](std::uint32_t code) {
        char buf[5];
        unsigned sz = to_utf8(code, static_cast<char*>(buf));
        return std::format("{:4?}", std::string_view(buf, sz));
    };

    std::vector<std::pair<std::uint32_t, std::uint32_t>> v;
    classify_utf(fn, utf_type_t::special, v);
    print_ranges(v, fn);

    v.clear();
    classify_utf(fn, utf_type_t::double_width, v);
    print_ranges(v, fn);
    return 0;
}
