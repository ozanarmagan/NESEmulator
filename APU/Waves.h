#ifndef WAVES_H
#define WAVES_H

#include "../Utils/handler.h"


struct ENVELOPE
{
    BYTE divider;
    BYTE dividerPeriod;
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
            divider = dividerPeriod;
            return;
        }
        if(divider == 0)
        {
            divider = dividerPeriod;
            if(decay > 0)
                decay--;
            else if(loop)
                decay = 0x0F;
        }
        else
            divider--;
    }

    BYTE output()
    {
        return constantVolume ? volume : decay;
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
    FLAG enabled;
    AUDIO timer;
    AUDIO timerPeriod;
    void timerTick()
    {
        if(--timer == 0xFFFF)
        {
            timer = timerPeriod;
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
        change = timerPeriod >> shiftCount;
        if(sweeperNegative)
        {
            change = -change;
            if(channel == 1)
                change -= 1;
        }
        targetPeriod = timerPeriod + change;
    }

    void sweeperTick()
    {
        if(targetPeriod > 0x07FF)
            sweeperMuted = true;
        else
            sweeperMuted = false;
        if(sweeperEnabled && !sweeperDivider && !sweeperMuted)
            timerPeriod = targetPeriod;
        if(sweeperDivider == 0 || sweeperReload)
        {
            sweeperDivider = sweeperReloadTime + 1;
            sweeperReload = false;
        }
        else
            sweeperDivider--;

    }


    BYTE sequence;
    double dutyCycle;
    BYTE sequencerClock = 0;
    void sequencerTick()
    {
        sequencerClock++;
        sequencerClock = sequencerClock % 8;
    }


    BYTE output()
    {
        if(enabled && lengthCounter.length > 0 && timerPeriod > 8 && !sweeperMuted && timerPeriod < 0x7FF)
            return CHECK_BIT(sequence,sequencerClock) * envelope.output();
        return 0x00;
    }    
};



struct TriangleWave
{
    AUDIO timer;
    AUDIO timerPeriod;
    FLAG enabled;
    void tick()
    {
        if(--timer == 0xFFFF)
        {
            timer = timerPeriod;
            if(counter > 0 && lengthCounter.length > 0)
                sequencerTick();
        }
    }
    LENGTH_COUNTER lengthCounter;
    static constexpr BYTE sequenceTable [] = {15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15};
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
        if(counter > 0 && lengthCounter.length > 0)
            if(++sequencerTimer == 32)
                sequencerTimer = 0;
    }

    BYTE output()
    {
        if(enabled &&  lengthCounter.length > 0 && counter > 0)
            return sequenceTable[sequencerTimer];
        return 0x00;
    }

};


struct Noise
{
    AUDIO timer;
    AUDIO timerPeriod;
    static constexpr AUDIO timerLookUpTable[] = {4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068}; // WARNING: This table is NTSC standard only!
    FLAG enabled;

    void tick()
    {
        if(--timer == 0xFFFF)
        {
            timer = timerPeriod;
            shiftTick();
        }
    }

    LENGTH_COUNTER lengthCounter;

    ENVELOPE envelope;

    AUDIO shiftRegister = 0x0001;
    FLAG fastShift;

    void shiftTick()
    {
        BYTE feed;
        if(fastShift)
            feed = CHECK_BIT(shiftRegister,0) ^ CHECK_BIT(shiftRegister,6);
        else
            feed = CHECK_BIT(shiftRegister,0) ^ CHECK_BIT(shiftRegister,1);
        if(feed)
            shiftRegister |=  1 << 14;
    }
    
    BYTE output()
    {
        if(enabled && lengthCounter.length > 0 && (shiftRegister & 0x0001 == 0))
            return envelope.output();
        return 0x00;
    }    
};



#endif