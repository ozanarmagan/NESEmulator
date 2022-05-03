#ifndef STATE_MANAGEMENT_H
#define STATE_MANAGEMENT_H

#include <cstring>

#include "../Utils/handler.h"
#include "../APU/AudioWaves.h"
#include "../Utils/Array.h"

namespace nesemulator
{

    namespace StateManagement
    {
        // Struct to store cartridge data for states
        struct CartridgeState
        {
            size_t PRGROMSize;
            size_t CHRROMSize;
            std::unique_ptr<BYTE[]> PRGROM;
            std::unique_ptr<BYTE[]> CHRROM;
            void initCartridge(const size_t& PRGROMSize,const size_t& CHRROMSize)
            {
                this->PRGROMSize = PRGROMSize;
                this->CHRROMSize = CHRROMSize;
                PRGROM = std::unique_ptr<BYTE[]>(new BYTE[PRGROMSize]);
                CHRROM = std::unique_ptr<BYTE[]>(new BYTE[CHRROMSize]);
            }

            void loadPRGData(std::shared_ptr<BYTE> PRGROM)
            {
                std::memcpy((char*)PRGROM.get(),(char*)PRGROM.get(),PRGROMSize);
            }

            void loadCHRData(std::shared_ptr<BYTE> CHRROM)
            {
                std::memcpy((char*)CHRROM.get(),(char*)CHRROM.get(),CHRROMSize);
            }
        };


        // Struct to store CPU state
        struct CPUState
        {
            BYTE A;
            BYTE X;
            BYTE Y;
            BYTE SP;
            BYTE STATUS;
            TIMER clock;
            ADDRESS programCounter;
            BYTE cycleRemaining;
            BYTE additionalCycle0,additionalCycle1; 
            BYTE currentOpCode;
            INSTRUCTION currentInstruction;
        };

        struct APUState
        {
            PulseWave pulse1;
            PulseWave pulse2;
            TriangleWave triangle;
            NoiseWave noise;
            DMC dmc;
            TIMER clock;
            FLAG incomingFrameCounterReset;
            BYTE countToFrameCounterReset;
            AUDIOINT frameCounter;
            BYTE frameCounterMode;
            HighPassFilter high1;
            HighPassFilter high2;
            LowPassFilter low;
        };

        struct BusState
        {
            BYTE memory[2048];
            BYTE controllerCache[2];
            BYTE controllerMemory[2];
            bool DMA,DMA_dummy;
            BYTE DMA_low;
            BYTE DMA_high;
            BYTE DMA_data;
        };

        struct PPUBusState
        {
            BYTE nameTables[4096];
            BYTE palettes[32];
            PIXEL_RGB colors[64];
            bool addressToggle = false;
            BYTE ppuBuffer;
            bool NMI = false;
            BYTE BG_PIXEL;
            BYTE BG_PALETTE;
            BYTE FG_PIXEL;
            BYTE FG_PALETTE;
            BYTE FG_PRIORITY;
            BYTE PPUSTATUS;
            BYTE PPUMASK;
            BYTE PPUCTRL;
            BYTE BG_NEXT_ID; // BG_RENDER_FETCH
            BYTE BG_NEXT_LOW;
            BYTE BG_NEXT_HIGH;
            BYTE BG_NEXT_ATTR;
            ADDRESS BG_SHIFTER_PATTERN_LOW;
            ADDRESS BG_SHIFTER_PATTERN_HIGH;
            ADDRESS BG_SHIFTER_ATTR_LOW;
            ADDRESS BG_SHIFTER_ATTR_HIGH;
            ADDRESS LOOPY;
            ADDRESS vRAM;
            ADDRESS tempRAM;
            BYTE FINE_X;
            OBJECT_ATTRIBUTE OAM[64];
            BYTE OAM_offset;
            Array<BYTE> SPRT_SHIFTER_LOW;
            Array<BYTE> SPRT_SHIFTER_HIGH;
            BYTE SPRT_PATTERN_LOW,SPRT_PATTERN_HIGH;
            ADDRESS SPRT_PATTERN_ADDR_L;
        };

        struct DisplayState
        {
            PIXEL currentFrame[RENDER_WIDTH * RENDER_HEIGHT]; // Pixel array of current frame
            PIXEL nextFrame[RENDER_WIDTH * RENDER_HEIGHT]; // Pixel array of next frame
#ifdef DEBUG
            PIXEL debug[DEBUG_WIDTH * DEBUG_HEIGHT]; // Pixel array of debug frame
#endif
        };

        namespace Mapper
        {
            struct MapperState
            {

            };

            struct Mapper1State : MapperState
            {
                BYTE PRGRAM[8*1024];
                struct {
                    BYTE controlReg,tempReg,tempRegNum;
                } REGISTERS;
                BYTE CHR0;
                BYTE CHR1;
                BYTE CHRFull;
                BYTE PRG0;
                BYTE PRG1;
                BYTE PRGFull;
            };

            struct Mapper2State : MapperState
            {
                BYTE lowerBankOffset;
                BYTE higherBankOffset;
            };

            struct Mapper3State : MapperState
            {
                BYTE selectedCHRbank;
            };
        }


        struct PPUState
        {
            short int col,row;
            FLAG frameDone;
            Array<OBJECT_ATTRIBUTE> nextRowSprites; 
            BYTE pixel;
            BYTE palette;
            FLAG spriteZeroIndicator;
            FLAG spriteZero;
            FLAG odd;
        };



        struct StateInfo
        {
            std::string fileName;
            short mapperID;
            TIMER systemTime;
            double innerClock;
            CartridgeState cartridge;
            CPUState cpu;
            APUState apu;
            BusState bus;
            PPUBusState ppuBus;
            BYTE controllerState[2];
            DisplayState display;
            Mapper::MapperState mapper;
            PPUState ppu;
        };
    }

}

#endif
