#include "Mapper1.h"


Mapper1::Mapper1(Cartridge* cart) : MapperBase(cart)
{ 
    REGISTERS.controlReg = 0x0C;
    PRGreg1 = cartridge->getPRGNum() - 1;
}


void Mapper1::reset()
{
    REGISTERS.controlReg |= 0x0C;
    REGISTERS.tempReg = 0x00;
    REGISTERS.tempRegNum = 0x00;
}


BYTE Mapper1::MapReadCpu(ADDRESS address)
{
    if(address >= 0x6000 && address <= 0x7FFF)
        return PRGRAM[address % 8192];
    else if(address >= 0x8000 && address <= 0xFFFF)
    {
        if(REGISTERS.controlReg & 0x10)
        {
            if(address < 0xC000)
                return cartridge->getPRGData(PRGreg0 * 0x4000 + (address & 0x3FFF));
            else
                return cartridge->getPRGData(PRGreg1 * 0x4000 + (address & 0x3FFF));
        }
        else
            return cartridge->getPRGData(PRGregFull * 0x8000 + (address & 0x7FFF));
    }
    else
        return 0x00;
}

void Mapper1::MapWriteCpu(ADDRESS address,BYTE value)
{
    if(address >= 0x6000 && address <= 0x7FFF)
        PRGRAM[address % 8192] = value;
    else if(address >= 0x8000 && address <= 0xFFFF)
    {
        if(value & 0x80) // Reset if MSB set to 1
            reset();
        else 
        {
            REGISTERS.tempReg = ((value & 0x01) << 4) | (REGISTERS.tempReg >> 1);
            if(++REGISTERS.tempRegNum == 5)
            {
                switch((address << 13) & 0x03)
                {
                    case 0:
                        REGISTERS.controlReg = REGISTERS.tempReg & 0x1F;
                        switch (REGISTERS.controlReg & 0x03)
                        {
                            case 0: setMirroring(MIRRORING::ONSCREENLOWER) ;break;
                            case 1: setMirroring(MIRRORING::ONSCREENHIGHER);break;
                            case 2: setMirroring(MIRRORING::VERTICAL)      ;break;
                            case 3: setMirroring(MIRRORING::HORIZONTAL)    ;break;
                        }
                        break;
                    case 1:
                        if(REGISTERS.controlReg & 0x10)
                            CHRreg0 =  REGISTERS.tempReg & 0x1F;
                        else
                            CHRregFull = REGISTERS.tempReg & 0x1E;
                    case 2:
                        CHRreg1 = ((REGISTERS.controlReg & 0X10) ? (REGISTERS.tempReg & 0X1F): CHRreg1); break;
                    case 3: 
                        BYTE mode = (REGISTERS.controlReg >> 2) & 0x03;
                        if(mode < 2)
                            PRGregFull = (REGISTERS.tempReg & 0x0E) >> 1;
                        else if(mode == 2)
                        {
                            PRGreg0 = 0x00;
                            PRGreg1 = REGISTERS.tempReg & 0x0F;
                        }
                        else
                        {
                            PRGreg0 = REGISTERS.tempReg & 0x0F;
                            PRGreg1 = cartridge->getPRGNum() - 1;
                        }
                        break;
                }  
                REGISTERS.tempReg = 0x00;
                REGISTERS.tempRegNum = 0;
            }
        }
    }
}



BYTE Mapper1::MapReadPpu(ADDRESS address)
{
    if(cartridge->getCHRNum() == 0)
        return cartridge->getCHRData(address);
    if(REGISTERS.controlReg & 0x10)
    {
        if(address < 0x1000)
            return cartridge->getCHRData(CHRreg0 * 4096  + (address & 0x0FFF));
        else
            return cartridge->getCHRData(CHRreg1 * 4096  + (address & 0x0FFF));
    }
    else
        return cartridge->getCHRData(CHRregFull * 8192 + (address & 0x1FFF));
    return 0x00;
}   


void Mapper1::MapWritePpu(ADDRESS address,BYTE value)
{
    if(cartridge->getCHRNum() == 0)
        cartridge->setCHRData(address,value);
    
}