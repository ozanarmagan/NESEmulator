#ifndef QUEUE_H
#define QUEUE_H


template <typename T>
class Queue
{
    public:
        Queue() { head = new T[capacity]; };
        ~Queue() { delete [] head;}
        void insert(const T& val);
        T pop();
        const size_t size() const { return total; };
    private:
        T* head;
        size_t total = 0;
        size_t capacity = 100000;
        void increaseCapacity();
};


template <typename T>
void Queue<T>::insert(const T& val)
{
    if(++total == capacity)
        increaseCapacity();
    head[total - 1] = val;
}

template <typename T>
T Queue<T>::pop()
{
    T val = head[0];
    for(int i = 1;i < total;i++)
        head[i - 1] = head[i];
    total--;
    return val;
}


template <typename T>
void Queue<T>::increaseCapacity()
{
    auto oldHead = head;
    capacity += 10000;
    head = new T[capacity];
    delete [] oldHead;
}

#endif