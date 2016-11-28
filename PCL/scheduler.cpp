//#include "scheduler.h"


namespace pcl {


   /* template<typename function_t>
    std::future<typename std::result_of<function_t()>::type> scheduler::add_task(function_t function) {

        typedef typename std::result_of<function_t()>::type result_type;

        std::packaged_task<result_type()> task(f);
        std::future<result_type> res(task.get_future());

        queues.push(std::move(task));
        return res;
    }*/
}