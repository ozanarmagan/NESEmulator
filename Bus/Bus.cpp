#include "Bus.h"
#include <iostream>
#include <iomanip>



namespace{
	std::string bTohex(uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};
}

BYTE Bus::readFromMemory(ADDRESS address) const
{
    if(address >= 0x0000 && address <= 0x1FFF)
        return memory[address & 0x07FF]; // Bitwise and because of mirroring 
    else if(address >= 0x2000 && address <= 0x3FFF)
        return ppuBus.readFromMemory(address & 0x0007); // Same for PPU
    else if(address >= 0x6000 && address <= 0xFFFF)
        return mapper->MapReadCpu(address);
    else
        return 0;
}

void Bus::writeToMemory(ADDRESS address,BYTE value) 
{
    if(address >= 0x0000 && address <= 0x1FFF)
        memory[address & 0x07FF] = value;
    else if(address >= 0x2000 && address <= 0x3FFF)
        ppuBus.writeToMemory(address & 0x0007,value);
    else if(address >= 0x6000 && address <= 0xFFFF)
        mapper->MapWriteCpu(address,value);
}

void Bus::clearMemoryBlock(ADDRESS start,ADDRESS end) // For debugging, not necessary
{
    for(ADDRESS i = start;i < end + 1;i++)
        if(i >= 0x0000 && i <= 0x1FFF)
            memory[i & 0x07FF] = 0x00;
}

void Bus::print(int end)
{
    std::cout << "Bus: " << std::endl;
    for(int i = 0;i < end;i++)
    {
        std::cout << std::hex << bTohex(memory[i],2) << " ";
        if(i != 0 && i  % 8 == 0)
            std::cout  << std::endl;
    }
    std::cout << std::endl;
}
