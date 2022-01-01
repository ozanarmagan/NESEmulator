#include "Mapper3.h"




BYTE Mapper3::MapReadCpu(ADDRESS address)
{
    if(address >= 0x8000 && address <= 0xFFFF)
        return cartridge->getPRGData((address - 0x8000) % (cartridge->getPRGNum() * 16 * 1024));
    else
        return 0x00;
}   


void Mapper3::MapWriteCpu(ADDRESS address,BYTE value)
{
    selectedCHRbank = value & 0x03;
}

BYTE Mapper3::MapReadPpu(ADDRESS address)
{
    return cartridge->getCHRData(selectedCHRbank * 0x2000 + address);
}

void Mapper3::MapWritePpu(ADDRESS address,BYTE value)
{   }