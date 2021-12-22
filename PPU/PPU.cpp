#include "PPU.h"


void PPU::tick()
{
    if(row < 240 && line < 256)
        display->setPixel(line,row,{155,155,155});
    else if(row > 240)
        VBLANK = true;
    line++;
    if(line >= 341)
    {
        line = 0;
        row++;
        if(row >= 261)
        {
            frameDone = true;
            row = -1;
            VBLANK = false;
        }
    }
}