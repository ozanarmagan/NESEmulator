#ifndef PPU_H
#define PPU_H

#include "../Utils/handler.h"
#include "../Bus/Bus.h"
#include "../Cartridge/Cartridge.h"
#include "../Bus/PPUBus.h"

class PPU
{
    public:
        PPU(Bus* mem,Cartridge *cartridge,PPUBus& ppuBus) : mainBus(mem),ppuBus(ppuBus),cartridge(cartridge) { };
        void tick() { } ;
        friend class PPUBus;
    private:
        Bus *mainBus;
        Cartridge *cartridge;
        PPUBus& ppuBus;
};

#endif