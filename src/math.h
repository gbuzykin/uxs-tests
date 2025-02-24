#pragma once

#include "uxs/string_alg.h"
#include "uxs/variant.h"

namespace vrc {
namespace math {

struct vec2 {
    std::array<float, 2> v_;
    vec2() { v_.fill(0); }
    vec2(float x, float y) {
        v_[0] = x;
        v_[1] = y;
    }
    const float* ptr() const { return v_.data(); }
    float* ptr() { return v_.data(); }
    float x() const { return v_[0]; }
    float& x() { return v_[0]; }
    float y() const { return v_[1]; }
    float& y() { return v_[1]; }
    friend bool operator==(const vec2& lhv, const vec2& rhv) { return lhv.v_ == rhv.v_; }
    friend bool operator!=(const vec2& lhv, const vec2& rhv) { return lhv.v_ != rhv.v_; }
    friend bool operator<(const vec2& lhv, const vec2& rhv) { return lhv.v_ < rhv.v_; }
};

struct vec3 {
    std::array<float, 3> v_;
    vec3() { v_.fill(0); }
    vec3(float x, float y, float z) {
        v_[0] = x;
        v_[1] = y;
        v_[2] = z;
    }
    const float* ptr() const { return v_.data(); }
    float* ptr() { return v_.data(); }
    float x() const { return v_[0]; }
    float& x() { return v_[0]; }
    float y() const { return v_[1]; }
    float& y() { return v_[1]; }
    float z() const { return v_[2]; }
    float& z() { return v_[2]; }
    friend bool operator==(const vec3& lhv, const vec3& rhv) { return lhv.v_ == rhv.v_; }
    friend bool operator!=(const vec3& lhv, const vec3& rhv) { return lhv.v_ != rhv.v_; }
    friend bool operator<(const vec3& lhv, const vec3& rhv) { return lhv.v_ < rhv.v_; }
};

struct vec4 {
    std::array<float, 4> v_;
    vec4() { v_.fill(0); }
    vec4(float x, float y, float z, float w) {
        v_[0] = x;
        v_[1] = y;
        v_[2] = z;
        v_[3] = w;
    }
    const float* ptr() const { return v_.data(); }
    float* ptr() { return v_.data(); }
    float x() const { return v_[0]; }
    float& x() { return v_[0]; }
    float y() const { return v_[1]; }
    float& y() { return v_[1]; }
    float z() const { return v_[2]; }
    float& z() { return v_[2]; }
    float w() const { return v_[3]; }
    float& w() { return v_[3]; }
    friend bool operator==(const vec4& lhv, const vec4& rhv) { return lhv.v_ == rhv.v_; }
    friend bool operator!=(const vec4& lhv, const vec4& rhv) { return lhv.v_ != rhv.v_; }
    friend bool operator<(const vec4& lhv, const vec4& rhv) { return lhv.v_ < rhv.v_; }
};

struct quat {
    std::array<float, 4> v_;
    quat() { v_.fill(0); }
    quat(float x, float y, float z, float w) {
        v_[0] = x;
        v_[1] = y;
        v_[2] = z;
        v_[3] = w;
    }
    const float* ptr() const { return v_.data(); }
    float* ptr() { return v_.data(); }
    float x() const { return v_[0]; }
    float& x() { return v_[0]; }
    float y() const { return v_[1]; }
    float& y() { return v_[1]; }
    float z() const { return v_[2]; }
    float& z() { return v_[2]; }
    float w() const { return v_[3]; }
    float& w() { return v_[3]; }
    friend bool operator==(const quat& lhq, const quat& rhq) { return lhq.v_ == rhq.v_; }
    friend bool operator!=(const quat& lhq, const quat& rhq) { return lhq.v_ != rhq.v_; }
    friend bool operator<(const quat& lhq, const quat& rhq) { return lhq.v_ < rhq.v_; }
};

struct mat4 {
    std::array<float, 16> v_;
    mat4() {
        v_.fill(0);
        v_[0] = v_[5] = v_[10] = v_[15] = 1;
    }
    const float* ptr() const { return v_.data(); }
    float* ptr() { return v_.data(); }
    friend bool operator==(const mat4& lhm, const mat4& rhm) { return lhm.v_ == rhm.v_; }
    friend bool operator!=(const mat4& lhm, const mat4& rhm) { return lhm.v_ != rhm.v_; }
    friend bool operator<(const mat4& lhm, const mat4& rhm) { return lhm.v_ < rhm.v_; }
};

}  // namespace math
}  // namespace vrc

namespace uxs {

inline u8ibuf& operator>>(u8ibuf& is, vrc::math::vec2& v) { return is >> v.x() >> v.y(); }
inline u8iobuf& operator<<(u8iobuf& os, const vrc::math::vec2& v) { return os << v.x() << v.y(); }

inline u8ibuf& operator>>(u8ibuf& is, vrc::math::vec3& v) { return is >> v.x() >> v.y() >> v.z(); }
inline u8iobuf& operator<<(u8iobuf& os, const vrc::math::vec3& v) { return os << v.x() << v.y() << v.z(); }

inline u8ibuf& operator>>(u8ibuf& is, vrc::math::vec4& v) { return is >> v.x() >> v.y() >> v.z() >> v.w(); }
inline u8iobuf& operator<<(u8iobuf& os, const vrc::math::vec4& v) { return os << v.x() << v.y() << v.z() << v.w(); }

inline u8ibuf& operator>>(u8ibuf& is, vrc::math::quat& q) { return is >> q.x() >> q.y() >> q.z() >> q.w(); }
inline u8iobuf& operator<<(u8iobuf& os, const vrc::math::quat& q) { return os << q.x() << q.y() << q.z() << q.w(); }

inline u8ibuf& operator>>(u8ibuf& is, vrc::math::mat4& m) {
    auto data = m.ptr();
    for (int i = 0; i < 16; i++) { is >> data[i]; }
    return is;
}

inline u8iobuf& operator<<(u8iobuf& os, const vrc::math::mat4& m) {
    auto data = m.ptr();
    for (int i = 0; i < 16; i++) { os << data[i]; }
    return os;
}

template<typename CharT>
struct string_converter<vrc::math::vec2, CharT> {
    const CharT* from_chars(const CharT* first, const CharT* last, vrc::math::vec2& val) const {
        uxs::basic_string_to_words(std::basic_string_view<CharT>{first, static_cast<size_t>(last - first)}, ',',
                                   uxs::from_basic_string<float, CharT>, val.ptr(), 2);
        return last;
    }
    template<typename StrTy>
    void to_string(StrTy& s, const vrc::math::vec2& val, fmt_opts fmt) const {
        uxs::join_basic_strings(s, uxs::make_range(val.ptr(), val.ptr() + 2), ' ',
                                [&fmt](StrTy& s, float f) -> StrTy& { return uxs::to_basic_string(s, f, fmt); });
    }
};

template<typename CharT>
struct string_converter<vrc::math::vec3, CharT> {
    const CharT* from_chars(const CharT* first, const CharT* last, vrc::math::vec3& val) const {
        uxs::basic_string_to_words(std::basic_string_view<CharT>{first, static_cast<size_t>(last - first)}, ',',
                                   uxs::from_basic_string<float, CharT>, val.ptr(), 3);
        return last;
    }
    template<typename StrTy>
    void to_string(StrTy& s, const vrc::math::vec3& val, fmt_opts fmt) const {
        uxs::join_basic_strings(s, uxs::make_range(val.ptr(), val.ptr() + 3), ' ',
                                [&fmt](StrTy& s, float f) -> StrTy& { return uxs::to_basic_string(s, f, fmt); });
    }
};

template<typename CharT>
struct string_converter<vrc::math::vec4, CharT> {
    const CharT* from_chars(const CharT* first, const CharT* last, vrc::math::vec4& val) const {
        uxs::basic_string_to_words(std::basic_string_view<CharT>{first, static_cast<size_t>(last - first)}, ',',
                                   uxs::from_basic_string<float, CharT>, val.ptr(), 4);
        return last;
    }
    template<typename StrTy>
    void to_string(StrTy& s, const vrc::math::vec4& val, fmt_opts fmt) const {
        uxs::join_basic_strings(s, uxs::make_range(val.ptr(), val.ptr() + 4), ' ',
                                [&fmt](StrTy& s, float f) -> StrTy& { return uxs::to_basic_string(s, f, fmt); });
    }
};

template<typename CharT>
struct string_converter<vrc::math::quat, CharT> {
    const CharT* from_chars(const CharT* first, const CharT* last, vrc::math::quat& val) const {
        uxs::basic_string_to_words(std::basic_string_view<CharT>{first, static_cast<size_t>(last - first)}, ',',
                                   uxs::from_basic_string<float, CharT>, val.ptr(), 4);
        return last;
    }
    template<typename StrTy>
    void to_string(StrTy& s, const vrc::math::quat& val, fmt_opts fmt) const {
        uxs::join_basic_strings(s, uxs::make_range(val.ptr(), val.ptr() + 4), ' ',
                                [&fmt](StrTy& s, float f) -> StrTy& { return uxs::to_basic_string(s, f, fmt); });
    }
};

template<typename CharT>
struct string_converter<vrc::math::mat4, CharT> {
    const CharT* from_chars(const CharT* first, const CharT* last, vrc::math::mat4& val) const {
        uxs::basic_string_to_words(std::basic_string_view<CharT>{first, static_cast<size_t>(last - first)}, ',',
                                   uxs::from_basic_string<float, CharT>, val.ptr(), 16);
        return last;
    }
    template<typename StrTy>
    void to_string(StrTy& s, const vrc::math::mat4& val, fmt_opts fmt) const {
        uxs::join_basic_strings(s, uxs::make_range(val.ptr(), val.ptr() + 16), ' ',
                                [&fmt](StrTy& s, float f) -> StrTy& { return uxs::to_basic_string(s, f, fmt); });
    }
};

UXS_DECLARE_VARIANT_TYPE(vrc::math::vec2, variant_id::vector2d);
UXS_DECLARE_VARIANT_TYPE(vrc::math::vec3, variant_id::vector3d);
UXS_DECLARE_VARIANT_TYPE(vrc::math::vec4, variant_id::vector4d);
UXS_DECLARE_VARIANT_TYPE(vrc::math::quat, variant_id::quaternion);
UXS_DECLARE_VARIANT_TYPE(vrc::math::mat4, variant_id::matrix4x4);

}  // namespace uxs
