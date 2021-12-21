#include <fstream>
#include "Cartridge.h"

Cartridge::Cartridge()
{
 
}


namespace{
	std::string bTohex(uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};
}

std::ostream& operator<<(std::ostream& stream,Cartridge& cart)
{
    stream << "CARTRIDGE INFO:\n" << "Number of PRG ROM : " << bTohex(cart.getPRGNum(),2) << std::endl;
    stream << "Number of CHR ROM : " << bTohex(cart.getCHRNum(),2) << std::endl << "Mapper ID: " << bTohex(cart.mapperID,2) << std::endl;
    stream << "PRG ROM : " << std::endl;
    for(int i = 0;i < 256;i++)
    {
        std::cout << std::hex << bTohex(cart.PRGmemory[i],2) << " ";
        if(i != 0 && i  % 8 == 0)
            std::cout  << std::endl;
    }
    stream << std::endl << "CHR ROM : " << std::endl;
    for(int i = 0;i < 256;i++)
    {
        std::cout << std::hex << bTohex(cart.CHRmemory[i],2) << " ";
        if(i != 0 && i  % 8 == 0)
            std::cout  << std::endl;
    }
    return stream;
}