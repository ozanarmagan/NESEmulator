#ifndef APU_H
#define APU_h

#include <cmath>

#include "../Utils/handler.h"
#include "Waves.h"






class APU
{
    public:
        void writeToMemory(ADDRESS address,BYTE value);
        AUDIO output();
        void tick();
    private:
        TIMER clock = 0;
        double frameClock = 0.0;
        FLAG fiveStepMode = false;
        FLAG halfFrame = false; // check if next quarter is half frame
        FLAG pulseTimer = false; // pulse channel timers tick every other CPU cycle
        PulseWave channel1,channel2;
        TriangleWave channel3;
        Noise channel4;
        void quarterTick(); // Tick  in every quarter
        void halfTick(); // Tick in every half
        
};



#endif