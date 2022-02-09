#pragma once

#include "util/common.h"

#include <string>

struct T {
    std::string text;
    static int64_t not_empty_cnt;
    static int64_t inst_cnt;
    static int64_t comp_cnt;

    T() { ++inst_cnt; }
    T(int a) : text(std::to_string(a)) {
        ++inst_cnt;
        ++not_empty_cnt;
    }
    T(const T& t) : text(t.text) {
        ++inst_cnt;
        if (!text.empty()) { ++not_empty_cnt; }
    }
    T(T&& t) NOEXCEPT : text(std::move(t.text)) {
        ++inst_cnt;
        t.text.clear();
    }
    ~T() {
        if (!text.empty()) { --not_empty_cnt; }
        --inst_cnt;
    }

    T& operator=(const T& t) {
        if (!text.empty()) { --not_empty_cnt; }
        text = t.text;
        if (!text.empty()) { ++not_empty_cnt; }
        return *this;
    }
    T& operator=(T&& t) NOEXCEPT {
        if (!text.empty()) { --not_empty_cnt; }
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
    friend bool operator<(const T& t1, const T& t2) {
        ++comp_cnt;
        return static_cast<int>(t1) < static_cast<int>(t2);
    }
    friend bool operator==(const T& t, int a) { return static_cast<int>(t) == a; }
    friend bool operator<(const T& t, int a) {
        ++comp_cnt;
        return static_cast<int>(t) < a;
    }
    friend bool operator==(int a, const T& t) { return a == static_cast<int>(t); }
    friend bool operator<(int a, const T& t) {
        ++comp_cnt;
        return a < static_cast<int>(t);
    }
};
