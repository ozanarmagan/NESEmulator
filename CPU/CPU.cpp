#include "CPU.h"
#include <fstream>

constexpr BYTE CPU::cycleCounts[];

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


CPU::CPU(Bus& mem) : bus(mem) { 

#ifdef CPUDEBUG
	logFile = fopen("./Logs/cpu.log","w+");
#endif
	
	COUNTER = 0;
	programCounter = 0;

    // Fill with NOP for empty OPCODES 
    INSTRUCTION temp; 
    temp.operation = &CPU::NOP;
    temp.addr = &CPU::IMP;
    for(int i = 0;i < 256;i++)
        table[i] = temp;

    // Fill table now
    temp.addr = &CPU::IMM;
	temp.operation = &CPU::ADC;
	table[0x69] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::ADC;
	table[0x6D] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::ADC;
	table[0x65] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::ADC;
	table[0x61] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::ADC;
	table[0x71] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::ADC;
	table[0x75] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::ADC;
	table[0x7D] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::ADC;
	table[0x79] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::ASL;
	table[0x0A] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::LSR;
	table[0x4A] = temp;
	temp.addr = &CPU::IMM;
	temp.operation = &CPU::AND;
	table[0x29] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::AND;
	table[0x2D] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::AND;
	table[0x25] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::AND;
	table[0x21] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::AND;
	table[0x31] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::AND;
	table[0x35] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::AND;
	table[0x3D] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::AND;
	table[0x39] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::ASL;
	table[0x0E] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::ASL;
	table[0x06] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::ASL;
	table[0x16] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::ASL;
	table[0x1E] = temp;
	temp.addr = &CPU::REL;
	temp.operation = &CPU::BCC;
	table[0x90] = temp;
	temp.addr = &CPU::REL;
	temp.operation = &CPU::BCS;
	table[0xB0] = temp;
	temp.addr = &CPU::REL;
	temp.operation = &CPU::BEQ;
	table[0xF0] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::BIT;
	table[0x2C] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::BIT;
	table[0x24] = temp;
	temp.addr = &CPU::REL;
	temp.operation = &CPU::BMI;
	table[0x30] = temp;
	temp.addr = &CPU::REL;
	temp.operation = &CPU::BNE;
	table[0xD0] = temp;
	temp.addr = &CPU::REL;
	temp.operation = &CPU::BPL;
	table[0x10] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::BRK;
	table[0x00] = temp;
	temp.addr = &CPU::REL;
	temp.operation = &CPU::BVC;
	table[0x50] = temp;
	temp.addr = &CPU::REL;
	temp.operation = &CPU::BVS;
	table[0x70] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::CLC;
	table[0x18] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::CLD;
	table[0xD8] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::CLI;
	table[0x58] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::CLV;
	table[0xB8] = temp;
	temp.addr = &CPU::IMM;
	temp.operation = &CPU::CMP;
	table[0xC9] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::CMP;
	table[0xCD] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::CMP;
	table[0xC5] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::CMP;
	table[0xC1] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::CMP;
	table[0xD1] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::CMP;
	table[0xD5] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::CMP;
	table[0xDD] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::CMP;
	table[0xD9] = temp;
	temp.addr = &CPU::IMM;
	temp.operation = &CPU::CPX;
	table[0xE0] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::CPX;
	table[0xEC] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::CPX;
	table[0xE4] = temp;
	temp.addr = &CPU::IMM;
	temp.operation = &CPU::CPY;
	table[0xC0] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::CPY;
	table[0xCC] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::CPY;
	table[0xC4] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::DEC;
	table[0xCE] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::DEC;
	table[0xC6] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::DEC;
	table[0xD6] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::DEC;
	table[0xDE] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::DEX;
	table[0xCA] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::DEY;
	table[0x88] = temp;
	temp.addr = &CPU::IMM;
	temp.operation = &CPU::EOR;
	table[0x49] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::EOR;
	table[0x4D] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::EOR;
	table[0x45] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::EOR;
	table[0x41] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::EOR;
	table[0x51] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::EOR;
	table[0x55] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::EOR;
	table[0x5D] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::EOR;
	table[0x59] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::INC;
	table[0xEE] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::INC;
	table[0xE6] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::INC;
	table[0xF6] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::INC;
	table[0xFE] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::INX_OP;
	table[0xE8] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::INY_OP;
	table[0xC8] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::JMP;
	table[0x4C] = temp;
	temp.addr = &CPU::ABI;
	temp.operation = &CPU::JMP;
	table[0x6C] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::JSR;
	table[0x20] = temp;
	temp.addr = &CPU::IMM;
	temp.operation = &CPU::LDA;
	table[0xA9] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::LDA;
	table[0xAD] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::LDA;
	table[0xA5] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::LDA;
	table[0xA1] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::LDA;
	table[0xB1] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::LDA;
	table[0xB5] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::LDA;
	table[0xBD] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::LDA;
	table[0xB9] = temp;
	temp.addr = &CPU::IMM;
	temp.operation = &CPU::LDX;
	table[0xA2] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::LDX;
	table[0xAE] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::LDX;
	table[0xA6] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::LDX;
	table[0xBE] = temp;
	temp.addr = &CPU::ZEY;
	temp.operation = &CPU::LDX;
	table[0xB6] = temp;
	temp.addr = &CPU::IMM;
	temp.operation = &CPU::LDY;
	table[0xA0] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::LDY;
	table[0xAC] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::LDY;
	table[0xA4] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::LDY;
	table[0xB4] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::LDY;
	table[0xBC] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::LSR;
	table[0x4E] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::LSR;
	table[0x46] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::LSR;
	table[0x56] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::LSR;
	table[0x5E] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::NOP;
	table[0xEA] = temp;
	temp.addr = &CPU::IMM;
	temp.operation = &CPU::ORA;
	table[0x09] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::ORA;
	table[0x0D] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::ORA;
	table[0x05] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::ORA;
	table[0x01] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::ORA;
	table[0x11] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::ORA;
	table[0x15] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::ORA;
	table[0x1D] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::ORA;
	table[0x19] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::PHA;
	table[0x48] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::PHP;
	table[0x08] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::PLA;
	table[0x68] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::PLP;
	table[0x28] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::ROL;
	table[0x2E] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::ROL;
	table[0x26] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::ROL;
	table[0x36] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::ROL;
	table[0x3E] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::ROL;
	table[0x2A] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::ROR;
	table[0x6E] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::ROR;
	table[0x66] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::ROR;
	table[0x76] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::ROR;
	table[0x7E] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::ROR;
	table[0x6A] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::RTI;
	table[0x40] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::RTS;
	table[0x60] = temp;
	temp.addr = &CPU::IMM;
	temp.operation = &CPU::SBC;
	table[0xE9] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::SBC;
	table[0xED] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::SBC;
	table[0xE5] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::SBC;
	table[0xE1] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::SBC;
	table[0xF1] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::SBC;
	table[0xF5] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::SBC;
	table[0xFD] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::SBC;
	table[0xF9] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::SEC;
	table[0x38] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::SED;
	table[0xF8] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::SEI;
	table[0x78] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::STA;
	table[0x8D] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::STA;
	table[0x85] = temp;
	temp.addr = &CPU::INX;
	temp.operation = &CPU::STA;
	table[0x81] = temp;
	temp.addr = &CPU::INY;
	temp.operation = &CPU::STA;
	table[0x91] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::STA;
	table[0x95] = temp;
	temp.addr = &CPU::ABX;
	temp.operation = &CPU::STA;
	table[0x9D] = temp;
	temp.addr = &CPU::ABY;
	temp.operation = &CPU::STA;
	table[0x99] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::STX;
	table[0x8E] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::STX;
	table[0x86] = temp;
	temp.addr = &CPU::ZEY;
	temp.operation = &CPU::STX;
	table[0x96] = temp;
	temp.addr = &CPU::ABS;
	temp.operation = &CPU::STY;
	table[0x8C] = temp;
	temp.addr = &CPU::ZER;
	temp.operation = &CPU::STY;
	table[0x84] = temp;
	temp.addr = &CPU::ZEX;
	temp.operation = &CPU::STY;
	table[0x94] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::TAX;
	table[0xAA] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::TAY;
	table[0xA8] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::TSX;
	table[0xBA] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::TXA;
	table[0x8A] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::TXS;
	table[0x9A] = temp;
	temp.addr = &CPU::IMP;
	temp.operation = &CPU::TYA;
	table[0x98] = temp;
};


void CPU::push(BYTE value)
{
    bus.writeToMemory(0x0100 + SP--,value);
}

BYTE CPU::pop()
{
    SP++;
    return bus.readFromMemory(0x0100 + SP);
}

void CPU::tick()
{
	if(cycleRemaining == 0)
	{
#ifdef CPUDEBUG
		logToFile();
#endif

		if(programCounter == 0xF216 && A == 0x2C)
			std::cout << "hey";

		currentOpCode = bus.readFromMemory(programCounter++); // Fetch

		currentInstruction = table[currentOpCode]; // Decode 

       
		cycleRemaining += CPU::cycleCounts[currentOpCode];

		source = (this->*currentInstruction.addr)();



		(this->*currentInstruction.operation)(source); // Execute


		cycleRemaining += (additionalCycle0 & additionalCycle1);

		additionalCycle0 = 0x00;
		additionalCycle1 = 0x00;
	}
		
    COUNTER++;

	cycleRemaining--;
}




std::ostream& operator<<(std::ostream &out,CPU &cpu)
{
	out << "REGISTERS: " << std::endl;
	out << "A: " << bTohex(cpu.A,2) << std::endl << "X: " << bTohex(cpu.X,2) << std::endl << "Y: " << bTohex(cpu.Y,2) << std::endl << "SP: " << bTohex(cpu.SP,2) << std::endl;
	out << "PROGRAM COUNTER: " << bTohex(cpu.programCounter,4) << std::endl;

	return out;
}


void CPU::reset()
{
	programCounter = 0xFFFC;
	BYTE low,high;
	low = bus.readFromMemory(programCounter);
	high = bus.readFromMemory(programCounter + 1);

	programCounter =  (high << 8) | low;

	A = 0;
	X = 0;
	Y = 0;
	SP = 0xFD;

	STATUS.CARRY = 0;
	STATUS.OVERFLOW = 0;
	STATUS.ZERO = 0;
	STATUS.NEGATIVE = 0;
	STATUS.BREAK = 0;
	STATUS.INTERRUPT = 1;
	STATUS.DECIMAL = 0;

	COUNTER = 0;

	cycleRemaining = 8;
}



void CPU::NMI()
{
	STATUS.BREAK = 0;
	STATUS.UNUSED = 1;
	STATUS.INTERRUPT = 1;
	push((programCounter >> 8) & 0xFF);
	push(programCounter & 0xFF);
	push(STATUS.combined);

	
	programCounter = (bus.readFromMemory(NMIVECTOR_H) << 8) | bus.readFromMemory(NMIVECTOR_L);

	cycleRemaining = 8;
}



ADDRESS CPU::IMM()
{
	return programCounter++; 
}; // IMMEDIATE

ADDRESS CPU::ABS()
{
	ADDRESS addrLower = bus.readFromMemory(programCounter++),
	addrHigher = bus.readFromMemory(programCounter++); 
	return (addrHigher << 8) | addrLower; 
};  // ABSOLUTE

ADDRESS CPU::ZER()
{ return bus.readFromMemory(programCounter++) % 256; }; // ZERO PAGE

ADDRESS CPU::ZEX()
{ return (bus.readFromMemory(programCounter++) + X) % 256; }; // INDEXED-X ZERO PAGE

ADDRESS CPU::ZEY()
{ return (bus.readFromMemory(programCounter++) + Y) % 256; }; // INDEXED-Y ZERO PAGE

ADDRESS CPU::ABX()
{ ADDRESS addrLower = bus.readFromMemory(programCounter++),
	addrHigher = bus.readFromMemory(programCounter++),absolute;
	absolute = (addrHigher << 8) | addrLower; 
	absolute += X; 
	if((absolute & 0xFF00) != (addrHigher << 8)) 
		additionalCycle0 = 1; 
	return absolute; 
}; // INDEXED-X ABSOLUTE

ADDRESS CPU::ABY()
{
	ADDRESS addrLower = bus.readFromMemory(programCounter++),
	addrHigher = bus.readFromMemory(programCounter++),absolute;
	absolute = (addrHigher << 8) | addrLower; 
	absolute += Y; 
	if((absolute & 0xFF00) != (addrHigher << 8)) 
		additionalCycle0 = 1; 
	return absolute;
}; // INDEXED-Y ABSOLUTE

ADDRESS CPU::IMP()
{ return 0; }; // IMPLIED

ADDRESS CPU::REL()
{ 
	ADDRESS offset = bus.readFromMemory(programCounter++); 
	if(offset & 0x80) offset |= 0xFF00; 
	return programCounter +  offset; 
}; // RELATIVE

ADDRESS CPU::INX()
{ 
	ADDRESS temp = bus.readFromMemory(programCounter++);
	ADDRESS lower = bus.readFromMemory((temp + (ADDRESS)X) % 256),higher = bus.readFromMemory((temp + (ADDRESS)X + 1) % 256);
	return (higher << 8) | lower;
}; // INDEXED-X INDIRECT

ADDRESS CPU::INY()
{ 
	ADDRESS temp = bus.readFromMemory(programCounter++);
	ADDRESS lower = bus.readFromMemory(temp % 256),higher = bus.readFromMemory((temp + 1) % 256);
	return ((higher << 8) | lower) + Y; 
}; // INDEXED-Y INDIRECT

ADDRESS CPU::ABI()
{
	ADDRESS addressLower = bus.readFromMemory(programCounter++);
	ADDRESS addressHigher = bus.readFromMemory(programCounter++);
	ADDRESS abs = (addressHigher << 8) | addressLower;
	ADDRESS result = 0x0000;
	if(addressLower == 0x00FF) result =  (bus.readFromMemory(abs &  0xFF00) << 8) | bus.readFromMemory(abs + 0);
	else result = ((bus.readFromMemory(abs + 1) << 8) | bus.readFromMemory(abs + 0));
	return result; 
}; // ABSOLUTE INDIRECT




CPU::OPEXEC CPU::ADC(ADDRESS source)
{
	BYTE data = bus.readFromMemory(source);
	ADDRESS temp = (ADDRESS) data + (ADDRESS) A + (ADDRESS)(STATUS.CARRY ? 1 : 0);
	STATUS.CARRY = temp > 255 ? 1 : 0;

	STATUS.ZERO = (temp & 0x00FF) == 0 ? 1 : 0;

	STATUS.OVERFLOW = ((~((ADDRESS)A ^ (ADDRESS) data) & ((ADDRESS)A ^ (ADDRESS)temp)) & 0x0080) ? 1 : 0;

	STATUS.NEGATIVE = temp & 0x0080 ? 1 : 0;

	A = temp & 0x00FF;
	additionalCycle1++;
};

CPU::OPEXEC CPU::AND(ADDRESS source)
{
	A = A & bus.readFromMemory(source);
	STATUS.ZERO = (A & 0xFF) == 0 ? 1 : 0;
	STATUS.NEGATIVE = (A & 0x80) ? 1 : 0;
	additionalCycle1++;
};

CPU::OPEXEC CPU::ASL(ADDRESS source)
{
	ADDRESS data = currentInstruction.addr == &CPU::IMP ? (ADDRESS) A : bus.readFromMemory(source);
	data <<= 1;
	STATUS.CARRY = (data > 0xFF) ? 1 : 0;
	STATUS.ZERO = (data & 0x00FF) == 0 ? 1 : 0;
	STATUS.NEGATIVE = 0;
	if(currentInstruction.addr == &CPU::IMP)
		A = data & 0x00FF;
	else
		bus.writeToMemory(source,data);
};


CPU::OPEXEC CPU::BCC(ADDRESS source)
{
	if(!STATUS.CARRY)
{
		cycleRemaining++;
		programCounter = source;
		if ((source & 0xFF00) != (programCounter & 0xFF00))
			cycleRemaining++;
	}
};

CPU::OPEXEC CPU::BCS(ADDRESS source)
{
	if(STATUS.CARRY)
{
		cycleRemaining++;
		programCounter = source;
		if ((source & 0xFF00) != (programCounter & 0xFF00))
			cycleRemaining++;
	}
};

CPU::OPEXEC CPU::BEQ(ADDRESS source)
{
	if(STATUS.ZERO)
{
		cycleRemaining++;
		programCounter = source;
		if ((source & 0xFF00) != (programCounter & 0xFF00))
			cycleRemaining++;
	}
};

CPU::OPEXEC CPU::BIT(ADDRESS source)
{
	ADDRESS data = bus.readFromMemory(source);
	STATUS.ZERO = (data & A) == 0 ? 1 : 0; 
	STATUS.NEGATIVE = (data & 0x80) ? 1 : 0;
	STATUS.OVERFLOW = CHECK_BIT(data,6) == 1 ? 1 : 0;
};

CPU::OPEXEC CPU::BMI(ADDRESS source)
{
	if(STATUS.NEGATIVE)
{
		cycleRemaining++;
		programCounter = source;
		if ((source & 0xFF00) != (programCounter & 0xFF00))
			cycleRemaining++;
	}
};

CPU::OPEXEC CPU::BNE(ADDRESS source)
{
	if(!STATUS.ZERO)
{
		cycleRemaining++;
		programCounter = source;
		if ((source & 0xFF00) != (programCounter & 0xFF00))
			cycleRemaining++;
	}
};

CPU::OPEXEC CPU::BPL(ADDRESS source)
{
	if(!STATUS.NEGATIVE)
{
		cycleRemaining++;
		programCounter = source;
		if ((source & 0xFF00) != (programCounter & 0xFF00))
			cycleRemaining++;
	}
};

CPU::OPEXEC CPU::BRK(ADDRESS source)
{
	STATUS.BREAK = 1;
	STATUS.INTERRUPT = 1;
	programCounter++;
	push((programCounter >> 8) & 0xFF);
	push(programCounter & 0xFF);
	push(STATUS.combined);
	STATUS.BREAK = 0;
	programCounter = bus.readFromMemory(IRQVECTOR_L) | (bus.readFromMemory(IRQVECTOR_H) << 8);
};

CPU::OPEXEC CPU::BVC(ADDRESS source)
{
	if(!STATUS.OVERFLOW)
{
		cycleRemaining++;
		programCounter = source;
		if ((source & 0xFF00) != (programCounter & 0xFF00))
			cycleRemaining++;
	}
};

CPU::OPEXEC CPU::BVS(ADDRESS source)
{
	if(STATUS.OVERFLOW)
{
		cycleRemaining++;
		programCounter = source;
		if ((source & 0xFF00) != (programCounter & 0xFF00))
			cycleRemaining++;
	}
};

CPU::OPEXEC CPU::CLC(ADDRESS source)
{
	STATUS.CARRY = 0;
};

CPU::OPEXEC CPU::CLD(ADDRESS source)
{
	STATUS.DECIMAL = 0;
};

CPU::OPEXEC CPU::CLI(ADDRESS source)
{
	STATUS.INTERRUPT = 0;
};

CPU::OPEXEC CPU::CLV(ADDRESS source)
{
	STATUS.OVERFLOW = 0;
};

CPU::OPEXEC CPU::CMP(ADDRESS source)
{
	BYTE data = bus.readFromMemory(source);
	BYTE temp = A - data;
	STATUS.CARRY = (A >= data) ? 1 : 0;
	STATUS.NEGATIVE = (0x80 & temp) ? 1 : 0;
	STATUS.ZERO = (temp & 0xFF) ? 0 : 1;
	additionalCycle1++;
};

CPU::OPEXEC CPU::CPX(ADDRESS source)
{
	BYTE data = bus.readFromMemory(source);
	BYTE temp = X - data;
	STATUS.CARRY = (X >= data) ? 1 : 0;
	STATUS.NEGATIVE = (0x80 & temp) ? 1 : 0;
	STATUS.ZERO = (temp & 0x00FF) == 0x0000 ? 1 : 0;
	additionalCycle1++;
};

CPU::OPEXEC CPU::CPY(ADDRESS source)
{
	BYTE data = bus.readFromMemory(source);
	BYTE temp = Y - data;
	STATUS.CARRY = (Y >= data) ? 1 : 0;
	STATUS.NEGATIVE = (0x80 & temp) ? 1 : 0;
	STATUS.ZERO = (temp & 0x00FF) == 0x0000 ? 1 : 0;
	additionalCycle1++;
};

CPU::OPEXEC CPU::DEC(ADDRESS source)
{
	BYTE data = bus.readFromMemory(source);
	ADDRESS temp = (ADDRESS) data - 1;
	STATUS.NEGATIVE = (temp & 0x0080) ? 1 : 0;
	STATUS.ZERO = (temp & 0x00FF) == 0x0000 ? 1 : 0;
	bus.writeToMemory(source,temp & 0x00FF);
};

CPU::OPEXEC CPU::DEX(ADDRESS source)
{
	BYTE data = X - 1;
	STATUS.NEGATIVE = (data & 0x80) ? 1 : 0;
	STATUS.ZERO = !data;
	X = data;
};

CPU::OPEXEC CPU::DEY(ADDRESS source)
{
	BYTE data = Y - 1;
	STATUS.NEGATIVE = (data & 0x80) ? 1 : 0;
	STATUS.ZERO = !data;
	Y = data;
};

CPU::OPEXEC CPU::EOR(ADDRESS source)
{
	BYTE data = A ^ bus.readFromMemory(source);
	STATUS.NEGATIVE = (data & 0x80) ? 1 : 0;
	STATUS.ZERO = data == 0 ? 1 : 0;
	A = data;
};

CPU::OPEXEC CPU::INC(ADDRESS source)
{
	BYTE data = (bus.readFromMemory(source) + 1) % 256;
	STATUS.ZERO = (data == 0x00) ? 1 : 0;
	STATUS.NEGATIVE = (data & 0x80) ? 1 : 0;
	bus.writeToMemory(source,data);
};

CPU::OPEXEC CPU::INX_OP(ADDRESS source)
{
	X = (X + 1) % 256;
	STATUS.ZERO = (X == 0x00) ? 1 : 0;
	STATUS.NEGATIVE = (X & 0x80) ? 1 : 0;
};

CPU::OPEXEC CPU::INY_OP(ADDRESS source)
{
	Y = (Y + 1) % 256;
	STATUS.ZERO = (Y == 0x00) ? 1 : 0;
	STATUS.NEGATIVE = (Y & 0x80) ? 1 : 0;
};

CPU::OPEXEC CPU::JMP(ADDRESS source)
{
	programCounter = source;
};

CPU::OPEXEC CPU::JSR(ADDRESS source)
{
	programCounter--;
	push((programCounter >> 8) &  0x00FF);
	push(programCounter & 0x00FF);
	programCounter = source;
};

CPU::OPEXEC CPU::LDA(ADDRESS source)
{
	A = bus.readFromMemory(source);
	STATUS.ZERO = (A == 0x00) ? 1 : 0;
	STATUS.NEGATIVE = 	(A & 0x80) ? 1 : 0;
	additionalCycle1++;
};

CPU::OPEXEC CPU::LDX(ADDRESS source)
{
	X = bus.readFromMemory(source);
	STATUS.ZERO = (X == 0) ? 1 : 0;
	STATUS.NEGATIVE = 	(X & 0x80) ? 1 : 0;
	additionalCycle1++;
};

CPU::OPEXEC CPU::LDY(ADDRESS source)
{
	Y = bus.readFromMemory(source);
	STATUS.ZERO = (Y == 0) ? 1 : 0;
	STATUS.NEGATIVE = 	(Y & 0x80) ? 1 : 0;
	additionalCycle1++;
};

CPU::OPEXEC CPU::LSR(ADDRESS source)
{
	ADDRESS data = currentInstruction.addr == &CPU::IMP ? A : bus.readFromMemory(source);
	STATUS.CARRY = data & 0x0001;
	data >>= 1;
	STATUS.ZERO = (data & 0x00FF) == 0 ? 1 : 0;
	STATUS.NEGATIVE = 0;
	if(currentInstruction.addr == &CPU::IMP)
		A = data & 0x00FF;
	else
		bus.writeToMemory(source,data);
};


CPU::OPEXEC CPU::NOP(ADDRESS source)
{ };

CPU::OPEXEC CPU::ORA(ADDRESS source)
{
	A = bus.readFromMemory(source) | A;
	STATUS.ZERO = (A == 0x00) ? 1 : 0;
	STATUS.NEGATIVE = (A & 0x80) ? 1 : 0;
};

CPU::OPEXEC CPU::PHA(ADDRESS source)
{
	push(A);
};

CPU::OPEXEC CPU::PHP(ADDRESS source)
{
	push(STATUS.combined);	
};

CPU::OPEXEC CPU::PLA(ADDRESS source)
{
	A = pop();
	STATUS.ZERO = (A == 0x00) ? 1 : 0;
	STATUS.NEGATIVE = (A & 0x80) ? 1 : 0;
};

CPU::OPEXEC CPU::PLP(ADDRESS source)
{
	STATUS.combined = pop();
};

CPU::OPEXEC CPU::ROL(ADDRESS source)
{
	ADDRESS data =  currentInstruction.addr == &CPU::IMP ? A : bus.readFromMemory(source);
	bool temp = data & 0x80;
	data <<= 1;
	if(STATUS.CARRY) data |= 0x01;
	STATUS.CARRY = temp ? 1 : 0;
	STATUS.ZERO = (data == 0x00) ? 1 : 0;
	STATUS.NEGATIVE = (data & 0x80) ? 1 : 0;
	if(currentInstruction.addr == &CPU::IMP)
		A = data;
	else
		bus.writeToMemory(source,data);
};

CPU::OPEXEC CPU::ROR(ADDRESS source)
{
	BYTE data = currentInstruction.addr == &CPU::IMP ? A : bus.readFromMemory(source);
	bool temp = CHECK_BIT(data,0);
	STATUS.NEGATIVE = STATUS.CARRY ? 1 : 0;
	data >>= 1;
	if(STATUS.CARRY) SET_BIT(data,7);
	STATUS.CARRY = temp ? 1 : 0;
	STATUS.ZERO = data == 0x00 ? 1 : 0;
	if(currentInstruction.addr == &CPU::IMP)
		A = data;
	else
		bus.writeToMemory(source,data);
};


CPU::OPEXEC CPU::RTI(ADDRESS source)
{
	BYTE low,high;

	STATUS.combined = pop();
	STATUS.BREAK = 0;
	STATUS.UNUSED = 0;
	low = pop();
	high = pop();

	programCounter = (high << 8) | low; 
};

CPU::OPEXEC CPU::RTS(ADDRESS source)
{
	BYTE low,high;

	low = pop();
	high = pop();

	programCounter = ((high << 8) | low) + 1;
};

CPU::OPEXEC CPU::SBC(ADDRESS source)
{
	BYTE data = bus.readFromMemory(source);

	ADDRESS val = (ADDRESS) data ^ 0x00FF;

	ADDRESS temp =  (ADDRESS) A + val + (ADDRESS)(STATUS.CARRY ? 1 : 0);
	STATUS.CARRY = temp & 0xFF00 ? 1 : 0;

	STATUS.ZERO = (temp & 0x00FF) == 0 ? 1 : 0;

	STATUS.OVERFLOW = ((temp ^ (ADDRESS) A) & (temp ^ val) & 0x0080) ? 1 : 0;

	STATUS.NEGATIVE = temp & 0x0080 ? 1 : 0;

	A = temp & 0x00FF;
	additionalCycle1++;
};

CPU::OPEXEC CPU::SEC(ADDRESS source)
{
	STATUS.CARRY = 1;
};

CPU::OPEXEC CPU::SED(ADDRESS source)
{
	STATUS.DECIMAL = 1;
};

CPU::OPEXEC CPU::SEI(ADDRESS source)
{
	STATUS.INTERRUPT = 1;
};

CPU::OPEXEC CPU::STA(ADDRESS source)
{
	bus.writeToMemory(source,A);
};

CPU::OPEXEC CPU::STX(ADDRESS source)
{
	bus.writeToMemory(source,X);
};

CPU::OPEXEC CPU::STY(ADDRESS source)
{
	bus.writeToMemory(source,Y);
};

CPU::OPEXEC CPU::TAX(ADDRESS source)
{
	X = A;
	STATUS.ZERO = (X == 0x00) ? 1 : 0;
	STATUS.NEGATIVE = (X & 0x80) ? 1 : 0;
};

CPU::OPEXEC CPU::TAY(ADDRESS source)
{
	Y = A;
	STATUS.ZERO = (Y == 0x00) ? 1 : 0;
	STATUS.NEGATIVE = (Y & 0x80) ? 1 : 0;
};

CPU::OPEXEC CPU::TSX(ADDRESS source)
{
	X = SP;
	STATUS.ZERO = (X == 0x00) ? 1 : 0;
	STATUS.NEGATIVE = (X & 0x80) ? 1 : 0;
};

CPU::OPEXEC CPU::TXA(ADDRESS source)
{
	A = X;
	STATUS.ZERO = (A== 0x00) ? 1 : 0;
	STATUS.NEGATIVE = (A & 0x80) ? 1 : 0;
};

CPU::OPEXEC CPU::TYA(ADDRESS source)
{
	A = Y;
	STATUS.ZERO = (A== 0x00) ? 1 : 0;
	STATUS.NEGATIVE = (A & 0x80) ? 1 : 0;
};

CPU::OPEXEC CPU::TXS(ADDRESS source)
{
	SP = X;	
};

CPU::OPEXEC CPU::ILLEGAL(ADDRESS source)
{

};












#ifdef CPUDEBUG

void CPU::logToFile()
{
		std::string op = "";
		if(currentInstruction.operation == &CPU::AND)
			op = "AND";
		else if(currentInstruction.operation == &CPU::ADC)
			op = "ADC";
		else if(currentInstruction.operation == &CPU::ASL)
			op = "ASL";
		else if(currentInstruction.operation == &CPU::BCC)
			op = "BCC";
		else if(currentInstruction.operation == &CPU::BCS)
			op = "BCS";
		else if(currentInstruction.operation == &CPU::BEQ)
			op = "BEQ";
		else if(currentInstruction.operation == &CPU::BIT)
			op = "BIT";
		else if(currentInstruction.operation == &CPU::BMI)
			op = "BMI";
		else if(currentInstruction.operation == &CPU::BNE)
			op = "BNE";
		else if(currentInstruction.operation == &CPU::BPL)
			op = "BPL";
		else if(currentInstruction.operation == &CPU::BRK)
			op = "BRK";
		else if(currentInstruction.operation == &CPU::BVC)
			op = "BC";
		else if(currentInstruction.operation == &CPU::BVS)
			op = "BVS";
		else if(currentInstruction.operation == &CPU::CLC)
			op = "CLC";
		else if(currentInstruction.operation == &CPU::CLD)
			op = "CLD";
		else if(currentInstruction.operation == &CPU::CLI)
			op = "CLI";
		else if(currentInstruction.operation == &CPU::CLV)
			op = "CLV";
		else if(currentInstruction.operation == &CPU::CMP)
			op = "CMP";
		else if(currentInstruction.operation == &CPU::CPX)
			op = "CPX";
		else if(currentInstruction.operation == &CPU::CPY)
			op = "CPY";
		else if(currentInstruction.operation == &CPU::DEC)
			op = "DEC";
		else if(currentInstruction.operation == &CPU::DEX)
			op = "DEX";
		else if(currentInstruction.operation == &CPU::DEY)
			op = "DEY";
		else if(currentInstruction.operation == &CPU::EOR)
			op = "EOR";
		else if(currentInstruction.operation == &CPU::ILLEGAL)
			op = "ILLEGAL";
		else if(currentInstruction.operation == &CPU::INC)
			op = "INC";
		else if(currentInstruction.operation == &CPU::INX_OP)
			op = "INX";
		else if(currentInstruction.operation == &CPU::INY_OP)
			op = "INY";
		else if(currentInstruction.operation == &CPU::JMP)
			op = "JMP";
		else if(currentInstruction.operation == &CPU::JSR)
			op = "JSR";
		else if(currentInstruction.operation == &CPU::LDA)
			op = "LDA";
		else if(currentInstruction.operation == &CPU::LDX)
			op = "LDX";
		else if(currentInstruction.operation == &CPU::LDY)
			op = "LDY";
		else if(currentInstruction.operation == &CPU::LSR)
			op = "LSR";
		else if(currentInstruction.operation == &CPU::NOP)
			op = "NOP";
		else if(currentInstruction.operation == &CPU::ORA)
			op = "ORA";
		else if(currentInstruction.operation == &CPU::PHA)
			op = "PHA";
		else if(currentInstruction.operation == &CPU::PHP)
			op = "PHP";
		else if(currentInstruction.operation == &CPU::PLA)
			op = "PLA";
		else if(currentInstruction.operation == &CPU::PLP)
			op = "PLP";
		else if(currentInstruction.operation == &CPU::ROL)
			op = "ROL";
		else if(currentInstruction.operation == &CPU::ROR)
			op = "ROR";
		else if(currentInstruction.operation == &CPU::RTI)
			op = "RTI";
		else if(currentInstruction.operation == &CPU::RTS)
			op = "RTS";
		else if(currentInstruction.operation == &CPU::SBC)
			op = "SBC";
		else if(currentInstruction.operation == &CPU::SEC)
			op = "SEC";
		else if(currentInstruction.operation == &CPU::SED)
			op = "SED";
		else if(currentInstruction.operation == &CPU::SEI)
			op = "SEI";
		else if(currentInstruction.operation == &CPU::STA)
			op = "STA";
		else if(currentInstruction.operation == &CPU::STX)
			op = "STX";
		else if(currentInstruction.operation == &CPU::STY)
			op = "STY";
		else if(currentInstruction.operation == &CPU::TAX)
			op = "TAX";
		else if(currentInstruction.operation == &CPU::TAY)
			op = "TAY";
		else if(currentInstruction.operation == &CPU::TSX)
			op = "TSX";
		else if(currentInstruction.operation == &CPU::TXA)
			op = "TXA";
		else if(currentInstruction.operation == &CPU::TXS)
			op = "TXS";
		else if(currentInstruction.operation == &CPU::TYA)
			op = "TYA";
		fprintf(logFile,"PC:%04X  A:%02X  X:%02X  Y:%02X  %d%d%d%d%d%d%d%d  SP:%02X OP:%-3s $:%04X\n",programCounter,A,X,Y,STATUS.CARRY,STATUS.ZERO,STATUS.INTERRUPT,STATUS.DECIMAL,STATUS.BREAK,STATUS.UNUSED,STATUS.OVERFLOW,STATUS.NEGATIVE,SP,op.c_str(),source);
}



#endif