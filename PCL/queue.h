#ifndef _PCL_QUEUE_HEADER_
#define _PCL_QUEUE_HEADER_

#include <mutex>


namespace pcl {
    template<typename T>
    class container_queue {
    private:
        T value;
        container_queue<T>* next;
    public:
        container_queue(const T& new_value, container_queue<T>* prev):
            value(std::move(new_value))
        { 
            next = nullptr;
            prev->next = this; 
        }

        container_queue(const T& new_value):
            value(std::move(new_value))
        {
            next = nullptr;
        }

        //container_queue(T new_value) :
        //    value(std::move(new_value))
        //{
        //    next = nullptr;
        //}


        //std::shared_ptr<T> get_value() { return value; }
        T get_value() { return value; }

        container_queue* get_next() { return next; }
    }; // container_queue

    template<typename T>
    class list {
    private:
        mutable size_t list_size;
        mutable std::mutex this_mutex;
        container_queue<T>* head;
        container_queue<T>* tail;
    public:
        void push(const T& value);
        void push(T& value);

        bool pop(T& value);

        void front(T* T_ptr) const;
        std::shared_ptr<T> front() const;


        void back(T* T_ptr) const;


        size_t size() const;
        bool empty() const;

        list() : head(nullptr), tail(nullptr), list_size(0) {}
        ~list();

        list* operator=(const list& rhs) = delete;
        list(const list &copy_list) = delete;
    }; // list


    template<typename T>
    list<T>::~list() {
        std::lock_guard<std::mutex> guard(this_mutex);
        container_queue<T>* tmp;

        while (head) {
            tmp = head->get_next();
            delete head;
            head = tmp;
        }
        tail = nullptr;
    }

    template<typename T>
    bool list<T>::pop(T& value) {
        std::lock_guard<std::mutex> guard(this_mutex);
        if (list_size == 0)
            return false;

        if (head == tail) tail = nullptr;

        container_queue<T>* tmp = head->get_next();
        value = head->get_value();
        delete head;
        head = tmp;
        --list_size;
        return true;
    }

    template<typename T>
    void list<T>::push(const T& value) {
        std::lock_guard<std::mutex> guard(this_mutex);

        if (list_size == 0) {
            head = new container_queue<T>(value);        // error
            tail = head;
        }
        else
            tail = new container_queue<T>(std::move(value), tail);

        ++list_size;
    }

    template<typename T>
    void list<T>::push(T& value) {
        std::lock_guard<std::mutex> guard(this_mutex);

        if (list_size == 0) {
            head = new container_queue<T>(value);        // error
            tail = head;
        }
        else
            tail = new container_queue<T>(std::move(value), tail);

        ++list_size;
    }


    template<typename T>
    size_t list<T>::size() const {
        std::lock_guard<std::mutex> guard(this_mutex);
        return list_size;
    }

    template<typename T>
    bool list<T>::empty() const {
        std::lock_guard<std::mutex> guard(this_mutex);
        return head == nullptr;
    }

    template<typename T>
    void list<T>::front(T* T_ptr) const {
        std::lock_guard<std::mutex> guard(this_mutex);
        
        if (head != nullptr)
            *T_ptr = head->get_value();
        else
            T_ptr = nullptr;
    }

    template<typename T>
    std::shared_ptr<T> list<T>::front() const {
        std::lock_guard<std::mutex> guard(this_mutex);
        std::shared_ptr<T> ptr;

        if (head != nullptr)
            ptr = std::shared_ptr<T>{ new T(head->get_value()) };
        else
            ptr = nullptr;

        return ptr;
    }


    template<typename T>
    void list<T>::back(T* T_ptr) const {
        std::lock_guard<std::mutex> guard(this_mutex);
        
        if (head == nullptr)
            T_ptr = nullptr;
        else
            *T_ptr = head->get_value();
    }


} // pcl

#endif
