#ifndef _PCL_QUEUE_HEADER_
#define _PCL_QUEUE_HEADER_

#include <queue>
#include <mutex>
#include <condition_variable>

namespace pcl {

    template <typename T>
    class queue {
    private:
        mutable std::mutex mut;
        std::queue<T> data_queue;
        std::condition_variable data_cond;

    public:
        queue() {}
        queue(queue const& other) {
            std::lock_guard<std::mutex> lk(mut);
            data_queue = other.data_queue;
        }

        void push(T new_value) {
            std::lock_guard<std::mutex> lk(mut);
            data_queue.push(new_value);
            data_cond.notify_one();
        }

        void wait_and_pop(T& value) {
            std::unique_lock<std::mutex> lk(mut);
            data_cond.wait(lk, [this] {return !data_queue.empty(); });
            value = data_queue.front();
            data_queue.pop();
        }

        bool try_pop(T& value) {
            std::lock_guard<std::mutex> lk(mut);
            if (data_queue.empty())
                return false;

            value = std::move(data_queue.front());

            data_queue.pop();
            return true;
        }

        bool empty() const {
            std::lock_guard<std::mutex> lk(mut);
            return data_queue.empty();
        }

    }; // queue

}

#endif
