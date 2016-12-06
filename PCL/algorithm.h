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

    // find
    template<typename iterator_t, typename match_t>
    iterator_t find(iterator_t first, iterator_t last, const match_t& val) {
        using pcl_impl::find_local;

        pcl::scheduler tasks;

        size_t n_threads = pcl::scheduler::max_threads();
        size_t length = last - first;
        size_t block_size = length / n_threads;

        if (block_size == 0)
            return pcl_impl::find_local(first, last, last, val);

        std::vector<pcl::promise_value<iterator_t> > promise(n_threads);

        for (size_t i = 0; i < n_threads - 1; ++i) {
            iterator_t start = first + (i * block_size);
            iterator_t end = first + ((i + 1) * block_size);
            promise[i] = tasks.add_task(pcl_impl::find_local<iterator_t, match_t>, start, end, last, val);
        }

        iterator_t start = first + ((n_threads - 1) * block_size);
        iterator_t end = last;

        promise[n_threads - 1] = tasks.add_task(pcl_impl::find_local<iterator_t, match_t>, start, end, last, val);

        tasks.wait();

        iterator_t min = last;
        iterator_t current = last;
        for (size_t i = 0; i < n_threads; ++i)
            if ((current = promise[i].get_value()) < min) min = current;

        return min;
    }

    // count
    template<typename iterator_t, typename match_t>
    size_t count(iterator_t first, iterator_t last, const match_t& val) {
        using pcl_impl::count_local;

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

    // equal
    template<typename _1_iterator_t, typename _2_iterator_t>
    bool equal(_1_iterator_t first_1, _1_iterator_t last_1, _2_iterator_t first_2) {
        using pcl_impl::equal_local;

        pcl::scheduler tasks;

        size_t n_threads = pcl::scheduler::max_threads();
        size_t length = last_1 - first_1;
        size_t block_size = length / n_threads;

        if (block_size == 0)
            return equal_local(first_1, last_1, first_2);

        std::vector<pcl::promise_value<bool> > promise(n_threads);

        for (size_t i = 0; i < n_threads - 1; ++i) {
            _1_iterator_t start_1 = first_1 + (i * block_size);
            _1_iterator_t end_1 = first_1 + ((i + 1) * block_size);
            _2_iterator_t start_2 = first_2 + ((i + 1) * block_size);
            promise[i] = tasks.add_task(equal_local<_1_iterator_t, _2_iterator_t>, start_1, end_1, start_2);
        }

        _1_iterator_t start_1 = first_1 + ((n_threads - 1) * block_size);
        _1_iterator_t start_2 = first_2 + ((n_threads - 1) * block_size);
        _2_iterator_t end_1 = last_1;

        promise[n_threads - 1] = tasks.add_task(equal_local<_1_iterator_t, _2_iterator_t>, start_1, end_1, start_2);

        tasks.wait();

        size_t sum = 0;
        for (size_t i = 0; i < n_threads; ++i)
            sum += promise[i].get_value();

        return (sum != 0);
    }

    // fill
    template<typename iterator_t, typename match_t>
    void fill(iterator_t first, iterator_t last, const match_t& val) {
        using pcl_impl::fill_local;

        pcl::scheduler tasks;

        size_t n_threads = pcl::scheduler::max_threads();
        size_t length = last - first;
        size_t block_size = length / n_threads;

        if (block_size == 0)
            return fill_local(first, last, val);

        for (size_t i = 0; i < n_threads - 1; ++i) {
            iterator_t start = first + (i * block_size);
            iterator_t end = first + ((i + 1) * block_size);
            tasks.add_task(fill_local<iterator_t, match_t>, start, end, val);
        }

        iterator_t start = first + ((n_threads - 1) * block_size);
        iterator_t end = last;
        tasks.add_task(fill_local<iterator_t, match_t>, start, end, val);
        tasks.wait();
    }

    // generate
    template<typename iterator_t, typename generator_t>
    void generate(iterator_t first, iterator_t last, generator_t generator) {
        using pcl_impl::generate_local;

        pcl::scheduler tasks;

        size_t n_threads = pcl::scheduler::max_threads();
        size_t length = last - first;
        size_t block_size = length / n_threads;

        typedef typename std::result_of<generator_t()>::type result_type;

        if (block_size == 0)
            return generate_local(first, last, std::function<result_type()>(generator));

        for (size_t i = 0; i < n_threads - 1; ++i) {
            iterator_t start = first + (i * block_size);
            iterator_t end = first + ((i + 1) * block_size);
            tasks.add_task(generate_local<iterator_t, result_type>, start, end, std::function<result_type()>(generator));
        }

        iterator_t start = first + ((n_threads - 1) * block_size);
        iterator_t end = last;
        tasks.add_task(generate_local<iterator_t, result_type>, start, end, std::function<result_type()>(generator));

        tasks.wait();
    }

    // min_element
    template<typename iterator_t>
    iterator_t min_element(iterator_t first, iterator_t last) {
        using pcl_impl::min_element_local;

        pcl::scheduler tasks;

        size_t n_threads = pcl::scheduler::max_threads();
        size_t length = last - first;
        size_t block_size = length / n_threads;

        if (block_size == 0)
            return min_element_local(first, last);

        std::vector<pcl::promise_value<iterator_t> > promise(n_threads);

        for (size_t i = 0; i < n_threads - 1; ++i) {
            iterator_t start = first + (i * block_size);
            iterator_t end = first + ((i + 1) * block_size);
            promise[i] = tasks.add_task(min_element_local<iterator_t>, start, end);
        }

        iterator_t start = first + ((n_threads - 1) * block_size);
        iterator_t end = last;
        promise[n_threads - 1] = tasks.add_task(min_element_local<iterator_t>, start, end);

        tasks.wait();

        iterator_t min = first;
        iterator_t current;
        for (size_t i = 0; i < n_threads; ++i)
            if (*(current = promise[i].get_value()) < *min) min = current;

        return min;
    }

    // max_element
    template<typename iterator_t>
    iterator_t max_element(iterator_t first, iterator_t last) {
        using pcl_impl::max_element_local;

        pcl::scheduler tasks;

        size_t n_threads = pcl::scheduler::max_threads();
        size_t length = last - first;
        size_t block_size = length / n_threads;

        if (block_size == 0)
            return max_element_local(first, last);

        std::vector<pcl::promise_value<iterator_t> > promise(n_threads);

        for (size_t i = 0; i < n_threads - 1; ++i) {
            iterator_t start = first + (i * block_size);
            iterator_t end = first + ((i + 1) * block_size);
            promise[i] = tasks.add_task(max_element_local<iterator_t>, start, end);
        }

        iterator_t start = first + ((n_threads - 1) * block_size);
        iterator_t end = last;
        promise[n_threads - 1] = tasks.add_task(max_element_local<iterator_t>, start, end);

        tasks.wait();

        iterator_t max = first;
        iterator_t current;
        for (size_t i = 0; i < n_threads; ++i)
            if (*(current = promise[i].get_value()) > *max) max = current;

        return max;
    }

    // sort
    template<typename iterator_t, typename value_t>
    void sort(iterator_t first, iterator_t last) {
        pcl_impl::sort_impl<iterator_t, value_t>(first, last);
        pcl::scheduler::wait();
    }

    //template<typename iterator_t, typename value_t>
    //void sort_scalar(iterator_t first, iterator_t last) {
    //    pcl::sort_impl_scalar<iterator_t, value_t>(first, last);
    //}
    //template<typename iterator_t, typename value_t>
    //void sort_impl_scalar(iterator_t first, iterator_t last) {
    //    if (last - first <= 1) return;
    //    auto mid = first + (last - first) / 2;
    //    const auto mid_value = *mid;
    //    std::swap(*mid, *(last - 1));
    //    auto p = std::partition(first, last, [&](const value_t& value) { return value < mid_value; });
    //    std::swap(*p, *(last - 1));
    //    sort_impl_scalar<iterator_t, value_t>(first, p);
    //    sort_impl_scalar<iterator_t, value_t>(p + 1, last);
    //}

} // pcl

namespace pcl_impl {

    template<typename iterator_t, typename value_t>
    void sort_impl(const iterator_t first, const iterator_t last) {
        if (last - first <= 1) return;

        pcl::scheduler tasks;

        auto mid = first + (last - first) / 2;
        const auto mid_value = *mid;

        std::swap(*mid, *(last - 1));
        auto p = std::partition(first, last, [&](const value_t& value) { return value < mid_value; });
        std::swap(*p, *(last - 1));

        const size_t max_size = 4000;
        if (last - first > max_size) {
            tasks.add_task(sort_impl<iterator_t, value_t>, first, p);
            sort_impl<iterator_t, value_t>(p + 1, last);
        }
        else {
            sort_impl<iterator_t, value_t>(first, p);
            sort_impl<iterator_t, value_t>(p + 1, last);
        }
    }

    template<class iterator_t, class match_t>
    iterator_t find_local(iterator_t first, iterator_t last, iterator_t out, const match_t& val)
    {
        while (first != last) {
            if (*first == val) return first;
            ++first;
        }
        return out;
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

    template<class iterator_t>
    iterator_t max_element_local(iterator_t first, iterator_t last) {
        if (first == last) return last;
        iterator_t largest = first;

        while (first != last) {
            if (*first > *largest)
                largest = first;
            ++first;
        }
        return largest;
    }

    template<class iterator_t>
    iterator_t min_element_local(iterator_t first, iterator_t last) {
        if (first == last) return last;
        iterator_t smallest = first;

        while (first != last) {
            if (*first < *smallest)
                smallest = first;
            ++first;
        }
        return smallest;
    }

    template<typename iterator_t, typename result_t>
    void generate_local(iterator_t first, iterator_t last, std::function<result_t()> generator) {
        while (first != last) {
            *first = generator();
            ++first;
        }
    }

    template<class iterator_t, class match_t>
    void fill_local(iterator_t first, iterator_t last, const match_t& val) {
        while (first != last) {
            *first = val;
            ++first;
        }
    }

    template<typename _1_iterator_t, typename _2_iterator_t>
    bool equal_local(_1_iterator_t first_1, _1_iterator_t last_1, _2_iterator_t first_2) {
        while (first_1 != last_1) {
            if (*first_1 != *first_2) return false;
            ++first_1;
            ++first_2;
        }

        return true;
    }
}

#endif
