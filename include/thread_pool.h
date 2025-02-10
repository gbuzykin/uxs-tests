#pragma once

#include "uxs/dllist.h"
#include "uxs/utility.h"

#include <condition_variable>
#include <future>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

namespace uxs_test_suite {

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

class work_item_base : protected uxs::dllist_node_t {
 public:
    friend class test_thread_pool;
    virtual ~work_item_base() = default;
    virtual void run() = 0;
};

class test_thread_pool : public thread_pool<test_thread_pool, uxs::dllist_node_t> {
 public:
    explicit test_thread_pool(unsigned worker_count)
        : thread_pool<test_thread_pool, uxs::dllist_node_t>("", worker_count) {}
    static void work(uxs::dllist_node_t& node, unsigned) {
        auto& item = static_cast<work_item_base&>(node);
        item.run();
    }
};

template<typename Fn, typename... Args>
class work_item : public work_item_base {
 public:
    using return_type = decltype(std::declval<typename std::decay<Fn>::type>()(std::declval<Args>()...));

    explicit work_item(Fn&& func, Args&&... args) : func_(std::forward<Fn>(func)), args_(std::forward<Args>(args)...) {}
    work_item(const work_item&) = delete;
    work_item& operator=(const work_item&) = delete;

    void submit(test_thread_pool& pool) { pool.queue(*this); }

    std::future<return_type> get_future() { return promise_.get_future(); }

    void run() override {
        try {
            run(std::index_sequence_for<Args...>{}, std::is_same<return_type, void>{});
        } catch (...) {
            try {
                promise_.set_exception(std::current_exception());
            } catch (...) {}
        }
    }

 private:
    Fn func_;
    std::tuple<typename std::decay<Args>::type...> args_;
    std::promise<return_type> promise_;

    template<std::size_t... Indices>
    void run(std::index_sequence<Indices...>, std::false_type) {
        promise_.set_value(func_(std::get<Indices>(args_)...));
    }

    template<std::size_t... Indices>
    void run(std::index_sequence<Indices...>, std::true_type) {
        func_(std::get<Indices>(args_)...);
        promise_.set_value();
    }
};

template<typename Fn, typename... Args>
work_item<Fn, Args...> make_work_item(Fn&& func, Args&&... args) {
    return work_item<Fn, Args...>(std::forward<Fn>(func), std::forward<Args>(args)...);
}

}  // namespace uxs_test_suite
