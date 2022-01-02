#ifndef WAVES_H
#define WAVES_H

#include "../Utils/handler.h"


struct ENVELOPE
{
    BYTE divider;
    BYTE volume;
    BYTE decay;
    FLAG constantVolume;
    FLAG loop;
    FLAG start;

    void tick()
    {
        if(start)
        {
            start = false;
            decay = 0x0F;
            divider = volume;
            return;
        }
        if(divider-- == 0xFF)
        {
            divider = volume;
            decay = ((--decay == 0 && loop) ? 0X0F : decay);
        }
    }
};

struct LENGTH_COUNTER
{
    BYTE length;
    FLAG halt, enabled;

    void tick()
    {
        if(length > 0 && !halt)
            length--;
    }
    static constexpr BYTE LookUpTable[32] = { 10, 254, 20, 2, 40, 4, 80, 6, 160, 8, 60, 10, 14, 12, 26, 14, 12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30 };
};

/* CHANNELS */
struct PulseWave
{   
    BYTE channel;
    AUDIO timer;
    AUDIO timerStart;
    void timerTick()
    {
        if(--timer == 0xFFFF)
        {
            timer = timerStart;
            sequencerTick();
        }
    }
    
    LENGTH_COUNTER lengthCounter;

    ENVELOPE envelope;

    FLAG sweeperEnabled;
    BYTE sweeperDivider;
    BYTE sweeperReloadTime;
    BYTE shiftCount;
    AUDIO targetPeriod;
    AUDIO change;
    FLAG sweeperNegative;
    FLAG sweeperReload;
    FLAG sweeperMuted;

    void sweeperUpdate()
    {   
        change = (sweeperNegative ? -(timerStart >> shiftCount) :  (timerStart >> shiftCount));
        if(sweeperNegative && channel == 1)
            change -= 1;
        targetPeriod = timerStart + change;
    }

    void sweeperTick()
    {
        if(targetPeriod > 0x07FF || timerStart < 8)
            sweeperEnabled = false;
        if(sweeperEnabled && !sweeperDivider)
            timerStart = targetPeriod;
        if(sweeperDivider-- == 0 || sweeperReload)
        {
            sweeperDivider = sweeperReloadTime;
            sweeperReload = false;
        }

    }

    BYTE sequencer;
    BYTE sequence;
    double dutyCycle;

    void sequencerTick()
    {
        sequencer = sequencer >> 1 | (sequencer & 0x01) << 7;
    }


    BYTE output()
    {
        if(lengthCounter.length > 0 && timerStart > 8)
            return ((sequencer & 0x80) >> 7) * (envelope.constantVolume ? envelope.volume : envelope.decay);
        return 0x00;
    }    
};



struct TriangleWave
{
    AUDIO timer;
    AUDIO timerStart;

    void tick()
    {
        if(--timer == 0xFFFF)
        {
            timer = timerStart;
            if(counter > 0 && lengthCounter.length > 0)
                sequencerTick();
        }
    }
    LENGTH_COUNTER lengthCounter;
    
    BYTE counter;
    BYTE counterStart;
    FLAG counterReload;
    FLAG counterControl;

    void counterTick()
    {
        if(counterReload)
            counter = counterStart;
        else if(counter > 0)
            counter--;
        
        if(!counterControl)
            counterReload = false;
    }

    BYTE sequencerTimer;
    
    void sequencerTick()
    {
        if(++sequencerTimer == 32)
        sequencerTimer = 0;
    }

    BYTE output()
    {
        if(timerStart > 2)
            return (sequencerTimer <= 15) ? (15 - sequencerTimer) : (sequencerTimer - 16);
        return 0x00;
    }

};


struct Noise
{
    AUDIO timer;
    AUDIO timerStart;
    static constexpr AUDIO timerLookUpTable[] = {4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068}; // WARNING: This table is NTSC standard only!
    

    void tick()
    {
        if(--timer == 0xFFFF)
        {
            timer = timerStart;
            shiftTick();
        }
    }

    LENGTH_COUNTER lengthCounter;

    ENVELOPE envelope;

    BYTE shiftRegister;
    FLAG fastShift;

    void shiftTick()
    {
        shiftRegister >>= 1;
        
        BYTE feed = shiftRegister & 0x01 ^ (shiftRegister & 0x02) >> (fastShift ? 6 : 1);

        if(feed)
            SET_BIT(shiftRegister,14);
    }
    
    BYTE output()
    {
        if(lengthCounter.length > 0)
            return (shiftRegister & 0x01) * (envelope.constantVolume ? envelope.volume : envelope.decay);
        return 0x00;
    }    
};



#endif