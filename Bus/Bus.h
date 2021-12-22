#ifndef BUS_H
#define BUS_H

#include <memory>
#include "../Utils/handler.h"
#include "PPUBus.h"
#include "../Cartridge/Cartridge.h"
#include "../Mapper/MapperBase.h"
#include "../Mapper/Mapper0.h"

class Bus
{
    public:
        Bus(PPUBus& ppuBus) : ppuBus(ppuBus) { };
        BYTE readFromMemory(ADDRESS address) const;
        void writeToMemory(ADDRESS address,BYTE value);
        void clearMemoryBlock(ADDRESS start,ADDRESS end);
        void setMapper(std::shared_ptr<MapperBase> _mapper) { mapper = _mapper; }; // not implemented yet
        void print(int end = 20);
    private:
        BYTE memory[2048]; // CPU RAM
        PPUBus& ppuBus; 
        std::shared_ptr<MapperBase> mapper;

};


#endif