#ifndef _PCL_ALGORITHM_HEADER_
#define _PCL_ALGORITHM_HEADER_

#include "scheduler.h"

namespace pcl {

    template<typename iterator_t, typename function_t>
    function_t for_each(iterator_t start, iterator_t end, function_t fun) {
        pcl::scheduler tasks;

        while (start != end) {
            tasks.add_task(fun, *start);
            ++start;
        }

        tasks.wait();

        return fun;
    }


}

#endif
