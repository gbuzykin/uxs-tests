#pragma once

#include "util/common.h"

#include <memory>
#include <string>

namespace util_test_suite {

struct T {
    std::string text;
    static int64_t not_empty_count;
    static int64_t instance_count;

    T() { ++instance_count; }
    T(int a) : text(std::to_string(a)) {
        ++instance_count;
        ++not_empty_count;
    }

    ~T() {
        if (!text.empty()) { --not_empty_count; }
        --instance_count;
    }

    T(const T& t) : text(t.text) {
        ++instance_count;
        if (!text.empty()) { ++not_empty_count; }
    }
    T& operator=(const T& t) {
        if (!text.empty()) { --not_empty_count; }
        text = t.text;
        if (!text.empty()) { ++not_empty_count; }
        return *this;
    }

    T(T&& t) NOEXCEPT : text(std::move(t.text)) {
        ++instance_count;
        t.text.clear();
    }
    T& operator=(T&& t) NOEXCEPT {
        if (!text.empty()) { --not_empty_count; }
        text = std::move(t.text);
        t.text.clear();
        return *this;
    }

    explicit operator std::string() const { return "'" + text + "'"; }
    explicit operator int() const {
        int v = 0;
        for (char ch : text) { v = 10 * v + ch - '0'; }
        return v;
    }

    friend bool operator==(const T& t1, const T& t2) { return static_cast<int>(t1) == static_cast<int>(t2); }
    friend bool operator<(const T& t1, const T& t2) { return static_cast<int>(t1) < static_cast<int>(t2); }
    friend bool operator==(const T& t, int a) { return static_cast<int>(t) == a; }
    friend bool operator<(const T& t, int a) { return static_cast<int>(t) < a; }
    friend bool operator==(int a, const T& t) { return a == static_cast<int>(t); }
    friend bool operator<(int a, const T& t) { return a < static_cast<int>(t); }
};

struct T_ThrowingMove : T {
    T_ThrowingMove() = default;
    T_ThrowingMove(int a) : T(a) {}
    ~T_ThrowingMove() = default;

    T_ThrowingMove(const T_ThrowingMove&) = default;
    T_ThrowingMove& operator=(const T_ThrowingMove&) = default;

    T_ThrowingMove(T_ThrowingMove&& t) : T(std::move(t)) {}
    T_ThrowingMove& operator=(T_ThrowingMove&& t) {
        static_cast<T&>(*this) = std::move(t);
        return *this;
    }
};

struct T_NotAssignable : T {
    T_NotAssignable() = default;
    T_NotAssignable(int a) : T(a) {}
    ~T_NotAssignable() = default;

    T_NotAssignable(const T_NotAssignable&) = default;
    T_NotAssignable& operator=(const T_NotAssignable&) = delete;

    T_NotAssignable(T_NotAssignable&& t) NOEXCEPT : T(std::move(t)) {}
    T_NotAssignable& operator=(T_NotAssignable&&) = delete;
};

struct T_ThrowingMove_NotAssignable : T {
    T_ThrowingMove_NotAssignable() = default;
    T_ThrowingMove_NotAssignable(int a) : T(a) {}
    ~T_ThrowingMove_NotAssignable() = default;

    T_ThrowingMove_NotAssignable(const T_ThrowingMove_NotAssignable&) = default;
    T_ThrowingMove_NotAssignable& operator=(const T_ThrowingMove_NotAssignable&) = delete;

    T_ThrowingMove_NotAssignable(T_ThrowingMove_NotAssignable&& t) : T(std::move(t)) {}
    T_ThrowingMove_NotAssignable& operator=(T_ThrowingMove_NotAssignable&&) = delete;
};

struct T_NothrowDefaultCopyMove {
    std::shared_ptr<std::string> text;

    T_NothrowDefaultCopyMove() NOEXCEPT { ++T::instance_count; }
    T_NothrowDefaultCopyMove(int a) : text(std::make_shared<std::string>(std::to_string(a))) {
        ++T::instance_count;
        ++T::not_empty_count;
    }

    ~T_NothrowDefaultCopyMove() {
        if (text) { --T::not_empty_count; }
        --T::instance_count;
    }

    T_NothrowDefaultCopyMove(const T_NothrowDefaultCopyMove& t) NOEXCEPT : text(t.text) {
        ++T::instance_count;
        if (text) { ++T::not_empty_count; }
    }
    T_NothrowDefaultCopyMove& operator=(const T_NothrowDefaultCopyMove& t) NOEXCEPT {
        if (text) { --T::not_empty_count; }
        text = t.text;
        if (text) { ++T::not_empty_count; }
        return *this;
    }

    T_NothrowDefaultCopyMove(T_NothrowDefaultCopyMove&& t) NOEXCEPT : text(std::move(t.text)) {
        ++T::instance_count;
        t.text = nullptr;
    }
    T_NothrowDefaultCopyMove& operator=(T_NothrowDefaultCopyMove&& t) NOEXCEPT {
        if (text) { --T::not_empty_count; }
        text = std::move(t.text);
        t.text = nullptr;
        return *this;
    }

    explicit operator std::string() const { return "'" + (text ? *text : std::string("")) + "'"; }
    explicit operator int() const {
        int v = 0;
        if (text) {
            for (char ch : *text) { v = 10 * v + ch - '0'; }
        }
        return v;
    }

    friend bool operator==(const T_NothrowDefaultCopyMove& t1, const T_NothrowDefaultCopyMove& t2) {
        return static_cast<int>(t1) == static_cast<int>(t2);
    }
    friend bool operator<(const T_NothrowDefaultCopyMove& t1, const T_NothrowDefaultCopyMove& t2) {
        return static_cast<int>(t1) < static_cast<int>(t2);
    }
    friend bool operator==(const T_NothrowDefaultCopyMove& t, int a) { return static_cast<int>(t) == a; }
    friend bool operator<(const T_NothrowDefaultCopyMove& t, int a) { return static_cast<int>(t) < a; }
    friend bool operator==(int a, const T_NothrowDefaultCopyMove& t) { return a == static_cast<int>(t); }
    friend bool operator<(int a, const T_NothrowDefaultCopyMove& t) { return a < static_cast<int>(t); }
};

struct T_NothrowDefaultCopyMove_NotAssignable : T_NothrowDefaultCopyMove {
    T_NothrowDefaultCopyMove_NotAssignable() NOEXCEPT = default;
    T_NothrowDefaultCopyMove_NotAssignable(int a) : T_NothrowDefaultCopyMove(a) {}
    ~T_NothrowDefaultCopyMove_NotAssignable() = default;

    T_NothrowDefaultCopyMove_NotAssignable(const T_NothrowDefaultCopyMove_NotAssignable&) NOEXCEPT = default;
    T_NothrowDefaultCopyMove_NotAssignable& operator=(const T_NothrowDefaultCopyMove_NotAssignable&) = delete;

    T_NothrowDefaultCopyMove_NotAssignable(T_NothrowDefaultCopyMove_NotAssignable&& t) NOEXCEPT
        : T_NothrowDefaultCopyMove(std::move(t)) {}
    T_NothrowDefaultCopyMove_NotAssignable& operator=(T_NothrowDefaultCopyMove_NotAssignable&&) = delete;
};

}  // namespace util_test_suite
