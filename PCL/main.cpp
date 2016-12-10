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
#include "matrix_test.h"




int main() {
    //scheduler_test y;
    //y.execute();

    //word_counter_test x;
    //x.execute();

    //algorithm_test algorithm;
    //algorithm.execute();

    //queue_test queue;
    //queue.execute();

    //vector_test vector;
    //vector.execute();

    //parallel_for_test prl_for;
    //prl_for.execute();

	matrix_test matrix;
	matrix.execute();

    pcl::scheduler::delete_scheduler();
    std::cout << "Press \"Enter\" to exit"<<std::endl;
    std::getchar();
    return 0;
}
