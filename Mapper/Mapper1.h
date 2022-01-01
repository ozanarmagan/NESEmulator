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
        void resetRegisters();
        void updatePRG();
        void updateCHR();
        void updateMirroring();
        BYTE PRGRAM[8*1024];
        struct {
            BYTE controlReg = 0x00,tempReg = 0x00,tempRegNum = 0x00;
        } REGISTERS;
        BYTE CHR0 = 0x00;
        BYTE CHR1 = 0x00;
        BYTE CHRFull = 0x00;
        BYTE PRG0 = 0x00;
        BYTE PRG1 = 0x00;
        BYTE PRGFull = 0x00;
};


#endif