#ifndef APU_H
#define APU_H

#include "../Utils/handler.h"
#include "AudioWaves.h"
#include "../Utils/AudioFilters.h"

namespace nesemulator
{
    class APU
    {
        public:
            APU();
            void writeToMemory(ADDRESS address,BYTE value);
            void tick();
            float output();
        private:
            void halfTick();
            void quarterTick();
            PulseWave pulse1 = PulseWave(1);
            PulseWave pulse2 = PulseWave(0);
            TriangleWave triangle;
            NoiseWave noise;
            DMC dmc;
            float pulseTable[31];
            float tndTable[203];
            TIMER clock;
            FLAG incomingFrameCounterReset;
            BYTE countToFrameCounterReset;
            AUDIOINT frameCounter = 0;
            enum class FRAMECOUNTERMODE { MODE4,MODE5} frameCounterMode;
            HighPassFilter high1;
            HighPassFilter high2;
            LowPassFilter low;
            friend class NES;
    };
}



#endif