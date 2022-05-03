#ifndef STRUCTS_H
#define STRUCTS_H

#include "typedefs.h"
#include "../APU/AudioWaves.h"

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

    // CPU Instruction
    struct INSTRUCTION 
    {
        OPEXEC_PTR operation;
        ADDRESSING_MODE addr;
    };



    struct PIXEL_RGB // Pixel struct for display
    {
        BYTE r,g,b;
    };

}


#endif 