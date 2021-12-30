#ifndef HANDLER_H
#define HANDLER_H


#define RUN_FOREVER while(1)

#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 600
#define RENDER_WIDTH 256
#define RENDER_HEIGHT 240
#define DEBUG_WIDTH 270
#define DEBUG_HEIGHT 270


#define SET_BIT(byte,bit)   ((byte) |=  (1<<(bit)))
#define CLEAR_BIT(byte,bit) ((byte) &= ~((1)<<(bit)))
#define CHECK_BIT(byte,bit) ((byte) &   (1<<(bit)))
#include <iostream>

using std::string;

typedef uint8_t BYTE;
typedef uint16_t ADDRESS;
typedef uint32_t PIXEL;
typedef uint64_t TIME;
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


enum class MIRRORING {
    HORIZONTAL,VERTICAL,FOURSCREENS,ONSCREENLOWER,ONSCREENHIGHER
};

struct OBJECT_ATTRIBUTE
{
    BYTE y,id,attribute,x;
};


void reverseByte(BYTE* byte);


#endif