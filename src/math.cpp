#include "math.h"

UXS_IMPLEMENT_VARIANT_TYPE_WITH_STRING_CONVERTER(vrc::math::vec2);
UXS_IMPLEMENT_VARIANT_TYPE_WITH_STRING_CONVERTER(vrc::math::vec3);
UXS_IMPLEMENT_VARIANT_TYPE_WITH_STRING_CONVERTER(vrc::math::vec4);
UXS_IMPLEMENT_VARIANT_TYPE(vrc::math::quat, convert_from, convert_to);
UXS_IMPLEMENT_VARIANT_TYPE_WITH_STRING_CONVERTER(vrc::math::mat4);

using namespace vrc;
using namespace vrc::math;

bool uxs::variant_type_impl<quat>::convert_from(variant_id type, void* to, const void* from) {
    auto& result = *static_cast<quat*>(to);
    switch (type) {
        case variant_id::string: {
            return uxs::stoval(*static_cast<const std::string*>(from), result) != 0;
        } break;
        case variant_id::vector4d: {
            const auto& v = *static_cast<const vec4*>(from);
            result = quat(v.x(), v.y(), v.z(), v.w());
        } break;
        default: return false;
    }
    return true;
}

bool uxs::variant_type_impl<quat>::convert_to(variant_id type, void* to, const void* from) {
    const auto& v = *static_cast<const quat*>(from);
    switch (type) {
        case variant_id::string: {
            *static_cast<std::string*>(to) = uxs::to_string(v);
        } break;
        case variant_id::vector4d: {
            *static_cast<vec4*>(to) = vec4(v.x(), v.y(), v.z(), v.w());
        } break;
        default: return false;
    }
    return true;
}
