#ifndef HANDLER_H
#define HANDLER_H


#define RUN_FOREVER while(1)

#define DISPLAY_WIDTH 1024
#define DISPLAY_HEIGHT 960
#define RENDER_WIDTH 256
#define RENDER_HEIGHT 240
#define DEBUG_WIDTH 270
#define DEBUG_HEIGHT 270



#define CYCLES_PER_SAMPLE 
#define CYCLES_PER_BUFFER CYCLES_PER_SAMPLE * 512


#define SET_BIT(byte,bit)   ((byte) |=  (1<<(bit)))
#define CLEAR_BIT(byte,bit) ((byte) &= ~((1)<<(bit)))
#define CHECK_BIT(byte,bit) ((byte) &   (1<<(bit)))
#include <iostream>

using std::string;

typedef uint8_t BYTE;
typedef uint16_t ADDRESS;
typedef uint16_t AUDIO;
typedef uint32_t PIXEL;
typedef uint32_t ADDRESS32;
typedef uint64_t TIMER;
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

enum class MIRRORING {
    HORIZONTAL,VERTICAL,FOURSCREENS,ONSCREENLOWER,ONSCREENHIGHER
};

struct OBJECT_ATTRIBUTE
{
    BYTE y,id,attribute,x;
};


void reverseByte(BYTE* byte);


#endif