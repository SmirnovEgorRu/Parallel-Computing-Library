#ifndef _PCL_WORD_COUNTER_HEADER_
#define _PCL_WORD_COUNTER_HEADER_

#include <string>
#include "scheduler.h"

class word_counter {
private:
    std::string const separators;
    std::string       text;

    size_t const n_threads;
    size_t       length;
    size_t       block_size;
    size_t       result = 0;

    std::vector<size_t> sum;

    void countup_words(const std::string& str,
        const size_t start,
        const size_t end,
        size_t& sum)
    {
        size_t count = 0;
        bool in_word = false;

        for (size_t i = start; i < end; ++i) {
            if (separators.find(str[i]) != std::string::npos)
                in_word = false;
            else if (!in_word) {
                in_word = true;
                count++;
            }
        }
        sum = count;
    }

    size_t word_counter::min(const size_t x, const size_t y) {
        return x <= y ? x : y;
    }

    void delete_repeats_on_joints() {
        for (size_t i = 1; i < n_threads; ++i)
            if (separators.find(text[i*block_size]) == std::string::npos && separators.find(text[i*block_size - 1]) == std::string::npos)
                --result;
    }

public:
    size_t size() {
        pcl::scheduler tasks;

        if (length == 1) {
            if (separators.find(text[0]) != std::string::npos) return 0;
            return 1;
        }
        else if (length == 0) return 0;

        for (size_t i = 0; i < n_threads; ++i) {
            tasks.add_task(&word_counter::countup_words,
                this,
                std::ref(text),
                i * block_size,
                min((i + 1) * block_size, length),
                std::ref(sum[i])
            );
        }
        tasks.wait();

        for (size_t i = 0; i < n_threads; ++i)
            result += sum[i];

        delete_repeats_on_joints();
        return result;
    }

    word_counter(const std::string& new_text) :
        n_threads(2),
        sum(n_threads, 0),
        separators("\t\n\r !.?%^+-*/=()[]{},;:-#$&`~\""),
        text(new_text)
    {
        length = text.size();
        block_size = length / n_threads;
    }
};

#endif
