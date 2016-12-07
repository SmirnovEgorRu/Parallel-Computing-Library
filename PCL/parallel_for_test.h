#ifndef _PCL_PARALLEL_FOR_TEST_HEADER_
#define _PCL_PARALLEL_FOR_TEST_HEADER_

#include <vector>
#include <ctime>
#include <iostream>
#include "parallel_for.h"

class parallel_for_test {
    void sort(std::vector<std::vector<int>>::iterator v) {
        for (int i = 0; i < (*v).size() - 1; ++i) {
            for (int k = i + 1; k < (*v).size(); ++k) {
                if ((*v)[i] > (*v)[k]) std::swap((*v)[i], (*v)[k]);
            }
        }
    }

    void pcl_parallel_for_test(const int n_vectors, const int size) {
        std::vector<std::vector<int> > v(n_vectors);

        for (auto i = v.begin(); i < v.end(); ++i) {
            for (int k = 0; k < size; ++k) {
                (*i).push_back(size - k);
            }
        }

        size_t start_time = clock();
        pcl::parallel_for(pcl::blocked_range<std::vector<std::vector<int>>::iterator>(v.begin(), v.end()), std::bind(&parallel_for_test::sort, this, std::placeholders::_1));
        pcl::scheduler::wait();
        std::cout << "pcl::parallel_for time: " << (clock() - start_time) / 1000.0 << std::endl;
        pcl::scheduler::delete_scheduler();
    }

    void std_for_test(const int n_vectors, const int size) {
        std::vector<std::vector<int> > v(n_vectors);

        for (auto i = v.begin(); i < v.end(); ++i) {
            for (int k = 0; k < size; ++k) {
                (*i).push_back(size - k);
            }
        }

        size_t start_time = clock();

        for(auto i=v.begin(); i<v.end(); ++i)
            sort(i);

        std::cout << "operator for time: " << (clock() - start_time) / 1000.0 << std::endl;
    }

public:
    void execute() {
        const int n_vectors = 2000;
        const int size = 1000;
        std_for_test(n_vectors, size);
        pcl_parallel_for_test(n_vectors, size);
        std::cout << std::endl;
    }
};


#endif
