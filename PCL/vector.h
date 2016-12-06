#ifndef _PCL_VECTOR_HEADER_
#define _PCL_VECTOR_HEADER_

#include <vector>
#include <mutex>

namespace pcl {

    template <typename T>
    class vector {
    private:
        mutable std::mutex mut;
        std::vector<T> data_vector;

    public:
        vector() = default;

        vector(vector<T> const& other) {
            std::lock_guard<std::mutex> lk(mut);
            data_vector = other.data_queue;
        }

        void push_back(T new_value) {
            std::lock_guard<std::mutex> lk(mut);
            data_vector.push_back(std::move(new_value));
        }

        size_t max_size() const {
            std::lock_guard<std::mutex> lk(mut);
            return data_vector.max_size();
        }

        size_t size() const {
            std::lock_guard<std::mutex> lk(mut);
            return data_vector.size();
        }

        auto operator[](size_t pos) {
            std::lock_guard<std::mutex> lk(mut);
            return data_vector[pos];
        }

        bool empty() const {
            std::lock_guard<std::mutex> lk(mut);
            return data_vector.empty();
        }

        auto begin() {
            std::lock_guard<std::mutex> lk(mut);
            return data_vector.begin();
        }

        auto end() {
            std::lock_guard<std::mutex> lk(mut);
            return data_vector.end();
        }

        void resize(size_t count) {
            std::lock_guard<std::mutex> lk(mut);
            data_vector.resize(count);
        }

        void pop_back() {
            std::lock_guard<std::mutex> lk(mut);
            data_vector.pop_back();
        }

    }; // vector

}


#endif
