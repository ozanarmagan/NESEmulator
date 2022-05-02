#ifndef CONSTANTS_H
#define CONSTANTS_H

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
}

#endif 

