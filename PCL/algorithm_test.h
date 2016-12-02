#ifndef _PCL_ALGORITHM_TEST_HEADER_
#define _PCL_ALGORITHM_TEST_HEADER_


#include <vector>
#include <algorithm>
#include <ctime>
#include <iostream>
#include "algorithm.h"


    void sort(std::vector<int>& v) {
        for (int i = 0; i < v.size()-1; ++i) {
            for (int k =i + 1; k < v.size(); ++k) {
                if (v[i] > v[k]) std::swap(v[i], v[k]);
            }
        }
    }

class algorithm_test {



    void pcl_for_each_test(const int n_vectors, const int size) {
        std::vector<std::vector<int> > v(n_vectors);

        for (auto i = v.begin(); i < v.end(); ++i) {
            for (int k = 0; k < size; ++k) {
                (*i).push_back(size - k);
            }
        }

        size_t start_time = clock();
        pcl::for_each(v.begin(), v.end(), sort);
        std::cout << "pcl::for_each time: " <<(clock() - start_time) / 1000.0 << std::endl;
    }

    void std_for_each_test(const int n_vectors, const int size) {
        std::vector<std::vector<int> > v(n_vectors);

        for (auto i = v.begin(); i < v.end(); ++i) {
            for (int k = 0; k < size; ++k) {
                (*i).push_back(size - k);
            }
        }

        size_t start_time = clock();
        std::for_each(v.begin(), v.end(), sort);
        std::cout << "std::for_each time: " << (clock() - start_time) / 1000.0 << std::endl;
    }

public:
    void for_each_test() {
        const int n_vectors = 1000;
        const int size = 1000;
        std_for_each_test(n_vectors, size);
        pcl_for_each_test(n_vectors, size);
    }

    void execute() {
        for_each_test();
    }


};




#endif
