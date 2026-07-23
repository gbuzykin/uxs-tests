#include "test_suite.h"

#include "fmt/compile.h"
#include "fmt/format.h"

#if __cplusplus >= 201703L && UXS_HAS_INCLUDE(<charconv>)
#    include <charconv>
#    define has_cpp_lib_charconv 1
#    if defined(_MSC_VER) || __GNUC__ >= 11 || defined(_LIBCPP_VERSION)
#        define has_to_chars_implementation_for_floats
#    endif
#    if defined(_MSC_VER) || __GNUC__ >= 11 || defined(_LIBCPP_VERSION)
#        define has_from_chars_implementation_for_floats
#    endif
#endif

#if __cplusplus >= 202002L && UXS_HAS_INCLUDE(<format>)
#    include <format>
#    define has_cpp_lib_format 1
#endif

size_t uxs_to_chars_int64(char* first, char* /*last*/, int64_t val) {
    return static_cast<size_t>(uxs::to_chars(first, val) - first);
}
size_t uxs_from_chars_int64(std::string_view s, int64_t& val) { return uxs::from_string(s, val); }
size_t uxs_to_chars_double(char* first, char* /*last*/, double val) {
    return static_cast<size_t>(uxs::to_chars(first, val) - first);
}
size_t uxs_to_chars_double_prec(char* first, char* /*last*/, double val, int prec) {
    return static_cast<size_t>(uxs::to_chars(first, val, uxs::fmt_opts{uxs::fmt_flags::none, prec}) - first);
}
size_t uxs_from_chars_double(std::string_view s, double& val) { return uxs::from_string(s, val); }

#if defined(has_cpp_lib_charconv)
size_t std_to_chars_int64(char* first, char* last, int64_t val) {
    return static_cast<size_t>(std::to_chars(first, last, val).ptr - first);
}
size_t std_from_chars_int64(std::string_view s, int64_t& val) {
    return static_cast<size_t>(std::from_chars(s.data(), s.data() + s.size(), val).ptr - s.data());
}
#    if defined(has_to_chars_implementation_for_floats)
size_t std_to_chars_double(char* first, char* last, double val) {
    return static_cast<size_t>(std::to_chars(first, last, val).ptr - first);
}
size_t std_to_chars_double_prec(char* first, char* last, double val, int prec) {
    return static_cast<size_t>(std::to_chars(first, last, val, std::chars_format::general, prec).ptr - first);
}
#    endif
#    if defined(has_from_chars_implementation_for_floats)
size_t std_from_chars_double(std::string_view s, double& val) {
    return static_cast<size_t>(std::from_chars(s.data(), s.data() + s.size(), val).ptr - s.data());
}
#    endif
#endif

size_t fmt_format_to_int64(char* first, char* /*last*/, int64_t val) {
    return static_cast<size_t>(fmt::format_to(first, FMT_COMPILE("{}"), val) - first);
}
size_t fmt_format_to_double(char* first, char* /*last*/, double val) {
    return static_cast<size_t>(fmt::format_to(first, FMT_COMPILE("{}"), val) - first);
}
size_t fmt_format_to_double_prec(char* first, char* /*last*/, double val, int prec) {
    return static_cast<size_t>(fmt::format_to(first, FMT_COMPILE("{:.{}}"), val, prec) - first);
}

#if defined(has_cpp_lib_format)
size_t std_format_to_int64(char* first, char* /*last*/, int64_t val) {
    return static_cast<size_t>(std::format_to(first, "{}", val) - first);
}
size_t std_format_to_double(char* first, char* /*last*/, double val) {
    return static_cast<size_t>(std::format_to(first, "{}", val) - first);
}
size_t std_format_to_double_prec(char* first, char* /*last*/, double val, int prec) {
    return static_cast<size_t>(std::format_to(first, "{:.{}}", val, prec) - first);
}
#endif
