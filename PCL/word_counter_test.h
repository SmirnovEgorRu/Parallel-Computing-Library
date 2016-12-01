#ifndef _PCL_WORD_COUNTER_TEST_HEADER_
#define _PCL_WORD_COUNTER_TEST_HEADER_

#include "word_counter.h"
#include <iostream>

class word_counter_test {
public:
    void execute() {
        size_t size = 0;

        std::cout << "test word_counter was started" << std::endl;

        std::string str[] = {"", " ", "g", " g", "g ", "dfgdfg", "hfghfghfghfghfghfghfghfghfghfghfghfghfghfghfghf","g j", "f f f f f f f f f f f ", "fghfghfgfgh fgh fghfghfghfgd dd ddg    "};
        int number[] = { 0, 0, 1, 1, 1, 1, 1, 2, 11, 5 };


        for (int i = 0; i < sizeof(number) / sizeof(int);++i) {
            pcl::word_counter counter(str[i]);
            size = counter.size();

            if (size != number[i]) {
                std::cout << "incorrect value!" << std::endl;
                std::cout << "string = \"" << str[i] << "\"" << std::endl;
                std::cout << "expected value = " << number[i] << std::endl;
                std::cout << "resulting value = " << size << std::endl;
                std::cout << "the test is failed!" << std::endl;
                return;
            }
        }

        std::cout << "the test was completed successfully!" << std::endl << std::endl;
    }
};

#endif
