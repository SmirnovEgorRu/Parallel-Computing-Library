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

        if (*pcl_itr == *std_itr) std::cout << "the test was completed successfully! iterators are equal." << std::endl << std::endl;
        else std::cout << "the test is failed! test iterators are not equal." << std::endl << std::endl;
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

        if (pcl_sum == std_sum) std::cout << "the test was completed successfully! amounts are equal." << std::endl << std::endl;
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

        if (pcl_bool == std_bool) std::cout << "the test was completed successfully! values are equal." << std::endl << std::endl;
        else std::cout << "the test is failed! values are not equal." << std::endl << std::endl;
    }

    // fill
    void pcl_fill_test(std::vector<double> &v, double value) {
        size_t start_time = clock();

        pcl::fill(v.begin(), v.end(), value);

        std::cout << "pcl::fill time: " << (clock() - start_time) / 1000.0 << std::endl;
    }
    void std_fill_test(std::vector<double> &v, double value) {
        size_t start_time = clock();

        std::fill(v.begin(), v.end(), value);

        std::cout << "std::fill time: " << (clock() - start_time) / 1000.0 << std::endl;
    }
    void fill_test() {
        const size_t size = 150000000;

        std::vector<double> v(size);
        double fill_value = 1.0;

        std_fill_test(v, fill_value);

        fill_value = 0;
        pcl_fill_test(v, fill_value);

        if (v[size - 1] == fill_value) std::cout << "the test was completed successfully! values are equal." << std::endl << std::endl;
        else std::cout << "the test is failed! values are not equal." << std::endl << std::endl;
    }

    // generate
    double fun_generate() {
        double number = 0;
        while (number != 4000) number++;
        return number;
    }
    template<typename function_t>
    void pcl_generate_test(std::vector<double> &v, function_t& fun) {
        size_t start_time = clock();

        pcl::generate(v.begin(), v.end(), fun);

        std::cout << "pcl::generate time: " << (clock() - start_time) / 1000.0 << std::endl;
    }
    template<typename function_t>
    void std_generate_test(std::vector<double> &v, function_t& fun) {
        size_t start_time = clock();

        std::generate(v.begin(), v.end(), fun);

        std::cout << "std::generate time: " << (clock() - start_time) / 1000.0 << std::endl;
    }
    void generate_test() {
        const size_t size = 400000;

        std::vector<double> v(size);

        std_generate_test(v, std::bind(&algorithm_test::fun_generate, this));
        pcl_generate_test(v, std::bind(&algorithm_test::fun_generate, this));

        if (v[size - 1] == fun_generate()) std::cout << "the test was completed successfully! values are equal." << std::endl << std::endl;
        else std::cout << "the test is failed! values are not equal." << std::endl << std::endl;
    }

    // min_element
    auto pcl_min_element_test(std::vector<size_t> &v) {

        size_t start_time = clock();

        auto itr = pcl::min_element(v.begin(), v.end());

        std::cout << "pcl::min_element time: " << (clock() - start_time) / 1000.0 << std::endl;
        return itr;
    }
    auto std_min_element_test(std::vector<size_t> &v) {

        size_t start_time = clock();

        auto itr = std::min_element(v.begin(), v.end());

        std::cout << "std::min_element time: " << (clock() - start_time) / 1000.0 << std::endl;
        return itr;
    }
    void min_element_test() {
        const size_t size = 100000000;

        std::vector<size_t> v(size);
        for (size_t i = 0; i < size; ++i)
            v[i] = size - i;

        auto std_itr = std_min_element_test(v);
        auto pcl_itr = pcl_min_element_test(v);

        if (*pcl_itr == *std_itr) std::cout << "the test was completed successfully! iterators are equal." << std::endl << std::endl;
        else std::cout << "the test is failed! test iterators are not equal." << std::endl << std::endl;
    }

    // max_element
    auto pcl_max_element_test(std::vector<size_t> &v) {

        size_t start_time = clock();

        auto itr = pcl::max_element(v.begin(), v.end());

        std::cout << "pcl::max_element time: " << (clock() - start_time) / 1000.0 << std::endl;
        return itr;
    }
    auto std_max_element_test(std::vector<size_t> &v) {

        size_t start_time = clock();

        auto itr = std::max_element(v.begin(), v.end());

        std::cout << "std::max_element time: " << (clock() - start_time) / 1000.0 << std::endl;
        return itr;
    }
    void max_element_test() {
        const size_t size = 100000000;

        std::vector<size_t> v(size);
        for (size_t i = 0; i < size; ++i)
            v[i] = size - i;

        auto std_itr = std_max_element_test(v);
        auto pcl_itr = pcl_max_element_test(v);

        if (*pcl_itr == *std_itr) std::cout << "the test was completed successfully! iterators are equal." << std::endl << std::endl;
        else std::cout << "the test is failed! test iterators are not equal." << std::endl << std::endl;
    }

    // sort
    void pcl_sort_test(std::vector<size_t> &v) {
        size_t start_time = clock();

        pcl::sort<std::vector<size_t>::iterator, size_t>(v.begin(), v.end());

        std::cout << "pcl::sort time: " << (clock() - start_time) / 1000.0 << std::endl;
    }
    void std_sort_test(std::vector<size_t> &v) {
        size_t start_time = clock();

        std::sort(v.begin(), v.end());

        std::cout << "std::sort time: " << (clock() - start_time) / 1000.0 << std::endl;
    }
    void sort_test() {
        size_t size = 1000000;
        std::vector<size_t> v(size);

        pcl::scheduler::delete_scheduler();

        for (size_t i = 0; i < size; ++i) v[i] = size - i;
        std_sort_test(v);

        for (size_t i = 0; i < size; ++i) v[i] = size - i;
        pcl_sort_test(v);
        pcl::scheduler::delete_scheduler();
    }

public:

    void execute() {
        for_each_test();
        //find_test();
        //count_test();
        equal_test();
        //fill_test();
        generate_test();
        min_element_test();
        max_element_test();
        /*sort_test();*/
    }
};

#endif
