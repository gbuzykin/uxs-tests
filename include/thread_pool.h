#pragma once

#include <uxs/dllist.h>
#include <uxs/optional.h>
#include <uxs/utility.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace util {

class thread_pool;

template<typename Ty>
class future;

namespace detail {

class spin_mutex {
 public:
    void lock() {
        while (flag_.test_and_set(std::memory_order_acquire)) {}
    }
    void unlock() { flag_.clear(std::memory_order_release); }

 private:
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};

template<typename ThreadPool, typename WorkListNodeTy>
class thread_pool_base {
 public:
    thread_pool_base(std::string name, unsigned worker_count) : name_(std::move(name)), head_{&head_, &head_} {
        workers_.reserve(worker_count);
        for (unsigned n_worker = 0; n_worker < worker_count; ++n_worker) {
            workers_.emplace_back(std::thread(run, std::ref(*this)));
        }
    }

    virtual ~thread_pool_base() { terminate(); }

    thread_pool_base(const thread_pool_base&) = delete;
    thread_pool_base& operator=(const thread_pool_base&) = delete;

    WorkListNodeTy* terminate() {
        decltype(head_.next) next = nullptr;
        std::unique_lock<spin_mutex> lk(mutex_);
        if (terminate_) { return nullptr; }
        if (!uxs::dllist_is_empty(&head_)) {
            next = head_.next;
            uxs::dllist_make_cycle(head_.next, head_.prev);
            uxs::dllist_make_cycle(&head_);
        }
        terminate_ = true;
        lk.unlock();
        cv_.notify_all();
        for (unsigned n_worker = 0; n_worker < workers_.size(); ++n_worker) {
            if (workers_[n_worker].joinable()) { workers_[n_worker].join(); }
        }
        return next;
    }

    bool queue(WorkListNodeTy& item) {
        std::unique_lock<spin_mutex> lk(mutex_);
        if (terminate_) { return false; }
        uxs::dllist_insert_before(&head_, &item);
        lk.unlock();
        cv_.notify_one();
        return true;
    }

 private:
    std::string name_;
    bool terminate_ = false;
    WorkListNodeTy head_;
    spin_mutex mutex_;
    std::condition_variable_any cv_;

    std::vector<std::thread> workers_;

    WorkListNodeTy* extract() {
        std::unique_lock<spin_mutex> lk(mutex_);
        cv_.wait(lk, [this]() { return terminate_ || !uxs::dllist_is_empty(&head_); });
        if (terminate_) { return nullptr; }
        auto* item = head_.next;
        uxs::dllist_remove(item);
        return item;
    }

    static void run(thread_pool_base& thread_pool_base) {
        while (auto* item = thread_pool_base.extract()) {
            if (!item) { break; }  // terminate
            static_cast<ThreadPool&>(thread_pool_base).work(*item);
        }
    }
};

class promise_base {
 public:
    promise_base() = default;
    virtual ~promise_base() = default;

    promise_base(const promise_base&) = delete;
    promise_base& operator=(const promise_base&) = delete;

 protected:
    bool is_signaled() const noexcept { return is_signaled_; }
    void set_autodelete() noexcept { autodelete_ = true; }
    void ref() noexcept { ++ref_count_; }
    void unref() noexcept {
        if (!--ref_count_) { delete this; }
    }

    void signal() {
        std::unique_lock<spin_mutex> lk(mutex_);
        is_signaled_ = true;
        cv_.notify_all();
        lk.unlock();
        if (autodelete_) { unref(); }
    }

    void wait() {
        std::unique_lock<spin_mutex> lk(mutex_);
        cv_.wait(lk, [this]() { return is_signaled_; });
    }

    template<class Rep, class Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& rel_time) {
        std::unique_lock<spin_mutex> lk(mutex_);
        return cv_.wait_for(lk, rel_time, [this]() { return is_signaled_; });
    }

    void wait_and_clear() {
        std::unique_lock<spin_mutex> lk(mutex_);
        cv_.wait(lk, [this]() { return is_signaled_; });
        is_signaled_ = false;
    }

    bool try_wait_and_clear() {
        std::unique_lock<spin_mutex> lk(mutex_);
        bool result = false;
        std::swap(is_signaled_, result);
        return result;
    }

 private:
    template<typename Ty>
    friend class util::future;

    std::atomic<std::uint64_t> ref_count_{1};
    bool is_signaled_ = false;
    bool autodelete_ = false;
    spin_mutex mutex_;
    std::condition_variable_any cv_;
};

class work_item_base : protected uxs::dllist_node_t {
 protected:
    friend class util::thread_pool;
    virtual ~work_item_base() = default;
    virtual void run() = 0;
};

}  // namespace detail

template<typename Ty>
class promise;

template<typename Ty>
class future {
 public:
    future() noexcept = default;
    future(const future& other) noexcept : ref_(other.ref_) {
        if (ref_) { ref_->ref(); }
    }
    future(future&& other) noexcept : ref_(other.ref_) { other.ref_ = nullptr; }
    ~future() {
        if (ref_) { ref_->unref(); }
    }

    future& operator=(const future& other) noexcept {
        if (this == &other) { return *this; }
        if (other.ref_) { other.ref_->ref(); }
        if (ref_) { ref_->unref(); }
        ref_ = other.ref_;
        return *this;
    }

    future& operator=(future&& other) noexcept {
        if (this == &other) { return *this; }
        if (ref_) { ref_->unref(); }
        ref_ = other.ref_;
        other.ref_ = nullptr;
        return *this;
    }

    bool valid() const noexcept { return ref_; }

    void wait() {
        if (!ref_) { invalid_future_error(); }
        ref_->wait();
    }

    template<class Rep, class Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& rel_time) {
        if (!ref_) { invalid_future_error(); }
        return ref_->wait_for(rel_time);
    }

    Ty get() {
        if (!ref_) { invalid_future_error(); }
        return get_dispatch(std::is_same<Ty, void>());
    }

    std::conditional_t<std::is_same<Ty, void>::value, bool, est::optional<Ty>> try_get() {
        if (!ref_) { invalid_future_error(); }
        auto result = ref_->try_get();
        if (!result) { return result; }
        ref_->unref();
        ref_ = nullptr;
        return result;
    }

 private:
    friend class promise<Ty>;
    promise<Ty>* ref_ = nullptr;
    explicit future(promise<Ty>* p) : ref_(p) { p->ref(); }

    [[noreturn]] void invalid_future_error() { throw std::logic_error("invalid future"); }

    template<typename Ty_ = Ty>
    Ty_ get_dispatch(std::true_type /* Ty == void */) {
        ref_->get();
        ref_->unref();
        ref_ = nullptr;
    }

    template<typename Ty_ = Ty>
    Ty_ get_dispatch(std::false_type /* Ty == void */) {
        Ty result = ref_->get();
        ref_->unref();
        ref_ = nullptr;
        return result;
    }
};

template<typename Ty>
class promise : public detail::promise_base {
 public:
    ~promise() override {
        if (is_signaled()) { destroy(); }
    }

    future<Ty> get_future() noexcept { return future<Ty>(this); }

    template<typename V>
    void set_value(V&& v) {
        new (&val_) Ty(std::forward<V>(v));
        signal();
    }

 private:
    friend class future<Ty>;
    alignas(std::alignment_of<Ty>::value) std::uint8_t val_[sizeof(Ty)];

    Ty get() {
        wait_and_clear();
        auto result = std::move(*reinterpret_cast<Ty*>(&val_));
        destroy();
        return result;
    }

    est::optional<Ty> try_get() {
        if (!try_wait_and_clear()) { return est::nullopt; }
        auto result = std::move(*reinterpret_cast<Ty*>(&val_));
        destroy();
        return result;
    }

    void destroy() noexcept { reinterpret_cast<Ty*>(&val_)->~Ty(); }
};

template<>
class promise<void> : public detail::promise_base {
 public:
    future<void> get_future() noexcept { return future<void>(this); }
    void set_value() { signal(); }

 private:
    friend class future<void>;
    void get() { wait_and_clear(); }
    bool try_get() { return try_wait_and_clear(); }
};

class thread_pool : public detail::thread_pool_base<thread_pool, uxs::dllist_node_t> {
 public:
    thread_pool(std::string name, unsigned worker_count)
        : detail::thread_pool_base<thread_pool, uxs::dllist_node_t>(std::move(name), worker_count) {}

    bool queue(detail::work_item_base& item) {
        return detail::thread_pool_base<thread_pool, uxs::dllist_node_t>::queue(item);
    }

    void work(uxs::dllist_node_t& node) {
        static_cast<detail::work_item_base&>(node).run();
        std::unique_lock<detail::spin_mutex> lk(sem_mutex_);
        --sem_counter_;
        lk.unlock();
        sem_cv_.notify_one();
    }

    void set_semaphore(std::size_t counter) {
        std::unique_lock<detail::spin_mutex> lk(sem_mutex_);
        sem_counter_ = counter;
    }

    void acquire_semaphore(std::size_t counter) {
        std::unique_lock<detail::spin_mutex> lk(sem_mutex_);
        sem_cv_.wait(lk, [this, counter]() { return sem_counter_ <= counter; });
        ++sem_counter_;
    }

 private:
    std::size_t sem_counter_ = 0;
    detail::spin_mutex sem_mutex_;
    std::condition_variable_any sem_cv_;
};

#if __cplusplus < 201703L
template<typename Fn, typename... Args>
using return_type_t = typename std::result_of<Fn(Args...)>::type;
#else
template<typename Fn, typename... Args>
using return_type_t = std::invoke_result_t<Fn, Args...>;
#endif

template<typename Fn, typename... Args>
class work_item : public detail::work_item_base, public promise<return_type_t<Fn, Args...>> {
 public:
    using return_type = return_type_t<Fn, Args...>;

    template<typename Fn_, typename... Args_>
    explicit work_item(Fn_&& func, Args_&&... args)
        : func_(std::forward<Fn_>(func)), args_(std::forward<Args_>(args)...) {}

    void set_autodelete() { promise<return_type_t<Fn, Args...>>::set_autodelete(); }

 private:
    Fn func_;
    std::tuple<Args...> args_;

    void run() override { run_dispatch(std::index_sequence_for<Args...>(), std::is_same<return_type, void>()); }

    template<std::size_t... Indices>
    void run_dispatch(std::index_sequence<Indices...>, std::true_type /* Ty == void */) {
        func_(std::get<Indices>(args_)...);
        this->set_value();
    }

    template<std::size_t... Indices>
    void run_dispatch(std::index_sequence<Indices...>, std::false_type /* Ty == void */) {
        this->set_value(func_(std::get<Indices>(args_)...));
    }
};

template<typename Fn, typename... Args>
using work_item_t = work_item<std::decay_t<Fn>, std::decay_t<Args>...>;

template<typename Fn, typename... Args>
work_item<Fn, Args...> make_work_item(Fn&& func, Args&&... args) {
    return work_item_t<Fn, Args...>(std::forward<Fn>(func), std::forward<Args>(args)...);
}

template<typename Fn, typename... Args>
future<return_type_t<Fn, Args...>> async(thread_pool& pool, Fn&& func, Args&&... args) {
    auto* item = ::new work_item_t<Fn, Args...>(std::forward<Fn>(func), std::forward<Args>(args)...);
    auto future = item->get_future();
    item->set_autodelete();
    pool.queue(*item);
    return future;
}

}  // namespace util
