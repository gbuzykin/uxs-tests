#pragma once

#include "test_allocators.h"
#include "test_suite.h"
#include "test_types.h"

#include "uxs/db/value.h"

#define MUST_THROW(x) \
    try { \
        x; \
        VERIFY(false); \
    } catch (const uxs::db::exception&) {}

#define CHECK_ARRAY(...) \
    if (!array_check(__VA_ARGS__)) { \
        throw std::runtime_error(uxs_test_suite::report_error(__FILE__, __LINE__, "db::value mismatched")); \
    }

#define CHECK_RECORD(...) \
    if (!record_check(__VA_ARGS__)) { \
        throw std::runtime_error(uxs_test_suite::report_error(__FILE__, __LINE__, "db::value mismatched")); \
    }

#define CHECK_ARRAY_EMPTY(x) \
    { \
        const auto& __v = x; \
        auto __r = __v.as_array(); \
        if (__v.type() != uxs::db::dtype::kArray || !__v.empty() || __v.size() != 0 || __r.begin() != __r.end()) { \
            throw std::runtime_error( \
                uxs_test_suite::report_error(__FILE__, __LINE__, "db::value is not an empty array")); \
        } \
    }

#define CHECK_RECORD_EMPTY(x) \
    { \
        const auto& __v = x; \
        auto __r = __v.as_record(); \
        if (!__v.empty() || __v.size() != 0 || __r.begin() != __r.end()) { \
            throw std::runtime_error( \
                uxs_test_suite::report_error(__FILE__, __LINE__, "db::value is not an empty record")); \
        } \
    }

namespace uxs_test_suite {

template<typename InputIt>
bool array_check(const uxs::db::value& v, size_t sz, InputIt src) {
    if (v.type() != uxs::db::dtype::kArray || v.size() != sz) { return false; }
    auto r = v.as_array();
    if (std::distance(r.begin(), r.end()) != static_cast<ptrdiff_t>(sz)) { return false; }
    for (auto it = r.begin(); it != r.end(); ++it) {
        if (!(*it == uxs::db::value(*src++))) { return false; }
    }
    return true;
}

template<typename InputIt>
bool record_check(const uxs::db::value& v, size_t sz, InputIt src) {
    if (v.type() != uxs::db::dtype::kRecord || v.size() != sz) { return false; }
    auto r = v.as_record();
    if (std::distance(r.begin(), r.end()) != static_cast<ptrdiff_t>(sz)) { return false; }
    for (auto it = r.begin(); it != r.end(); ++it, ++src) {
        if (!(it->first == src->first && it->second == uxs::db::value(src->second))) { return false; }
    }
    return true;
}

}  // namespace uxs_test_suite
