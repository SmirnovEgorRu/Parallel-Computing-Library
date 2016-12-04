#ifndef _PCL_ALGORITHM_TEST_HEADER_
#define _PCL_ALGORITHM_TEST_HEADER_

#include <vector>
#include <algorithm>
#include <ctime>
#include <iostream>
#include "algorithm.h"

class algorithm_test {

    // for_each
    void sort(std::vector<int>& v) {
        for (int i = 0; i < v.size() - 1; ++i) {
            for (int k = i + 1; k < v.size(); ++k) {
                if (v[i] > v[k]) std::swap(v[i], v[k]);
            }
        }
    }
    void pcl_for_each_test(const int n_vectors, const int size) {
        std::vector<std::vector<int> > v(n_vectors);

        for (auto i = v.begin(); i < v.end(); ++i) {
            for (int k = 0; k < size; ++k) {
                (*i).push_back(size - k);
            }
        }

        size_t start_time = clock();
        pcl::for_each(v.begin(), v.end(), std::bind(&algorithm_test::sort, this, std::placeholders::_1));
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
        std::for_each(v.begin(), v.end(), std::bind(&algorithm_test::sort, this, std::placeholders::_1));
        std::cout << "std::for_each time: " << (clock() - start_time) / 1000.0 << std::endl;
    }
    void for_each_test() {
        const int n_vectors = 1000;
        const int size = 1000;
        std_for_each_test(n_vectors, size);
        pcl_for_each_test(n_vectors, size);
        std::cout << std::endl;
    }

    // find
    auto pcl_find_test(std::vector<size_t> &v, size_t value) {

        size_t start_time = clock();

        auto itr = pcl::find(v.begin(), v.end(), value);

        std::cout << "pcl::find time: " << (clock() - start_time) / 1000.0 << std::endl;
        return itr;
    }
    auto std_find_test(std::vector<size_t> &v, size_t value) {

        size_t start_time = clock();

        auto itr = std::find(v.begin(), v.end(), value);

        std::cout << "std::find time: " << (clock() - start_time) / 1000.0 << std::endl;
        return itr;
    }
    void find_test() {
        const size_t size = 100000000;

        std::vector<size_t> v(size);
        for (size_t i = 0; i < size; ++i)
            v[i] = i;

        const size_t find_value = size - 1;

        auto std_itr = std_find_test(v, find_value);
        auto pcl_itr = pcl_find_test(v, find_value);

        if (*pcl_itr == *std_itr) std::cout << "the test was completed successfully! iterators are equals." << std::endl << std::endl;
        else std::cout << "the test is failed! test iterators are not equals." << std::endl << std::endl;
    }

    // count
    size_t pcl_count_test(std::vector<size_t> &v, size_t value) {
        size_t start_time = clock();

        size_t sum = pcl::count(v.begin(), v.end(), value);

        std::cout << "pcl::count time: " << (clock() - start_time) / 1000.0 << std::endl;
        return sum;
    }
    size_t std_count_test(std::vector<size_t> &v, size_t value) {
        size_t start_time = clock();

        size_t sum = std::count(v.begin(), v.end(), value);

        std::cout << "std::count time: " << (clock() - start_time) / 1000.0 << std::endl;
        return sum;
    }
    void count_test() {
        const size_t size = 150000000;

        std::vector<size_t> v(size);
        for (size_t i = 0; i < size; ++i)
            v[i] = i % (size / 100);

        const size_t find_value = size/100 - 1;

        size_t std_sum = std_count_test(v, find_value);
        size_t pcl_sum = pcl_count_test(v, find_value);

        if (pcl_sum == std_sum) std::cout << "the test was completed successfully! amounts are equals." << std::endl << std::endl;
        else std::cout << "the test is failed! amounts are not equals." << std::endl << std::endl;
    }

    // equal
    size_t pcl_equal_test(std::vector<size_t> &v) {
        size_t start_time = clock();

        size_t sum = pcl::equal(v.begin(), v.end(), v.begin());

        std::cout << "pcl::equal time: " << (clock() - start_time) / 1000.0 << std::endl;
        return sum;
    }
    size_t std_equal_test(std::vector<size_t> &v) {
        size_t start_time = clock();

        size_t sum = std::equal(v.begin(), v.end(), v.begin());

        std::cout << "std::equal time: " << (clock() - start_time) / 1000.0 << std::endl;
        return sum;
    }
    void equal_test() {
        const size_t size = 150000000;

        std::vector<size_t> v(size);
        for (size_t i = 0; i < size; ++i)
            v[i] = i;

        bool std_bool = std_equal_test(v);
        bool pcl_bool = pcl_equal_test(v);

        if (pcl_bool == std_bool) std::cout << "the test was completed successfully! values are equals." << std::endl << std::endl;
        else std::cout << "the test is failed! values are not equals." << std::endl << std::endl;
    }




public:

    void execute() {
        for_each_test();
        find_test();
        count_test();
        equal_test();
    }
};

#endif
