#ifndef CPU_H
#define CPU_H



#include <functional>

using std::function;

#include "../Utils/handler.h"
#include "../Bus/Bus.h"

#ifdef CPUDEBUG

#include <stdio.h>
#include <thread>

#endif

class CPU
{
    public:
        CPU(Bus& bus);
        void tick();
        void reset();
        void NMI();
        void IRQ();
        friend std::ostream& operator<<(std::ostream &out,CPU &cpu);
    private:

#ifdef CPUDEBUG
        void log();
        FILE* logFile;
#endif

        /* REGISTERS */
        BYTE A = 0x00;
        BYTE X = 0x00;
        BYTE Y = 0x00;
        BYTE SP = 0xFF;
        union
        {
            struct
            {
                FLAG CARRY : 1, ZERO : 1, INTERRUPT : 1, DECIMAL : 1, BREAK : 1, UNUSED : 1, OVERFLOW : 1, NEGATIVE : 1;
            };
            BYTE combined;
        } STATUS;


        using OPEXEC = void;
        using OPEXEC_PTR = OPEXEC (CPU::*)(ADDRESS source);
        using ADDRESSING_MODE = ADDRESS(CPU::*)();
        struct INSTRUCTION 
        {
            OPEXEC_PTR operation;
            ADDRESSING_MODE addr;
        };

        INSTRUCTION table[256];

        static constexpr BYTE cycleCounts[256] = {
		7,6,2,8,3,3,5,5,3,2,2,2,4,4,6,6,
		2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
		6,6,2,8,3,3,5,5,4,2,2,2,4,4,6,6,
		2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
		6,6,2,8,3,3,5,5,3,2,2,2,3,4,6,6,
		2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
		6,6,2,8,3,3,5,5,4,2,2,2,5,4,6,6,
		2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
		2,6,2,6,3,3,3,3,2,2,2,2,4,4,4,4,
		2,6,2,6,4,4,4,4,2,5,2,5,5,5,5,5,
		2,6,2,6,3,3,3,3,2,2,2,2,4,4,4,4,
		2,5,2,5,4,4,4,4,2,4,2,4,4,4,4,4,
		2,6,2,8,3,3,5,5,2,2,2,2,4,4,6,6,
		2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
		2,6,2,8,3,3,5,5,2,2,2,2,4,4,6,6,
		2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
	};

        static const ADDRESS IRQVECTOR_H = 0xFFFF;
        static const ADDRESS IRQVECTOR_L = 0xFFFE;
        static const ADDRESS RSTVECTOR_H = 0xFFFD;
        static const ADDRESS RSTVECTOR_L = 0xFFFC;
        static const ADDRESS NMIVECTOR_H = 0xFFFB;
        static const ADDRESS NMIVECTOR_L = 0xFFFA;

        ADDRESS programCounter = 0x0000;

        /* CYCLE INDEX */
        BYTE cycleRemaining = 0;

        BYTE additionalCycle0 = 0,additionalCycle1 = 0;

        BYTE currentOpCode;

        INSTRUCTION currentInstruction;

        Bus& bus;

        int COUNTER = 0;

        ADDRESS source;

        void push(BYTE value);

        BYTE pop();
        /*------------------------OPERATIONS------------------------*/
        OPEXEC ADC(ADDRESS source);

        OPEXEC AND(ADDRESS source);

        OPEXEC ASL(ADDRESS source);
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

        OPEXEC INX_OP(ADDRESS source);
        OPEXEC INY_OP(ADDRESS source);

        OPEXEC JMP(ADDRESS source);

        OPEXEC JSR(ADDRESS source);

        OPEXEC LDA(ADDRESS source);

        OPEXEC LDX(ADDRESS source);

        OPEXEC LDY(ADDRESS source);

        OPEXEC LSR(ADDRESS source);


        OPEXEC NOP(ADDRESS source);

        OPEXEC ORA(ADDRESS source);
        OPEXEC PHA(ADDRESS source);
        OPEXEC PHP(ADDRESS source);

        OPEXEC PLA(ADDRESS source);

        OPEXEC PLP(ADDRESS source);

        OPEXEC ROL(ADDRESS source);

        OPEXEC ROR(ADDRESS source);


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

        OPEXEC TYA(ADDRESS source);
        OPEXEC TXS(ADDRESS source);
        OPEXEC ILLEGAL(ADDRESS source);
      
        /*------------ADDRESSING MODES--------*/

        ADDRESS IMM(); // IMMEDIATE
        
        ADDRESS ABS();  // ABSOLUTE

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