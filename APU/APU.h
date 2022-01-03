#ifndef APU_H
#define APU_H

#include <cmath>

#include "../Utils/handler.h"
#include "Waves.h"






class APU
{
    public:
        APU();
        void writeToMemory(ADDRESS address,BYTE value);
        float output();
        void tick();
    private:
        TIMER clock = 0;
        double frameClock = 0.0;
        FLAG fiveStepMode = false;
        FLAG halfFrame = false; // check if next quarter is half frame
        FLAG pulseTimer = true; // pulse channel timers tick every other CPU cycle
        PulseWave channel1,channel2;
        TriangleWave channel3;
        Noise channel4;
        void quarterTick(); // Tick  in every quarter
        void halfTick(); // Tick in every half
        float pulseTable[31];
        float tndTable [203]; // This two table for outputs of APU,they can be calculated real time but a look-up table will reduce computation
};



#endif