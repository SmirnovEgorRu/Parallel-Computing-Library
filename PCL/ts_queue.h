#ifndef _PCL_TF_QUEUE_HEADER_
#define _PCL_TF_QUEUE_HEADER_

//#include <queue>
#include <deque>
//#include <memory>
#include <mutex>
//#include <condition_variable>

namespace pcl {
    template <typename T>
    class queue {
        std::deque<T> container;
        mutable std::mutex queue_mutex;

    public:
        void push(T data) {
            std::lock_guard<std::mutex> lock(queue_mutex);
            container.push_front(std::move(data));
        }

        bool empty() const {
            std::lock_guard<std::mutex> lock(queue_mutex);
            return container.empty();
        }

        bool try_pop(T& data) {
            std::lock_guard<std::mutex> lock(queue_mutex);
            if (container.empty())
                return false;
            data = std::move(container.front());
            container.pop_front();
            return true;
        }

        // TODO test it!
        /*bool try_steal(T& data) {
            std::lock_guard<std::mutex> lock(queue_mutex);
            if (container.empty())
                return false;
            data = std::move(container.back());
            container.pop_back();
            return true;
        }
        */

        queue() = default;
        queue(const queue& other) = delete;
        queue& operator= (const queue& other) = delete;
    }; // queue

} // pcl

#endif 
