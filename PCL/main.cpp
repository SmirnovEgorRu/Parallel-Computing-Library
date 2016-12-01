#include "scheduler.h"
#include "word_counter.h"

#include <iostream>
#include <ctime>

#include "scheduler_test.h"
#include "word_counter_test.h"



int main() {
    scheduler_test y;
    y.execute();

    word_counter_test x;
    x.execute();

    std::cout << "Press \"Enter\" to exit"<<std::endl;
    std::getchar();
    return 0;
}
