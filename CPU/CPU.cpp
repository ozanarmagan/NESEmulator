#include "CPU.h"
#include <iostream>
#include <functional>
#include <iomanip>
using namespace std;


namespace{
	std::string bTohex(uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};
}


CPU::CPU(Bus& mem,PPU& ppu) : bus(mem),ppu(ppu) { 

	programCounter = 0;

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
	temp.operation = &CPU::INX_OP;
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



void CPU::reset()
{
    A = 0x00;
    X = 0x00;
    Y = 0x00;
    SP = 0xFF;
}

void CPU::push(BYTE value)
{
    bus.writeToMemory(0x0100 + SP,value);
    if(SP == 0x00) SP = 0xFF;
    else SP--;
}

BYTE CPU::pop()
{
    if(SP = 0xFF) SP == 0x00;
    else SP++;
    return bus.readFromMemory(0x0100 + SP);
}

void CPU::tick()
{
	currentOpCode = bus.readFromMemory(programCounter++); // Fetch

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

std::ostream& operator<<(std::ostream &out,CPU &cpu)
{
	out << "REGISTERS: " << std::endl;
	out << "A: " << bTohex(cpu.A,2) << std::endl << "X: " << bTohex(cpu.X,2) << std::endl << "Y: " << bTohex(cpu.Y,2) << std::endl << "SP: " << bTohex(cpu.SP,2) << std::endl;
	out << "PROGRAM COUNTER: " << bTohex(cpu.programCounter,2) << std::endl;

	return out;
}