
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
#include "../APU/APU.h"
#include "../Display/Display.h"
#include "../Controller/Controller.h"
#include "../Audio/Audio.h"
#include "../Utils/Queue.h"

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
        APU apu;
        CPU cpu;    
        Audio audio;
        Display display;
        void setMapper();
        void mainLoop();
        void tick();
        void DMA();
        uint64_t clock = 0;
        SDL_Event events;
        Controller controller;
        //Queue<AUDIO> audioQueue;
        double innerClock = 0,outerClock = 0;
        inline static double cyclesPerSample = 121.53;
        inline static double cyclesPerBuffer = 121.53 * 735;
};