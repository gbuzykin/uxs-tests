#pragma once

#include <uxs/dllist.h>
#include <uxs/utility.h>

#include <atomic>
#include <condition_variable>
#include <exception>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

namespace uxs {

template<typename ThreadPoolDerived, typename WorkListNodeTy>
class thread_pool {
 public:
    thread_pool(std::string name, unsigned worker_count) : name_(std::move(name)), head_{&head_, &head_} {
        workers_.reserve(worker_count);
        for (unsigned n_worker = 0; n_worker < worker_count; ++n_worker) {
            workers_.emplace_back(std::thread(run, std::ref(*this), n_worker));
        }
    }

    virtual ~thread_pool() { terminate(); }

    thread_pool(const thread_pool&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;

    WorkListNodeTy* terminate() {
        WorkListNodeTy* next = nullptr;
        std::unique_lock<std::mutex> lk(mutex_);
        if (terminate_) { return nullptr; }
        if (head_.next != &head_) { next = head_.next, head_.prev->next = nullptr; }
        head_.next = head_.prev = &head_;
        terminate_ = true;
        lk.unlock();
        cv_.notify_all();
        for (unsigned n_worker = 0; n_worker < workers_.size(); ++n_worker) {
            if (workers_[n_worker].joinable()) { workers_[n_worker].join(); }
        }
        return next;
    }

    bool queue(WorkListNodeTy& item) {
        std::unique_lock<std::mutex> lk(mutex_);
        if (terminate_) { return false; }
        item.next = &head_;
        item.prev = head_.prev;
        head_.prev->next = &item;
        head_.prev = &item;
        lk.unlock();
        cv_.notify_one();
        return true;
    }

 private:
    std::string name_;
    WorkListNodeTy head_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool terminate_ = false;

    std::vector<std::thread> workers_;

    WorkListNodeTy* extract() {
        std::unique_lock<std::mutex> lk(mutex_);
        cv_.wait(lk, [this]() { return terminate_ || head_.next != &head_; });
        if (terminate_) { return nullptr; }
        WorkListNodeTy* item = head_.next;
        WorkListNodeTy* next = item->next;
        head_.next = next;
        next->prev = &head_;
        return item;
    }

    static void run(thread_pool& thread_pool, unsigned n_worker) {
        while (WorkListNodeTy* item = thread_pool.extract()) {
            if (!item) { break; }  // terminate
            static_cast<ThreadPoolDerived&>(thread_pool).work(*item, n_worker);
        }
    }
};

template<typename Ty>
class promise;

template<typename Ty>
class future;

namespace detail {

template<typename Ty>
class promise_base {
 public:
    promise_base() = default;
    virtual ~promise_base() = default;

    promise_base(const promise_base&) = delete;
    promise_base& operator=(const promise_base&) = delete;

    future<Ty> get_future();

    template<typename Ty_ = Ty>
    void set_value(const std::enable_if_t<!std::is_same<Ty_, void>::value, Ty>& v) {
        static_cast<promise<Ty>&>(*this).set_value_impl(v);
    }

    template<typename Ty_ = Ty>
    void set_value(std::enable_if_t<!std::is_same<Ty_, void>::value, Ty>&& v) {
        static_cast<promise<Ty>&>(*this).set_value_impl(std::move(v));
    }

    template<typename Ty_ = Ty>
    std::enable_if_t<std::is_same<Ty_, void>::value> set_value() {
        std::unique_lock<std::mutex> lk(mutex_);
        is_finished_ = true;
        cv_.notify_all();
        if (!autodelete_) { return; }
        lk.unlock();
        unref();
    }

 protected:
    void set_autodelete() { autodelete_ = true; }
    void ref() { ++ref_count_; }
    void unref() {
        if (!--ref_count_) { delete this; }
    }

    template<typename Ty_>
    void set_value_impl(Ty_&& v) {
        std::unique_lock<std::mutex> lk(mutex_);
        static_cast<promise<Ty>&>(*this).assign_value_impl(std::move(v));
        is_finished_ = true;
        cv_.notify_all();
        if (!autodelete_) { return; }
        lk.unlock();
        unref();
    }

    void wait() {
        std::unique_lock<std::mutex> lk(mutex_);
        cv_.wait(lk, [this]() { return is_finished_; });
    }

    template<class Rep, class Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& rel_time) {
        std::unique_lock<std::mutex> lk(mutex_);
        return cv_.wait_for(lk, rel_time, [this]() { return is_finished_; });
    }

    template<typename Ty_ = Ty>
    std::enable_if_t<!std::is_same<Ty_, void>::value, Ty> get() {
        std::unique_lock<std::mutex> lk(mutex_);
        cv_.wait(lk, [this]() { return is_finished_; });
        return static_cast<promise<Ty>&>(*this).get_value_impl();
    }

    template<typename Ty_ = Ty>
    std::enable_if_t<std::is_same<Ty_, void>::value> get() {
        wait();
    }

 private:
    friend class future<Ty>;

    bool is_finished_ = false;
    bool autodelete_ = false;
    std::atomic<std::uint64_t> ref_count_{1};
    std::mutex mutex_;
    std::condition_variable cv_;
};
}  // namespace detail

template<typename Ty>
class promise : public detail::promise_base<Ty> {
 private:
    friend class detail::promise_base<Ty>;

    alignas(std::alignment_of<Ty>::value) std::uint8_t val_[sizeof(Ty)];

    template<typename Ty_>
    void assign_value_impl(Ty_&& v) {
        new (&val_) Ty(std::forward<Ty_>(v));
    }

    Ty get_value_impl() { return std::move(*reinterpret_cast<Ty*>(&val_)); }
};

template<>
class promise<void> : public detail::promise_base<void> {};

template<typename Ty>
class future {
 public:
    future() = default;
    future(const future& f) : ref_(f.ref_) {
        if (ref_) { ref_->ref(); }
    }
    future(future&& f) : ref_(f.ref_) { f.ref_ = nullptr; }
    ~future() { reset(); }

    future& operator=(const future& f) {
        if (this == &f) { return *this; }
        if (f.ref_) { f.ref_->ref(); }
        if (ref_) { ref_->unref(); }
        ref_ = f.ref_;
        return *this;
    }

    future& operator=(future&& f) {
        if (this == &f) { return *this; }
        if (ref_) { ref_->unref(); }
        ref_ = f.ref_;
        f.ref_ = nullptr;
        return *this;
    }

    bool valid() const { return ref_; }

    void reset() {
        if (!ref_) { return; }
        ref_->unref();
        ref_ = nullptr;
    }

    void wait() {
        if (!ref_) { throw std::logic_error("invalid future"); }
        ref_->wait();
    }

    template<class Rep, class Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& rel_time) {
        if (!ref_) { throw std::logic_error("invalid future"); }
        return ref_->wait_for(rel_time);
    }

    Ty get() {
        if (!ref_) { throw std::logic_error("invalid future"); }
        return ref_->get();
    }

 private:
    friend class detail::promise_base<Ty>;
    explicit future(promise<Ty>* p) : ref_(p) { p->ref(); }
    promise<Ty>* ref_ = nullptr;
};

template<typename Ty>
future<Ty> detail::promise_base<Ty>::get_future() {
    return future<Ty>(static_cast<promise<Ty>*>(this));
}

class work_item_base : protected uxs::dllist_node_t {
 protected:
    friend class test_thread_pool;
    virtual ~work_item_base() = default;
    virtual void run() = 0;
};

class test_thread_pool : public thread_pool<test_thread_pool, uxs::dllist_node_t> {
 public:
    explicit test_thread_pool(unsigned worker_count)
        : thread_pool<test_thread_pool, uxs::dllist_node_t>("", worker_count) {}
    bool queue(work_item_base& item) { return thread_pool<test_thread_pool, uxs::dllist_node_t>::queue(item); }
    static void work(uxs::dllist_node_t& node, unsigned) { static_cast<work_item_base&>(node).run(); }
};

#if __cplusplus < 201703L
template<typename Fn, typename... Args>
using return_type_t = typename std::result_of<Fn(Args...)>::type;
#else
template<typename Fn, typename... Args>
using return_type_t = std::invoke_result_t<Fn, Args...>;
#endif

template<typename Fn, typename... Args>
class work_item : public work_item_base, public promise<return_type_t<Fn, Args...>> {
 public:
    using return_type = return_type_t<Fn, Args...>;

    template<typename Fn_, typename... Args_>
    explicit work_item(Fn_&& func, Args_&&... args)
        : func_(std::forward<Fn_>(func)), args_(std::forward<Args_>(args)...) {}

    void set_autodelete() { promise<return_type_t<Fn, Args...>>::set_autodelete(); }

 private:
    Fn func_;
    std::tuple<Args...> args_;

    void run() override { run(std::index_sequence_for<Args...>{}, std::is_same<return_type, void>{}); }

    template<std::size_t... Indices>
    void run(std::index_sequence<Indices...>, std::false_type) {
        this->set_value(func_(std::get<Indices>(args_)...));
    }

    template<std::size_t... Indices>
    void run(std::index_sequence<Indices...>, std::true_type) {
        func_(std::get<Indices>(args_)...);
        this->set_value();
    }
};

template<typename Fn, typename... Args>
work_item<Fn, Args...> make_work_item(Fn&& func, Args&&... args) {
    return work_item<typename std::decay<Fn>::type, typename std::decay<Args>::type...>(std::forward<Fn>(func),
                                                                                        std::forward<Args>(args)...);
}

template<typename Fn, typename... Args>
future<return_type_t<Fn, Args...>> async(test_thread_pool& pool, Fn&& func, Args&&... args) {
    auto* item = new work_item<typename std::decay<Fn>::type, typename std::decay<Args>::type...>(
        std::forward<Fn>(func), std::forward<Args>(args)...);
    auto future = item->get_future();
    item->set_autodelete();
    pool.queue(*item);
    return future;
}

}  // namespace uxs
