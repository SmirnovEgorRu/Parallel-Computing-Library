#ifndef _PCL_PARALLEL_FOR_HEADER_
#define _PCL_PARALLEL_FOR_HEADER_

#include "scheduler.h"


namespace pcl {

    template<typename T>
    class blocked_range {
        T current;
        T end;

    public:
        bool operator()(T& value) {
            if (current == end)
                return false;

            value = current;
            ++current;
            return true;
        }

        blocked_range(const T& first, const T& last):current(first), end(last){}

    };

    template <typename T, typename F>
    void parallel_for(blocked_range<T>& range, F fun){
        pcl:scheduler tasks;

        T iteration;

        while (range(iteration)) {
            tasks.add_task(fun, iteration);
        }
    }

}

#endif
