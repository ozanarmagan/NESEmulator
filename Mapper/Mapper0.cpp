#include "Mapper0.h"

Mapper0::Mapper0(BYTE PRGNum,BYTE CHRNum) : MapperBase(PRGNum,CHRNum)
{   }

BYTE Mapper0::MapReadCpu(ADDRESS address)
{
    return cartridge->getPRGData(address & (cartridge->getPRGNum() > 1 ? 0x7FFF : 0x3FFF));
}

void Mapper0::MapWriteCpu(ADDRESS address,BYTE value)
{
    cartridge->setPRGData(address & (cartridge->getPRGNum() > 1 ? 0x7FFF : 0x3FFF),value);
}



BYTE Mapper0::MapReadPpu(ADDRESS address)
{
    return cartridge->getCHRData(address);
}   


void Mapper0::MapWritePpu(ADDRESS address,BYTE value)
{
    if(cartridge->getCHRNum() == 0)
        cartridge->setCHRData(address,value);
}


