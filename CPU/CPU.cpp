#include "CPU.h"
#include <iostream>

using namespace std;

CPU::CPU(RAM& mem,PPU& ppu) : memory(mem),ppu(ppu) { 

    // Fill with ILLEGAL for empty OPCODES 
    INSTRUCTION temp; 
    temp.operation = &CPU::ILLEGAL;
    temp.addr = &CPU::IMP;
    temp.cycles = 0;
    for(int i;i < 256;i++)
        table[i] = temp;
    
    // Fill table now
    temp.addr = &CPU::IMM;
	temp.operation = &CPU::ADC;
	temp.cycles = 2;
	table[0x69] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::ADC;
	temp.cycles = 4;
	table[0x6D] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::ADC;
	temp.cycles = 3;
	table[0x65] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::ADC;
	temp.cycles = 6;
	table[0x61] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::ADC;
	temp.cycles = 6;
	table[0x71] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::ADC;
	temp.cycles = 4;
	table[0x75] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::ADC;
	temp.cycles = 4;
	table[0x7D] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::ADC;
	temp.cycles = 4;
	table[0x79] = temp;

	temp.addr = &CPU::IMM;
	temp.operation = &CPU::AND;
	temp.cycles = 2;
	table[0x29] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::AND;
	temp.cycles = 4;
	table[0x2D] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::AND;
	temp.cycles = 3;
	table[0x25] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::AND;
	temp.cycles = 6;
	table[0x21] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::AND;
	temp.cycles = 5;
	table[0x31] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::AND;
	temp.cycles = 4;
	table[0x35] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::AND;
	temp.cycles = 4;
	table[0x3D] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::AND;
	temp.cycles = 4;
	table[0x39] = temp;

	temp.addr = &CPU::ABS;
	temp.operation = &CPU::ASL;
	temp.cycles = 6;
	table[0x0E] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::ASL;
	temp.cycles = 5;
	table[0x06] = temp;
	temp.addr = &CPU::ACC;
	temp.operation = &CPU::ASL_ACC;
	temp.cycles = 2;
	table[0x0A] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::ASL;
	temp.cycles = 6;
	table[0x16] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::ASL;
	temp.cycles = 7;
	table[0x1E] = temp;

	temp.addr = &CPU::REL;
	temp.operation = &CPU::BCC;
	temp.cycles = 2;
	table[0x90] = temp;

	temp.addr = &CPU::REL;
	temp.operation = &CPU::BCS;
	temp.cycles = 2;
	table[0xB0] = temp;

	temp.addr = &CPU::REL;
	temp.operation = &CPU::BEQ;
	temp.cycles = 2;
	table[0xF0] = temp;

	temp.addr = &CPU::ABS;
	temp.operation = &CPU::BIT;
	temp.cycles = 4;
	table[0x2C] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::BIT;
	temp.cycles = 3;
	table[0x24] = temp;

	temp.addr = &CPU::REL;
	temp.operation = &CPU::BMI;
	temp.cycles = 2;
	table[0x30] = temp;

	temp.addr = &CPU::REL;
	temp.operation = &CPU::BNE;
	temp.cycles = 2;
	table[0xD0] = temp;

	temp.addr = &CPU::REL;
	temp.operation = &CPU::BPL;
	temp.cycles = 2;
	table[0x10] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::BRK;
	temp.cycles = 7;
	table[0x00] = temp;

	temp.addr = &CPU::REL;
	temp.operation = &CPU::BVC;
	temp.cycles = 2;
	table[0x50] = temp;

	temp.addr = &CPU::REL;
	temp.operation = &CPU::BVS;
	temp.cycles = 2;
	table[0x70] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::CLC;
	temp.cycles = 2;
	table[0x18] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::CLD;
	temp.cycles = 2;
	table[0xD8] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::CLI;
	temp.cycles = 2;
	table[0x58] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::CLV;
	temp.cycles = 2;
	table[0xB8] = temp;

	temp.addr = &CPU::IMM;
	temp.operation = &CPU::CMP;
	temp.cycles = 2;
	table[0xC9] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::CMP;
	temp.cycles = 4;
	table[0xCD] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::CMP;
	temp.cycles = 3;
	table[0xC5] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::CMP;
	temp.cycles = 6;
	table[0xC1] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::CMP;
	temp.cycles = 3;
	table[0xD1] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::CMP;
	temp.cycles = 4;
	table[0xD5] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::CMP;
	temp.cycles = 4;
	table[0xDD] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::CMP;
	temp.cycles = 4;
	table[0xD9] = temp;

	temp.addr = &CPU::IMM;
	temp.operation = &CPU::CPX;
	temp.cycles = 2;
	table[0xE0] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::CPX;
	temp.cycles = 4;
	table[0xEC] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::CPX;
	temp.cycles = 3;
	table[0xE4] = temp;

	temp.addr = &CPU::IMM;
	temp.operation = &CPU::CPY;
	temp.cycles = 2;
	table[0xC0] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::CPY;
	temp.cycles = 4;
	table[0xCC] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::CPY;
	temp.cycles = 3;
	table[0xC4] = temp;

	temp.addr = &CPU::ABS;
	temp.operation = &CPU::DEC;
	temp.cycles = 6;
	table[0xCE] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::DEC;
	temp.cycles = 5;
	table[0xC6] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::DEC;
	temp.cycles = 6;
	table[0xD6] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::DEC;
	temp.cycles = 7;
	table[0xDE] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::DEX;
	temp.cycles = 2;
	table[0xCA] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::DEY;
	temp.cycles = 2;
	table[0x88] = temp;

	temp.addr = &CPU::IMM;
	temp.operation = &CPU::EOR;
	temp.cycles = 2;
	table[0x49] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::EOR;
	temp.cycles = 4;
	table[0x4D] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::EOR;
	temp.cycles = 3;
	table[0x45] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::EOR;
	temp.cycles = 6;
	table[0x41] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::EOR;
	temp.cycles = 5;
	table[0x51] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::EOR;
	temp.cycles = 4;
	table[0x55] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::EOR;
	temp.cycles = 4;
	table[0x5D] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::EOR;
	temp.cycles = 4;
	table[0x59] = temp;

	temp.addr = &CPU::ABS;
	temp.operation = &CPU::INC;
	temp.cycles = 6;
	table[0xEE] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::INC;
	temp.cycles = 5;
	table[0xE6] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::INC;
	temp.cycles = 6;
	table[0xF6] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::INC;
	temp.cycles = 7;
	table[0xFE] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::INX;
	temp.cycles = 2;
	table[0xE8] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::INY_OP;
	temp.cycles = 2;
	table[0xC8] = temp;

	temp.addr = &CPU::ABS;
	temp.operation = &CPU::JMP;
	temp.cycles = 3;
	table[0x4C] = temp;
	temp.addr = &CPU::ABI;
	temp.operation = &CPU::JMP;
	temp.cycles = 5;
	table[0x6C] = temp;

	temp.addr = &CPU::ABS;
	temp.operation = &CPU::JSR;
	temp.cycles = 6;
	table[0x20] = temp;

	temp.addr = &CPU::IMM;
	temp.operation = &CPU::LDA;
	temp.cycles = 2;
	table[0xA9] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::LDA;
	temp.cycles = 4;
	table[0xAD] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::LDA;
	temp.cycles = 3;
	table[0xA5] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::LDA;
	temp.cycles = 6;
	table[0xA1] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::LDA;
	temp.cycles = 5;
	table[0xB1] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::LDA;
	temp.cycles = 4;
	table[0xB5] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::LDA;
	temp.cycles = 4;
	table[0xBD] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::LDA;
	temp.cycles = 4;
	table[0xB9] = temp;

	temp.addr = &CPU::IMM;
	temp.operation = &CPU::LDX;
	temp.cycles = 2;
	table[0xA2] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::LDX;
	temp.cycles = 4;
	table[0xAE] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::LDX;
	temp.cycles = 3;
	table[0xA6] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::LDX;
	temp.cycles = 4;
	table[0xBE] = temp;
	temp.addr = &CPU::ZEY;
	temp.operation = &CPU::LDX;
	temp.cycles = 4;
	table[0xB6] = temp;

	temp.addr = &CPU::IMM;
	temp.operation = &CPU::LDY;
	temp.cycles = 2;
	table[0xA0] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::LDY;
	temp.cycles = 4;
	table[0xAC] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::LDY;
	temp.cycles = 3;
	table[0xA4] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::LDY;
	temp.cycles = 4;
	table[0xB4] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::LDY;
	temp.cycles = 4;
	table[0xBC] = temp;

	temp.addr = &CPU::ABS;
	temp.operation = &CPU::LSR;
	temp.cycles = 6;
	table[0x4E] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::LSR;
	temp.cycles = 5;
	table[0x46] = temp;
	temp.addr = &CPU::ACC;
	temp.operation = &CPU::LSR_ACC;
	temp.cycles = 2;
	table[0x4A] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::LSR;
	temp.cycles = 6;
	table[0x56] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::LSR;
	temp.cycles = 7;
	table[0x5E] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::NOP;
	temp.cycles = 2;
	table[0xEA] = temp;

	temp.addr = &CPU::IMM;
	temp.operation = &CPU::ORA;
	temp.cycles = 2;
	table[0x09] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::ORA;
	temp.cycles = 4;
	table[0x0D] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::ORA;
	temp.cycles = 3;
	table[0x05] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::ORA;
	temp.cycles = 6;
	table[0x01] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::ORA;
	temp.cycles = 5;
	table[0x11] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::ORA;
	temp.cycles = 4;
	table[0x15] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::ORA;
	temp.cycles = 4;
	table[0x1D] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::ORA;
	temp.cycles = 4;
	table[0x19] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::PHA;
	temp.cycles = 3;
	table[0x48] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::PHP;
	temp.cycles = 3;
	table[0x08] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::PLA;
	temp.cycles = 4;
	table[0x68] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::PLP;
	temp.cycles = 4;
	table[0x28] = temp;

	temp.addr = &CPU::ABS;
	temp.operation = &CPU::ROL;
	temp.cycles = 6;
	table[0x2E] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::ROL;
	temp.cycles = 5;
	table[0x26] = temp;
	temp.addr = &CPU::ACC;
	temp.operation = &CPU::ROL_ACC;
	temp.cycles = 2;
	table[0x2A] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::ROL;
	temp.cycles = 6;
	table[0x36] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::ROL;
	temp.cycles = 7;
	table[0x3E] = temp;

	temp.addr = &CPU::ABS;
	temp.operation = &CPU::ROR;
	temp.cycles = 6;
	table[0x6E] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::ROR;
	temp.cycles = 5;
	table[0x66] = temp;
	temp.addr = &CPU::ACC;
	temp.operation = &CPU::ROR_ACC;
	temp.cycles = 2;
	table[0x6A] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::ROR;
	temp.cycles = 6;
	table[0x76] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::ROR;
	temp.cycles = 7;
	table[0x7E] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::RTI;
	temp.cycles = 6;
	table[0x40] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::RTS;
	temp.cycles = 6;
	table[0x60] = temp;

	temp.addr = &CPU::IMM;
	temp.operation = &CPU::SBC;
	temp.cycles = 2;
	table[0xE9] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::SBC;
	temp.cycles = 4;
	table[0xED] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::SBC;
	temp.cycles = 3;
	table[0xE5] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::SBC;
	temp.cycles = 6;
	table[0xE1] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::SBC;
	temp.cycles = 5;
	table[0xF1] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::SBC;
	temp.cycles = 4;
	table[0xF5] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::SBC;
	temp.cycles = 4;
	table[0xFD] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::SBC;
	temp.cycles = 4;
	table[0xF9] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::SEC;
	temp.cycles = 2;
	table[0x38] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::SED;
	temp.cycles = 2;
	table[0xF8] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::SEI;
	temp.cycles = 2;
	table[0x78] = temp;

	temp.addr = &CPU::ABS;
	temp.operation = &CPU::STA;
	temp.cycles = 4;
	table[0x8D] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::STA;
	temp.cycles = 3;
	table[0x85] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::STA;
	temp.cycles = 6;
	table[0x81] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::STA;
	temp.cycles = 6;
	table[0x91] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::STA;
	temp.cycles = 4;
	table[0x95] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::STA;
	temp.cycles = 5;
	table[0x9D] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::STA;
	temp.cycles = 5;
	table[0x99] = temp;

	temp.addr = &CPU::ABS;
	temp.operation = &CPU::STX;
	temp.cycles = 4;
	table[0x8E] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::STX;
	temp.cycles = 3;
	table[0x86] = temp;
	temp.addr = &CPU::ZEY;
	temp.operation = &CPU::STX;
	temp.cycles = 4;
	table[0x96] = temp;

	temp.addr = &CPU::ABS;
	temp.operation = &CPU::STY;
	temp.cycles = 4;
	table[0x8C] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::STY;
	temp.cycles = 3;
	table[0x84] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::STY;
	temp.cycles = 4;
	table[0x94] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::TAX;
	temp.cycles = 2;
	table[0xAA] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::TAY;
	temp.cycles = 2;
	table[0xA8] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::TSX;
	temp.cycles = 2;
	table[0xBA] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::TXA;
	temp.cycles = 2;
	table[0x8A] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::TXS;
	temp.cycles = 2;
	table[0x9A] = temp;

	temp.addr = &CPU::IMP;
	temp.operation = &CPU::TYA;
	temp.cycles = 2;
	table[0x98] = temp;
};


// This addressing mode is unused
ADDRESS CPU::ACC()
{
    return A;    
}


// IMMEDIATE 
ADDRESS CPU::IMM()
{
    return programCounter++;
}


//ABSOLUTE
ADDRESS CPU::ABS()
{
    uint16_t addrLower = memory.readFromMemory(programCounter++),
             addrHigher = memory.readFromMemory(programCounter++);
    return addrLower + (addrHigher << 8);
}


// ZERO PAGE
ADDRESS CPU::ZER()
{
    return memory.readFromMemory(programCounter++);
}


//IMPLIED
ADDRESS CPU::IMP()
{
    return 0;
}


//RELATIVE
ADDRESS CPU::REL()
{
    uint16_t offset = (uint16_t) memory.readFromMemory(programCounter++);
    if(offset & 0x80) offset |= 0xFF00;
    return programCounter + (int16_t) offset;
}


//ABSOLUTE INDIRECT
ADDRESS CPU::ABI()
{
    uint16_t addressLower = memory.readFromMemory(programCounter++),
             addressHigher = memory.readFromMemory(programCounter++),
             abs = (addressHigher << 8) | addressLower,
             effLower = memory.readFromMemory(abs),
             effHigher = memory.readFromMemory((abs & 0xFF00) + ((abs + 1) & 0x00FF));
    
    return effLower + 0x100 * effHigher;
}


//ZERO PAGE X
ADDRESS CPU::ZEX()
{
    return (memory.readFromMemory(programCounter++) + X) % 256;
}


//ZERO PAGE Y
ADDRESS CPU::ZEY()
{
    return (memory.readFromMemory(programCounter++) + Y) % 256;
}


//INDEXED X 
ADDRESS CPU::ABX()
{
    return ABS() + X;
}


//INDEXED Y 
ADDRESS CPU::ABY()
{
    return ABS() + Y;
}


//INDEXED X INDIRECT
ADDRESS CPU::INX()
{
    uint16_t zeroLower = ZEX(),zeroHigher = (zeroLower + 1) % 256;
    return memory.readFromMemory(zeroLower) + (memory.readFromMemory(zeroHigher) << 8);
}


//INDEXED Y INDIRECT
ADDRESS CPU::INY()
{
    uint16_t zeroLower = memory.readFromMemory(programCounter++),
             zeroHigher = (zeroLower + 1) % 256;
    return memory.readFromMemory(zeroLower) + (memory.readFromMemory(zeroHigher) << 8) + Y;
}

void CPU::reset()
{
    A = 0x00;
    X = 0x00;
    Y = 0x00;
    SP = 0xFF;
}

void CPU::push(uint8_t value)
{
    memory.writeToMemory(0x0100 + SP,value);
    if(SP == 0x00) SP = 0xFF;
    else SP--;
}

uint8_t CPU::pop()
{
    if(SP = 0xFF) SP == 0x00;
    else SP++;
    return memory.readFromMemory(0x0100 + SP);
}

void CPU::tick()
{
	currentOpCode = memory.readFromMemory(programCounter++); // Fetch

	ppu.tick();
	ppu.tick();
	ppu.tick();	

	currentInstruction = table[currentOpCode]; // Decode 

	ppu.tick();
	ppu.tick();
	ppu.tick();	

	currentCycle += currentInstruction.cycles;

	execute(); // Execute
}


void CPU::execute()
{
	ppu.tick();
	ppu.tick();
	ppu.tick();
	
	(this->*currentInstruction.operation)((this->*currentInstruction.addr)());
}

CPU::OPEXEC CPU::ADC(ADDRESS source)
{
	uint8_t data = memory.readFromMemory(source);
	unsigned int temp = data + A + (CARRY ? 1 : 0);
	ZERO = !(temp & 0xFF);

	if(DECIMAL)
	{
		if(((A & 0xF) + (data & 0xF) + (CARRY ? 1 : 0)) > 9) temp += 6;
		NEGATIVE = temp & 0x80;
		OVERFLOWBIT = !((A ^ data) & 0x80) && ((A ^ temp) & 0x80);
		if(temp > 0x99) temp += 96;
		CARRY = temp > 0x99; 
	}
	else
	{
		NEGATIVE = temp & 0x80;
		OVERFLOWBIT = !((A ^ data) & 0x80) && ((A ^ temp) & 0x80);
		CARRY = temp > 0xFF;
	}

	A = temp & 0xFF;
}

CPU::OPEXEC CPU::AND(ADDRESS source)
{
	A = A | memory.readFromMemory(source);
	ZERO = !A;
	NEGATIVE = A & 0x80;
}

CPU::OPEXEC CPU::ASL(ADDRESS source)
{
	uint8_t data = memory.readFromMemory(source);
	CARRY = data & 0x80;
	data <<= 1;
	data &= 0xFF;
	NEGATIVE = data & 0x80;
	ZERO = !data;
	memory.writeToMemory(source,data);
}

CPU::OPEXEC CPU::ASL_ACC(ADDRESS source)
{
	CARRY = A & 0x80;
	A <<= 1;
	A &= 0xFF;
	NEGATIVE = A & 0x80;
	ZERO = !A;
}

CPU::OPEXEC CPU::BCC(ADDRESS source)
{
	if(!CARRY)
		programCounter = source;
}

CPU::OPEXEC CPU::BCS(ADDRESS source)
{
	if(CARRY)
		programCounter = source;
}

CPU::OPEXEC CPU::BEQ(ADDRESS source)
{
	if(ZERO)
		programCounter = source;
}

CPU::OPEXEC CPU::BIT(ADDRESS source)
{
	uint16_t data = memory.readFromMemory(source) & A;
	ZERO = !data;
	NEGATIVE = data & 0x80;
	
}

CPU::OPEXEC CPU::BMI(ADDRESS source)
{
	if(NEGATIVE)
		programCounter = source;
}

CPU::OPEXEC CPU::BNE(ADDRESS source)
{
	if(!ZERO)
		programCounter = source;
}

CPU::OPEXEC CPU::BPL(ADDRESS source)
{
	if(!NEGATIVE)
		programCounter = source;
}

CPU::OPEXEC CPU::BRK(ADDRESS source)
{
	uint8_t flagByte = 0xFF;
	flagByte |= 1UL << BREAK_BIT;
	CARRY ? flagByte |= 1UL << CARRY_BIT : flagByte &= ~(1UL << CARRY_BIT);
	ZERO ? flagByte |= 1UL << ZERO_BIT : flagByte &= ~(1UL << ZERO_BIT);
	INTERRUPT_DISABLE ? flagByte |= 1UL << INTERRUPT_DISABLE_BIT : flagByte &= ~(1UL << INTERRUPT_DISABLE_BIT);
	DECIMAL ? flagByte |= 1UL << DECIMAL_MODE_BIT : flagByte &= ~(1UL << DECIMAL_MODE_BIT);
	OVERFLOWBIT ? flagByte |= 1UL << OVERFLOW_BIT : flagByte &= ~(1UL << OVERFLOW_BIT);
	NEGATIVE ? flagByte |= 1UL << NEGATIVE_BIT : flagByte &= ~(1UL << NEGATIVE_BIT);
	programCounter++;
	push((programCounter >> 8) & 0xFF);
	push(programCounter & 0xFF);
	push(flagByte);

	INTERRUPT_DISABLE = 1;
	programCounter = (memory.readFromMemory(IRQVECTOR_H) << 8) + memory.readFromMemory(IRQVECTOR_L);
}

CPU::OPEXEC CPU::BVC(ADDRESS source)
{
	if(!OVERFLOWBIT)
		programCounter = source;
}

CPU::OPEXEC CPU::BVS(ADDRESS source)
{
	if(OVERFLOWBIT)
		programCounter = source;
}

CPU::OPEXEC CPU::CLC(ADDRESS source)
{
	CARRY = 0;
}

CPU::OPEXEC CPU::CLD(ADDRESS source)
{
	DECIMAL = 0;
}

CPU::OPEXEC CPU::CLI(ADDRESS source)
{
	INTERRUPT_DISABLE = 0;
}

CPU::OPEXEC CPU::CLV(ADDRESS source)
{
	OVERFLOWBIT = 0;
}

CPU::OPEXEC CPU::CMP(ADDRESS source)
{
	uint8_t data = A - memory.readFromMemory(source);
	CARRY = (0x100 > data) ? 1 : 0;
	NEGATIVE = (0x80 & data) ? 1 : 0;
	ZERO = (data & 0xFF) ? 0 : 1;
}

CPU::OPEXEC CPU::CPX(ADDRESS source)
{
	uint8_t data = X - memory.readFromMemory(source);
	CARRY = (0x100 > data) ? 1 : 0;
	NEGATIVE = (0x80 & data) ? 1 : 0;
	ZERO = (data & 0xFF) ? 0 : 1;
}

CPU::OPEXEC CPU::CPY(ADDRESS source)
{
	uint8_t data = Y - memory.readFromMemory(source);
	CARRY = (0x100 > data) ? 1 : 0;
	NEGATIVE = (0x80 & data) ? 1 : 0;
	ZERO = (data & 0xFF) ? 0 : 1;
}

CPU::OPEXEC CPU::DEC(ADDRESS source)
{
	uint8_t data = memory.readFromMemory(source) - 1;
	NEGATIVE = data & 0x80;
	ZERO = !data;
	memory.writeToMemory(source,data);
}

CPU::OPEXEC CPU::DEX(ADDRESS source)
{
	uint8_t data = X - 1;
	NEGATIVE = data & 0x80;
	ZERO = !data;
	X = data;
}

CPU::OPEXEC CPU::DEY(ADDRESS source)
{
	uint8_t data = Y - 1;
	NEGATIVE = data & 0x80;
	ZERO = !data;
	Y = data;
}

CPU::OPEXEC CPU::EOR(ADDRESS source)
{
	uint8_t data = A ^ memory.readFromMemory(source);
	NEGATIVE = data & 0x80;
	ZERO = data == 0 ? 1 : 0;
	A = data;
}

CPU::OPEXEC CPU::INC(ADDRESS source)
{
	uint8_t data = (memory.readFromMemory(source) + 1) % 256;
	ZERO = !data;
	NEGATIVE = data & 0x80;
	memory.writeToMemory(source,data);
}

CPU::OPEXEC CPU::INX(ADDRESS source)
{
	X = (X + 1) % 256;
	ZERO = !X;
	NEGATIVE = X & 0x80;
}

CPU::OPEXEC CPU::INY_OP(ADDRESS source)
{
	Y = (Y + 1) % 256;
	ZERO = !Y;
	NEGATIVE = Y & 0x80;
}

CPU::OPEXEC CPU::JMP(ADDRESS source)
{
	programCounter = source;
}

CPU::OPEXEC CPU::JSR(ADDRESS source)
{
	programCounter--;
	push((programCounter >> 8) &  0xFF);
	push(programCounter & 0xFF);
	programCounter = source;
}

CPU::OPEXEC CPU::LDA(ADDRESS source)
{
	A = memory.readFromMemory(source);
	ZERO = !A;
	NEGATIVE = 	A & 0x80;
}

CPU::OPEXEC CPU::LDX(ADDRESS source)
{
	X = memory.readFromMemory(source);
	ZERO = !X;
	NEGATIVE = 	X & 0x80;
}

CPU::OPEXEC CPU::LDY(ADDRESS source)
{
	Y = memory.readFromMemory(source);
	ZERO = !Y;
	NEGATIVE = 	Y & 0x80;
}

CPU::OPEXEC CPU::LSR(ADDRESS source)
{
	uint8_t data = memory.readFromMemory(source);
	CARRY = data & 0x01;
	data >>= 1;
	ZERO = !data;
	NEGATIVE = 0;
	memory.writeToMemory(source,data);
}

CPU::OPEXEC CPU::LSR_ACC(ADDRESS source)
{
	CARRY = A & 0x01;
	A >>= 1;
	ZERO = !A;
	NEGATIVE = 0;
}

CPU::OPEXEC CPU::NOP(ADDRESS source) { };

CPU::OPEXEC CPU::ORA(ADDRESS source)
{
	A = memory.readFromMemory(source) | A;
	ZERO = !A;
	NEGATIVE = A & 0x80;
}

CPU::OPEXEC CPU::PHA(ADDRESS source)
{
	push(A);
}

CPU::OPEXEC CPU::PHP(ADDRESS source)
{
	uint8_t flagByte = 0xFF;
	flagByte |= 1UL << BREAK_BIT;
	CARRY ? flagByte |= 1UL << CARRY_BIT : flagByte &= ~(1UL << CARRY_BIT);
	ZERO ? flagByte |= 1UL << ZERO_BIT : flagByte &= ~(1UL << ZERO_BIT);
	INTERRUPT_DISABLE ? flagByte |= 1UL << INTERRUPT_DISABLE_BIT : flagByte &= ~(1UL << INTERRUPT_DISABLE_BIT);
	DECIMAL ? flagByte |= 1UL << DECIMAL_MODE_BIT : flagByte &= ~(1UL << DECIMAL_MODE_BIT);
	OVERFLOWBIT ? flagByte |= 1UL << OVERFLOW_BIT : flagByte &= ~(1UL << OVERFLOW_BIT);
	NEGATIVE ? flagByte |= 1UL << NEGATIVE_BIT : flagByte &= ~(1UL << NEGATIVE_BIT);

	push(flagByte);	
}

CPU::OPEXEC CPU::PLA(ADDRESS source)
{
	A = pop();
}

CPU::OPEXEC CPU::PLP(ADDRESS source)
{
	uint8_t data = pop();
	CARRY = (data >> CARRY_BIT) & 1;
	ZERO = (data >> ZERO_BIT) & 1;
	DECIMAL = (data >> DECIMAL_MODE_BIT) & 1;
	BREAK = (data >> BREAK_BIT) & 1;
	OVERFLOWBIT = (data >> OVERFLOW_BIT) & 1;
	NEGATIVE = (data >> NEGATIVE_BIT) & 1;	
}

CPU::OPEXEC CPU::ROL(ADDRESS source)
{
	uint8_t data = memory.readFromMemory(source);
	data <<= 1;
	if(CARRY) data |= 0x01;
	CARRY = data > 0xFF;
	data &= 0xFF;
	ZERO = !data;
	NEGATIVE = data & 0x80;
	memory.writeToMemory(source,data);
}


CPU::OPEXEC CPU::ROL_ACC(ADDRESS source)
{
	A <<= 1;
	if(CARRY) A |= 0x01;
	CARRY = A > 0xFF;
	A &= 0xFF;
	ZERO = !A;
	NEGATIVE = A & 0x80;
}

CPU::OPEXEC CPU::ROR(ADDRESS source)
{
	uint8_t data = memory.readFromMemory(source);
	if(CARRY) data |= 0x100;
	CARRY = data & 0x01;
	data >>= 1;
	data &= 0xFF;
	NEGATIVE = data & 0x80;
	ZERO = !data;
	memory.writeToMemory(source,data);
}

CPU::OPEXEC CPU::ROR_ACC(ADDRESS source)
{
	uint8_t data = A;
	if(CARRY) data |= 0x100;
	CARRY = data & 0x01;
	data >>= 1;
	data &= 0xFF;
	NEGATIVE = data & 0x80;
	ZERO = !data;
	A = data;
}

CPU::OPEXEC CPU::RTI(ADDRESS source)
{
	uint8_t low,high,flagByte;
	flagByte = pop();
	
	CARRY = (flagByte >> CARRY_BIT) & 1;
	ZERO = (flagByte >> ZERO_BIT) & 1;
	DECIMAL = (flagByte >> DECIMAL_MODE_BIT) & 1;
	BREAK = (flagByte >> BREAK_BIT) & 1;
	OVERFLOWBIT = (flagByte >> OVERFLOW_BIT) & 1;
	NEGATIVE = (flagByte >> NEGATIVE_BIT) & 1;	

	low = pop();
	high = pop();

	programCounter = (high << 8) | low; 
}

CPU::OPEXEC CPU::RTS(ADDRESS source)
{
	uint8_t low,high;

	low = pop();
	high = pop();

	programCounter = ((high << 8) | low) + 1;
}

CPU::OPEXEC CPU::SBC(ADDRESS source)
{
	uint8_t data = memory.readFromMemory(source);
	uint32_t temp = A - data - (CARRY ? 1 : 0);
	NEGATIVE = temp & 0x80;
	ZERO = !(temp & 0xFF);
	OVERFLOWBIT = ((A ^ temp) & 0x80) && ((A ^ data) & 0x80);

	if(DECIMAL)
	{
		if( ((A & 0x0F)  - CARRY)  < (data & 0x0F)) temp -= 6;
		if(temp > 0x99)
			temp -= 0x60; 
	}  
	CARRY = temp < 0x100;
	A = (temp & 0xFF);
}

CPU::OPEXEC CPU::SEC(ADDRESS source)
{
	CARRY = 1;
}

CPU::OPEXEC CPU::SED(ADDRESS source)
{
	DECIMAL = 1;
}

CPU::OPEXEC CPU::SEI(ADDRESS source)
{
	INTERRUPT_DISABLE = 1;
}

CPU::OPEXEC CPU::STA(ADDRESS source)
{
	memory.writeToMemory(source,A);
}

CPU::OPEXEC CPU::STX(ADDRESS source)
{
	memory.writeToMemory(source,X);
}

CPU::OPEXEC CPU::STY(ADDRESS source)
{
	memory.writeToMemory(source,Y);
}

CPU::OPEXEC CPU::TAX(ADDRESS source)
{
	X = A;
	ZERO = !X;
	NEGATIVE = X & 0x80;
}

CPU::OPEXEC CPU::TAY(ADDRESS source)
{
	Y = A;
	ZERO = !Y;
	NEGATIVE = Y & 0x80;
}

CPU::OPEXEC CPU::TSX(ADDRESS source)
{
	X = SP;
	ZERO = !X;
	NEGATIVE = X & 0x80;
}

CPU::OPEXEC CPU::TXA(ADDRESS source)
{
	A = X;
	ZERO = !X;
	NEGATIVE = X & 0x80;
}

CPU::OPEXEC CPU::TYA(ADDRESS source)
{
	A = Y;
	ZERO = !Y;
	NEGATIVE = Y & 0x80;
}

CPU::OPEXEC CPU::TXS(ADDRESS source)
{
	SP = X;	
}

CPU::OPEXEC CPU::ILLEGAL(ADDRESS source)
{
	exit(1);
}