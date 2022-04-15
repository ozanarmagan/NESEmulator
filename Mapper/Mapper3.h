#ifndef MAPPER3_H
#define MAPPER3_H


#include "MapperBase.h"

namespace nesemulator
{
    class Mapper3 : public MapperBase
    {
        public:
            Mapper3(Cartridge* cart) : MapperBase(cart) {   };
            BYTE MapReadCpu(ADDRESS address) override;
            void MapWriteCpu(ADDRESS address,BYTE value) override;
            BYTE MapReadPpu(ADDRESS address) override;
            void MapWritePpu(ADDRESS address,BYTE value) override;
        private:
            BYTE selectedCHRbank = 0x00; // Selected CHR bank on Cartridge 
    };
}



#endif


