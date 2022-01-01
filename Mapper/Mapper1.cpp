#include "Mapper1.h"


Mapper1::Mapper1(Cartridge* cart) : MapperBase(cart)
{ 
    REGISTERS.controlReg = 0x1C;
    PRG1 = cartridge->getPRGNum() - 1;
}


void Mapper1::resetRegisters()
{
    REGISTERS.controlReg |= 0x0C;
    REGISTERS.tempReg = 0x00;
    REGISTERS.tempRegNum = 0x00;
    updatePRG();
}


BYTE Mapper1::MapReadCpu(ADDRESS address)
{
    if(address < 0x8000)
        return PRGRAM[address % 8192];
    else if(address <= 0xFFFF)
    {
        if(REGISTERS.controlReg & 0x08)
        {
            if(address < 0xC000)
                return cartridge->getPRGData(PRG0 * 0x4000 + (address & 0x3FFF));
            else
                return cartridge->getPRGData(PRG1 * 0x4000 + (address & 0x3FFF));
        }
        else
            return cartridge->getPRGData(PRGFull * 0x8000 + (address & 0x7FFF));
    }
    else
        return 0x00;
}


void Mapper1::updatePRG()
{
    BYTE mode = (REGISTERS.controlReg >> 2) & 0x03;
    if(mode < 2)
        PRGFull = (REGISTERS.tempReg & 0x0E) >> 1;
    else if(mode == 2)
    {
        PRG0 = 0x00;
        PRG1 = REGISTERS.tempReg & 0x0F;
    }
    else
    {
        PRG0 = REGISTERS.tempReg & 0x0F;
        PRG1 = cartridge->getPRGNum() - 1;
    }
}


void Mapper1::updateCHR()
{

}

void Mapper1::MapWriteCpu(ADDRESS address,BYTE value)
{
    if(address < 0x8000)
        PRGRAM[address % 8192] = value;
    else
    {
        if(value & 0x80) // Reset if MSB set to 1
            resetRegisters();
        else 
        {
            REGISTERS.tempReg = ((value & 0x01) << 4) | (REGISTERS.tempReg >> 1);
            if(++REGISTERS.tempRegNum == 5)
            {
                if(address < 0xA000)
                {
                        REGISTERS.controlReg = REGISTERS.tempReg & 0x1F;
                        switch (REGISTERS.controlReg & 0x03)
                        {
                            case 0: setMirroring(MIRRORING::ONSCREENLOWER) ;break;
                            case 1: setMirroring(MIRRORING::ONSCREENHIGHER);break;
                            case 2: setMirroring(MIRRORING::VERTICAL)      ;break;
                            case 3: setMirroring(MIRRORING::HORIZONTAL)    ;break;
                        }
                }
                else if(address < 0xC000)
                {
                        if(REGISTERS.controlReg & 0x10)
                            CHR0 =  REGISTERS.tempReg & 0x1F;
                        else
                            CHRFull = REGISTERS.tempReg & 0x1E;
                }
                else if(address < 0xE000)
                    CHR1 = ((REGISTERS.controlReg & 0x10) ? (REGISTERS.tempReg & 0x1F): CHR1);
                else
                    updatePRG();
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
            return cartridge->getCHRData(CHR0 * 4096  + (address & 0x0FFF));
        else
            return cartridge->getCHRData(CHR1 * 4096  + (address & 0x0FFF));
    }
    else
        return cartridge->getCHRData(CHRFull * 8192 + (address & 0x1FFF));
    return 0x00;
}   


void Mapper1::MapWritePpu(ADDRESS address,BYTE value)
{
    if(cartridge->getCHRNum() == 0)
        cartridge->setCHRData(address,value);
    
}