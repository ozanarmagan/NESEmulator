#ifndef BUS_H
#define BUS_H

#include <memory>
#include <algorithm>
#include "../Utils/handler.h"
#include "PPUBus.h"
#include "../Cartridge/Cartridge.h"
#include "../Mapper/MapperBase.h"
#include "../Mapper/Mapper0.h"

class Bus
{
    public:
        Bus(PPUBus& ppuBus) : ppuBus(ppuBus) { std::fill_n(memory, 2049, 0xCC); };
        BYTE readFromMemory(ADDRESS address);
        void writeToMemory(ADDRESS address,BYTE value);
        void clearMemoryBlock(ADDRESS start,ADDRESS end);
        void setMapper(std::shared_ptr<MapperBase> _mapper) { mapper = _mapper; }; // not implemented yet
        void setControllerMemory(BYTE controllers[2]) { controllerCache[0] = controllers[0]; controllerCache[1] = controllerCache[1]; };
        void print(int end = 20);
    private:
        BYTE memory[2048]; // CPU RAM
        BYTE controllerCache[2]; // Temporary cache to write in every frame
        BYTE controllerMemory[2]; // Bytes for controllers
        PPUBus& ppuBus; 
        std::shared_ptr<MapperBase> mapper;

};


#endif