#ifndef PPUBUS_H
#define PPUBUS_H

#include "../Utils/handler.h"
#include "../Mapper/MapperBase.h"
#include <memory>


class PPUBus
{
    public:
        PPUBus();
        BYTE readFromMemory_mainBus(ADDRESS address) const;
        void writeToMemory_mainBus(ADDRESS address,BYTE value);
        BYTE readFromMemory(ADDRESS address) const { return 0; /* TO BE IMPLEMENTED */};
        void writeToMemory(ADDRESS address,BYTE value) { /* TO BE IMPLEMENTED */};
        void setMapper(std::shared_ptr<MapperBase> _mapper);
        friend class PPU;
        friend class Bus;
    private:
        BYTE nameTables[2048];
        BYTE palettes[32];
        PIXEL_RGB colors[64];
        std::shared_ptr<MapperBase> mapper;
};


#endif