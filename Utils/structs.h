#ifndef STRUCTS_H
#define STRUCTS_H


namespace nesemulator
{
    
    struct FileHeader  // iNES File Header
    {
        char name[4]; // Validator for iNES files and versions
        BYTE PRGROMChunks;
        BYTE CHRROMChunks;
        BYTE mapper1;
        BYTE mapper2;
        BYTE PRGRAMSize;
        BYTE TVSystem1;
        BYTE TVSystem2;
        BYTE unused[5];
    };



    // Struct to store cartridge data for states
    struct CartridgeState
    {
        size_t PRGROMSize;
        size_t CHRROMSize;
        std::unique_ptr<BYTE[]> PRGROM;
        std::unique_ptr<BYTE[]> CHRROM;
        CartridgeState(const size_t& PRGROMSize, const size_t& CHRROMSize) : PRGROMSize(PRGROMSize),CHRROMSize(CHRROMSize) 
        {
            PRGROM = std::unique_ptr<BYTE[]>(new BYTE[PRGROMSize]);
            CHRROM = std::unique_ptr<BYTE[]>(new BYTE[CHRROMSize]);
        }
    };

    struct INSTRUCTION 
    {
        OPEXEC_PTR operation;
        ADDRESSING_MODE addr;
    };

    // Struct to store CPU state
    struct CPUState
    {
        BYTE A;
        BYTE X;
        BYTE Y;
        BYTE SP;
        BYTE STATUS;
        TIMER clock;
        ADDRESS programCounter;
        BYTE cycleRemaining;
        BYTE additionalCycle0,additionalCycle1; 
        BYTE currentOpCode;
        INSTRUCTION currentInstruction;
    };


    struct StateInfo
    {
        std::string fileName;
        short mapperID;
        TIMER playTime;
        CartridgeState cartridge;
        CPUState cpu;
    };


    struct PIXEL_RGB // Pixel struct for display
    {
        BYTE r,g,b;
    };

}


#endif 