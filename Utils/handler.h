#ifndef HANDLER_H
#define HANDLER_H

#define CARRY_BIT 0
#define ZERO_BIT 1
#define INTERRUPT_DISABLE_BIT 2
#define DECIMAL_MODE_BIT 3
#define BREAK_BIT 5
#define OVERFLOW_BIT 6
#define NEGATIVE_BIT 7



#define DISPLAY_WIDTH 1024
#define DISPLAY_HEIGHT 960
#define RENDER_WIDTH 256
#define RENDER_HEIGHT 240


#include <iostream>

using std::string;

typedef uint8_t BYTE;
typedef uint16_t ADDRESS;
typedef uint32_t PIXEL;
typedef uint8_t FLAG;

struct FileHeader  // iNES File Header
{
    char name[4];
    BYTE PRGROMChunks;
    BYTE CHRROMChunks;
    BYTE mapper1;
    BYTE mapper2;
    BYTE PRGRAMSize;
    BYTE TVSystem1;
    BYTE TVSystem2;
    BYTE unused[5];
};


struct PIXEL_RGB
{
    BYTE r,g,b;
};

enum class BUTTONS {
    UP,DOWN,LEFT,RIGHT,START,SELECT,A,B,COUNT
};




#endif