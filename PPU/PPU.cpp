#include "PPU.h"


void PPU::shift()
{
    if(ppuBus->PPUMASK.RENDER_BCKGRD == 1)
    {
        ppuBus->BG_SHIFTER_PATTERN_LOW.combined     <<= 1;
        ppuBus->BG_SHIFTER_PATTERN_HIGH.combined    <<= 1;
        ppuBus->BG_SHIFTER_ATTR_LOW.combined        <<= 1;
        ppuBus->BG_SHIFTER_ATTR_HIGH.combined       <<= 1;
    }

    if(ppuBus->PPUMASK.RENDER_SPRTS && col >= 1 && col <= 257)
        for(int i = 0;i < nextRowSprites.getSize();i++)
        {
            if(nextRowSprites[i].x > 0) nextRowSprites[i].x--;
            else
            {
                ppuBus->SPRT_SHIFTER_LOW[i]  <<= 1;
                ppuBus->SPRT_SHIFTER_HIGH[i] <<= 1;
            }
        }
}


void PPU::tick()
{

#ifdef PPUDEBUG
    log();
#endif



    if(row >= -1 && row < 240)
    {
        if(row == 0 && col == 0)
            col = 1;
        if(row == -1 && col == 1)
        {
            ppuBus->PPUSTATUS.VBLANK = 0;
            ppuBus->PPUSTATUS.SPRT_OVERFLOW = 0;
            for(auto element : ppuBus->SPRT_SHIFTER_LOW)
                element = 0;
            for(auto element : ppuBus->SPRT_SHIFTER_HIGH)
                element = 0;
        }
        if((col >= 2 && col < 258) || (col >= 321 && col < 338))
        {
            shift(); // To update both backgorund and foreground shifters
            int a = (col - 1) % 8;
            
            if(a == 0)
            {
                ppuBus->BG_SHIFTER_PATTERN_LOW.NEXT   =   ppuBus->BG_RENDER_FETCH.BG_NEXT_LOW;
                ppuBus->BG_SHIFTER_PATTERN_HIGH.NEXT  =   ppuBus->BG_RENDER_FETCH.BG_NEXT_HIGH;
                ppuBus->BG_SHIFTER_ATTR_LOW.NEXT      =   (ppuBus->BG_RENDER_FETCH.BG_NEXT_ATTR & 0x01) ? 0xFF : 0x00;
                ppuBus->BG_SHIFTER_ATTR_HIGH.NEXT     =  (ppuBus->BG_RENDER_FETCH.BG_NEXT_ATTR & 0x02) ? 0xFF : 0x00;

                ppuBus->BG_RENDER_FETCH.BG_NEXT_ID = ppuBus->readFromMemory(0x2000 | (ppuBus->vRAM.combined & 0x0FFF));
            } 
            else if(a == 2)
            {
                ppuBus->BG_RENDER_FETCH.BG_NEXT_ATTR = ppuBus->readFromMemory(0x23C0 | (ppuBus->vRAM.NT_Y << 11)
                                                                                     | (ppuBus->vRAM.NT_X << 10)
                                                                                     | ((ppuBus->vRAM.CO_Y >> 2) << 3)
                                                                                     | (ppuBus->vRAM.CO_X >> 2));
                if(ppuBus->vRAM.CO_Y & 0x02) ppuBus->BG_RENDER_FETCH.BG_NEXT_ATTR  >>= 4;
                if(ppuBus->vRAM.CO_X & 0x02) ppuBus->BG_RENDER_FETCH.BG_NEXT_ATTR  >>= 2;
                ppuBus->BG_RENDER_FETCH.BG_NEXT_ATTR &= 0x03;
            }
            else if(a == 4)
                ppuBus->BG_RENDER_FETCH.BG_NEXT_LOW = ppuBus->readFromMemory((ppuBus->PPUCTRL.PATTERN_BCKGRND << 12)
                                                                          + ((ADDRESS)ppuBus->BG_RENDER_FETCH.BG_NEXT_ID << 4)
                                                                          + (ppuBus->vRAM.FINE_Y));
            else if(a == 6)
                ppuBus->BG_RENDER_FETCH.BG_NEXT_HIGH = ppuBus->readFromMemory((ppuBus->PPUCTRL.PATTERN_BCKGRND << 12)
                                                                            + ((ADDRESS)ppuBus->BG_RENDER_FETCH.BG_NEXT_ID << 4)
                                                                            + (ppuBus->vRAM.FINE_Y) + 8);
            else if(a == 7)
                if(ppuBus->PPUMASK.RENDER_BCKGRD || ppuBus->PPUMASK.RENDER_SPRTS)
                {
                    if(ppuBus->vRAM.CO_X == 31)
                    {
                        ppuBus->vRAM.CO_X = 0;
                        ppuBus->vRAM.NT_X = ~ppuBus->vRAM.NT_X;
                    }
                    else
                        ppuBus->vRAM.CO_X++;
                }
        }
        if(col == 256)
            if(ppuBus->PPUMASK.RENDER_BCKGRD || ppuBus->PPUMASK.RENDER_SPRTS)
            {
                if(ppuBus->vRAM.FINE_Y < 7)
                    ppuBus->vRAM.FINE_Y++;
                else
                {
                    ppuBus->vRAM.FINE_Y = 0;
                    if(ppuBus->vRAM.CO_Y == 29)
                    {
                        ppuBus->vRAM.CO_Y = 0;
                        ppuBus->vRAM.NT_Y = ~ppuBus->vRAM.NT_Y;
                    }
                    else if(ppuBus->vRAM.CO_Y == 31)
                        ppuBus->vRAM.CO_Y = 0;
                    else
                        ppuBus->vRAM.CO_Y++;
                }
            }
        if(col == 257)
        {
            ppuBus->BG_SHIFTER_PATTERN_LOW.NEXT   =   ppuBus->BG_RENDER_FETCH.BG_NEXT_LOW;
            ppuBus->BG_SHIFTER_PATTERN_HIGH.NEXT  =   ppuBus->BG_RENDER_FETCH.BG_NEXT_HIGH;
            ppuBus->BG_SHIFTER_ATTR_LOW.NEXT      =   (ppuBus->BG_RENDER_FETCH.BG_NEXT_ATTR & 0x01) ? 0xFF : 0x00;
            ppuBus->BG_SHIFTER_ATTR_HIGH.NEXT     =  (ppuBus->BG_RENDER_FETCH.BG_NEXT_ATTR & 0x02) ? 0xFF : 0x00;
            if(ppuBus->PPUMASK.RENDER_BCKGRD || ppuBus->PPUMASK.RENDER_SPRTS)
            {
                ppuBus->vRAM.CO_X = ppuBus->tempRAM.CO_X;
                ppuBus->vRAM.NT_X = ppuBus->tempRAM.NT_X;
            }
        }
        if(col == 338 || col == 340)
            ppuBus->BG_RENDER_FETCH.BG_NEXT_ID = ppuBus->readFromMemory(0x2000 | (ppuBus->vRAM.combined & 0x0FFF));
        if(row == -1 && col >= 280 && col < 305)
            if(ppuBus->PPUMASK.RENDER_BCKGRD || ppuBus->PPUMASK.RENDER_SPRTS)
            {
                ppuBus->vRAM.CO_Y = ppuBus->tempRAM.CO_Y;
                ppuBus->vRAM.NT_Y = ppuBus->tempRAM.NT_Y;
                ppuBus->vRAM.FINE_Y = ppuBus->tempRAM.FINE_Y;
            }
        
        if(col == 257 && row >= 0)
        {
            nextRowSprites.clear();
            for(auto element : ppuBus->SPRT_SHIFTER_LOW)
                element = 0;
            for(auto element : ppuBus->SPRT_SHIFTER_HIGH)
                element = 0;

            for(int i = 0;i < 64 && nextRowSprites.getSize() < 9;i++)
            {
                ADDRESS offset = ((ADDRESS)row - ((ADDRESS)ppuBus->OAM[i].y));
                if(offset >= 0 && offset < (ppuBus->PPUCTRL.SPRT_SIZE ? 16 : 8))
                {
                    if(nextRowSprites.getSize() < 8)
                        nextRowSprites.add(ppuBus->OAM[i]);
                    
                }
            }
            ppuBus->PPUSTATUS.SPRT_OVERFLOW = (nextRowSprites.getSize() > 8) ? 1 : 0; 
        }

        if(col == 340)
        {
            for(int i = 0;i < nextRowSprites.getSize();i++)
            {
                ppuBus->SPRT_PATTERN_LOW = 0x00;ppuBus->SPRT_PATTERN_HIGH = 0x00;
                ppuBus->SPRT_PATTERN_ADDR_L = 0x0000;

                if(ppuBus->PPUCTRL.SPRT_SIZE == 0)
                {
                    if(!(nextRowSprites[i].attribute & 0x80)) // Vertical Flip Orientation
                        ppuBus->SPRT_PATTERN_ADDR_L = (ppuBus->PPUCTRL.PATTERN_SPRT << 12)  | (nextRowSprites[i].id << 4)  | (row - nextRowSprites[i].y);          
                
                    else
                        ppuBus->SPRT_PATTERN_ADDR_L = (ppuBus->PPUCTRL.PATTERN_SPRT << 12)  | (nextRowSprites[i].id << 4)  | (7 - row + nextRowSprites[i].y); 
                }
                else
                {
                    if(!(nextRowSprites[i].attribute & 0x80))
                    {
                        if(row - nextRowSprites[i].y < 8)
                            ppuBus->SPRT_PATTERN_ADDR_L = ((ppuBus->PPUCTRL.PATTERN_SPRT & 0x01) << 12)  | ((nextRowSprites[i].id & 0xFE) << 4)  | ((row - nextRowSprites[i].y) & 0x07);     
                        else
                            ppuBus->SPRT_PATTERN_ADDR_L = ((ppuBus->PPUCTRL.PATTERN_SPRT & 0x01) << 12)  | (((nextRowSprites[i].id & 0xFE) + 1) << 4)  | ((row - nextRowSprites[i].y) & 0x07);
                    }
                    else
                    {
                        if(row - nextRowSprites[i].y < 8)
                            ppuBus->SPRT_PATTERN_ADDR_L = ((ppuBus->PPUCTRL.PATTERN_SPRT & 0x01) << 12)  | (((nextRowSprites[i].id & 0xFE) + 1) << 4)  | ((7 - row + nextRowSprites[i].y) & 0x07);
                        else
                            ppuBus->SPRT_PATTERN_ADDR_L = ((ppuBus->PPUCTRL.PATTERN_SPRT & 0x01) << 12)  | ((nextRowSprites[i].id & 0xFE) << 4)  | ((7 - row + nextRowSprites[i].y) & 0x07);    
                    }
                }

                ppuBus->SPRT_PATTERN_LOW = ppuBus->readFromMemory(ppuBus->SPRT_PATTERN_ADDR_L);
                ppuBus->SPRT_PATTERN_HIGH = ppuBus->readFromMemory(ppuBus->SPRT_PATTERN_ADDR_L + 8);

                if(nextRowSprites[i].attribute & 0x40)
                {
                   reverseByte(&ppuBus->SPRT_PATTERN_LOW);
                   reverseByte(&ppuBus->SPRT_PATTERN_HIGH);
                }

                ppuBus->SPRT_SHIFTER_LOW[i]  = ppuBus->SPRT_PATTERN_LOW;
                ppuBus->SPRT_SHIFTER_HIGH[i] = ppuBus->SPRT_PATTERN_HIGH;
            }
        }
    }

    if(row == 241 && col == 1)
    {
        ppuBus->PPUSTATUS.VBLANK = 1;
        if(ppuBus->PPUCTRL.ENABLE_NMI)
            ppuBus->setNMI(true);
    }

    if(ppuBus->PPUMASK.RENDER_BCKGRD) // Check background
    {
        BYTE pL = (ppuBus->BG_SHIFTER_PATTERN_LOW.combined & (0x8000 >> ppuBus->FINE_X)) ? 1 : 0;
        BYTE pH = (ppuBus->BG_SHIFTER_PATTERN_HIGH.combined & (0x8000 >> ppuBus->FINE_X)) ? 1 : 0;

        ppuBus->BG_PIXEL = (pH << 1) | pL;

        pL = (ppuBus->BG_SHIFTER_ATTR_LOW.combined & (0x8000 >> ppuBus->FINE_X)) ? 1 : 0;
        pH = (ppuBus->BG_SHIFTER_ATTR_HIGH.combined & (0x8000 >> ppuBus->FINE_X)) ? 1 : 0;

        ppuBus->BG_PALETTE = (pH << 1) | pL;

    }
    else
    {
        ppuBus->BG_PIXEL = 0x00;
        ppuBus->BG_PALETTE = 0x00;
    }

    ppuBus->FG_PALETTE  = 0x00;
    ppuBus->FG_PIXEL    = 0x00;
    ppuBus->FG_PRIORITY = 0x00;

    if(ppuBus->PPUMASK.RENDER_SPRTS) // Check foreground 
    {
        for(int i = 0;i < nextRowSprites.getSize(); i++)
        {
            if(nextRowSprites[i].x == 0)
            {
                BYTE pL = (ppuBus->SPRT_SHIFTER_LOW[i] & 0x80) > 0;
                BYTE pH = (ppuBus->SPRT_SHIFTER_HIGH[i] & 0x80) > 0;
                ppuBus->FG_PIXEL = (pH << 1) | pL;

                ppuBus->FG_PALETTE = (nextRowSprites[i].attribute & 0x03) + 0x04;
                ppuBus->FG_PRIORITY = (nextRowSprites[i].attribute & 0x20) == 0;

                if(ppuBus->FG_PIXEL != 0)
                    break;
            }
        }
    }



    if(ppuBus->BG_PIXEL == 0x00 && ppuBus->FG_PIXEL == 0x00)
    {
        pixel = 0x00;
        palette = 0x00;
    }
    else if(ppuBus->BG_PIXEL == 0x00 && ppuBus->FG_PIXEL > 0x00)
    {
        pixel = ppuBus->FG_PIXEL;
        palette = ppuBus->FG_PALETTE;
    }
    else if(ppuBus->BG_PIXEL > 0x00 && ppuBus->FG_PIXEL == 0x00)
    {
        pixel = ppuBus->BG_PIXEL;
        palette = ppuBus->BG_PALETTE;
    }
    else if(ppuBus->BG_PIXEL > 0x00 && ppuBus->FG_PIXEL > 0x00)
    {
        pixel = ((ppuBus->FG_PRIORITY) ? ppuBus->FG_PIXEL : ppuBus->BG_PIXEL);
        palette = ((ppuBus->FG_PRIORITY) ? ppuBus->FG_PALETTE : ppuBus->BG_PALETTE);
    }



    if(row < 240 && col < 257 && row > -1 && col > 0)
        display->setPixel(col, row, ppuBus->colors[ ppuBus->readFromMemory(0x3F00 + (palette << 2) + pixel) & 0x3F]);
    
    if(++col >= 341)
    {
        col = 0;
        if(++row >= 261)
        {
            row = -1;
            frameDone = true;
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


#ifdef DEBUG
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
#endif

#ifdef PPUDEBUG

void PPU::log()
{
    fprintf(ppuLog,"VRAM:%04X TRAM:%04X TILE_ID:%02X SC:%d CY:%d SHIFTER:%04X MSB:%02X LSB:%02X\n",ppuBus->vRAM.combined,ppuBus->tempRAM.combined,ppuBus->BG_RENDER_FETCH.BG_NEXT_ID,row,col,ppuBus->BG_SHIFTER_PATTERN_HIGH.combined,ppuBus->BG_RENDER_FETCH.BG_NEXT_HIGH,ppuBus->BG_RENDER_FETCH.BG_NEXT_LOW);
}


#endif
 