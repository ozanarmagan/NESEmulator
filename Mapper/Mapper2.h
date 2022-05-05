#ifndef MAPPER2_H
#define MAPPER2_H


#include "MapperBase.h"

namespace nesemulator
{
    class Mapper2 : public MapperBase
    {
        public:
            Mapper2(Cartridge* cart);
            BYTE MapReadCpu(ADDRESS address) override;
            void MapWriteCpu(ADDRESS address,BYTE value) override;
            BYTE MapReadPpu(ADDRESS address) override;
            void MapWritePpu(ADDRESS address,BYTE value) override;
        private:
            BYTE lowerBankOffset = 0x00; // Offset for first 16KB switchable PRG ROM
            BYTE higherBankOffset = 0x00; // Offset for lasat 16KB fixed PRG ROM
            friend class NES;
    };
}

#endif


