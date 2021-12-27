#ifndef PPU_H
#define PPU_H


//#define PPUDEBUG

#include "../Utils/handler.h"
#include "../Bus/Bus.h"
#include "../Cartridge/Cartridge.h"
#include "../Bus/PPUBus.h"
#include "../Display/Display.h"


#ifdef PPUDEBUG

#include <stdio.h>

#endif


class PPU
{
    public:
        PPU(Display* display,Bus* mem,Cartridge* cartridge,PPUBus* ppuBus) : display(display),mainBus(mem),cartridge(cartridge),ppuBus(ppuBus) { 
#ifdef PPUDEBUG
        ppuLog = fopen("./Logs/ppu.log","w+"); 
#endif
        };
        void tick();
        const bool inline isFrameDone() { return frameDone; };
        void clearFrameDone() { frameDone = false; };
        friend std::ostream& operator<<(std::ostream &out,PPU &ppu); // For logging stuff
        void getPatternTable();
        friend class PPUBus;
    private:
        Bus* mainBus;
        Cartridge* cartridge;
        PPUBus* ppuBus;
        Display* display;
        BYTE PPU_BUFFER;
        short int col = 0,row = 0;
        BYTE OFFSET_X = 0;
        bool frameDone = false;
        bool oddFrame = false;
#ifdef PPUDEBUG
        FILE* ppuLog;
        void log();
#endif

};

#endif