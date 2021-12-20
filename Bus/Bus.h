#ifndef BUS_H
#define BUS_H
#include "../Utils/handler.h"
#include "PPUBus.h"
#include "../Cartridge/Cartridge.h"
#include "../Mapper/MapperBase.h"

class Bus
{
    public:
        Bus(PPUBus* ppuBus) : ppuBus(ppuBus) {  };
        BYTE readFromMemory(ADDRESS address) const;
        void writeToMemory(ADDRESS address,BYTE value);
        void clearMemoryBlock(ADDRESS start,ADDRESS end);
        void setMapper(int mappperID); // not implemented yet
        void print(int end = 20);
    private:
        BYTE memory[2048]; // CPU RAM
        PPUBus* ppuBus;
        MapperBase* mapper;

};


#endif