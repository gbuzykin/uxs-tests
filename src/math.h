#pragma once

#include <uxs/string_alg.h>
#include <uxs/string_conv.h>
#include <uxs/variant.h>

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

namespace uxs {

inline bibuf& operator>>(bibuf& is, math::vec2& v) { return is >> v.x() >> v.y(); }
inline biobuf& operator<<(biobuf& os, const math::vec2& v) { return os << v.x() << v.y(); }

inline bibuf& operator>>(bibuf& is, math::vec3& v) { return is >> v.x() >> v.y() >> v.z(); }
inline biobuf& operator<<(biobuf& os, const math::vec3& v) { return os << v.x() << v.y() << v.z(); }

inline bibuf& operator>>(bibuf& is, math::vec4& v) { return is >> v.x() >> v.y() >> v.z() >> v.w(); }
inline biobuf& operator<<(biobuf& os, const math::vec4& v) { return os << v.x() << v.y() << v.z() << v.w(); }

inline bibuf& operator>>(bibuf& is, math::quat& q) { return is >> q.x() >> q.y() >> q.z() >> q.w(); }
inline biobuf& operator<<(biobuf& os, const math::quat& q) { return os << q.x() << q.y() << q.z() << q.w(); }

inline bibuf& operator>>(bibuf& is, math::mat4& m) {
    auto data = m.ptr();
    for (int i = 0; i < 16; i++) { is >> data[i]; }
    return is;
}

inline biobuf& operator<<(biobuf& os, const math::mat4& m) {
    auto data = m.ptr();
    for (int i = 0; i < 16; i++) { os << data[i]; }
    return os;
}

namespace detail {
template<typename CharT>
from_chars_result<CharT> parse_array(const CharT* first, const CharT* last, float* fvec, unsigned count) {
    if (first == last) { return {first, sconv_errc::empty}; }
    sconv_errc ec = sconv_errc::ok;
    unsigned n = 0;
    uxs::string_to_words_to(
        uxs::to_string_view(first, last), ',', fvec,
        [&ec](std::basic_string_view<CharT> s) { return uxs::from_string_errc<float>(s, ec); },
        [&ec, &n, count](typename std::basic_string_view<CharT>::iterator,
                         typename std::basic_string_view<CharT>::iterator) {
            return ec == sconv_errc::ok && ++n <= count;
        });
    if (ec != sconv_errc::ok || n != count) { return {first, sconv_errc::invalid}; }
    return {last, sconv_errc::ok};
}
template<typename StrTy>
void fmt_array(StrTy& s, const float* fvec, unsigned count) {
    uxs::join_strings_append(s, est::make_range(fvec, fvec + count), ' ',
                             [](StrTy& s, float f) { uxs::to_string_append(s, f); });
}
}  // namespace detail

template<typename CharT>
struct from_string_impl<math::vec2, CharT> {
    from_chars_result<CharT> operator()(const CharT* first, const CharT* last, math::vec2& val) const {
        return detail::parse_array(first, last, val.ptr(), 2);
    }
};

template<typename CharT>
struct to_string_impl<math::vec2, CharT> {
    template<typename StrTy>
    void operator()(StrTy& s, const math::vec2& val) const {
        detail::fmt_array(s, val.ptr(), 2);
    }
};

template<typename CharT>
struct from_string_impl<math::vec3, CharT> {
    from_chars_result<CharT> operator()(const CharT* first, const CharT* last, math::vec3& val) const {
        return detail::parse_array(first, last, val.ptr(), 3);
    }
};

template<typename CharT>
struct to_string_impl<math::vec3, CharT> {
    template<typename StrTy>
    void operator()(StrTy& s, const math::vec3& val) const {
        detail::fmt_array(s, val.ptr(), 3);
    }
};

template<typename CharT>
struct from_string_impl<math::vec4, CharT> {
    from_chars_result<CharT> operator()(const CharT* first, const CharT* last, math::vec4& val) const {
        return detail::parse_array(first, last, val.ptr(), 4);
    }
};

template<typename CharT>
struct to_string_impl<math::vec4, CharT> {
    template<typename StrTy>
    void operator()(StrTy& s, const math::vec4& val) const {
        detail::fmt_array(s, val.ptr(), 4);
    }
};

template<typename CharT>
struct from_string_impl<math::quat, CharT> {
    from_chars_result<CharT> operator()(const CharT* first, const CharT* last, math::quat& val) const {
        return detail::parse_array(first, last, val.ptr(), 4);
    }
};

template<typename CharT>
struct to_string_impl<math::quat, CharT> {
    template<typename StrTy>
    void operator()(StrTy& s, const math::quat& val) const {
        detail::fmt_array(s, val.ptr(), 4);
    }
};

template<typename CharT>
struct from_string_impl<math::mat4, CharT> {
    from_chars_result<CharT> operator()(const CharT* first, const CharT* last, math::mat4& val) const {
        return detail::parse_array(first, last, val.ptr(), 16);
    }
};

template<typename CharT>
struct to_string_impl<math::mat4, CharT> {
    template<typename StrTy>
    void operator()(StrTy& s, const math::mat4& val) const {
        detail::fmt_array(s, val.ptr(), 16);
    }
};

namespace variant_id {
constexpr variant_id_t vector2d = variant_id::custom + 0;
constexpr variant_id_t vector3d = variant_id::custom + 1;
constexpr variant_id_t vector4d = variant_id::custom + 2;
constexpr variant_id_t quaternion = variant_id::custom + 3;
constexpr variant_id_t matrix4x4 = variant_id::custom + 4;
}  // namespace variant_id

UXS_DECLARE_VARIANT_TYPE(math::vec2, variant_id::vector2d);
UXS_DECLARE_VARIANT_TYPE(math::vec3, variant_id::vector3d);
UXS_DECLARE_VARIANT_TYPE(math::vec4, variant_id::vector4d);
UXS_DECLARE_VARIANT_TYPE(math::quat, variant_id::quaternion);
UXS_DECLARE_VARIANT_TYPE(math::mat4, variant_id::matrix4x4);

}  // namespace uxs
