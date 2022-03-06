#pragma once

#include "util/variant.h"

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

namespace util {

template<>
struct string_converter<vrc::math::vec2> : string_converter_base<vrc::math::vec2> {
    static const char* from_string(const char* first, const char* last, vrc::math::vec2& val) {
        separate_words(std::string_view(first, last - first), ',', util::from_string<float>, val.ptr(), 2);
        return last;
    }
    template<typename StrTy>
    static StrTy& to_string(StrTy& s, const vrc::math::vec2& val, const fmt_state& fmt) {
        using namespace std::placeholders;
        return join_strings_append(s, util::make_range(val.ptr(), val.ptr() + 2), ' ',
                                   std::bind(util::to_string_append<float, StrTy>, _1, _2, fmt));
    }
};

template<>
struct string_converter<vrc::math::vec3> : string_converter_base<vrc::math::vec3> {
    static const char* from_string(const char* first, const char* last, vrc::math::vec3& val) {
        separate_words(std::string_view(first, last - first), ',', util::from_string<float>, val.ptr(), 3);
        return last;
    }
    template<typename StrTy>
    static StrTy& to_string(StrTy& s, const vrc::math::vec3& val, const fmt_state& fmt) {
        using namespace std::placeholders;
        return join_strings_append(s, util::make_range(val.ptr(), val.ptr() + 3), ' ',
                                   std::bind(util::to_string_append<float, StrTy>, _1, _2, fmt));
    }
};

template<>
struct string_converter<vrc::math::vec4> : string_converter_base<vrc::math::vec4> {
    static const char* from_string(const char* first, const char* last, vrc::math::vec4& val) {
        separate_words(std::string_view(first, last - first), ',', util::from_string<float>, val.ptr(), 4);
        return last;
    }
    template<typename StrTy>
    static StrTy& to_string(StrTy& s, const vrc::math::vec4& val, const fmt_state& fmt) {
        using namespace std::placeholders;
        return join_strings_append(s, util::make_range(val.ptr(), val.ptr() + 4), ' ',
                                   std::bind(util::to_string_append<float, StrTy>, _1, _2, fmt));
    }
};

template<>
struct string_converter<vrc::math::quat> : string_converter_base<vrc::math::quat> {
    static const char* from_string(const char* first, const char* last, vrc::math::quat& val) {
        separate_words(std::string_view(first, last - first), ',', util::from_string<float>, val.ptr(), 4);
        return last;
    }
    template<typename StrTy>
    static StrTy& to_string(StrTy& s, const vrc::math::quat& val, const fmt_state& fmt) {
        using namespace std::placeholders;
        return join_strings_append(s, util::make_range(val.ptr(), val.ptr() + 4), ' ',
                                   std::bind(util::to_string_append<float, StrTy>, _1, _2, fmt));
    }
};

template<>
struct string_converter<vrc::math::mat4> : string_converter_base<vrc::math::mat4> {
    static const char* from_string(const char* first, const char* last, vrc::math::mat4& val) {
        separate_words(std::string_view(first, last - first), ',', util::from_string<float>, val.ptr(), 16);
        return last;
    }
    template<typename StrTy>
    static StrTy& to_string(StrTy& s, const vrc::math::mat4& val, const fmt_state& fmt) {
        using namespace std::placeholders;
        return join_strings_append(s, util::make_range(val.ptr(), val.ptr() + 16), ' ',
                                   std::bind(util::to_string_append<float, StrTy>, _1, _2, fmt));
    }
};

template<>
struct variant_type_impl<vrc::math::vec2>
    : variant_type_with_string_converter_impl<vrc::math::vec2, variant_id::kVector2D> {};

template<>
struct variant_type_impl<vrc::math::vec3>
    : variant_type_with_string_converter_impl<vrc::math::vec3, variant_id::kVector3D> {};

template<>
struct variant_type_impl<vrc::math::vec4>
    : variant_type_with_string_converter_impl<vrc::math::vec4, variant_id::kVector4D> {};

template<>
struct variant_type_impl<vrc::math::quat>
    : variant_type_with_string_converter_impl<vrc::math::quat, variant_id::kQuaternion> {
    variant_type_impl() {
        vtable()->set_cvt(variant_id::kVector4D, from_vec4_cvt);
        variant_type_impl<vrc::math::vec4>::vtable()->set_cvt(variant_id::kQuaternion, to_vec4_cvt);
    }
    static void from_vec4_cvt(void* to, const void* from) {
        const auto& v = *reinterpret_cast<const vrc::math::vec4*>(from);
        *reinterpret_cast<vrc::math::quat*>(to) = vrc::math::quat(v.x(), v.y(), v.z(), v.w());
    }
    static void to_vec4_cvt(void* to, const void* from) {
        const auto& q = *reinterpret_cast<const vrc::math::quat*>(from);
        *reinterpret_cast<vrc::math::vec4*>(to) = vrc::math::vec4(q.x(), q.y(), q.z(), q.w());
    }
};

template<>
struct variant_type_impl<vrc::math::mat4>
    : variant_type_with_string_converter_impl<vrc::math::mat4, variant_id::kMatrix4x4> {};

}  // namespace util
