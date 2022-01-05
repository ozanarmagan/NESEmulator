#include "APU.h"



APU::APU() : high1(90),high2(440),low(14000)
{
    for(int i = 0;i < 31;i++)
        pulseTable[i] =  95.52 / (8128.0 / i + 100);
    for(int i = 0;i < 203;i++)
        tndTable[i]   =  163.67 / (24329.0 / i + 100);
}


void APU::quarterTick()
{
    pulse1.envelope.tick();
    pulse2.envelope.tick();
    triangle.linearCounterTick();
    noise.envelope.tick();
}

void APU::halfTick()
{
    quarterTick();

    pulse1.lengthCounter.tick();
    pulse2.lengthCounter.tick();
    triangle.lengthCounter.tick();
    noise.lengthCounter.tick();

    pulse1.sweeper.tick();
    pulse2.sweeper.tick();
}

void APU::tick()
{
    pulse1.sweeper.calculateTargetPeriod();
    pulse2.sweeper.calculateTargetPeriod();
    if(incomingFrameCounterReset)
    {
        if(countToFrameCounterReset == 0)
        {
            incomingFrameCounterReset = false;
            frameCounter = 0;
        }
        else
            countToFrameCounterReset--;
    }

    triangle.tick();

    if(clock % 2 == 0) // APU clocks in every-other-CPU-cycle
    {

        frameCounter++;

        if(frameCounter == 3729 || frameCounter == 11186)
            quarterTick();
        if((frameCounterMode == FRAMECOUNTERMODE::MODE5 && frameCounter == 18641) || (frameCounterMode == FRAMECOUNTERMODE::MODE4 && frameCounter == 14915) || frameCounter == 7457)
            halfTick();
        if((frameCounterMode == FRAMECOUNTERMODE::MODE5 && frameCounter == 18641) || (frameCounterMode == FRAMECOUNTERMODE::MODE4 && frameCounter == 14915))
            frameCounter = 0;
        
        pulse1.tick();
        pulse2.tick();
        noise.tick();
    }

    clock++;
}



void APU::writeToMemory(ADDRESS address,BYTE value)
{
    switch (address)
    {
    case 0x4000:
        pulse1.envelope.volume = value & 0x0F;
        pulse1.envelope.constantVolume = (value & 0x10) ? true : false;
        pulse1.lengthCounter.halt = (value & 0x20) ? true : false;
        pulse1.envelope.loop = (value & 0x20) ? true : false;
        pulse1.sequencerMode = (value & 0xC0) >> 6;
        break;
    case 0x4001:
        pulse1.sweeper.enabled = (value & 0x80) ? true : false;
        pulse1.sweeper.dividerPeriod = (value & 0x70) >> 4;
        pulse1.sweeper.calculateTargetPeriod();
        pulse1.sweeper.negative = (value & 0x08) ? true : false;
        pulse1.sweeper.shiftCount = value & 0x07;
        pulse1.sweeper.reload = true;
        break;
    case 0x4002:
        pulse1.period = (pulse1.period & 0xFF00) | value;
        pulse1.timer = pulse1.period;
        break;
    case 0x4003:
        pulse1.lengthCounter.value = LENGTHCOUNTER::lengthTable[(value & 0xF8) >> 3];
        pulse1.period |= (AUDIO)((value & 0x07)) << 8;
        pulse1.timer = pulse1.period;
        pulse1.dutyCycle = 0;
        pulse1.envelope.start = true;
        break;   
    case 0x4004:
        pulse2.envelope.volume = value & 0x0F;
        pulse2.envelope.constantVolume = (value & 0x10) ? true : false;
        pulse2.lengthCounter.halt = (value & 0x20) ? true : false;
        pulse2.envelope.loop = (value & 0x20) ? true : false;
        pulse2.sequencerMode = (value & 0xC0) >> 6;
        break;
    case 0x4005:
        pulse2.sweeper.enabled = (value & 0x80) ? true : false;
        pulse2.sweeper.dividerPeriod = (value & 0x70) >> 4;
        pulse2.sweeper.calculateTargetPeriod();
        pulse2.sweeper.negative = (value & 0x08) ? true : false;
        pulse2.sweeper.shiftCount = value & 0x07;
        pulse2.sweeper.reload = true;
        break;
    case 0x4006:
        pulse2.period = (pulse2.period & 0xFF00) | value;
        pulse2.timer = pulse2.period;
        break;
    case 0x4007:
        pulse2.lengthCounter.value = LENGTHCOUNTER::lengthTable[(value & 0xF8) >> 3];
        pulse2.period |= (AUDIO)((value & 0x07)) << 8;
        pulse2.timer = pulse2.period;
        pulse2.dutyCycle = 0;
        pulse2.envelope.start = true;
        break; 
    case 0x4008:
        triangle.lengthCounter.halt = (value & 0x80) ? true : false;
        triangle.linearCounterControl = (value & 0x80) ? true : false;
        triangle.linerCounterReloadValue = value & 0x7F;
        break;
    case 0x400A:
        triangle.period = (triangle.period & 0xFF00) | value;
        triangle.timer = triangle.period;
        break;
    case 0x400B:
        triangle.lengthCounter.value = LENGTHCOUNTER::lengthTable[(value & 0xF8) >> 3]; 
        triangle.period |= (AUDIO)((value & 0x07)) << 8;
        triangle.timer = triangle.period;
        triangle.linearCounterReload = true;
        break;
    case 0x400C:
        noise.lengthCounter.halt = (value & 0x20) ? true : false;
        noise.envelope.constantVolume = (value & 0x10) ? true : false;
        noise.envelope.volume = value & 0x0F;
        break;
    case 0x400E:
        noise.mode = (value & 0x80) ? NoiseWave::NOISEMODE::MODE6 : NoiseWave::NOISEMODE::MODE1;
        noise.period = NoiseWave::noiseTable[value & 0x0F];
        break;
    case 0x400F:
        noise.lengthCounter.value = LENGTHCOUNTER::lengthTable[(value & 0xF8) >> 3];
        noise.envelope.start = true;
        break;
    case 0x4015:
        if((value & 0x01) == 0)
            pulse1.lengthCounter.disable();
        else
            pulse1.lengthCounter.enable();

        if((value & 0x02) == 0)
            pulse2.lengthCounter.disable();
        else
            pulse2.lengthCounter.enable();

        if((value & 0x04) == 0)
            triangle.lengthCounter.disable();
        else
            triangle.lengthCounter.enable();

        if((value & 0x08) == 0)
            noise.lengthCounter.disable();
        else
            noise.lengthCounter.enable();
        
        break;
    case 0x4017:
        frameCounterMode = (value & 0x80) ? FRAMECOUNTERMODE::MODE5 : FRAMECOUNTERMODE::MODE4;
        incomingFrameCounterReset = true;
        countToFrameCounterReset = 3 + ((clock % 6) ? 1 : 0);
        if(frameCounterMode == FRAMECOUNTERMODE::MODE5)
            halfTick();
        break;
    default:
        break;
    }
}


float APU::output()
{
    float output = pulseTable[pulse1.output() + pulse2.output()] + tndTable[3 * triangle.output() + 2 * noise.output()];
    // std::cout << "pulse 1 " << pulse1.output() << std::endl;
    // std::cout << "pulse 2 " << pulse2.output() << std::endl;
    // std::cout << "triangle " << triangle.output() << std::endl;
    // std::cout << "noise " << noise.output() << std::endl;
    // if(pulse1.output() != 0 || pulse2.output() != 0)
    // {
    //     auto a = pulse1.output();
    //     auto b = pulse2.output();
    //     std::cout << "not that " << std::endl;
    // }
    output = high1.filter(output);
    output = high2.filter(output);
    output = low.filter(output);
    return output;
}