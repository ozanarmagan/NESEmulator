#ifndef PPUBUS_H
#define PPUBUS_H

#include "../Utils/handler.h"
#include "../Mapper/MapperBase.h"
#include <memory>


class PPUBus
{
    public:
        PPUBus();
        BYTE readFromMemory_mainBus(ADDRESS address);
        void writeToMemory_mainBus(ADDRESS address,BYTE value);
        BYTE readFromMemory(ADDRESS address);
        void writeToMemory(ADDRESS address,BYTE value);
        void setMapper(std::shared_ptr<MapperBase> _mapper);
        bool getNMI() const {return NMI; };
        void setNMI(bool val) { NMI = val; };
        friend class PPU;
        friend class Bus;
    private:
        BYTE nameTables[2048];
        BYTE palettes[32];
        PIXEL_RGB colors[64];
        std::shared_ptr<MapperBase> mapper;
        BYTE addressToggle = 0x00;
        FLAG ppuBuffer = 0x00;
        ADDRESS ppuAddress = 0x0000;
        bool oddFrame = false;
        bool NMI = false;
        union
        {
            struct 
            {
                ADDRESS CO_X : 5 ;
                ADDRESS CO_Y : 5 ;
                ADDRESS NAMETABLE_X : 1;
                ADDRESS NAMETABLE_Y : 1;
                ADDRESS OFFSET_Y : 3;
                ADDRESS blank : 1;
            };
            ADDRESS combined;
        } LOOPY_TRANSFER; // This register from Loopy's docs
        
        union
        {
            struct 
            {
                FLAG unused : 5;
                FLAG SPRT_OVERFLOW : 1;
                FLAG SPRT_ZERO_HIT : 1;
                FLAG VBLANK : 1;
            };
            BYTE combined;
        } PPUSTATUS;

        union 
        {
            struct
            {
                FLAG GRAY_SCALE : 1;
                FLAG RENDER_BCKGRND_LEFT : 1;
                FLAG RENDER_SPRTS_LEFT : 1;
                FLAG RENDER_BCKGRD : 1;
                FLAG RENDER_SPRTS : 1;
                FLAG ENCHANCED_R : 1;
                FLAG ENCHANCED_G : 1;
                FLAG ENCHANCED_B : 1; 
            };
            BYTE combined; 
        } PPUMASK;

        union
        {
            struct
            {
                FLAG NAMETABLE_X : 1;
                FLAG NAMETABLE_Y : 1;
                FLAG INCREMENT_MODE : 1;
                FLAG PATTERN_SPRT : 1;
                FLAG PATTERN_BCKGRND : 1;
                FLAG SPRT_SIZE : 1;
                FLAG SLAVE : 1;
                FLAG ENABLE_NMI : 1;
            };
            BYTE combined; 
        } PPUCTRL;

        enum class RENDERSTATE
        {
            PRE_RENDER,RENDER,POST_RENDER,VBLANK
        } renderState;

};


#endif