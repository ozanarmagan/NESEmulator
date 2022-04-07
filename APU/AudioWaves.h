#ifndef WAVES_H
#define WAVES_H

#include "../Utils/handler.h"

struct ENVELOPE
{
    FLAG start; 
    FLAG loop;
    FLAG constantVolume;
    BYTE divider;
    BYTE volume;
    BYTE decay;

    void tick()
    {
        if(start)
        {
            start = false;
            decay = 15;
            divider = volume;
        }
        else
        {
            if(divider == 0)
            {
                divider = volume;
                if(decay == 0)
                {
                    if(loop)
                        decay = 15;
                }
                else
                    decay--;
            }
            else
                divider--;
        }
    }

    AUDIOINT output()
    {   
        return constantVolume ? volume : decay;
    }
};


struct SWEEPER  
{
    SWEEPER(AUDIOINT& timerRef,FLAG& pulse1,AUDIOINT& period) : timer(timerRef),pulse1(pulse1),period(period) { }; 
    FLAG& pulse1;
    FLAG enabled;
    FLAG negative;
    FLAG reload;
    FLAG silenceChannel;
    BYTE dividerPeriod;
    AUDIOINT targetPeriod;
    BYTE shiftCount;
    BYTE divider;
    AUDIOINT& period;
    AUDIOINT& timer;

    void calculateTargetPeriod()
    {
        AUDIOINT change = period >> shiftCount;
        if(negative)
        {
            change = -change;
            if(pulse1)
                change -= 1;
        }

        targetPeriod = period + change;

        silenceChannel = (period < 8 || targetPeriod > 0x7FF) ? true : false;
    }

    void tick()
    {
        calculateTargetPeriod();
        if(divider == 0 && enabled && !silenceChannel && shiftCount != 0)
        {
            period = targetPeriod;
            
        }
        if(divider == 0 || reload)
        {
            divider = dividerPeriod;
            reload = false;
        }
        else
            divider--;
    }
};


struct LENGTHCOUNTER
{
    FLAG enabled;
    FLAG halt;
    BYTE value;
    

    static constexpr BYTE lengthTable[32] = {10, 254, 20,  2, 40,  4, 80,  6, 160,  8, 60, 10, 14, 12, 26, 14, 12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30};
    void disable()
    {
        enabled = false;
        value = 0;
    }

    void enable()
    {
        enabled = true;
    }
    
    void tick()
    {
        if(value > 0 && !halt)
            value--;
    }
};


struct PulseWave
{
    PulseWave(FLAG isPulse1) : pulse1(isPulse1) {   };
    FLAG pulse1;
    FLAG enabled;
    AUDIOINT timer;
    AUDIOINT period;
    
    void tick()
    {
        if(timer == 0)
        {
            timer = period;
            sequencerTick();
        }
        else
            timer--;
    }

    ENVELOPE envelope;
    SWEEPER sweeper = SWEEPER(timer,pulse1,period);
    LENGTHCOUNTER lengthCounter;

    static constexpr BYTE dutySequenceTable[4][8] = {
                                 {0, 1, 0, 0, 0, 0, 0, 0},
                                 {0, 1, 1, 0, 0, 0, 0, 0},
                                 {0, 1, 1, 1, 1, 0, 0, 0},
                                 {1, 0, 0, 1, 1, 1, 1, 1}
                                 };
    BYTE sequencer = 0;
    BYTE sequencerMode;
    void sequencerTick()
    {
        sequencer++;
        sequencer = sequencer % 8;
    }

    AUDIOINT output()
    {
        sweeper.calculateTargetPeriod();
        if(dutySequenceTable[sequencerMode][sequencer] == 0 || sweeper.silenceChannel || lengthCounter.value == 0 || enabled == false)
            return 0x00;
        else
            return envelope.output();
    }
};


struct TriangleWave
{
    FLAG enabled;
    AUDIOINT timer;
    AUDIOINT period;

    void tick()
    {
        if(timer == 0)
        {
            timer = period;
            sequencerTick();
        }
        else
            timer--;
    }


    LENGTHCOUNTER lengthCounter;

    FLAG linearCounterReload;
    FLAG linearCounterControl;
    BYTE linearCounter;
    BYTE linerCounterReloadValue;

    void linearCounterTick()
    {
        if(linearCounterReload)
            linearCounter = linerCounterReloadValue;
        else if(linearCounter > 0)
            linearCounter--;
        
        if(!linearCounterControl)
            linearCounterReload = false;
    }

    static constexpr BYTE sequenceTable[32] = {15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15};

    BYTE sequencerIndex;

    void sequencerTick()
    {
        if(lengthCounter.value > 0 && linearCounter > 0)
        {
            sequencerIndex++;
            sequencerIndex = sequencerIndex % 32;
        }
    }   

    AUDIOINT output()
    {
        // if(linearCounter == 0 || lengthCounter.value == 0 || enabled == false)
        //     return 0x00;
        // else
            return sequenceTable[sequencerIndex];
    }
};


struct NoiseWave
{

    enum class NOISEMODE{ MODE1,MODE6};
    NOISEMODE mode;
    FLAG enabled;
    AUDIOINT timer;
    AUDIOINT period;

    void tick()
    {
        if(timer == 0)
        {
            timer = period;
            shifterTick();
        }
        else
            timer--;
    }

    static constexpr AUDIOINT noiseTable[16] = {4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068}; 

    ENVELOPE envelope;

    LENGTHCOUNTER lengthCounter;

    AUDIOINT shifter = 0x0001;

    void shifterTick()
    {
        BYTE feedBack = (shifter & 0x1) ^ (shifter >> ((mode == NOISEMODE::MODE6) ? 6 : 1) & 0x1);

        shifter >>= 1;

        if(feedBack)
            shifter |= feedBack << 14;
    }

    AUDIOINT output()
    {
        if(!(shifter & 0x0001) || lengthCounter.value == 0 || enabled == false)
            return 0x00;
        else
            return envelope.output();
    }
};

struct DMC
{
    FLAG enabled;
    FLAG IRQEnabled;
    FLAG IRQ;
    FLAG loop;
    FLAG bufferEmpty;
    FLAG silenceChannel;
    FLAG stall;
    ADDRESS sampleAddress;
    ADDRESS currentAddresss;
    AUDIOINT period;
    AUDIOINT timer;
    AUDIOINT sampleLength;
    AUDIOINT bytesRemaining;
    BYTE outputLoad;

    static constexpr AUDIOINT periodTable[] = {428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106, 84, 72, 54};

    void tick()
    {
        if(timer == 0)
        {
            timer = period - 1;
        }
        else
            timer--;
    }

    AUDIOINT output()
    {
        return outputLoad;
    }
    
};


#endif