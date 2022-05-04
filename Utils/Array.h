#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <iterator>
#include <initializer_list>
#include <cstring>

template <typename T>
class Array
{
    public:
        Array();
        Array(Array<T> &other);
        Array(std::initializer_list<T> list);
        Array(const T* rawArray,const size_t& size);
        int inline getSize() const { return size; };
        void clear();
        void add(const T element);
        void insert(const T element,int index);
        T& operator[](int i) { return head[i]; };
        inline T& last() { return head[size - 1]; };
        inline T& first() {return *head; };
        bool isEmpty() const { return size == 0; };
        ~Array();
        class iterator : public std::iterator<std::random_access_iterator_tag, T>
        {
            public:
                using diffType = typename std::iterator<std::random_access_iterator_tag, T>::difference_type;    
                iterator() : ptr(nullptr) {  };
                iterator(T* rhs) : ptr(rhs) {   };
                iterator(const T& rhs) : ptr(rhs) { };
                inline iterator& operator+=(diffType rhs) { ptr += rhs; return *this; };
                inline iterator& operator-=(diffType rhs) { ptr -= rhs; return *this; };
                inline T& operator*() const { return *ptr; };
                inline T& operator->() const { return ptr; };
                inline T& operator[](diffType rhs) const { return ptr[rhs]; };
                inline iterator& operator++() { ptr++; return *this; };
                inline iterator& operator--() { ptr--; return *this; };
                inline iterator& operator++(int) { iterator temp(*this);++ptr; return temp; };
                inline iterator& operator--(int) { iterator temp(*this);--ptr; return temp; };
                inline diffType operator-(const iterator& rhs) const { return ptr - rhs.ptr; };
                inline friend iterator operator+(diffType lhs, const iterator& rhs) { return iterator(lhs + rhs.ptr); };
                inline friend iterator operator-(diffType lhs, const iterator& rhs) { return iterator(lhs - rhs.ptr); };

                inline bool operator==(const iterator& rhs) const { return ptr == rhs.ptr; };
                inline bool operator!=(const iterator& rhs) const { return ptr != rhs.ptr; };
                inline bool operator<(const iterator& rhs) const { return ptr < rhs.ptr; };
                inline bool operator>(const iterator& rhs) const { return ptr > rhs.ptr; };
                inline bool operator<=(const iterator& rhs) const { return ptr <= rhs.ptr; };
                inline bool operator>=(const iterator& rhs) const { return ptr >= rhs.ptr; };
            private:
                T* ptr;
        };
        inline iterator begin() { return iterator(head); };
        inline iterator end()  { return iterator(head + size); };
    private:
        T* head;
        size_t size,capacity;
        bool isFull() const { return size == capacity; };
        increaseCapacity();
};



template <typename T>
Array<T>::Array() : capacity(10000),size(0)
{
    head = new T[capacity];
}

template <typename T>
Array<T>::Array(Array<T>& other) : capacity(other.getCapacity()),size(other.getSize())
{
    head = new T[other.getCapacity()];
    for(int i = 0;i < other.getSize();i++)
        head[i] = other[i];
}

template <typename T>
Array<T>::Array(std::initializer_list<T> list) : capacity(10000),size(0)
{
    head = new T[capacity];
    for(auto element : list)
        add(element);
}





template <typename T>
void Array<T>::add(T element)
{    
    head[size++] = element;
    if(isFull())
        increaseCapacity();
}


template <typename T>
void Array<T>::insert(T element,int index)
{
    if(index >= size || index < 0)
        return;
    for(int i = size;i > index;i--)
        head[i] = head[i-1];
    head[index] = element;
    size++;
    if(isFull())
        increaseCapacity();
}



template <typename T>
void Array<T>::clear()
{
    std::memset(head, 0xFF, size * sizeof(T));
    size = 0;
}





template <typename T>
Array<T>::~Array<T>()
{
    delete [] head;
}

template <typename T>
Array::Array(const T* rawArray,const size_t& size)
{
    head = rawArray;
    this->size = size;
    this->capacity = size;
    increaseCapacity();
}

template <typename T>
Array<T>::increaseCapacity()
{
    auto oldHead = head;
    head = new T[capacity * 2];
    capacity *= 2;
    for(int i = 0;i < size;i++)
        head[i] = oldHead[i];
}




#endif