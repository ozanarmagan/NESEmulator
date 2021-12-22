#include <fstream>
#include <thread>

#include "../Utils/handler.h"
#include "../Cartridge/Cartridge.h"
#include "../Mapper/MapperBase.h"
#include "../Bus/Bus.h"
#include "../Bus/PPUBus.h"
#include "../PPU/PPU.h"
#include "../CPU/CPU.h"
#include "../Display/Display.h"


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
        uint64_t clock = 0;
};