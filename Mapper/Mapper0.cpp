#include "Mapper0.h"

Mapper0::Mapper0(BYTE PRGNum,BYTE CHRNum,Cartridge* cart) : MapperBase(PRGNum,CHRNum,cart)
{  PRGNum = cartridge->getPRGNum(); }

BYTE Mapper0::MapReadCpu(ADDRESS address)
{
    return cartridge->getPRGData(address & (PRGNum > 1 ? 0x7FFF : 0x3FFF));
}

void Mapper0::MapWriteCpu(ADDRESS address,BYTE value)
{
    cartridge->setPRGData(address & (PRGNum > 1 ? 0x7FFF : 0x3FFF),value);
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


