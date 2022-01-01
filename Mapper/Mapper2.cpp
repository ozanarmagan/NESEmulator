#include "Mapper2.h"

Mapper2::Mapper2(Cartridge* cart) : MapperBase(cart)
{
    higherBankOffset = cartridge->getPRGNum() - 1;
}



BYTE Mapper2::MapReadCpu(ADDRESS address)
{
    if(address < 0xC000)
        return cartridge->getPRGData(lowerBankOffset * 0x4000 + (address - 0x8000));
    else 
        return cartridge->getPRGData(higherBankOffset * 0x4000 + (address - 0xC000));
}


void Mapper2::MapWriteCpu(ADDRESS address,BYTE value)
{
    if(address >= 0x8000 && address <= 0xFFFF)
        lowerBankOffset = value & 0x0F;
}


BYTE Mapper2::MapReadPpu(ADDRESS address)
{
    return cartridge->getCHRData(address);
}   


void Mapper2::MapWritePpu(ADDRESS address,BYTE value)
{
    if(cartridge->getCHRNum() == 0)
        cartridge->setCHRData(address,value);
}
