#include "scheduler.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>


size_t do_nothing() {
    size_t n = 100000000;
    size_t sum = 0;
    for (size_t i = 0; i < n;++i)
        ++sum;

    return sum;
}

size_t do_something(int x, int y) {
    size_t n = 100000000;
    size_t sum = 0;
    for (size_t i = 0; i < n; ++i)
        ++sum;

    return sum;
}

void run() {

    const int n = 100;

    size_t sum = 0;

    std::future<size_t> f[n];
        pcl::scheduler tasks;
        for (int i = 0; i < n; ++i)
            f[i] = tasks.add_task(do_nothing);
        tasks.wait();

    try {
        for (int i = 0; i < n; ++i)
            sum += f[i].get();
    }
    catch (...) {
        std::cout << "exception!!!!" << sum << std::endl;
    }

}

int main() {
    size_t start_time;
    size_t sum = 0;
    const int n = 1000;

    start_time = clock();
    for (int i = 0; i < n; ++i)
        sum+=do_nothing();
    std::cout << (clock() - start_time) / 1000.0 << std::endl;
    std::cout << "sum = " << sum << std::endl;

        start_time = clock();
    for (int i = 0; i < 1000; ++i) {
        run();
    }
        std::cout << (clock() - start_time) / 1000.0 << std::endl;

    std::cout << "Press \"Enter\" to exit"<<std::endl;
    std::getchar();
    return 0;
}
