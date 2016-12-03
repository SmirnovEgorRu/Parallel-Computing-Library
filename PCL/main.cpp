#include "scheduler.h"
#include "word_counter.h"
#include "sorter.h"

#include <iostream>
#include <ctime>
#include <algorithm>

#include "scheduler_test.h"
#include "word_counter_test.h"
#include "algorithm_test.h"

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
    //scheduler_test y;
    //y.execute();

    //word_counter_test x;
    //x.execute();

    int n = 1000000;
    size_t start_time;

    std::vector<int> arr(n);
    std::vector<int> arr_1(n);

    for (int i = 0; i < n;++i) {
        arr[i] = rand();
        arr_1[i] = rand();
    }

    //start_time = clock();
    //pcl::sorter<std::vector<int> > sort;
    //sort(arr, 0, n-1);
    //std::cout << (clock() - start_time) / 1000.0 << std::endl;

    //start_time = clock();
    ////std::sort(arr_1.begin(), arr_1.end());
    //qsort(arr_1, 0, n - 1);


    ////for (int i = 0; i < n; ++i)
    ////    std::cout << arr_1[i] <<" ";

    //std::cout << (clock() - start_time) / 1000.0 << std::endl;

    algorithm_test algorithm;
    algorithm.execute();


    std::cout << "Press \"Enter\" to exit"<<std::endl;
    std::getchar();
    return 0;
}
