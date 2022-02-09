#pragma once

#include "util/pool_allocator.h"

template<typename Ty>
class unfriendly_pool_allocator : public util::pool_allocator<Ty> {
 private:
    using super = util::pool_allocator<Ty>;

 public:
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::false_type;

    unfriendly_pool_allocator() = default;
    explicit unfriendly_pool_allocator(uint32_t partition_size) : super(partition_size) {}
    ~unfriendly_pool_allocator() = default;

    template<typename Ty2>
    unfriendly_pool_allocator(const unfriendly_pool_allocator<Ty2>& other) : super(other) {}
    template<typename Ty2>
    unfriendly_pool_allocator& operator=(const unfriendly_pool_allocator<Ty2>& other) {
        super::operator=(other);
        return *this;
    }

    template<typename Ty2>
    unfriendly_pool_allocator(unfriendly_pool_allocator<Ty2>&& other) : super(std::move(other)) {}
    template<typename Ty2>
    unfriendly_pool_allocator& operator=(unfriendly_pool_allocator<Ty2>&& other) {
        super::operator=(std::move(other));
        return *this;
    }
};
