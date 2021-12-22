#include "PPU.h"


void PPU::tick()
{
    if(row < 240 && line < 256 && line > -1)
        display->setPixel(line,row,{155,0,155});
    else if(row > 240)
        VBLANK = true;
    if(++line >= 341)
    {
        line = 0;
        if(row++ >= 261)
        {
            frameDone = true;
            row = 0;
            VBLANK = false;
        }
    }
}


std::ostream& operator<<(std::ostream &out,PPU &ppu)
{
    out << std::dec;
    out << "\nPPU LINE : " << ppu.line <<" ROW : " << ppu.row;
    out << std::endl;

    return out;
}

