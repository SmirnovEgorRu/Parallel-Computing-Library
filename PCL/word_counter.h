#ifndef _PCL_TF_FUNCTION_HEADER_
#define _PCL_TF_FUNCTION_HEADER_

#include <string>

/*
size_t words_counter(const std::string& str) {
    pcl::scheduler tasks;

    for (size_t i = 0; i < pcl::scheduler::max_threads(); ++i) {
        tasks.add_task();
        threads[i] = std::thread(
            &std_word_counter::countup_words,
            this,
            std::ref(text),
            i * block_size,
            min((i + 1) * block_size, length),
            std::ref(sum[i])
        );
    }
}


void countup_words(const std::string& str,
                   const size_t start,
                   const size_t end,
                   size_t& sum)
{
    size_t count = 0;
    bool in_word = false;
    
    std::string separators("\t\n\r !.?%^+*-/=()[]{},;:-#$&`~\"");

    for (size_t i = start; i < end; ++i) {
        if (separators.find(str[i]) != EOF)
            in_word = false;
        else if (!in_word) {
            in_word = true;
            count++;
        }
    }

    sum = count;
}

*/

#endif
