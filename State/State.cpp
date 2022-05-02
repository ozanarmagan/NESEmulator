#include "State.h"

namespace nesemulator
{
    void State::increaseCapacity()
    {
        auto old = state;
        capacity *= 2;
        state = std::shared_ptr<BYTE>(new BYTE[capacity],[](unsigned char* p){ delete[] p; });
        std::memcpy(state.get(), old.get(), size);
    }

    State::State() : state(std::shared_ptr<BYTE>(new BYTE[DEFAULT_STATE_BUFFER],[](unsigned char* p){ delete[] p; }))
    {

    }

    State::State(const State& other) : state(std::shared_ptr<BYTE>(new BYTE[other.capacity],[](unsigned char* p) { delete[] p; })), size(other.size), capacity(other.capacity) 
    {
        std::memcpy((char*)state.get(), (char*)other.state.get(), size);
    }

    State::State(State&& other) : state(other.state), size(other.size), capacity(other.capacity)
    {
        other.state = nullptr;
    }

    void State::read(void*& object,size_t& size,int cursor)
    {
        if(cursor == -1)
            cursor = this->cursor;
        if(cursor + size >= this->size)
            return;
        std::memcpy(object, (char*) state.get() + cursor, size);
    }

    void State::save(void*& object,size_t& size, int cursor)
    {
        if(cursor == -1)
            cursor = this->cursor;
        if(capacity <= size + cursor)
            increaseCapacity();
        
        std::memcpy((char*)(state.get() + cursor), object, size);
    }

    void State::saveToFile(const std::string&& fileName)
    {
        std::ofstream file(fileName, std::ios::binary);

        file.write((char*)state.get(), size);

        file.close();
    }

    void State::saveToFile(const std::string& fileName)
    {
        std::ofstream file(fileName, std::ios::binary);

        file.write((char*)state.get(), size);

        file.close();
    }
}