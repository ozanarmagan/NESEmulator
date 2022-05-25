#include "System.h"

namespace nesemulator
{
    namespace
    {
        std::string bTohex(uint32_t n, uint8_t d)
        {
            std::string s(d, '0');
            for (int i =  d - 1; i >= 0; i--, n >>= 4)
                s[i] = "0123456789ABCDEF"[n & 0xF];
            return s;
        };
    }


    NES::NES() : cartridge(),apu(),ppuBus(),bus(ppuBus,apu),ppu(&display,&bus,&cartridge,&ppuBus),cpu(bus),controller(&bus),display(&events,controller)
    {
    }

    NES::NES(std::string fileName) : cartridge(),apu(),ppuBus(),bus(ppuBus,apu),ppu(&display,&bus,&cartridge,&ppuBus),cpu(bus),controller(&bus),display(&events,controller)
    {
        insertNESFile(fileName);   
    }

    void NES::insertNESFile(std::string fileName)
    {
        std::ifstream file(fileName,std::ios::binary);
        FileHeader header;
        if(file.is_open())
        {
            // Read header first
            file.read((char*)&header.name,4);
            file.read((char*)&header.PRGROMChunks,1);
            file.read((char*)&header.CHRROMChunks,1);
            file.read((char*)&header.mapper1,1);
            file.read((char*)&header.mapper2,1);
            file.read((char*)&header.PRGRAMSize,1);
            file.read((char*)&header.TVSystem1,1);
            file.read((char*)&header.TVSystem2,1);
            file.read((char*)&header.unused,5);
            if(header.mapper1 & 0x04)
            file.seekg(512,std::ios_base::cur);
            BYTE mapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
            bool isNES2 = (header.mapper2 & 0x0C) == 0x08;
            if(!isNES2)
            {
                std::cout << "NES File Type : 1.0" << std::endl;
                cartridge.setPRGNum(header.PRGROMChunks);
                BYTE* PRGmem = new BYTE[header.PRGROMChunks * 16384];
                file.read((char*) PRGmem,header.PRGROMChunks * 16384);
                cartridge.loadPRGData(PRGmem);
                cartridge.setCHRNum(header.CHRROMChunks);
                BYTE* CHRmem;
                if(header.CHRROMChunks == 0) // If it is CHR RAM
                {
                    cartridge.setCHRNum(1);
                    CHRmem = new BYTE[8192];
                }
                else
                {
                    CHRmem = new BYTE[header.CHRROMChunks * 8192];
                    cartridge.setCHRNum(header.CHRROMChunks);
                }
                file.read((char*) CHRmem,(header.CHRROMChunks != 0 ? header.CHRROMChunks : 1) * 8192);
                cartridge.loadCHRData(CHRmem);
            }
            else
            {
                std::cout << "NES File Type : 2.0" << std::endl;
                cartridge.setPRGNum(((header.PRGRAMSize & 0x07) << 8) | header.PRGROMChunks);
                BYTE* PRGmem = new BYTE[cartridge.getPRGNum() * 16384];
                file.read((char*) PRGmem,cartridge.getPRGNum() * 16384);
                cartridge.loadPRGData(PRGmem);
                cartridge.setCHRNum(((header.PRGRAMSize & 0x38) << 8) | header.CHRROMChunks);
                BYTE* CHRmem = new BYTE[cartridge.getCHRNum() * 8192];
                file.read((char*) CHRmem,cartridge.getCHRNum() * 8192);
                cartridge.loadCHRData(CHRmem);
            }
            std::cout << "Mapper ID:" << mapperID;
            cartridge.setMapperID(mapperID);
            setMapper();
            mapper->setMirroring((header.mapper1 & 0x01) ? MIRRORING::VERTICAL : MIRRORING::HORIZONTAL);
            file.close();
        }   
        else
        {
            std::cout << "FATAL ERROR: File couldn't read";
        }
    }


    void NES::setMapper()
    {
        switch (cartridge.getMapperID())
        {
        case 0:mapper = std::make_shared<Mapper0>(&cartridge);break;   
        case 1:mapper = std::make_shared<Mapper1>(&cartridge);break; 
        case 2:mapper = std::make_shared<Mapper2>(&cartridge);break; 
        case 3:mapper = std::make_shared<Mapper3>(&cartridge);break; 
        default:
            break;
        }

        bus.setMapper(mapper);
        ppuBus.setMapper(mapper);
    }   



    void NES::log()
    {

        bus.print();
        std::cout << cartridge;
    }


    void NES::start()
    {
        cpu.reset();
        std::cout << "\nLOG: " << "Main Loop Has Started!\n";

        mainLoop();
    }


    void NES::mainLoop()
    {
        RUN_FOREVER
        {
            while(SDL_GetQueuedAudioSize(1) < SAMPLE_PER_FRAME)
            {
                tick();
                if(innerClock >= cyclesPerSample)
                {
                    Audio::addToQueue(apu.output());
                    innerClock -= cyclesPerSample;
                }
                innerClock += 1.0;
            }
            display.renderFrame();

        if(controller.getSaveButtonStatus() && !hasSaved)
            saveCurrentState();
        if(controller.getLoadButtonStatus() && !hasLoaded)
            loadState();
        if(hasSaved && !controller.getSaveButtonStatus())
            hasSaved = false;
        if(hasLoaded && !controller.getLoadButtonStatus())
            hasLoaded = false;


    #ifdef DEBUG
            ppu.getPatternTable();
            display.renderDebugFrame();
    #endif
        }
    }


    void NES::tick()
    {
        ppu.tick();


        if(clock % 3 == 0) // PPU of NES is 3x faster than CPU in clock-speed wise,so there happens a CPU tick for every 3x PPU tick (which also means a system tick)
        {
            apu.tick();
            if(bus.getDMAStatus()) // Check DMA status from main bus before every CPU tick 
                DMA();
            else
                cpu.tick();
        }

        if(ppuBus.getNMI()) // If a Non-maskable Interrupt about to happen
        {
            cpu.NMI();
            ppuBus.setNMI(false);
        }


        clock++;
    }





    void NES::DMA() // Direct Memory Access 
    {
        if(bus.DMA_dummy) // Because of limitations of NES hardware,first odd CPU clock in start of DMA is a dummy-write
        {
            if(clock % 2 == 1)
                bus.DMA_dummy = false;
        }
        else // AFter dummy-write,DMA reads address from bus for next write in every even CPU cycle and writes to PPU Bus OAM via main bus in every odd CPU cycle
        {
            if(clock % 2 == 0)
                bus.DMAReadNext();
            else
            {
                bus.writeOAM();
                if(bus.DMA_low == 0x00) // DMA is done after writing 256 bytes (which is equal to approximately 1 CPU page)
                {
                    bus.DMA = false;
                    bus.DMA_dummy = true;
                }
            }
        }
    }

    void NES::saveCurrentState()
    {
        display.getNotificationManager().addNotification("Saving current state...",ui::notificationColors::info, 3000);
        StateManagement::State state;
        state.cartridge.initCartridge(cartridge.getPRGNum() * 16384, cartridge.getCHRNum() * 8192);
        state.cartridge.loadPRGData(cartridge.PRGmemory.get());
        state.cartridge.loadCHRData(cartridge.CHRmemory.get());
        state.apu.clock = apu.clock;
        state.apu.countToFrameCounterReset = apu.countToFrameCounterReset;
        state.apu.dmc = apu.dmc;
        state.apu.frameCounter = apu.frameCounter;
        state.apu.frameCounterMode = apu.frameCounterMode == APU::FRAMECOUNTERMODE::MODE4 ? 0 : 1;
        state.apu.high1 = apu.high1;
        state.apu.high2 = apu.high2;
        state.apu.incomingFrameCounterReset = apu.incomingFrameCounterReset;
        state.apu.low = apu.low;
        state.apu.noise = apu.noise;
        state.apu.pulse1 = apu.pulse1;
        state.apu.pulse2 = apu.pulse2;
        state.apu.triangle = apu.triangle;

        state.cpu.A = cpu.A;
        state.cpu.additionalCycle0 = cpu.additionalCycle0;
        state.cpu.additionalCycle1 = cpu.additionalCycle1;
        state.cpu.clock = cpu.clock;
        state.cpu.currentInstruction = cpu.currentInstruction;
        state.cpu.currentOpCode = cpu.currentOpCode;
        state.cpu.cycleRemaining = cpu.cycleRemaining;
        state.cpu.programCounter = cpu.programCounter;
        state.cpu.SP = cpu.SP;
        state.cpu.STATUS = cpu.STATUS.combined;
        state.cpu.X = cpu.X;
        state.cpu.Y = cpu.Y;

        std::memcpy(state.bus.controllerCache,bus.controllerCache,2);
        std::memcpy(state.bus.controllerMemory,bus.controllerMemory,2);
        state.bus.DMA = bus.DMA;
        state.bus.DMA_data = bus.DMA_data;
        state.bus.DMA_dummy = bus.DMA_dummy;
        state.bus.DMA_high = bus.DMA_high;
        state.bus.DMA_low = bus.DMA_low;
        std::memcpy(state.bus.memory,bus.memory,2048);
        

        std::memcpy(state.controllerState,controller.controllerStatus,2);

        state.innerClock = innerClock;
        state.systemTime = clock;

        state.ppu.col = ppu.col;
        state.ppu.frameDone = ppu.frameDone;
        state.ppu.nextRowSprites = ppu.nextRowSprites;
        state.ppu.odd = ppu.odd;
        state.ppu.palette = ppu.palette;
        state.ppu.pixel = ppu.pixel;
        state.ppu.row = ppu.row;
        state.ppu.spriteZero = ppu.spriteZero;
        state.ppu.spriteZeroIndicator = ppu.spriteZeroIndicator;


        state.ppuBus.addressToggle = ppuBus.addressToggle;
        state.ppuBus.BG_NEXT_ATTR = ppuBus.BG_RENDER_FETCH.BG_NEXT_ATTR;
        state.ppuBus.BG_NEXT_HIGH = ppuBus.BG_RENDER_FETCH.BG_NEXT_HIGH;
        state.ppuBus.BG_NEXT_ID = ppuBus.BG_RENDER_FETCH.BG_NEXT_ID;
        state.ppuBus.BG_NEXT_LOW = ppuBus.BG_RENDER_FETCH.BG_NEXT_LOW;
        state.ppuBus.BG_PALETTE = ppuBus.BG_PALETTE;
        state.ppuBus.BG_PIXEL = ppuBus.BG_PIXEL;
        state.ppuBus.BG_SHIFTER_ATTR_HIGH = ppuBus.BG_SHIFTER_ATTR_HIGH.combined;
        state.ppuBus.BG_SHIFTER_ATTR_LOW = ppuBus.BG_SHIFTER_ATTR_LOW.combined;
        state.ppuBus.BG_SHIFTER_PATTERN_HIGH = ppuBus.BG_SHIFTER_PATTERN_HIGH.combined;
        state.ppuBus.BG_SHIFTER_PATTERN_LOW = ppuBus.BG_SHIFTER_PATTERN_LOW.combined;
        state.ppuBus.FG_PALETTE = ppuBus.FG_PALETTE;
        state.ppuBus.FG_PIXEL = ppuBus.FG_PIXEL;
        state.ppuBus.FG_PRIORITY = ppuBus.FG_PRIORITY;
        state.ppuBus.FINE_X = ppuBus.FINE_X;
        state.ppuBus.vRAM = ppuBus.vRAM.combined;
        state.ppuBus.tempRAM = ppuBus.tempRAM.combined;
        std::memcpy(state.ppuBus.nameTables,ppuBus.nameTables,4096);
        std::memcpy(state.ppuBus.palettes,ppuBus.palettes,32);
        std::memcpy(state.ppuBus.OAM,ppuBus.OAM,64 * sizeof(OBJECT_ATTRIBUTE));
        state.ppuBus.NMI = ppuBus.NMI;
        state.ppuBus.OAM_offset = ppuBus.OAM_offset;
        state.ppuBus.PPUCTRL = ppuBus.PPUCTRL.combined;
        state.ppuBus.PPUMASK = ppuBus.PPUMASK.combined;
        state.ppuBus.PPUSTATUS = ppuBus.PPUSTATUS.combined;
        state.ppuBus.SPRT_PATTERN_ADDR_L = ppuBus.SPRT_PATTERN_ADDR_L;
        state.ppuBus.SPRT_PATTERN_HIGH = ppuBus.SPRT_PATTERN_HIGH;
        state.ppuBus.ppuBuffer = ppuBus.ppuBuffer;
        state.ppuBus.SPRT_SHIFTER_HIGH = ppuBus.SPRT_SHIFTER_HIGH;
        state.ppuBus.SPRT_SHIFTER_LOW = ppuBus.SPRT_SHIFTER_LOW;


        state.mirroringMode = mapper->getMirroring() == MIRRORING::VERTICAL ? 1 : 0;
        switch(cartridge.getMapperID())
        {
            case 0:
                state.mapperID = 0;
                break;
            case 1:
                state.mapperID = 1;
                state.mapper = std::make_shared<StateManagement::Mapper::Mapper1State>();
                std::memcpy(static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->PRGRAM,static_cast<Mapper1*>(mapper.get())->PRGRAM,8*1024);
                static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->REGISTERS.controlReg = static_cast<Mapper1*>(mapper.get())->REGISTERS.controlReg;
                static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->REGISTERS.tempReg = static_cast<Mapper1*>(mapper.get())->REGISTERS.tempReg;
                static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->REGISTERS.tempRegNum = static_cast<Mapper1*>(mapper.get())->REGISTERS.tempRegNum;
                static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->CHR0 = static_cast<Mapper1*>(mapper.get())->CHR0;
                static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->CHR1 = static_cast<Mapper1*>(mapper.get())->CHR1;
                static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->CHRFull = static_cast<Mapper1*>(mapper.get())->CHRFull;
                static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->PRG0 = static_cast<Mapper1*>(mapper.get())->PRG0;
                static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->PRG1 = static_cast<Mapper1*>(mapper.get())->PRG1;
                static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->PRGFull  = static_cast<Mapper1*>(mapper.get())->PRGFull;
                break;
            case 2:
                state.mapperID = 2;
                state.mapper = std::make_shared<StateManagement::Mapper::Mapper2State>();
                static_cast<StateManagement::Mapper::Mapper2State*>(state.mapper.get())->lowerBankOffset = static_cast<Mapper2*>(mapper.get())->lowerBankOffset;
                static_cast<StateManagement::Mapper::Mapper2State*>(state.mapper.get())->higherBankOffset = static_cast<Mapper2*>(mapper.get())->higherBankOffset;
                break;
            case 3:
                state.mapperID = 3;
                state.mapper = std::make_shared<StateManagement::Mapper::Mapper3State>();
                static_cast<StateManagement::Mapper::Mapper3State*>(state.mapper.get())->selectedCHRbank = static_cast<Mapper3*>(mapper.get())->selectedCHRbank;
                break;
            default:
                break;
        }

        state.writeToFile("save.bin");
        hasSaved = true;
    }

    void NES::loadState()
    {
        StateManagement::State state;
        state.readFromFile("save.bin");
        display.getNotificationManager().addNotification("Loaded state",ui::notificationColors::warning, 3000);
        apu.clock = state.apu.clock;
        apu.countToFrameCounterReset = state.apu.countToFrameCounterReset;
        apu.dmc = state.apu.dmc;
        apu.frameCounter = state.apu.frameCounter;
        apu.frameCounterMode =  state.apu.frameCounterMode ? APU::FRAMECOUNTERMODE::MODE5 : APU::FRAMECOUNTERMODE::MODE4;
        apu.high1 = state.apu.high1;
        apu.high2 = state.apu.high2;
        apu.incomingFrameCounterReset = state.apu.incomingFrameCounterReset;
        apu.low = state.apu.low;
        apu.noise =state.apu.noise;
        apu.pulse1 =state.apu.pulse1;
        apu.pulse2 =state.apu.pulse2;
        apu.triangle =state.apu.triangle;

        cpu.A = state.cpu.A;
        cpu.additionalCycle0 = state.cpu.additionalCycle0;
        cpu.additionalCycle1 = state.cpu.additionalCycle1;
        cpu.clock = state.cpu.clock;
        cpu.currentInstruction = cpu.table[state.cpu.currentOpCode];
        cpu.currentOpCode = state.cpu.currentOpCode;
        cpu.cycleRemaining = state.cpu.cycleRemaining;
        cpu.programCounter = state.cpu.programCounter;
        cpu.SP = state.cpu.SP;
        cpu.STATUS.combined = state.cpu.STATUS;
        cpu.X = state.cpu.X;
        cpu.Y = state.cpu.Y;

        std::memcpy(bus.controllerCache,state.bus.controllerCache,2);
        std::memcpy(bus.controllerMemory,state.bus.controllerMemory,2);
        bus.DMA = state.bus.DMA;
        bus.DMA_data = state.bus.DMA_data;
        bus.DMA_dummy = state.bus.DMA_dummy;
        bus.DMA_high = state.bus.DMA_high;
        bus.DMA_low = state.bus.DMA_low;
        std::memcpy(bus.memory,state.bus.memory,2048);
        

        std::memcpy(controller.controllerStatus,state.controllerState,2);

        innerClock = state.innerClock;
        clock = state.systemTime;

        ppu.col = state.ppu.col;
        ppu.frameDone = state.ppu.frameDone;
        ppu.nextRowSprites = state.ppu.nextRowSprites;
        ppu.odd = state.ppu.odd;
        ppu.palette = state.ppu.palette;
        ppu.pixel = state.ppu.pixel;
        ppu.row = state.ppu.row;
        ppu.spriteZero = state.ppu.spriteZero;
        ppu.spriteZeroIndicator = state.ppu.spriteZeroIndicator;


        ppuBus.addressToggle = state.ppuBus.addressToggle;
        ppuBus.BG_RENDER_FETCH.BG_NEXT_ATTR = state.ppuBus.BG_NEXT_ATTR;
        ppuBus.BG_RENDER_FETCH.BG_NEXT_HIGH = state.ppuBus.BG_NEXT_HIGH;
        ppuBus.BG_RENDER_FETCH.BG_NEXT_ID = state.ppuBus.BG_NEXT_ID;
        ppuBus.BG_RENDER_FETCH.BG_NEXT_LOW = state.ppuBus.BG_NEXT_LOW;
        ppuBus.BG_PALETTE = state.ppuBus.BG_PALETTE;
        ppuBus.BG_PIXEL = state.ppuBus.BG_PIXEL;
        ppuBus.BG_SHIFTER_ATTR_HIGH.combined = state.ppuBus.BG_SHIFTER_ATTR_HIGH;
        ppuBus.BG_SHIFTER_ATTR_LOW.combined = state.ppuBus.BG_SHIFTER_ATTR_LOW;
        ppuBus.BG_SHIFTER_PATTERN_HIGH.combined = state.ppuBus.BG_SHIFTER_PATTERN_HIGH;
        ppuBus.BG_SHIFTER_PATTERN_LOW.combined = state.ppuBus.BG_SHIFTER_PATTERN_LOW;
        ppuBus.FG_PALETTE = state.ppuBus.FG_PALETTE;
        ppuBus.FG_PIXEL = state.ppuBus.FG_PIXEL;
        ppuBus.FG_PRIORITY = state.ppuBus.FG_PRIORITY;
        ppuBus.FINE_X = state.ppuBus.FINE_X;
        ppuBus.vRAM.combined = state.ppuBus.vRAM;
        ppuBus.tempRAM.combined = state.ppuBus.tempRAM;
        std::memcpy(ppuBus.nameTables,state.ppuBus.nameTables,4096);
        std::memcpy(ppuBus.palettes,state.ppuBus.palettes,32);
        std::memcpy(ppuBus.OAM,state.ppuBus.OAM,64 * sizeof(OBJECT_ATTRIBUTE));
        ppuBus.NMI = state.ppuBus.NMI;
        ppuBus.OAM_offset = state.ppuBus.OAM_offset;
        ppuBus.PPUCTRL.combined = state.ppuBus.PPUCTRL;
        ppuBus.PPUMASK.combined = state.ppuBus.PPUMASK;
        ppuBus.PPUSTATUS.combined = state.ppuBus.PPUSTATUS;
        ppuBus.SPRT_PATTERN_ADDR_L = state.ppuBus.SPRT_PATTERN_ADDR_L;
        ppuBus.SPRT_PATTERN_HIGH = state.ppuBus.SPRT_PATTERN_HIGH;
        ppuBus.ppuBuffer = state.ppuBus.ppuBuffer;
        ppuBus.SPRT_SHIFTER_HIGH = state.ppuBus.SPRT_SHIFTER_HIGH;
        ppuBus.SPRT_SHIFTER_LOW = state.ppuBus.SPRT_SHIFTER_LOW;

        cartridge.setMapperID(state.mapperID);
        setMapper();

        switch(cartridge.getMapperID())
        {
            case 1:
                std::memcpy(static_cast<Mapper1*>(mapper.get())->PRGRAM,static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->PRGRAM,8*1024);
                static_cast<Mapper1*>(mapper.get())->REGISTERS.controlReg = static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->REGISTERS.controlReg;
                static_cast<Mapper1*>(mapper.get())->REGISTERS.tempReg = static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->REGISTERS.tempReg;
                static_cast<Mapper1*>(mapper.get())->REGISTERS.tempRegNum = static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->REGISTERS.tempRegNum;
                static_cast<Mapper1*>(mapper.get())->CHR0 = static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->CHR0;
                static_cast<Mapper1*>(mapper.get())->CHR1 = static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->CHR1;
                static_cast<Mapper1*>(mapper.get())->CHRFull = static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->CHRFull;
                static_cast<Mapper1*>(mapper.get())->PRG0 = static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->PRG0;
                static_cast<Mapper1*>(mapper.get())->PRG1 = static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->PRG1;
                static_cast<Mapper1*>(mapper.get())->PRGFull = static_cast<StateManagement::Mapper::Mapper1State*>(state.mapper.get())->PRGFull;
                break;
            case 2:
                static_cast<Mapper2*>(mapper.get())->lowerBankOffset = static_cast<StateManagement::Mapper::Mapper2State*>(state.mapper.get())->lowerBankOffset;
                static_cast<Mapper2*>(mapper.get())->higherBankOffset = static_cast<StateManagement::Mapper::Mapper2State*>(state.mapper.get())->higherBankOffset;
                break;
            case 3:
                static_cast<Mapper3*>(mapper.get())->selectedCHRbank = static_cast<StateManagement::Mapper::Mapper3State*>(state.mapper.get())->selectedCHRbank;
                break;
            default:
                break;
        }

        mapper->setMirroring(state.mirroringMode == 0 ? MIRRORING::HORIZONTAL : MIRRORING::VERTICAL);

        cartridge.CHRNum = state.cartridge.CHRROMSize / 8192;
        cartridge.PRGNum = state.cartridge.PRGROMSize / 16384;

        cartridge.PRGmemory = std::move(state.cartridge.PRGROM);
        cartridge.CHRmemory = std::move(state.cartridge.CHRROM);

        hasLoaded = true;
    }

}


