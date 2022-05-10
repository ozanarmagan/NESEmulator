#ifndef STATE_MANAGEMENT_H
#define STATE_MANAGEMENT_H

#include <cstring>
#include <memory>

#include "../Utils/handler.h"
#include "../APU/AudioWaves.h"
#include "../Utils/AudioFilters.h"
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

            void loadPRGData(BYTE* PRGROM)
            {
                std::memcpy((char*)this->PRGROM.get(),(char*)PRGROM,PRGROMSize);
            }

            void loadCHRData(BYTE* CHRROM)
            {
                std::memcpy((char*)this->CHRROM.get(),(char*)CHRROM,CHRROMSize);
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
            PulseWave pulse1 = PulseWave(1);
            PulseWave pulse2 = PulseWave(0);
            TriangleWave triangle;
            NoiseWave noise;
            DMC dmc;
            TIMER clock;
            FLAG incomingFrameCounterReset;
            BYTE countToFrameCounterReset;
            AUDIOINT frameCounter;
            BYTE frameCounterMode;
            HighPassFilter high1 = HighPassFilter(90);
            HighPassFilter high2 = HighPassFilter(440);
            LowPassFilter low  = LowPassFilter(14000);
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
            std::shared_ptr<Mapper::MapperState> mapper;
            PPUState ppu;
            void writeToFile(const std::string& fileName)
            {
                std::ofstream file(fileName, std::ios::binary);

                file.write((char*)&mapperID, sizeof(short));
                file.write((char*)&systemTime, sizeof(TIMER));
                file.write((char*)&innerClock, sizeof(double));
                
                file.write((char*)&apu,sizeof(APUState));
                file.write((char*)&cpu,sizeof(CPUState));
                file.write((char*)&bus,sizeof(BusState));

                switch (mapperID)
                {
                    case 0: break;
                    case 1: file.write((char*)mapper.get(),sizeof(Mapper::Mapper1State)); break;
                    case 2: file.write((char*)mapper.get(),sizeof(Mapper::Mapper2State)); break;
                    case 3: file.write((char*)mapper.get(),sizeof(Mapper::Mapper3State)); break;
                    default:
                        break;
                }

                file.write((char*)&ppu.col, sizeof(short));
                file.write((char*)&ppu.row, sizeof(short));
                file.write((char*)&ppu.frameDone, 1);
                file.write((char*)&ppu.odd, 1);
                file.write((char*)&ppu.palette, 1);
                file.write((char*)&ppu.pixel, 1);
                file.write((char*)&ppu.spriteZero, 1);
                file.write((char*)&ppu.spriteZeroIndicator, 1);
                int size = ppu.nextRowSprites.getSize();
                file.write((char*)&size,sizeof(int));
                file.write((char*)ppu.nextRowSprites.getRaw(),sizeof(OBJECT_ATTRIBUTE) * size);

                file.write((char*)&ppuBus.addressToggle, sizeof(bool));
                file.write((char*)&ppuBus.BG_NEXT_ATTR, 1);
                file.write((char*)&ppuBus.BG_NEXT_HIGH, 1);
                file.write((char*)&ppuBus.BG_NEXT_ID, 1);
                file.write((char*)&ppuBus.BG_NEXT_LOW, 1);
                file.write((char*)&ppuBus.BG_PALETTE, 1);
                file.write((char*)&ppuBus.BG_PIXEL, 1);
                file.write((char*)&ppuBus.BG_SHIFTER_ATTR_HIGH, sizeof(ADDRESS));
                file.write((char*)&ppuBus.BG_SHIFTER_ATTR_LOW, sizeof(ADDRESS));
                file.write((char*)&ppuBus.BG_SHIFTER_PATTERN_HIGH, sizeof(ADDRESS));
                file.write((char*)&ppuBus.BG_SHIFTER_PATTERN_LOW, sizeof(ADDRESS));
                file.write((char*)&ppuBus.colors, sizeof(PIXEL_RGB) * 64);
                file.write((char*)&ppuBus.FG_PALETTE, 1);
                file.write((char*)&ppuBus.FG_PIXEL, 1);
                file.write((char*)&ppuBus.FG_PRIORITY, 1);
                file.write((char*)&ppuBus.FINE_X, 1);
                file.write((char*)&ppuBus.nameTables, 4096);
                file.write((char*)&ppuBus.NMI, sizeof(bool));
                file.write((char*)&ppuBus.OAM, sizeof(OBJECT_ATTRIBUTE) * 64);
                file.write((char*)&ppuBus.OAM_offset, 1);
                file.write((char*)&ppuBus.palettes, 32);
                file.write((char*)&ppuBus.ppuBuffer, 1);
                file.write((char*)&ppuBus.PPUCTRL, 1);
                file.write((char*)&ppuBus.PPUMASK, 1);
                file.write((char*)&ppuBus.PPUSTATUS, 1);
                file.write((char*)&ppuBus.SPRT_PATTERN_ADDR_L, sizeof(ADDRESS));
                file.write((char*)&ppuBus.SPRT_PATTERN_HIGH, 1);
                file.write((char*)&ppuBus.SPRT_PATTERN_LOW, 1);
                file.write((char*)&ppuBus.tempRAM, sizeof(ADDRESS));
                file.write((char*)&ppuBus.vRAM, sizeof(ADDRESS));
                size = ppuBus.SPRT_SHIFTER_HIGH.getSize();
                file.write((char*)&size,sizeof(int));
                file.write((char*)ppuBus.SPRT_SHIFTER_HIGH.getRaw(), size);
                size = ppuBus.SPRT_SHIFTER_LOW.getSize();
                file.write((char*)&size,sizeof(int));
                file.write((char*)ppuBus.SPRT_SHIFTER_LOW.getRaw(), size);
                
                file.write((char*)&cartridge.PRGROMSize,sizeof(size_t));
                file.write((char*)cartridge.PRGROM.get(), cartridge.PRGROMSize);
                file.write((char*)&cartridge.CHRROMSize,sizeof(size_t));
                file.write((char*)cartridge.CHRROM.get(), cartridge.CHRROMSize);
            }

            void readFromFile(const std::string& fileName)
            {
                std::ifstream file(fileName, std::ios::binary);

                file.read((char*)&mapperID, sizeof(short));
                file.read((char*)&systemTime, sizeof(TIMER));
                file.read((char*)&innerClock, sizeof(double));
                
                file.read((char*)&apu,sizeof(APUState));
                file.read((char*)&cpu,sizeof(CPUState));
                file.read((char*)&bus,sizeof(BusState));

                switch (mapperID)
                {
                    case 0: break;
                    case 1: mapper = std::make_shared<Mapper::Mapper1State>();  file.read((char*)mapper.get(),sizeof(Mapper::Mapper1State)); break;
                    case 2: mapper = std::make_shared<Mapper::Mapper2State>();  file.read((char*)mapper.get(),sizeof(Mapper::Mapper2State)); break;
                    case 3: mapper = std::make_shared<Mapper::Mapper3State>();  file.read((char*)mapper.get(),sizeof(Mapper::Mapper3State)); break;
                    default:
                        break;
                }

                

                file.read((char*)&ppu.col, sizeof(short));
                file.read((char*)&ppu.row, sizeof(short));
                file.read((char*)&ppu.frameDone, 1);
                file.read((char*)&ppu.odd, 1);
                file.read((char*)&ppu.palette, 1);
                file.read((char*)&ppu.pixel, 1);
                file.read((char*)&ppu.spriteZero, 1);
                file.read((char*)&ppu.spriteZeroIndicator, 1);
                int size;
                file.read((char*)&size,sizeof(int));
                OBJECT_ATTRIBUTE* arr = new OBJECT_ATTRIBUTE[size];
                file.read((char*)arr,sizeof(OBJECT_ATTRIBUTE) * size);
                ppu.nextRowSprites = Array<OBJECT_ATTRIBUTE>(arr,size);
                file.read((char*)&ppuBus.addressToggle, sizeof(bool));
                file.read((char*)&ppuBus.BG_NEXT_ATTR, 1);
                file.read((char*)&ppuBus.BG_NEXT_HIGH, 1);
                file.read((char*)&ppuBus.BG_NEXT_ID, 1);
                file.read((char*)&ppuBus.BG_NEXT_LOW, 1);
                file.read((char*)&ppuBus.BG_PALETTE, 1);
                file.read((char*)&ppuBus.BG_PIXEL, 1);
                file.read((char*)&ppuBus.BG_SHIFTER_ATTR_HIGH, sizeof(ADDRESS));
                file.read((char*)&ppuBus.BG_SHIFTER_ATTR_LOW, sizeof(ADDRESS));
                file.read((char*)&ppuBus.BG_SHIFTER_PATTERN_HIGH, sizeof(ADDRESS));
                file.read((char*)&ppuBus.BG_SHIFTER_PATTERN_LOW, sizeof(ADDRESS));
                file.read((char*)&ppuBus.colors, sizeof(PIXEL_RGB) * 64);
                file.read((char*)&ppuBus.FG_PALETTE, 1);
                file.read((char*)&ppuBus.FG_PIXEL, 1);
                file.read((char*)&ppuBus.FG_PRIORITY, 1);
                file.read((char*)&ppuBus.FINE_X, 1);
                file.read((char*)&ppuBus.nameTables, 4096);
                file.read((char*)&ppuBus.NMI, sizeof(bool));
                file.read((char*)&ppuBus.OAM, sizeof(OBJECT_ATTRIBUTE) * 64);
                file.read((char*)&ppuBus.OAM_offset, 1);
                file.read((char*)&ppuBus.palettes, 32);
                file.read((char*)&ppuBus.ppuBuffer, 1);
                file.read((char*)&ppuBus.PPUCTRL, 1);
                file.read((char*)&ppuBus.PPUMASK, 1);
                file.read((char*)&ppuBus.PPUSTATUS, 1);
                file.read((char*)&ppuBus.SPRT_PATTERN_ADDR_L, sizeof(ADDRESS));
                file.read((char*)&ppuBus.SPRT_PATTERN_HIGH, 1);
                file.read((char*)&ppuBus.SPRT_PATTERN_LOW, 1);
                file.read((char*)&ppuBus.tempRAM, sizeof(ADDRESS));
                file.read((char*)&ppuBus.vRAM, sizeof(ADDRESS));

                file.read((char*)&size,sizeof(int));
                BYTE* bArr = new BYTE[size];
                file.read((char*)bArr, size);
                ppuBus.SPRT_SHIFTER_HIGH = Array<BYTE>(bArr,size);
                file.read((char*)&size,sizeof(int));
                bArr = new BYTE[size];
                file.read((char*)bArr, size);
                ppuBus.SPRT_SHIFTER_LOW = Array<BYTE>(bArr,size);
                

                file.read((char*)&cartridge.PRGROMSize,sizeof(size_t));
                cartridge.PRGROM = std::unique_ptr<BYTE[]>(new BYTE[cartridge.PRGROMSize]);
                file.read((char*)cartridge.PRGROM.get(), cartridge.PRGROMSize);
                file.read((char*)&cartridge.CHRROMSize,sizeof(size_t));
                cartridge.CHRROM = std::unique_ptr<BYTE[]>(new BYTE[cartridge.CHRROMSize]);
                file.read((char*)cartridge.CHRROM.get(), cartridge.CHRROMSize);
            }
        };
    }

}

#endif
