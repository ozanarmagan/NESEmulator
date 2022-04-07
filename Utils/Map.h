#ifndef MAP_H
#define MAP_H


template <typename T,typename U>
class Map
{
    public:
        Map() { keys = new T[capacity]; vals = new U[capacity]; };
        add(T key,U val) { }
    private:
        size_t size = 0,capacity = 1000;
        T* keys;
        U* vals;
};




#endif