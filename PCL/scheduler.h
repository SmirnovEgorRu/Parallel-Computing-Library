#ifndef _PCL_SCHEDULER_HEADER_
#define _PCL_SCHEDULER_HEADER_

#include <thread>
#include <future>
#include <vector>
#include <deque>
#include <mutex>

namespace pcl {

    // WRAPPER FOR STD::FUTURE
    template <typename result_t>
    class promise_value {
        std::future<result_t> future_value;

    public:
        result_t get_value() {
            return future_value.get();
        }

        promise_value(std::future<result_t>&& other_future_value) :
            future_value(std::move(other_future_value))
        {}

        promise_value() = default;
    }; // promise_value

}

namespace pcl_impl {

    // QUEUE FOR TASKS
    template <typename T>
    class tasks_queue {
        std::deque<T> container;
        mutable std::mutex queue_mutex;

    public:
        void push(T data) {
            std::lock_guard<std::mutex> lock(queue_mutex);
            container.push_front(std::move(data));
        }

        bool try_pop(T& data) {
            std::lock_guard<std::mutex> lock(queue_mutex);
            if (container.empty())
                return false;
            data = std::move(container.front());
            container.pop_front();
            return true;
        }

        tasks_queue() = default;
        tasks_queue(const tasks_queue& other) = delete;
        tasks_queue& operator= (const tasks_queue& other) = delete;
    }; // queue

    // PROVIDE JOINING OF ALL THE THREADS
    class join_quard {
    private:
        std::vector<std::thread>& threads;

    public:
        explicit join_quard(std::vector<std::thread>& new_threads): threads(new_threads) {}

        ~join_quard() {
            for (size_t i = 0; i < threads.size(); ++i)
                if (threads[i].joinable())
                    threads[i].join();
        }
    }; // join_quard

    // WRAPPER FOR TASKS
    class task_t {
    private:
        struct fun_wrapper_base {
            virtual void call() = 0;
            virtual ~fun_wrapper_base() {};
        };
        
        template <typename function_t>
        struct fun_wrapper:fun_wrapper_base {
            function_t function;

            void call() {
                function();
            }

            fun_wrapper(function_t&& fun): function(std::move(fun))
            {}
        };

        std::unique_ptr<fun_wrapper_base> task;

    public:
        template <typename function_t>
        task_t(function_t&& fun):
            task(new fun_wrapper<function_t>(std::move(fun)))
        {}

        template <typename function_t>
        task_t(fun_wrapper_base&& fun):
            task(new fun_wrapper<function_t>(std::move(fun)))
        {}

        void operator()() { task->call(); }

        task_t(task_t&& other):
            task(std::move(other.task))
        {}

        task_t& operator=(task_t&& other) {
            task = std::move(other.task);
            return *this;
        };

        task_t() = default;
        task_t(const task_t&) = delete;
        task_t(task_t&) = delete;
        task_t& operator=(const task_t& fun) = delete;
    }; // task_t

    // IMPLEMENTATION OF TASK SCHEDULER
    class scheduler_impl {
    private:
        static scheduler_impl* _instance;

        std::vector<std::unique_ptr<pcl_impl::tasks_queue<task_t>>> queues;
        std::vector<std::thread> threads;

        size_t n_threads;

        std::atomic_bool   done = false;
        std::atomic_ullong prepare_count = 0;
        std::atomic_ullong finish_count = 0;

        join_quard join_threads;

        void execute_tasks(const size_t index) {
            size_t steal_number = 0;

            while (!done || !queue_are_empty()) {
                task_t task;

                if (queue_are_empty())
                    std::this_thread::yield();

                else if (queues[index]->try_pop(task))
                    call_task(task);

                else if (queues[(steal_number++) % n_threads]->try_pop(task))
                    call_task(task);
            }
        }

        void call_task(task_t &task) {
            task();
            ++finish_count;
        }

        bool queue_are_empty() {
            if (finish_count < prepare_count) return false;
            return true;
        }

        scheduler_impl() : n_threads(max_threads()), join_threads(threads) {
            for (size_t i = 0; i < n_threads; ++i)
                queues.push_back(std::unique_ptr<pcl_impl::tasks_queue<task_t> >(new pcl_impl::tasks_queue<task_t>));
            for (size_t i = 0; i < n_threads; ++i)
                threads.push_back(std::thread(&pcl_impl ::scheduler_impl::execute_tasks, this, i));
        }

    public:


        template<typename function_t, typename... args_type>
        pcl::promise_value <typename std::result_of<function_t(args_type...)>::type> 
                                add_task(function_t function, args_type &&... args) {

            typedef typename std::result_of<function_t(args_type...)>::type result_type;

            static std::atomic_ullong add_number = 0;

            ++prepare_count;

            std::packaged_task<result_type()> task(std::bind(function, std::forward<args_type>(args)...));
            std::future<result_type> res(task.get_future());

            queues[(add_number++) % n_threads]->push(std::move(task));
            return pcl::promise_value<result_type>(std::move(res));
        }


        void wait() {
            while (!(queue_are_empty())) std::this_thread::yield();
        }

        void delete_scheduler() {
            if (_instance) {
                done = true;
                for (size_t i = 0; i < n_threads; ++i)
                    if (threads[i].joinable()) threads[i].join();
            }
        }

        static scheduler_impl* instance() {
            if (_instance == nullptr)
                return _instance = new scheduler_impl;
            return _instance;
        }

        static void delete_instance() {
            if (_instance == nullptr) return;

            delete _instance;
            _instance = nullptr;
        }

        static size_t max_threads() {
            return std::thread::hardware_concurrency();
        }

    }; // scheduler_impl

    scheduler_impl* scheduler_impl::_instance = nullptr;

} // pcl_impl

namespace pcl {

    // CONTROLER FOR TASK SCHEDULER
    class scheduler {
        static pcl_impl::scheduler_impl* impl;

    public:
        template<typename function_t, typename... args_type>
        promise_value<typename std::result_of<function_t(args_type...)>::type> add_task(function_t function, args_type &&... args) {
            return impl->add_task(function, std::forward<args_type>(args)...);
        }

        static void wait() {
            if (impl) impl->wait();
        }

        static void delete_scheduler() {
            if (impl) { 
                impl->delete_scheduler();
                pcl_impl::scheduler_impl::delete_instance();
            }
        }

        static size_t max_threads() {
            return std::thread::hardware_concurrency();
        };

        scheduler() {
            impl = pcl_impl::scheduler_impl::instance();
        }
    }; // pcl::scheduler
    pcl_impl::scheduler_impl* scheduler::impl = nullptr;

} // pcl


#endif