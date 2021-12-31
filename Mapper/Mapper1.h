#ifndef MAPPER1_H
#define MAPPER1_H

#include "MapperBase.h"
#include "../Cartridge/Cartridge.h"

class Mapper1 : public MapperBase
{
    public:
        Mapper1(Cartridge *cart);
        virtual BYTE MapReadCpu(ADDRESS address) final;
        virtual void MapWriteCpu(ADDRESS address,BYTE value) final;
        virtual BYTE MapReadPpu(ADDRESS address) final;
        virtual void MapWritePpu(ADDRESS address,BYTE value) final;
    private:
        void reset();
        BYTE PRGRAM[32*1024];
        struct {
            BYTE controlReg = 0x00,tempReg = 0x00,tempRegNum = 0x00;
        } REGISTERS;
        BYTE CHRreg0 = 0x00;
        BYTE CHRreg1 = 0x00;
        BYTE CHRregFull = 0x00;
        BYTE PRGreg0 = 0x00;
        BYTE PRGreg1 = 0x00;
        BYTE PRGregFull = 0x00;
};


#endif