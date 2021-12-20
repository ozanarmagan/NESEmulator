#ifndef MAPPER_H
#define MAPPER_H

#include "../Utils/handler.h"
#include "../Cartridge/Cartridge.h"

/*
    This is an abstract base class for implementation of each mapper 
    All the mappers MUST be derived from this class
*/

class MapperBase
{
    public:
        MapperBase(BYTE PRGnum,BYTE CHRnum) : PRGnum(PRGnum),CHRnum(CHRnum) {    };
        virtual BYTE MapReadCpu(ADDRESS address) = 0;
        virtual void MapWriteCpu(ADDRESS address,BYTE value) = 0;
        virtual BYTE MapReadPpu(ADDRESS address) = 0;
        virtual void MapWritePpu(ADDRESS address,BYTE value) = 0;
        void setCartridge(Cartridge *cart) { cartridge = cart; };
    protected:
        BYTE PRGnum,CHRnum;
        Cartridge *cartridge;
};


#endif
