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


NES::NES() : cartridge(),ppuBus(),bus(ppuBus),ppu(&display,&bus,&cartridge,&ppuBus),cpu(bus),controller(&bus),display(&events,controller)
{

}

NES::NES(std::string fileName) : cartridge(),ppuBus(),bus(ppuBus),ppu(&display,&bus,&cartridge,&ppuBus),cpu(bus),controller(&bus),display(&events,controller)
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
        BYTE* CHRmem;
        if(header.CHRROMChunks == 0) // If it is CHR RAM
            CHRmem = new BYTE[8192];
        else
            CHRmem = new BYTE[header.CHRROMChunks * 8192];
        file.read((char*) CHRmem,(header.CHRROMChunks != 0 ? header.CHRROMChunks : 1) * 8192);
        cartridge.loadCHRData(CHRmem);
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
    while(1)
    {
        do
            tick();
        while(!ppu.isFrameDone());
        display.renderFrame();
#ifdef DEBUG
        ppu.getPatternTable();
        display.renderDebugFrame();
#endif
        ppu.clearFrameDone();
        controller.handleInput();
    }
}


void NES::tick()
{


    ppu.tick();

    
    if(clock % 3 == 0)
    {
        if(bus.getDMAStatus())
            DMA();
        else
            cpu.tick();
    }
    
    if(ppuBus.getNMI())
    {
        cpu.NMI();
        ppuBus.setNMI(false);
    }

    controller.handleInput();

    clock++;
}


void NES::DMA() // Direct Memory Access 
{
    if(bus.DMA_dummy)
    {
        if(clock % 2 == 1)
            bus.DMA_dummy = false;
    }
    else
    {
        if(clock % 2 == 0)
            bus.DMAReadNext();
        else
        {
            bus.writeOAM();
            
            if(bus.DMA_low == 0x00)
            {
                bus.DMA = false;
                bus.DMA_dummy = true;
            }
        }
    }
}



