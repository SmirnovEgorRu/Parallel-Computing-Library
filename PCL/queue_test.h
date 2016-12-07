#ifndef _PCL_QUEUE_TEST_HEADER_
#define _PCL_QUEUE_TEST_HEADER_

#include "queue.h"
#include "scheduler.h"
#include <iostream>

class queue_test {

public:
    void execute() {
        const int n = 100000;
        double number = 0;

        pcl::queue<double> parallel_queue;
        std::vector<double> values(n);

        std::cout << "test queue_test was started" << std::endl;

        pcl::scheduler tasks;

        try {
            for (int i = 0; i < n; ++i)
                tasks.add_task(&pcl::queue<double>::push, &parallel_queue, number);

            for (int i = 0; i < n; ++i) tasks.add_task(&pcl::queue<double>::try_pop, &parallel_queue, std::ref(values[i]));

            pcl::scheduler::wait();

            for (int i = 1; i < n; ++i) {
                if (values[i - 1] != values[i]) {
                    std::cout << "incorrect value!" << std::endl << "the test is failed!" << std::endl;
                    std::cout << values[i - 1] << " != " << values[i] << std::endl;
                    std::cout << i << std::endl;
                    return;
                }
            }
            std::cout << "the test was completed successfully!" << std::endl << std::endl;
        }
        catch (...) {
            std::cout << "some exception was caught!" << std::endl << "the test is failed!" << std::endl << std::endl;
            return;
        }
    }
};




#endif
