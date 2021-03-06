#include "Mapper0.h"

namespace nesemulator
{
    Mapper0::Mapper0(Cartridge* cart) : MapperBase(cart)
    {  }

    BYTE Mapper0::MapReadCpu(ADDRESS address)
    {
        if(address >= 0x8000 && address <= 0xFFFF)
            return cartridge->getPRGData((address - 0x8000) % (cartridge->getPRGNum() * 16 * 1024));
        else
            return 0x00;
    }

    void Mapper0::MapWriteCpu(ADDRESS address,BYTE value)
    {
        
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

}

