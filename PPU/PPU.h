#ifndef PPU_H
#define PPU_H

#include "../Utils/handler.h"
#include "../Bus/Bus.h"
#include "../Cartridge/Cartridge.h"
#include "../Bus/PPUBus.h"
#include "../Display/Display.h"



class PPU
{
    public:
        PPU(Display* display,Bus* mem,Cartridge* cartridge,PPUBus* ppuBus) : display(display),mainBus(mem),cartridge(cartridge),ppuBus(ppuBus) {  };
        void tick();
        const bool inline isFrameDone() { return frameDone; };
        void clearFrameDone() { frameDone = false; };
        friend class PPUBus;
    private:
        Bus* mainBus;
        Cartridge* cartridge;
        PPUBus* ppuBus;
        Display* display;
        ADDRESS line = 0,row = 0;
        bool frameDone = false;
        bool VBLANK = false;
};

#endif