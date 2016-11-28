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

            template <typename function_t>
            fun_wrapper(function_t&& fun): function(std::move(fun)){}
            void call() { 
                //function();
            }
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

        task_t() = default;
        task_t(fun_wrapper_base& fun) = delete;
        task_t(const fun_wrapper_base& fun) = delete;
        
        task_t(task_t& x) {
            task = std::move(x.task);
        };
        task_t operator=(task_t& x) { 
            task = std::move(x.task);
            return this;
        };

        //fun_wrapper_base& operator=(fun_wrapper_base& fun) = delete;
        //fun_wrapper_base& operator=(const fun_wrapper_base& fun) {}
    }; // task_t

    ///5657
   

    //---------------------------TASK SCHEDULER---------------------------
    class scheduler {
    private:
        std::vector<std::unique_ptr<pcl::queue<task_t> > > queues;
        std::vector<std::thread> threads;
        
        size_t n_threads;
        join_quard threads_join;
        std::atomic_bool done;

        void execute_tasks(size_t index) {
            while (!done) {
                task_t task;
                if (queues[index]->try_pop(task))
                    task();
                else {
                    while (!queues[random()]->try_pop(task) && !done);
                    if (!done) return;
                    task();
                }
            }
        }

        int random() {
            //return rand() % n_threads;
            return 0;
        }

    public:
        static size_t max_threads() {
            return std::thread::hardware_concurrency();
        };

        template<typename function_t, typename... args_t>
        std::future<typename std::result_of<function_t()>::type> add_task(function_t function, args_t args) {
            typedef typename std::result_of<function_t()>::type result_type;
            
            std::packaged_task<result_type()> task(std::move(function));
            std::future<result_type> res(task.get_future());
            
            queues[random()]->push(std::move(task));
            return res;
        }

        void wait() {
            for (size_t i = 0; i < n_threads; ++i)
                if (threads[i].joinable())
                    threads[i].join();
        }

        scheduler() :
            //n_threads(std::thread::hardware_concurrency()),
            n_threads(1),
            threads_join(threads),
            done(false)
        {
            for (size_t i = 0; i < n_threads; ++i)
                queues.push_back(std::unique_ptr<pcl::queue<task_t> >(new queue<task_t>));
            for (size_t i = 0; i < n_threads; ++i)
                threads.push_back(std::thread(&pcl::scheduler::execute_tasks, this, i));
        }

        ~scheduler() {
            done = true;
        }

    }; // scheduler
} // pcl

#endif