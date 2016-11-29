#include "scheduler.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>


size_t do_nothing() {
    size_t n = 1000000;
    size_t sum=0;
    for (size_t i = 0; i < n;++i)
        ++sum;
    //std::cout << "do_nothing   ";
    return sum;
}

size_t do_something(int x, int y) {
    size_t n = 1000;
    size_t sum = 0;
    for (size_t i = 0; i < n; ++i)
        ++sum;

    std::cout << x + y;
    return sum;
}

void run() {

    const int n = 1000;
    int x = 5;
    int y = 6;

    size_t sum = 0;

    std::future<size_t> f[n];
    {
        pcl::scheduler tasks;
        for (int i = 0; i < n; ++i)
            f[i] = tasks.add_task(do_nothing);
    }

    for (int i = 0; i < n; ++i)
        sum += f[i].get();

    std::cout << "sum = " << sum<<std::endl;
}

int main() {
    size_t start_time;
    size_t sum;
    const int n = 1000;

    start_time = clock();
    for (int i = 0; i < n; ++i)
        do_nothing();
    std::cout << (clock() - start_time) / 1000.0 << std::endl;

    start_time = clock();
    run();
    std::cout << (clock() - start_time) / 1000.0 << std::endl;

    std::cout << "Press \"Enter\" to exit"<<std::endl;
    std::getchar();
    return 0;
}
