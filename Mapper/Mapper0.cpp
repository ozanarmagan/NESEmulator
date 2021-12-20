#include "Mapper0.h"

BYTE Mapper0::MapReadCpu(ADDRESS address)
{
    return cartridge->getPRGdata(address & (cartridge->getPRGnum() > 1 ? 0x7FFF : 0x3FFF));
}

void Mapper0::MapWriteCpu(ADDRESS address,BYTE value)
{
    cartridge->setPGRdata(address & (cartridge->getPRGnum() > 1 ? 0x7FFF : 0x3FFF),value);
}



BYTE Mapper0::MapReadPpu(ADDRESS address)
{
    return cartridge->getCHRdata(address);
}   


void Mapper0::MapWritePpu(ADDRESS address,BYTE value)
{
    if(cartridge->getCHRnum() == 0)
        cartridge->setCHRdata(address,value);
}


