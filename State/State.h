#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <fstream>
#include <cstring>
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
            void read(void*& object, size_t& size, int cursor = -1);
            void save(void*& object, size_t& size, int cursor = -1);
            const size_t& getSize() { return size; };
            void saveToFile(const std::string&& fileName);
            void saveToFile(const std::string& fileName);
            void reset() { state = nullptr; size = 0; cursor = 0;}
        private:
            std::shared_ptr<BYTE> state;
            size_t size = 0;
            size_t cursor = 0;
            int capacity = DEFAULT_STATE_BUFFER;
            void increaseCapacity();
    };



}




#endif