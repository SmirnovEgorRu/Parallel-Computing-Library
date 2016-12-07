#include "scheduler.h"
#include "word_counter.h"
#include "sorter.h"

#include <iostream>
#include <ctime>
#include <algorithm>

#include "scheduler_test.h"
#include "word_counter_test.h"
#include "algorithm_test.h"
#include "queue_test.h"
#include "vector_test.h"
#include "parallel_for_test.h"

void qsort(std::vector<int>& arr, int b, int e)
{
    int l = b, r = e;
    int piv = arr[(l + r) / 2]; // Опорным элементом для примера возьмём средний
    while (l <= r)
    {
        while (arr[l] < piv)
            l++;
        while (arr[r] > piv)
            r--;
        if (l <= r)
            std::swap(arr[l++], arr[r--]);
    }
    if (b < r)
        qsort(arr, b, r);
    if (e > l)
        qsort(arr, l, e);
}    /* ----- end of function qsort ----- */



int main() {
    scheduler_test y;
    y.execute();

    word_counter_test x;
    x.execute();

    algorithm_test algorithm;
    algorithm.execute();

    queue_test queue;
    queue.execute();

    vector_test vector;
    vector.execute();

    parallel_for_test prl_for;
    prl_for.execute();

    pcl::scheduler::delete_scheduler();
    std::cout << "Press \"Enter\" to exit"<<std::endl;
    std::getchar();
    return 0;
}
