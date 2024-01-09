#pragma once

#include "uxs/common.h"

#include <memory>
#include <string>

namespace uxs_test_suite {

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

    T(T&& t) noexcept : text(std::move(t.text)) {
        ++instance_count;
        t.text.clear();
    }
    T& operator=(T&& t) noexcept {
        if (!text.empty()) { --not_empty_count; }
        text = std::move(t.text);
        t.text.clear();
        return *this;
    }

    bool empty() const { return text.empty(); }
    void set(int a) {
        if (!text.empty()) { --not_empty_count; }
        text = std::to_string(a);
        if (!text.empty()) { ++not_empty_count; }
    }

    T& operator=(const int&) = delete;
    T& operator=(int&&) = delete;
    T& operator=(int) = delete;

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
        T::operator=(std::move(t));
        return *this;
    }

    T& operator=(const int&) = delete;
    T& operator=(int&&) = delete;
    T& operator=(int) = delete;
};

}  // namespace uxs_test_suite
