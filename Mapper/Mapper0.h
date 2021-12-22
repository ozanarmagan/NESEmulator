#ifndef MAPPER0_H
#define MAPPER0_H


#include "MapperBase.h"

class Mapper0 : public MapperBase
{
    public:
        Mapper0(BYTE PRGNum,BYTE CHRNum,Cartridge* cart);
        BYTE MapReadCpu(ADDRESS address) override;
        void MapWriteCpu(ADDRESS address,BYTE value) override;
        BYTE MapReadPpu(ADDRESS address) override;
        void MapWritePpu(ADDRESS address,BYTE value) override;
    private:
        BYTE PRGNum;
};



#endif


