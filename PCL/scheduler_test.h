#ifndef _PCL_SCHEDULER_TEST_HEADER_
#define _PCL_SCHEDULER_TEST_HEADER_

#include "scheduler.h"
#include <iostream>

class scheduler_test {
    const size_t n_iteration = 100000000;

    size_t fun_with_arg(int x, int y) {
        size_t sum = 0;

        for (size_t i = 0; i < n_iteration; ++i)
            sum+=x+y;

        return sum;
    }

public:

    void execute() {
        const size_t n_tasks = 10000;
        size_t sum = 0;

        pcl::promise_value<size_t> f[n_tasks];
        pcl::scheduler tasks;

        std::cout << "test scheduler_test was started" << std::endl;

        try {
            for (int i = 0; i < n_tasks; ++i)
                f[i] = tasks.add_task(&scheduler_test::fun_with_arg, this, 1, 0);

            tasks.wait();

            for (int i = 0; i < n_tasks; ++i)
                sum += f[i].get_value();
        }
        catch (...) {
            std::cout << "some exception was caught!" << std::endl << "the test is failed!" << std::endl;
            return;
        }

        if (sum == n_tasks*n_iteration)
            std::cout << "the test was completed successfully!" << std::endl;
        else
            std::cout << "incorrect value!" << std::endl << "the test is failed!" << std::endl;

        std::cout << std::endl;
    }

}; // scheduler_test

#endif
