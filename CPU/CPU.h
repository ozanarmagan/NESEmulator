#ifndef NES_CPU_H
#define NES_CPU_H

#include "../Utils/handler.h"
#include "../Bus/RAM.h"
#include "../PPU/PPU.h"

/*  

    This is a solid emulation of 6502 Proccessor which used in NES ,
    
    CPU EMULATION TYPE : Jump Table Based

    EXPLANATION :
    Each addressing mode (ADDRESSING_MODE) and operation (OPEXEC) is a function 
    and they are forming INSTRUCTION struct with cycle count for each OPCODE
    Since there is 256 OPCODE but 6502 using only 151 of them,remaining
    OPCODEs are illegal and they literally do nothing (NOP)

*/





class CPU
{
    public:
        CPU(RAM& mem,PPU& ppu); 

        void setProgramCounter(uint16_t address);

        void moveProgramCounter(uint8_t offset); // move program counter from current location

        void UpdateCurrentCycle(std::uint8_t offset); // add cycle offset

        uint16_t getProgramCounter() const; 

        uint16_t getStackPointerAddress() const; // Stack pointer address from RAM
 

        uint64_t getCycleIndex() const; // current cycle index
        
        void tick();

        //friend std::ostream operator<<(std::ostream &out,CPU &cpu); // For logging stuff

    private:
        using OPEXEC_PTR = void(CPU::*)(uint16_t);
        using OPEXEC = void;
        using ADDRESSING_MODE =  ADDRESS(CPU::*)();
        struct INSTRUCTION 
        {
            ADDRESSING_MODE addr;
            OPEXEC_PTR operation;
            uint8_t cycles;
        };

        /*----------CORE REGISTERS------------*/
        uint8_t A   = 0x00; // Accumulator
        uint8_t X   = 0x00; //
        uint8_t Y   = 0x00; // Index registers
        uint8_t SP  = 0xFF; // Stack Pointer is between 0xFF and 0x00

        FLAG CARRY = 0,OVERFLOWBIT = 0,ZERO = 0,NEGATIVE = 0,BREAK = 0,INTERRUPT_DISABLE = 0,DECIMAL = 0; // Processor flags

        static const ADDRESS IRQVECTOR_H = 0xFFFF;
        static const ADDRESS IRQVECTOR_L = 0xFFFE;
        static const ADDRESS RSTVECTOR_H = 0xFFFD;
        static const ADDRESS RSTVECTOR_L = 0xFFFC;
        static const ADDRESS NMIVECTOR_H = 0xFFFB;
        static const ADDRESS NMIVECTOR_L = 0xFFFA;

        /* PROGRAM COUNTER */
        ADDRESS programCounter = 0x0000;

        /* CYCLE INDEX */
        uint64_t currentCycle = 0x0000000000000000;

        uint8_t currentOpCode;

        INSTRUCTION currentInstruction;

        RAM memory;

        PPU ppu;
        
        void reset(); // CPU to default state

        void push(uint8_t value); // Push value to stack

        uint8_t pop(); // Pop from stack 

        INSTRUCTION table[256];

        void execute();

        /*------------------------OPERATIONS------------------------*/
        OPEXEC ADC(ADDRESS source);
        OPEXEC AND(ADDRESS source);
        OPEXEC ASL(ADDRESS source); 	OPEXEC ASL_ACC(ADDRESS source);
        OPEXEC BCC(ADDRESS source);
        OPEXEC BCS(ADDRESS source);

        OPEXEC BEQ(ADDRESS source);
        OPEXEC BIT(ADDRESS source);
        OPEXEC BMI(ADDRESS source);
        OPEXEC BNE(ADDRESS source);
        OPEXEC BPL(ADDRESS source);

        OPEXEC BRK(ADDRESS source);
        OPEXEC BVC(ADDRESS source);
        OPEXEC BVS(ADDRESS source);
        OPEXEC CLC(ADDRESS source);
        OPEXEC CLD(ADDRESS source);

        OPEXEC CLI(ADDRESS source);
        OPEXEC CLV(ADDRESS source);
        OPEXEC CMP(ADDRESS source);
        OPEXEC CPX(ADDRESS source);
        OPEXEC CPY(ADDRESS source);

        OPEXEC DEC(ADDRESS source);
        OPEXEC DEX(ADDRESS source);
        OPEXEC DEY(ADDRESS source);
        OPEXEC EOR(ADDRESS source);
        OPEXEC INC(ADDRESS source);

        OPEXEC INX(ADDRESS source);
        OPEXEC INY_OP(ADDRESS source);
        OPEXEC JMP(ADDRESS source);
        OPEXEC JSR(ADDRESS source);
        OPEXEC LDA(ADDRESS source);

        OPEXEC LDX(ADDRESS source);
        OPEXEC LDY(ADDRESS source);
        OPEXEC LSR(ADDRESS source); 	OPEXEC LSR_ACC(ADDRESS source);
        OPEXEC NOP(ADDRESS source);
        OPEXEC ORA(ADDRESS source);

        OPEXEC PHA(ADDRESS source);
        OPEXEC PHP(ADDRESS source);
        OPEXEC PLA(ADDRESS source);
        OPEXEC PLP(ADDRESS source);
        OPEXEC ROL(ADDRESS source); 	OPEXEC ROL_ACC(ADDRESS source);

        OPEXEC ROR(ADDRESS source);	OPEXEC ROR_ACC(ADDRESS source);
        OPEXEC RTI(ADDRESS source);
        OPEXEC RTS(ADDRESS source);
        OPEXEC SBC(ADDRESS source);
        OPEXEC SEC(ADDRESS source);
        OPEXEC SED(ADDRESS source);

        OPEXEC SEI(ADDRESS source);
        OPEXEC STA(ADDRESS source);
        OPEXEC STX(ADDRESS source);
        OPEXEC STY(ADDRESS source);
        OPEXEC TAX(ADDRESS source);

        OPEXEC TAY(ADDRESS source);
        OPEXEC TSX(ADDRESS source);
        OPEXEC TXA(ADDRESS source);
        OPEXEC TXS(ADDRESS source);
        OPEXEC TYA(ADDRESS source);

        OPEXEC ILLEGAL(ADDRESS source);          

        /*------------ADDRESSING MODES--------*/
        ADDRESS ACC(); // ACCUMULATOR
        ADDRESS IMM(); // IMMEDIATE
        ADDRESS ABS(); // ABSOLUTE
        ADDRESS ZER(); // ZERO PAGE
        ADDRESS ZEX(); // INDEXED-X ZERO PAGE
        ADDRESS ZEY(); // INDEXED-Y ZERO PAGE
        ADDRESS ABX(); // INDEXED-X ABSOLUTE
        ADDRESS ABY(); // INDEXED-Y ABSOLUTE
        ADDRESS IMP(); // IMPLIED
        ADDRESS REL(); // RELATIVE
        ADDRESS INX(); // INDEXED-X INDIRECT
        ADDRESS INY(); // INDEXED-Y INDIRECT
        ADDRESS ABI(); // ABSOLUTE INDIRECT


};


#endif