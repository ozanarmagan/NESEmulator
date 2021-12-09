#ifndef PPU_H
#define PPU_H

#include "../Utils/handler.h"


class PPU
{
    public:
        PPU(RAM& mem);
        void tick() { } ;
    private:
        RAM memory;
};

#endif