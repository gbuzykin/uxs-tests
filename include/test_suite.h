#pragma once

#include <uxs/format.h>

#include <chrono>
#include <stdexcept>

#define ADD_TEST_CASE(cat, group, fn) \
    static uxs_test_suite::TestCase UXS_TOKENPASTE2(g_test_case_, __LINE__)(cat, group, fn)

#define VERIFY(...) \
    do { \
        if (!(__VA_ARGS__)) { \
            throw std::runtime_error(uxs_test_suite::report_error(__FILE__, __LINE__, #__VA_ARGS__)); \
        } \
    } while (false)

namespace uxs_test_suite {

using curr_clock_t = std::chrono::high_resolution_clock;
template<class Rep, class Period>
int64_t as_ns_duration(const std::chrono::duration<Rep, Period>& d) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(d).count();
}

std::string report_error(const char* file, int line, const char* msg);

struct TestCase {
    static const TestCase* first_avail;
    const TestCase* next_avail;
    TestCase(std::string cat, std::string group, int (*fn)())
        : next_avail(first_avail), category(std::move(cat)), group_name(std::move(group)), test(fn) {
        first_avail = this;
    }
    std::string category;
    std::string group_name;
    int (*test)();
};

template<typename Ty, typename Alloc = std::allocator<Ty>>
class not_relocatable_vector : protected std::allocator_traits<Alloc>::template rebind_alloc<Ty> {
 private:
    static_assert(std::is_same<std::remove_cv_t<Ty>, Ty>::value,
                  "not_relocatable_vector must have a non-const, non-volatile value type");

    using alloc_type = typename std::allocator_traits<Alloc>::template rebind_alloc<Ty>;
    using alloc_traits = std::allocator_traits<alloc_type>;

 public:
    using value_type = Ty;
    using allocator_type = Alloc;
    using size_type = typename alloc_traits::size_type;
    using difference_type = typename alloc_traits::difference_type;
    using pointer = typename alloc_traits::pointer;
    using const_pointer = typename alloc_traits::const_pointer;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = uxs::array_iterator<not_relocatable_vector, pointer, false>;
    using const_iterator = uxs::array_iterator<not_relocatable_vector, pointer, true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    explicit not_relocatable_vector(size_type capacity, const allocator_type& alloc = allocator_type())
        : alloc_type(alloc) {
        if (!capacity) { return; }
        if (capacity > alloc_traits::max_size(*this)) { throw std::length_error("too much to reserve"); }
        v_.begin = v_.end = alloc_traits::allocate(*this, capacity);
        v_.boundary = v_.begin + capacity;
    }

    not_relocatable_vector(const not_relocatable_vector&) = delete;
    not_relocatable_vector& operator=(const not_relocatable_vector&) = delete;

    not_relocatable_vector(not_relocatable_vector&& other) noexcept : alloc_type(std::move(other)) {
        v_ = other.v_;
        other.v_.nullify();
    }

    ~not_relocatable_vector() { tidy(); }

    void swap(not_relocatable_vector& other) noexcept {
        if (std::addressof(other) == this) { return; }
        swap_impl(other, typename alloc_traits::propagate_on_container_swap());
    }

    allocator_type get_allocator() const noexcept { return allocator_type(*this); }

    bool empty() const noexcept { return v_.end == v_.begin; }
    size_type size() const noexcept { return static_cast<size_type>(v_.end - v_.begin); }
    size_type capacity() const noexcept { return static_cast<size_type>(v_.boundary - v_.begin); }
    size_type max_size() const noexcept { return alloc_traits::max_size(*this); }

    iterator begin() noexcept { return iterator(v_.begin, v_.begin, v_.end); }
    const_iterator begin() const noexcept { return const_iterator(v_.begin, v_.begin, v_.end); }
    const_iterator cbegin() const noexcept { return begin(); }

    iterator end() noexcept { return iterator(v_.end, v_.begin, v_.end); }
    const_iterator end() const noexcept { return const_iterator(v_.end, v_.begin, v_.end); }
    const_iterator cend() const noexcept { return end(); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return rend(); }

    pointer data() noexcept { return v_.begin; }
    const_pointer data() const noexcept { return v_.begin; }

    reference operator[](size_type i) {
        assert(i < size());
        return v_.begin[i];
    }
    const_reference operator[](size_type i) const {
        assert(i < size());
        return v_.begin[i];
    }

    reference at(size_type i) {
        if (i < size()) { return v_.begin[i]; }
        throw std::out_of_range("index out of range");
    }
    const_reference at(size_type i) const {
        if (i < size()) { return v_.begin[i]; }
        throw std::out_of_range("index out of range");
    }

    reference front() {
        assert(v_.begin != v_.end);
        return *begin();
    }
    const_reference front() const {
        assert(v_.begin != v_.end);
        return *begin();
    }

    reference back() {
        assert(v_.begin != v_.end);
        return *rbegin();
    }
    const_reference back() const {
        assert(v_.begin != v_.end);
        return *rbegin();
    }

    void clear() noexcept { v_.end = helpers::truncate(*this, v_.begin, v_.end); }

    void push_back(const value_type& val) { emplace_back(val); }
    void push_back(value_type&& val) { emplace_back(std::move(val)); }
    template<typename... Args>
    reference emplace_back(Args&&... args) {
        if (v_.end != v_.boundary) {
            alloc_traits::construct(*this, std::addressof(*v_.end), std::forward<Args>(args)...);
            return *v_.end++;
        }
        throw std::runtime_error("insufficient capacity");
    }

    void pop_back() {
        assert(v_.begin != v_.end);
        alloc_traits::destroy(*this, std::addressof(*(--v_.end)));
    }

 private:
    struct vector_ptrs_t {
        vector_ptrs_t() = default;
        vector_ptrs_t(pointer p1, pointer p2, pointer p3) : begin(p1), end(p2), boundary(p3) {}
        void nullify() { begin = end = boundary = nullptr; }
        pointer begin{nullptr};
        pointer end{nullptr};
        pointer boundary{nullptr};
    };

    vector_ptrs_t v_;

    void tidy() {
        if (v_.begin != v_.boundary) {
            v_.end = helpers::truncate(*this, v_.begin, v_.end);
            alloc_traits::deallocate(*this, v_.begin, static_cast<size_type>(v_.boundary - v_.begin));
        }
        v_.nullify();
    }

    void swap_impl(not_relocatable_vector& other, std::false_type) noexcept { std::swap(v_, other.v_); }
    void swap_impl(not_relocatable_vector& other, std::true_type) noexcept {
        std::swap(static_cast<alloc_type&>(*this), static_cast<alloc_type&>(other));
        std::swap(v_, other.v_);
    }

    struct helpers {
        static pointer truncate(alloc_type& alloc, pointer first, pointer last) {
            return helpers::truncate_impl(alloc, first, last, std::is_trivially_destructible<Ty>());
        }

        static pointer truncate_impl(std::allocator<Ty>& alloc, pointer first, pointer last,
                                     std::true_type /* trivially destructible */) {
            return first;
        }

        template<typename Ty_ = Ty>
        static pointer truncate_impl(
            std::enable_if_t<!std::is_same<alloc_type, std::allocator<Ty_>>::value, alloc_type>& alloc, pointer first,
            pointer last, std::true_type /* trivially destructible */) {
            return helpers::truncate_impl(alloc, first, last, std::false_type());
        }

        static pointer truncate_impl(alloc_type& alloc, pointer first, pointer last,
                                     std::false_type /* trivially destructible */) {
            for (auto p = first; p != last; ++p) { alloc_traits::destroy(alloc, std::addressof(*p)); }
            return first;
        }
    };
};

}  // namespace uxs_test_suite
