#ifndef HANDLER_H
#define HANDLER_H

#include <iostream>

#define RUN_FOREVER while(1)


namespace nesemulator
{
    // Display size (Render size downsampled or upsampled to this size for displaying the frame)
    static constexpr int DISPLAY_WIDTH = 1024;
    static constexpr int DISPLAY_HEIGHT = 960;
    // Actual Render size
    static constexpr int RENDER_WIDTH = 256;
    static constexpr int RENDER_HEIGHT = 240;
    // Debug frame size
    static constexpr int DEBUG_WIDTH = 270;
    static constexpr int DEBUG_HEIGHT = 270;


    static constexpr double PI = 3.14159265359;


    // Maximum state count to store 
    static constexpr int STATE_COUNT = 3;
    // Default state buffer size
    static constexpr int DEFAULT_STATE_BUFFER = 2048 * 1024;

    // Audio samples per frame
    static constexpr int SAMPLE_PER_FRAME =  8192;


    // TODO: Change this bit operation macros with conexpr static functions
    #define SET_BIT(byte,bit)   ((byte) |=  (1<<(bit)))
    #define CLEAR_BIT(byte,bit) ((byte) &= ~((1)<<(bit)))
    #define CHECK_BIT(byte,bit) ((byte) &   (1<<(bit)))

    // Type declerations
    typedef uint8_t BYTE;
    typedef uint16_t ADDRESS;
    typedef uint16_t AUDIOINT;
    typedef uint32_t PIXEL;
    typedef uint32_t ADDRESS32;
    typedef uint64_t TIMER;
    typedef uint8_t FLAG;
    typedef uint32_t HERTZ;

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

    struct StateInfo
    {
        std::string fileName;
        short mapperID;
        TIMER playTime;
    };


    struct PIXEL_RGB // Pixel struct for display
    {
        BYTE r,g,b;
    };

    enum class MIRRORING { // Mirroring mode of the cartridge
        HORIZONTAL,VERTICAL,FOURSCREENS,ONSCREENLOWER,ONSCREENHIGHER
    };

    struct OBJECT_ATTRIBUTE // PPU OAM Object Attribute
    {
        BYTE y,id,attribute,x;
    };


    void reverseByte(BYTE* byte); // Function to reverse a byte (binary)
}


#endif