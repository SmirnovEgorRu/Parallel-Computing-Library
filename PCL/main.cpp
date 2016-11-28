#include "scheduler.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>


size_t do_nothing() {
    size_t n = 1000000;
    size_t sum=0;
    for (size_t i = 0; i < n;++i)
        ++sum;
    return sum;
}

int main() {
    const int n = 2000;
    size_t start_time;

    start_time = clock();
    {
        pcl::scheduler tasks;

        for (int i = 0; i < n; ++i) {
            tasks.add_task(do_nothing);
        }
    }
    std::cout << (clock() - start_time) / 1000.0 << std::endl;


    start_time = clock();
    for (int i = 0; i < n; ++i) {
        do_nothing();
    }
    std::cout << (clock() - start_time) / 1000.0 << std::endl;



    std::cout << "Press \"Enter\" to exit"<<std::endl;
    std::getchar();
    return 0;
}