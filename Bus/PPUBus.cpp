#include "PPUBus.h"


PPUBus::PPUBus()
{
    colors[0x00] = {84, 84, 84};
	colors[0x01] = {0, 30, 116};
	colors[0x02] = {8, 16, 144};
	colors[0x03] = {48, 0, 136};
	colors[0x04] = {68, 0, 100};
	colors[0x05] = {92, 0, 48};
	colors[0x06] = {84, 4, 0};
	colors[0x07] = {60, 24, 0};
	colors[0x08] = {32, 42, 0};
	colors[0x09] = {8, 58, 0};
	colors[0x0A] = {0, 64, 0};
	colors[0x0B] = {0, 60, 0};
	colors[0x0C] = {0, 50, 60};
	colors[0x0D] = {0, 0, 0};
	colors[0x0E] = {0, 0, 0};
	colors[0x0F] = {0, 0, 0};
	colors[0x10] = {152, 150, 152};
	colors[0x11] = {8, 76, 196};
	colors[0x12] = {48, 50, 236};
	colors[0x13] = {92, 30, 228};
	colors[0x14] = {136, 20, 176};
	colors[0x15] = {160, 20, 100};
	colors[0x16] = {152, 34, 32};
	colors[0x17] = {120, 60, 0};
	colors[0x18] = {84, 90, 0};
	colors[0x19] = {40, 114, 0};
	colors[0x1A] = {8, 124, 0};
	colors[0x1B] = {0, 118, 40};
	colors[0x1C] = {0, 102, 120};
	colors[0x1D] = {0, 0, 0};
	colors[0x1E] = {0, 0, 0};
	colors[0x1F] = {0, 0, 0};
	colors[0x20] = {236, 238, 236};
	colors[0x21] = {76, 154, 236};
	colors[0x22] = {120, 124, 236};
	colors[0x23] = {176, 98, 236};
	colors[0x24] = {228, 84, 236};
	colors[0x25] = {236, 88, 180};
	colors[0x26] = {236, 106, 100};
	colors[0x27] = {212, 136, 32};
	colors[0x28] = {160, 170, 0};
	colors[0x29] = {116, 196, 0};
	colors[0x2A] = {76, 208, 32};
	colors[0x2B] = {56, 204, 108};
	colors[0x2C] = {56, 180, 204};
	colors[0x2D] = {60, 60, 60};
	colors[0x2E] = {0, 0, 0};
	colors[0x2F] = {0, 0, 0};
	colors[0x30] = {236, 238, 236};
	colors[0x31] = {168, 204, 236};
	colors[0x32] = {188, 188, 236};
	colors[0x33] = {212, 178, 236};
	colors[0x34] = {236, 174, 236};
	colors[0x35] = {236, 174, 212};
	colors[0x36] = {236, 180, 176};
	colors[0x37] = {228, 196, 144};
	colors[0x38] = {204, 210, 120};
	colors[0x39] = {180, 222, 120};
	colors[0x3A] = {168, 226, 144};
	colors[0x3B] = {152, 226, 180};
	colors[0x3C] = {160, 214, 228};
	colors[0x3D] = {160, 162, 160};
	colors[0x3E] = {0, 0, 0};
	colors[0x3F] = {0, 0, 0};


	PPUSTATUS.combined = 0x00;
	PPUCTRL.combined   = 0x00;
	PPUMASK.combined   = 0x00;

	vRAM.combined = 0x0000;
	tempRAM.combined = 0x0000;

	BG_RENDER_FETCH.BG_NEXT_ATTR = 0x00;
	BG_RENDER_FETCH.BG_NEXT_HIGH = 0x00;
	BG_RENDER_FETCH.BG_NEXT_LOW  = 0x00;
	BG_RENDER_FETCH.BG_NEXT_ID   = 0x00;

	BG_SHIFTER_ATTR_HIGH.combined    = 0x0000;
	BG_SHIFTER_ATTR_LOW.combined     = 0x0000;
	BG_SHIFTER_PATTERN_HIGH.combined = 0x0000;
	BG_SHIFTER_PATTERN_LOW.combined  = 0x0000;

	ppuBuffer = 0x00;

}


void PPUBus::setMapper(std::shared_ptr<MapperBase> _mapper)
{
	mapper = _mapper;
}


BYTE PPUBus::readFromMemory(ADDRESS address)
{
	address &= 0x3FFF;
	if(address >= 0x0000 && address <= 0x1FFF)
		return mapper->MapReadPpu(address);
	else if(address >= 0x2000 && address <= 0x3EFF)
	{
		address &= 0x0FFF;
		if(mapper->getMirroring() == MIRRORING::VERTICAL)
		{
			return nameTables[address % 0x800];
		}
		else if(mapper->getMirroring() == MIRRORING::HORIZONTAL)
		{
			if(address <= 0x07FF)
				return nameTables[address % 1024];
			else
				return nameTables[(address % 1024) + 1024];
		}
		else
			return 0x00;
	}
	else if(address >= 0x3F00 && address <= 0x3FFF)
	{
		address &= 0x001F;
		if(address == 0x0010) address = 0x0000;
		if(address == 0x0014) address = 0x0004;
		if(address == 0x0018) address = 0x0008;
		if(address == 0x001C) address = 0x000C;
		return palettes[address];
	}

	return 0x00;
}


void PPUBus::writeToMemory(ADDRESS address,BYTE value)
{
	address &= 0x3FFF;
	if(address >= 0x0000 && address <= 0x1FFF)
		mapper->MapWritePpu(address,value);
	else if(address >= 0x2000 && address <= 0x3EFF)
	{
		address &= 0x0FFF;
		if(mapper->getMirroring() == MIRRORING::VERTICAL)
		{
			nameTables[address % 0x800] = value;
		}
		else if(mapper->getMirroring() == MIRRORING::HORIZONTAL)
		{
			if(address <= 0x07FF)
				nameTables[address & 0x3FF] = value;
			else
				nameTables[(address % 1024) + 1024] = value;
		}
	}
	else if(address >= 0x3F00 && address <= 0x3FFF)
	{
		address &= 0x001F;
		if(address == 0x0010) address = 0x0000;
		if(address == 0x0014) address = 0x0004;
		if(address == 0x0018) address = 0x0008;
		if(address == 0x001C) address = 0x000C;
		palettes[address] = value;
	}
}


BYTE PPUBus::readFromMemory_mainBus(ADDRESS address)
{
	if(address == 0x0000)
	{	
		return 0x00;
	}
	else if(address == 0x0001)
	{	
		//return OAM_offset;
	}
	else if(address == 0x0002)
	{	
		BYTE temp = (PPUSTATUS.combined & 0xE0) | (ppuBuffer & 0x1F);
		PPUSTATUS.VBLANK = 0;
		addressToggle = false;

		return temp;
	}
	else if(address == 0x0003)
	{	}
	else if(address == 0x0004)
		return ((BYTE*)&OAM)[OAM_offset];
	else if(address == 0x0005)
	{	}
	else if(address == 0x0006)
	{	}
	else if(address == 0x0007)
	{
		BYTE data = ppuBuffer;
		ppuBuffer = readFromMemory(vRAM.combined);

		if(vRAM.combined >= 0x3F00) return ppuBuffer;
		vRAM.combined += (PPUCTRL.INCREMENT_MODE ? 32 : 1);
		return data;
	}


	return 0x00;

}


void PPUBus::writeToMemory_mainBus(ADDRESS address,BYTE value)
{
	if(address == 0x0000)
	{
		PPUCTRL.combined = value;
		tempRAM.NT_X = PPUCTRL.NAMETABLE_X;
		tempRAM.NT_Y = PPUCTRL.NAMETABLE_Y;
	}
	else if(address == 0x0001)
		PPUMASK.combined = value;
	else if(address == 0x0002)
	{
		
	}
	else if(address == 0x0003)
		OAM_offset = value;
	else if(address == 0x0004)
		((BYTE*)OAM)[OAM_offset] = value;
	else if(address == 0x0005)
	{
		if(!addressToggle)
		{
			FINE_X = value & 0x07;
			tempRAM.CO_X = value >> 3;
			addressToggle = true;
		}
		else
		{
			tempRAM.FINE_Y = value & 0x07;
			tempRAM.CO_Y = value >> 3;
			addressToggle = false;
		}
	}
	else if(address == 0x0006)
	{
		if(!addressToggle)
		{
			tempRAM.combined = (ADDRESS)((value & 0x3F) << 8) | (tempRAM.combined & 0x00FF);
			addressToggle = true;
		}
		else
		{
			tempRAM.combined = (tempRAM.combined & 0xFF00) | value;
			vRAM = tempRAM;
			addressToggle = false;
		}

	}
	else if(address == 0x0007)
	{
		writeToMemory(vRAM.combined,value);
		vRAM.combined += (PPUCTRL.INCREMENT_MODE ? 32 : 1);
	}
}
