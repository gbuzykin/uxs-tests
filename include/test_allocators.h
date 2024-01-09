#pragma once

#include "uxs/memory.h"

#include <cassert>
#include <stdexcept>
#include <unordered_map>

namespace uxs_test_suite {

struct alloc_stats_t {
    uint32_t ref_count = 1;
    uint64_t alloc_count = 0;
    uint64_t alloc_detected = 0;
    std::unordered_map<void*, size_t> ptrs;
};

template<typename Ty>
class test_allocator {
 public:
    using value_type = typename std::remove_cv<Ty>::type;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::false_type;

    test_allocator() : alloc_stats_(new alloc_stats_t) {}
    ~test_allocator() { reset(nullptr); }

    test_allocator(const test_allocator& other) noexcept : alloc_stats_(other.alloc_stats_) {
        if (alloc_stats_) { ++alloc_stats_->ref_count; }
    }
    test_allocator& operator=(const test_allocator& other) noexcept {
        if (alloc_stats_ != other.alloc_stats_) { reset(other.alloc_stats_); }
        return *this;
    }

    template<typename Ty2>
    test_allocator(const test_allocator<Ty2>& other) noexcept : alloc_stats_(other.alloc_stats_) {
        if (alloc_stats_) { ++alloc_stats_->ref_count; }
    }
    template<typename Ty2>
    test_allocator& operator=(const test_allocator<Ty2>& other) noexcept {
        if (alloc_stats_ != other.alloc_stats_) { reset(other.alloc_stats_); }
        return *this;
    }

    test_allocator select_on_container_copy_construction() const noexcept { return *this; }

    Ty* allocate(size_t sz) {
        alloc_stats_->alloc_count += sz, alloc_stats_->alloc_detected += sz;
        Ty* p = std::allocator<Ty>().allocate(sz);
        alloc_stats_->ptrs.emplace(p, sz);
        return p;
    }

    void deallocate(Ty* p, size_t sz) {
        auto it = alloc_stats_->ptrs.find(p);
        if (it == alloc_stats_->ptrs.end() || it->second != sz) { throw std::runtime_error("invalid deallocation"); }
        alloc_stats_->ptrs.erase(it);
        alloc_stats_->alloc_count -= sz;
        std::allocator<Ty>().deallocate(p, sz);
    }

    void swap(test_allocator& other) noexcept { std::swap(alloc_stats_, other.alloc_stats_); }

    template<typename Ty2>
    bool is_equal_to(const test_allocator<Ty2>& other) const noexcept {
        return alloc_stats_ == other.alloc_stats_;
    }

    uint64_t get_alloc_count() const { return alloc_stats_->alloc_count; }
    uint64_t get_alloc_detected() const { return alloc_stats_->alloc_detected; }
    void reset_alloc_detected() { alloc_stats_->alloc_detected = 0; }

 private:
    template<typename>
    friend class test_allocator;

    alloc_stats_t* alloc_stats_;

    void reset(alloc_stats_t* stats) {
        if (stats) { ++stats->ref_count; }
        if (alloc_stats_ && !--alloc_stats_->ref_count) {
            assert(alloc_stats_->ptrs.empty());
            delete alloc_stats_;
        }
        alloc_stats_ = stats;
    }
};

template<typename Ty1, typename Ty2>
bool operator==(const test_allocator<Ty1>& lhs, const test_allocator<Ty2>& rhs) noexcept {
    return lhs.is_equal_to(rhs);
}
template<typename Ty1, typename Ty2>
bool operator!=(const test_allocator<Ty1>& lhs, const test_allocator<Ty2>& rhs) noexcept {
    return !(lhs == rhs);
}

template<typename Ty>
class unfriendly_test_allocator : public test_allocator<Ty> {
 private:
    using super = test_allocator<Ty>;

 public:
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::false_type;

    unfriendly_test_allocator() = default;
    ~unfriendly_test_allocator() = default;

    unfriendly_test_allocator(const unfriendly_test_allocator& other) noexcept : super(other) {}
    unfriendly_test_allocator& operator=(const unfriendly_test_allocator& other) noexcept {
        super::operator=(other);
        return *this;
    }

    template<typename Ty2>
    unfriendly_test_allocator(const unfriendly_test_allocator<Ty2>& other) noexcept : super(other) {}
    template<typename Ty2>
    unfriendly_test_allocator& operator=(const unfriendly_test_allocator<Ty2>& other) noexcept {
        super::operator=(other);
        return *this;
    }

    unfriendly_test_allocator select_on_container_copy_construction() const noexcept { return *this; }
};

}  // namespace uxs_test_suite
