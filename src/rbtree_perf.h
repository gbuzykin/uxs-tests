#include "rbtree_tests.h"

#include <vector>

template<typename SetType>
int perf_insert(int iter_count) {
    srand(0);

    std::vector<int> v;
    v.reserve(1000);
    for (size_t i = 0; i < 1000; ++i) { v.push_back(rand() % 500); }

    auto start = std::clock();
    SetType s;
    for (int iter = 0; iter < iter_count; ++iter) {
        s.clear();
        for (size_t i = 0; i < 1000; ++i) { s.emplace(v[i]); }
    }
    return static_cast<int>(std::clock() - start);
}

template<typename SetType>
int perf_forward(int iter_count) {
    srand(0);

    SetType s;
    for (size_t i = 0; i < 1000; ++i) { s.emplace(rand()); }

    auto start = std::clock();
    int result = 0;
    for (int iter = 0; iter < iter_count; ++iter) {
        for (auto it = s.begin(); it != s.end(); ++it) { result += static_cast<int>(*it); }
    }
    return result ? static_cast<int>(std::clock() - start) : 0;
}

template<typename SetType>
int perf_backward(int iter_count) {
    srand(0);

    SetType s;
    for (size_t i = 0; i < 1000; ++i) { s.emplace(rand()); }

    auto start = std::clock();
    int result = 0;
    for (int iter = 0; iter < iter_count; ++iter) {
        for (auto it = s.rbegin(); it != s.rend(); ++it) { result += static_cast<int>(*it); }
    }
    return result ? static_cast<int>(std::clock() - start) : 0;
}

template<typename SetType>
int perf_find(int iter_count) {
    srand(0);

    SetType s;
    for (size_t i = 0; i < 1000; ++i) { s.emplace(rand()); }

    std::vector<int> v;
    v.reserve(iter_count);
    for (int iter = 0; iter < iter_count; ++iter) { v.push_back(rand()); }

    auto start = std::clock();
    int result = 0;
    for (int iter = 0; iter < iter_count; ++iter) {
        int val = v[iter];

        auto lower = s.lower_bound(val);
        auto upper = s.upper_bound(val);
        auto range = s.equal_range(val);

        result += lower != s.end() ? static_cast<int>(*lower) : 0;
        result += upper != s.end() ? static_cast<int>(*upper) : 0;
        result += range.first != s.end() ? static_cast<int>(*range.first) : 0;
        result += range.second != s.end() ? static_cast<int>(*range.second) : 0;
    }

    return result ? static_cast<int>(std::clock() - start) : 0;
}

template<typename SetType>
int perf_integral(int iter_count) {
    srand(0);

    std::vector<int> v1, v2;
    v1.reserve(1000);
    v2.reserve(1000);
    for (size_t i = 0; i < 1000; ++i) {
        v1.push_back(rand() % 500);
        v2.push_back(rand() % 500);
    }

    auto start = std::clock();

    SetType s;
    int result = 0;
    for (int iter = 0; iter < iter_count; ++iter) {
        s.clear();
        for (size_t i = 0; i < 1000; ++i) { s.emplace(v1[i]); }

        for (size_t i = 0; i < 30; ++i) {
            for (auto it = s.begin(); it != s.end(); ++it) { result += static_cast<int>(*it); }
            for (auto it = s.rbegin(); it != s.rend(); ++it) { result += static_cast<int>(*it); }
        }

        for (size_t i = 0; i < 1000; ++i) {
            int val = v2[i];

            auto lower = s.lower_bound(val);
            auto upper = s.upper_bound(val);
            auto range = s.equal_range(val);

            result += lower != s.end() ? static_cast<int>(*lower) : 0;
            result += upper != s.end() ? static_cast<int>(*upper) : 0;
            result += range.first != s.end() ? static_cast<int>(*range.first) : 0;
            result += range.second != s.end() ? static_cast<int>(*range.second) : 0;

            s.erase(lower, upper);
        }
    }

    return result ? static_cast<int>(std::clock() - start) : 0;
}
