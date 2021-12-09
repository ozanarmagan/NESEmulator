#ifndef RAM_H
#define RAM_H
#include "../Utils/handler.h"


class RAM
{
    public:
        RAM();
        uint8_t readFromMemory(ADDRESS address) const;
        void writeToMemory(ADDRESS address,uint8_t value);
        void clearMemoryBlock(ADDRESS start,ADDRESS end);
        friend class CPU;
    private:
        uint8_t memory[64 * 1024]; // main memory will be allocated in construction
};


#endif