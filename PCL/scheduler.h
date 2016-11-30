#ifndef _PCL_SCHEDULER_HEADER_
#define _PCL_SCHEDULER_HEADER_

#include "ts_queue.h"
#include <thread> 
#include <future> 
#include <vector> 


namespace pcl {

    // -------------------PROVIDE JOINING OF ALL THE THREADS-------------------
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

    //----------------------------WRAPPER FOR TASKS----------------------------
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

    //---------------------------TASK SCHEDULER---------------------------
    class scheduler {
    private:
        std::vector<std::unique_ptr<pcl::queue<task_t> > > queues;
        std::vector<std::thread> threads;

        size_t n_threads;

        std::atomic_bool add_finish = false;
        //std::atomic_bool done = false;
        std::atomic_ullong prepare_count = 0;
        std::atomic_ullong finish_count = 0;

        void execute_tasks(size_t index) {
            while (!add_finish || !queue_are_empty()) {
                task_t task;
                if (queues[index]->try_pop(task)) {
                    task();
                    ++finish_count;
                }
                else {
                    bool add_task = false;
                    // (queue_are_empty() && add_finish)) == true - if all tasks was completed
                    // (add_task = queues[random()]->try_pop(task)) == true - if we have got task from queues
                    // !(add_task = queues[random()]->try_pop(task)) && !(queue_are_empty() && add_finish) == false - if we have got task from queues or all tasks was completed
                    while (!(add_task = queues[random()]->try_pop(task)) && !(queue_are_empty() && add_finish));
                    if (add_task) {
                        task();
                        ++finish_count;
                    }
                    else if (queue_are_empty() && add_finish) {
                        return;
                    }
                }
            }
        }

        bool queue_are_empty() {
            if (finish_count < prepare_count) return false;
            return true;
        }

        size_t random() {
            static thread_local size_t number = 0;
            return (number++) % n_threads;
        }

    public:
        static size_t max_threads() {
            return std::thread::hardware_concurrency();
        };

        //template<typename function_t, typename... args_type>
        //std::future<typename std::result_of<function_t(args_type...)>::type> add_task(function_t function, args_type &&... args) {
        //    typedef typename std::result_of<function_t(args_type...)>::type result_type;
        //    std::packaged_task<result_type()> task(std::bind(function, std::forward<args_type>(args)...));
        //    std::future<result_type> res(task.get_future());
        //    queues[random()]->push(std::move(task));
        //    return res;
        //}

        template<typename function_t>
        std::future<typename std::result_of<function_t()>::type> add_task(function_t function) {
            typedef typename std::result_of<function_t()>::type result_type;

            ++prepare_count;

            std::packaged_task<result_type()> task(std::move(function));
            std::future<result_type> res(task.get_future());

            queues[random()]->push(std::move(task));
            return res;
        }

        void wait() {
            add_finish = true;
            for (size_t i = 0; i < n_threads; ++i)
                if (threads[i].joinable())
                    threads[i].join();
        }

        scheduler() : n_threads(max_threads()) {
            for (size_t i = 0; i < n_threads; ++i)
                queues.push_back(std::unique_ptr<pcl::queue<task_t> >(new queue<task_t>));
            for (size_t i = 0; i < n_threads; ++i)
                threads.push_back(std::thread(&pcl::scheduler::execute_tasks, this, i));
        }

        ~scheduler() {
            wait();
        }

    }; // scheduler
} // pcl

#endif