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
        BYTE readFromMemory(ADDRESS address);
        void writeToMemory(ADDRESS address,BYTE value);
        void clearMemoryBlock(ADDRESS start,ADDRESS end);
        void setMapper(std::shared_ptr<MapperBase> _mapper) { mapper = _mapper; };
        void setControllerMemory(BYTE controllers[2]) { controllerCache[0] = controllers[0]; controllerCache[1] = controllerCache[1]; };
        bool getDMAStatus() const { return DMA; };
        void print(int end = 20);
        friend class NES;
    private:
        void DMAReadNext(){ DMA_data = readFromMemory((DMA_high << 8) | (DMA_low)); };
        void writeOAM() { ppuBus.writeOAM(DMA_low++, DMA_data); };
        BYTE memory[2048]; // CPU RAM
        BYTE controllerCache[2]; // Temporary cache to write in every frame
        BYTE controllerMemory[2]; // Bytes for controllers
        PPUBus& ppuBus; 
        std::shared_ptr<MapperBase> mapper;
        bool DMA = false,DMA_dummy = true;
        BYTE DMA_low  = 0x00;
        BYTE DMA_high = 0x00;
        BYTE DMA_data = 0x00;
};


#endif