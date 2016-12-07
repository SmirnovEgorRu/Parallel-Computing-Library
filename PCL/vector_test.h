#ifndef _PCL_VECTOR_TEST_HEADER_
#define _PCL_VECTOR_TEST_HEADER_

#include "vector.h"
#include "scheduler.h"
#include <iostream>

class vector_test {

public:
    void execute() {
        const int n = 100000;
        double number = 0;

        pcl::vector<double> parallel_vector;

        std::cout << "test vector_test was started" << std::endl;

        pcl::scheduler tasks;

        try {
            for (int i = 0; i < n; ++i)
                tasks.add_task(&pcl::vector<double>::push_back, &parallel_vector, number);

            for (int i = 0; i < n; ++i) tasks.add_task(&pcl::vector<double>::pop_back, &parallel_vector);

            pcl::scheduler::wait();

            if (!parallel_vector.empty())
                std::cout << "incorrect value!" << std::endl << "the test is failed!" << std::endl;
            else std::cout << "the test was completed successfully!" << std::endl << std::endl;;
        }
        catch (...) {
            std::cout << "some exception was caught!" << std::endl << "the test is failed!" << std::endl;
            return;
        }
    }
};



#endif
