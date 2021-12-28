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
        MapperBase(Cartridge *cartridge_): cartridge(cartridge_) {    };
        virtual BYTE MapReadCpu(ADDRESS address) = 0;
        virtual void MapWriteCpu(ADDRESS address,BYTE value) = 0;
        virtual BYTE MapReadPpu(ADDRESS address) = 0;
        virtual void MapWritePpu(ADDRESS address,BYTE value) = 0;
        void setCartridge(Cartridge *cart) { cartridge = cart; };
        void setMirroring(MIRRORING mirrorirngType) { mirroring = mirrorirngType; };
        MIRRORING getMirroring() { return mirroring; };
    protected:
        Cartridge *cartridge;
        MIRRORING mirroring = MIRRORING::HORIZONTAL;
};


#endif
