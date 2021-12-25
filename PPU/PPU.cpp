#include "PPU.h"


void PPU::tick()
{
    if(row == -1 && col == 0)
        ppuBus->PPUSTATUS.VBLANK = 0;
    if(row < 240 && row > -1 && col < 256 && col > -1)
        display->setPixel(col,row,{155,0,155});
    else if(row == 241 && col == 1)
    {
        ppuBus->PPUSTATUS.VBLANK = 1;
        if(ppuBus->PPUCTRL.ENABLE_NMI == 1)
            ppuBus->NMI = true;
    }
    if(++col >= 341)
    {
        col = 0;
        if(row++ >= 261)
        {
            frameDone = true;
            row = -1;
        }
    }
}


std::ostream& operator<<(std::ostream &out,PPU &ppu)
{
    out << std::dec;
    out << "\nPPU COL : " << ppu.col <<" ROW : " << ppu.row;
    out << std::endl;

    return out;
}

void PPU::getPatternTable()
{
    for(ADDRESS tileY = 0; tileY < 16; tileY++)
        for(ADDRESS tileX = 0; tileX < 16; tileX++)
        {
            ADDRESS offset = tileY * 256 + tileX * 16;
            for(BYTE row = 0;row < 8;row++)
            {
                BYTE lsb_1 = ppuBus->readFromMemory(0x1000 + offset + row + 0x0000);
                BYTE msb_1 = ppuBus->readFromMemory(0x1000 + offset + row + 0x0008);
                BYTE lsb_0 = ppuBus->readFromMemory(offset + row + 0x0000);
                BYTE msb_0 = ppuBus->readFromMemory(offset + row + 0x0008);
                for(BYTE col = 0;col < 8;col++)
                {
                    BYTE pixel0 = (lsb_0 & 0x01) + (msb_0 & 0x01);
                    BYTE pixel1 = (lsb_1 & 0x01) + (msb_1 & 0x01);
                    lsb_1 >>= 1;
                    msb_1 >>= 1;
                    lsb_0 >>= 1;
                    msb_0 >>= 1;
                    display->setPixelDebug(tileX * 8 + (7 - col) + 142, (tileY * 8 + 5) + row, ppuBus->colors[ ppuBus->readFromMemory(0x3F00 + pixel1) & 0x3F]);
                    display->setPixelDebug(tileX * 8 + (7 - col), (tileY * 8 + 5) + row, ppuBus->colors[ ppuBus->readFromMemory(0x3F00 + pixel0) & 0x3F]);
                }
            }
        }
}

 