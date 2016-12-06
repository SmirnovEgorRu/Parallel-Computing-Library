#ifndef _PCL_QUEUE_HEADER_
#define _PCL_QUEUE_HEADER_

#include <queue>
#include <memory>
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



    //template <typename T>
    //class queue {
    //private:
    //    struct node {
    //        std::shared_ptr<T> data;
    //        std::unique_ptr<node> next;
    //    };

    //    std::mutex head_mutex;
    //    std::unique_ptr<node> head;
    //    std::mutex tail_mutex;
    //    node* tail;
    //    std::condition_variable data_cond;

    //    node* get_tail() {
    //        std::lock_guard<std::mutex> tail_lock(tail_mutex);
    //        return tail;
    //    }

    //    std::unique_ptr<node> pop_head() {
    //        std::unique_ptr<node> old_head = std::move(head);
    //        head = std::move(old_head->next);
    //        return old_head;
    //    }

    //    std::unique_ptr<node> try_pop_head() {
    //        std::lock_guard<std::mutex> head_lock(head_mutex);
    //        if (head.get() == get_tail()) {
    //            return std::unique_ptr<node>();
    //        }
    //        return pop_head();
    //    }

    //    std::unique_ptr<node> try_pop_head(T& value) {
    //        std::lock_guard<std::mutex> head_lock(head_mutex);
    //        if (head.get() == get_tail()) {
    //            return std::unique_ptr<node>();
    //        }
    //        value = std::move(*head->data);
    //        return pop_head();
    //    }

    //public:
    //    queue(): head(new node), tail(head.get()){}
    //    queue(const queue& other) = delete;
    //    queue& operator= (const queue& other) = delete;
    //    

    //    //std::shared_ptr<T> wait_and_pop();
    //    //void wait_and_pop(T& value);

    //    void push(T new_value) {
    //        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));

    //        std::unique_ptr<node> p(new node);
    //        {
    //            std::lock_guard<std::mutex> tail_lock(tail_mutex);
    //            tail->data = new_data;
    //            node* const new_tail = p.get();
    //            tail->next = std::move(p);
    //            tail = new_tail;
    //        }
    //        data_cond.notify_one();
    //    }

    //    std::shared_ptr<T> try_pop() {
    //        std::unique_ptr<node> old_head = try_pop_head();
    //        return old_head ? old_head->data::std::shared_ptr<T>();
    //    }

    //    bool try_pop(T& value) {
    //        std::unique_ptr<node> const old_head = try_pop_head(value);
    //        return (bool)old_head;
    //    }

    //    bool empty() {
    //        std::lock_guard<std::mutex> head_lock(head_mutex);
    //        return (head.get() == get_tail());
    //    }

    //};

}

#endif
