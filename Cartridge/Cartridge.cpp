#include <fstream>
#include "Cartridge.h"

namespace nesemulator
{
	namespace
	{
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
		return stream;
	}
}