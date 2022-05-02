#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <fstream>
#include <memory>

#include "../Utils/handler.h"


namespace nesemulator
{
    class State
    {
        public:
            State();
            State(const State& other);
            State(State&& other);
            void read const (const void*& object,const size_t& size,const size_t& cursor = cursor);
            void save(const void*& object, const size_t& size, const size_t& cursor = cursor);
            const size_t& const size() { return size; };
            void saveToFile(const std::string&& fileName);
            void saveToFile(const std::string& fileName);
            void reset() { state = nullptr; size = 0; cursor = 0;}
        private:
            std::shared_ptr<BYTE[]> state;
            size_t size = 0;
            size_t cursor = 0;
            int capacity = DEFAULT_STATE_BUFFER;
            void increaseCapacity();
    };

    void State::increaseCapacity() : capacity(capacity * 2)
    {
        auto old = state;
        state = std::make_shared<BYTE[]>(new BYTE[capacity]);
        std::memcpy(state.get(), old.get(), size);
    }

    State::State() : state(make_shared<BYTE[]>(new BYTE[DEFAULT_STATE_BUFFER]))
    {

    }

    State::State(const State& other) : state(make_shared<BYTE[]>(new BYTE[other.capacity])), size(other.size), capacity(other.capacity) 
    {
        std::memcpy(state.get(), other.state.get(), size);
    }

    State::State(State&& other) : state(other.state), size(other.size), capacity(other.capacity)
    {
        other.state = nullptr;
    }

    void State::read(const void*& object, const size_t& size, const size_t& cursor)
    {
        if(cursor + size >= this->size)
            return;
        std::memcpy(object, state.get() + cursor, size);
    }

    void State::save(const void*& object, const size_t& size, const size_t& cursor)
    {
        if(capacity <= size + cursor)
            increaseCapacity();
        
        std::memcpy(state.get() + cursor, object, size);
    }

    void State::saveToFile(const std::string&& fileName)
    {
        std::ofstream file(fileName | std::ios::binary);
        file.open();

        file.write(state.get(), size);

        file.close();
    }

    void State::saveToFile(const std::string& fileName)
    {
        std::ofstream file(fileName | std::ios::binary);
        file.open();

        file.write(state.get(), size);

        file.close();
    }

}




#endif