#ifndef _PCL_SORTER_HEADER_
#define _PCL_SORTER_HEADER_

#include "scheduler.h"

namespace pcl {

    //template<typename T>
    //class sorter {
    //    pcl::scheduler tasks;

    //    template<typename G>
    //    void swap(G& x, G&y) {
    //        G tmp = std::move(x);
    //        x = std::move(y);
    //        y = std::move(tmp);
    //    }


    //    //template<typename T>
    //    void qsort(T& list, int start, int end) {
    //        int left = start;
    //        int right = end;
    //        int mid = list[(left / 2) + (right / 2)];

    //        while (left <= right)
    //        {
    //            while (list[left] < mid)
    //                left++;
    //            while (list[right] > mid)
    //                right--;
    //            if (left <= right)
    //                swap(list[left++], list[right--]);
    //        }

    //        if (start < right)
    //            tasks.add_task(&sorter::qsort, this, std::ref(list), start, right);
    //        if (end > left)
    //            tasks.add_task(&sorter::qsort, this, std::ref(list), left, end);
    //    }

    //public:
    //    void operator()(T& list, int start, int end) {
    //        tasks.add_task(&sorter::qsort, this, std::ref(list), start, end);

    //        tasks.wait();
    //    }
    //};


    //template<typename T>
    //void qsort(T& list, int start, int end) {
    //    pcl::scheduler tasks;

    //    int left = start;
    //    int right = end;
    //    int mid = list[(left / 2) + (right / 2)];

    //    while (left <= right)
    //    {
    //        while (list[left] < mid)
    //            left++;
    //        while (list[right] > mid)
    //            right--;
    //        if (left <= right)
    //            swap(list[left++], list[right--]);
    //    }

    //    if (start < right)
    //        tasks.add_task(qsort<T>, std::ref(list), start, right);
    //    if (end > left)
    //        tasks.add_task(qsort<T>, std::ref(list), left, end);
    //}

    //void qsort_test() {
    //    size_t size = 100000;

    //    std::vector<size_t> v(size);
    //    for (size_t i = 0; i < size; ++i)
    //        v[i] = rand();

    //    size_t start_time = clock();
    //    //std_sort_test(v);
    //    std::sort(v.begin(), v.end());
    //    std::cout << "std::sort time: " << (clock() - start_time) / 1000.0 << std::endl;

    //    for (size_t i = 0; i < size; ++i)
    //        v[i] = rand();

    //    start_time = clock();
    //    //pcl_sort_test(v);
    //    //pcl::sort<std::vector<size_t>::iterator, size_t>(v.begin(), v.end());

    //    pcl::qsort(v, 0, size-1);
    //    pcl::scheduler tasks;
    //    tasks.wait();
    //    std::cout << "std::sort time: " << (clock() - start_time) / 1000.0 << std::endl;
    //}

}

#endif
