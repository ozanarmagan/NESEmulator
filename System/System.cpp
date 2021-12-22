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


NES::NES() : display(&events),cartridge(),ppuBus(),bus(ppuBus),ppu(&display,&bus,&cartridge,&ppuBus),cpu(bus,ppu),controller(bus)
{

}

NES::NES(std::string fileName) : display(&events),cartridge(),ppuBus(),bus(ppuBus),ppu(&display,&bus,&cartridge,&ppuBus),cpu(bus,ppu),controller(bus)
{
    insertNESFile(fileName);   
}

void NES::insertNESFile(std::string fileName)
{
    std::ifstream file(fileName,std::ios::binary);
    FileHeader header;
    if(file.is_open())
    {
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
        std::cout << "MAGIC CHARS: " << header.name << std::endl;
        BYTE mapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
        cartridge.setPRGNum(header.PRGROMChunks);
        BYTE* PRGmem = new BYTE[header.PRGROMChunks * 16384];
        file.read((char*) PRGmem,header.PRGROMChunks * 16384);
        cartridge.loadPRGData(PRGmem);

        cartridge.setCHRNum(header.CHRROMChunks);
        BYTE* CHRmem = new BYTE[header.CHRROMChunks * 8192];
        file.read((char*) CHRmem,header.CHRROMChunks * 8192);
        cartridge.loadCHRData(CHRmem);
        
        cartridge.setMapperID(mapperID);
        setMapper();
        bus.setMapper(mapper);
        ppuBus.setMapper(mapper);
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
    case 0:mapper = std::make_shared<Mapper0>(cartridge.getPRGNum(), cartridge.getCHRNum(),&cartridge);break;   
    default:
        break;
    }
}



void NES::log()
{
    std::cout << cpu;
    bus.print();
    std::cout << cartridge;
}


void NES::start()
{
    std::cout << "\nLOG: " << "Main Loop Has Started!\n";

    mainLoop();
}


void NES::mainLoop()
{
    while(1)
    {
        while(!ppu.isFrameDone())
            tick();
        display.renderFrame();
#ifdef DEBUG
        display.renderDebugFrame();
#endif
        ppu.clearFrameDone();
    }
}


void NES::tick()
{
    ppu.tick();
    updateControllers();
    controller.handleInput();
    if(clock++ % 3 == 0)
        cpu.tick();
}


void NES::updateControllers()
{
    while(SDL_PollEvent(&events))
    {
        switch (events.type)
        {
        case SDL_QUIT:
            std::cout << "LOG: Quit has called! Good bye!\n";
            delete this;
            break;
        case SDL_KEYDOWN:
            controller.setKeyStatus(events.key.keysym.sym);
            break;
        case SDL_KEYUP:
            controller.clearKeyStatus(events.key.keysym.sym);
            break;
        default:
            break;
        }
    }
}


