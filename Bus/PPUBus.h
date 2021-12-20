#ifndef PPUBUS_H
#define PPUBUS_H

#include "../Utils/handler.h"

class PPUBus
{
    public:
        PPUBus();
        BYTE readFromMemory_mainBus(ADDRESS address) const;
        void writeToMemory_mainBus(ADDRESS address,BYTE value);
        BYTE readFromMemory(ADDRESS address) const;
        void writeToMemory(ADDRESS address,BYTE value);
        void clearMemoryBlock(ADDRESS start,ADDRESS end);
        friend class PPU;
    private:
        BYTE nameTables[2048];
        BYTE palettes[32];
        PIXEL colors[64];
};


#endif