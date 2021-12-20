#ifndef CARTRIDGE_H
#define CARTRIDE_H

#include <iostream>
#include <vector>
#include "../CPU/CPU.h"
#include "../PPU/PPU.h"
#include "../Utils/handler.h"
#include "../Bus/Bus.h"
#include "../Bus/PPUBus.h"

class Cartridge 
{
    public:
        Cartridge(const std::string fileName);
        void setCPU(CPU *cpu) {cpu = cpu; };
        void setPPU(PPU *ppu) {ppu = ppu; };
        BYTE getPRGnum() { return PRGnum; };
        BYTE getCHRnum() { return CHRnum; };
        BYTE getPRGdata(ADDRESS address);
        BYTE getCHRdata(ADDRESS address);
        void setPGRdata(ADDRESS address,BYTE value);
        void setCHRdata(ADDRESS address,BYTE value);
    private:
        CPU *cpu;
        PPU *ppu;
        BYTE* PRGmemory;
        BYTE* CHRmemory;

        BYTE mapperID = 0;
        BYTE PRGnum = 0;
        BYTE CHRnum = 0;
};

#endif