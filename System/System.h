
#include <fstream>

#include "../Utils/handler.h"
#include "../Cartridge/Cartridge.h"
#include "../Mapper/MapperBase.h"
#include "../Mapper/Mapper0.h"
#include "../Mapper/Mapper1.h"
#include "../Mapper/Mapper2.h"
#include "../Mapper/Mapper3.h"
#include "../Bus/Bus.h"
#include "../Bus/PPUBus.h"
#include "../PPU/PPU.h"
#include "../CPU/CPU.h"
#include "../Display/Display.h"
#include "../Controller/Controller.h"

class NES
{
    public:
        NES();
        NES(std::string fileName);
        void insertNESFile(std::string fileName);
        void start();
        void reset();
        void log();
    private:
        Cartridge cartridge;
        std::shared_ptr<MapperBase> mapper;
        PPUBus ppuBus;
        Bus bus;
        PPU ppu;
        CPU cpu;    
        Display display;
        void setMapper();
        void mainLoop();
        void tick();
        void DMA();
        uint64_t clock = 0;
        SDL_Event events;
        Controller controller;
};