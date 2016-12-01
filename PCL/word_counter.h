#ifndef _PCL_WORD_COUNTER_HEADER_
#define _PCL_WORD_COUNTER_HEADER_

#include <string>
#include "scheduler.h"

namespace pcl {

    class word_counter {
    private:
        const std::string  separators;
        std::string text;

        const size_t n_threads;
        mutable size_t result = 0;
        size_t length;
        size_t block_size;

        mutable std::vector<pcl::promise_value<size_t> > sum;

        size_t countup_words(const std::string& str, const size_t start, const size_t end) const
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
            return count;
        }

        size_t word_counter::min(const size_t x, const size_t y) const{
            return x <= y ? x : y;
        }

        void delete_repeats_on_joints() const{
            if (block_size == 0) return;
            for (size_t i = 1; i < n_threads; ++i)
                if (separators.find(text[i*block_size]) == std::string::npos && separators.find(text[i*block_size - 1]) == std::string::npos) {
                        --result;
                }
        }

    public:
        size_t size() const {
            pcl::scheduler tasks;

            for (size_t i = 0; i < n_threads-1; ++i) {
                sum[i] = tasks.add_task(&word_counter::countup_words,
                    this,
                    std::ref(text),
                    i * block_size,
                    (i + 1) * block_size
                );
            }

            sum[n_threads-1] = tasks.add_task(&word_counter::countup_words,
                this,
                std::ref(text),
                (n_threads - 1) * block_size,
                length
            );

            tasks.wait();

            for (size_t i = 0; i < n_threads; ++i)
                result += sum[i].get_value();

            delete_repeats_on_joints();
            return result;
        }

        word_counter(const std::string& new_text) :
            n_threads(pcl::scheduler::max_threads()),
            separators("\t\n\r !.?%^+-*/=()[]{},;:-#$&`~\""),
            text(new_text),
            sum(n_threads)
        {
            length = text.size();
            block_size = length / n_threads;
        }

    }; // word_counter
} // pcl

#endif
