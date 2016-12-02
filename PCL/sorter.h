#ifndef _PCL_SORTER_HEADER_
#define _PCL_SORTER_HEADER_

#include "scheduler.h"

namespace pcl {

    template<typename T>
    class sorter {
        pcl::scheduler tasks;

        template<typename G>
        void swap(G& x, G&y) {
            G tmp = std::move(x);
            x = std::move(y);
            y = std::move(tmp);
        }


        //template<typename T>
        void qsort(T& list, int start, int end) {
            int left = start;
            int right = end;
            int mid = list[(left / 2) + (right / 2)];

            while (left <= right)
            {
                while (list[left] < mid)
                    left++;
                while (list[right] > mid)
                    right--;
                if (left <= right)
                    swap(list[left++], list[right--]);
            }

            if (start < right)
                tasks.add_task(&sorter::qsort, this, std::ref(list), start, right);
            if (end > left)
                tasks.add_task(&sorter::qsort, this, std::ref(list), left, end);
        }

    public:
        void operator()(T& list, int start, int end) {
            tasks.add_task(&sorter::qsort, this, std::ref(list), start, end);

            tasks.wait();
        }
    };

}

#endif
