#ifndef _PCL_SCHEDULER_TEST_HEADER_
#define _PCL_SCHEDULER_TEST_HEADER_


class scheduler_test {
    size_t n_tasks = 1000;
    size_t n_itration = 100000;

    size_t fun_without_arg() {
        size_t sum = 0;
        for (size_t i = 0; i < n_itration; ++i)
            ++sum;

        return sum;
    }

    size_t fun_with_arg(int x, int y) {
        size_t sum = 0;
        for (size_t i = 0; i < n_itration; ++i)
            ++sum;

        return sum;
    }





};


#endif
