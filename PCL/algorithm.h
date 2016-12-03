#ifndef _PCL_ALGORITHM_HEADER_
#define _PCL_ALGORITHM_HEADER_

#include "scheduler.h"
#include <algorithm>

namespace pcl {

    // for_each

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
    /*template<typename iterator_t, typename function_t>
    function_t for_each(iterator_t start, iterator_t end, function_t fun) {
        pcl::scheduler tasks;
        size_t n_threads = pcl::scheduler::max_threads();
        size_t length = end - start;
        size_t block_size = length/ n_threads;
        if (block_size == 0)
            std::for_each(start, end, fun);
        else {
            for (size_t i = 0; i < n_threads - 1; ++i)
                tasks.add_task(std::bind(std::for_each, start + (i * block_size), start + ((i + 1) * block_size), fun));
            tasks.add_task(std::bind(std::for_each, start + ((n_threads - 1) * block_size), end, fun));
        }
        tasks.wait();
        return fun;
    }*/

    // find
    template<class iterator_t, class match_t>
    iterator_t find_local(iterator_t first, iterator_t last, iterator_t out, const match_t& val)
    {
        while (first != last) {
            if (*first == val) return first;
            ++first;
        }
        return out;
    }

    template<typename iterator_t, typename match_t>
    iterator_t find(iterator_t first, iterator_t last, const match_t& val){
        pcl::scheduler tasks;

        size_t n_threads = pcl::scheduler::max_threads();
        size_t length = last - first;
        size_t block_size = length/ n_threads;

        if (block_size == 0)
            return find_local(first, last, last, val);

        std::vector<pcl::promise_value<iterator_t> > promise(n_threads);

        for (size_t i = 0; i < n_threads - 1; ++i) {
            iterator_t start = first + (i * block_size);
            iterator_t end = first + ((i + 1) * block_size);
            promise[i] = tasks.add_task(find_local<iterator_t, match_t>, start, end, last, val);
        }

        iterator_t start = first + ((n_threads - 1) * block_size);
        iterator_t end = last;

        promise[n_threads - 1] = tasks.add_task(find_local<iterator_t, match_t>, start, end, last, val);

        tasks.wait();

        iterator_t min = last;
        iterator_t current = last;
        for (size_t i = 0; i < n_threads; ++i)
            if ((current=promise[i].get_value()) < min) min = current;

        return min;
    }

    // count
    template<typename iterator_t, typename match_t>
    size_t count(iterator_t first, iterator_t last, const match_t& val) {
        pcl::scheduler tasks;

        size_t n_threads = pcl::scheduler::max_threads();
        size_t length = last - first;
        size_t block_size = length / n_threads;

        if (block_size == 0)
            return count_local(first, last, val);

        std::vector<pcl::promise_value<size_t> > promise(n_threads);

        for (size_t i = 0; i < n_threads - 1; ++i) {
            iterator_t start = first + (i * block_size);
            iterator_t end = first + ((i + 1) * block_size);
            promise[i] = tasks.add_task(count_local<iterator_t, match_t>, start, end, val);
        }

        iterator_t start = first + ((n_threads - 1) * block_size);
        iterator_t end = last;
        promise[n_threads - 1] = tasks.add_task(count_local<iterator_t, match_t>, start, end, val);

        tasks.wait();

        size_t sum = 0;
        for (size_t i = 0; i < n_threads; ++i)
            sum += promise[i].get_value();

        return sum;
    }

    template<class iterator_t, class match_t>
    size_t count_local(iterator_t first, iterator_t last, const match_t& val) {
        size_t sum = 0;
        while (first != last) {
            if (*first == val) ++sum;
            ++first;
        }

        return sum;
    }

}

#endif
