#include "System.h"

namespace{
	std::string bTohex(uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};
}


NES::NES() : cartridge(),apu(),ppuBus(),audio(),bus(ppuBus,apu),ppu(&display,&bus,&cartridge,&ppuBus),cpu(bus),controller(&bus),display(&events,controller,audio)
{
}

NES::NES(std::string fileName) : cartridge(),apu(),audio(),ppuBus(),bus(ppuBus,apu),ppu(&display,&bus,&cartridge,&ppuBus),cpu(bus),controller(&bus),display(&events,controller,audio)
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
                CHRmem = new BYTE[8192];
            else
                CHRmem = new BYTE[header.CHRROMChunks * 8192];
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
    //std::cout << cpu;
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
        // do
        //     tick();
        // while(!ppu.isFrameDone());
        while(SDL_GetQueuedAudioSize(1) < SAMPLE_PER_FRAME)
        {
            tick();
            if(innerClock >= cyclesPerSample)
            {
                audio.addToQueue(apu.output());
                innerClock -= cyclesPerSample;
            }
            innerClock += 1.0;
        }
        display.renderFrame();
#ifdef DEBUG
        ppu.getPatternTable();
        display.renderDebugFrame();
#endif
        // ppu.clearFrameDone();
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



