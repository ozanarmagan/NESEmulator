#include "APU.h"

constexpr BYTE LENGTH_COUNTER::LookUpTable[];
constexpr AUDIO Noise::timerLookUpTable[];


APU::APU()
{
    channel1.channel = 1;
    channel2.channel = 2;
    // Fill output look-up tables
    for (int i = 0; i < 31; i++) {
        pulseTable[i] = 95.52 / (8128.0/i + 100);
    }
    for (int j = 0; j < 203; j++) {
        tndTable[j] = 163.67 / (24329.0/j + 100);
    }
}


float APU::output()
{
	return pulseTable[channel1.output() + channel2.output()] + tndTable[3 * channel3.output() + 2 * channel4.output()];
}

void APU::quarterTick()
{
    channel1.envelope.tick();
    channel2.envelope.tick();
    channel3.counterTick();
    channel4.envelope.tick();
}

void APU::halfTick()
{
    quarterTick(); // Every half frame is also a quarter frame
    
    channel1.lengthCounter.tick();
    channel2.lengthCounter.tick();
    channel3.lengthCounter.tick();
    channel4.lengthCounter.tick();

    channel1.sweeperTick();
    channel2.sweeperTick();
}

void APU::tick()
{
    channel1.sweeperUpdate();
    channel2.sweeperUpdate();

    if(clock % 3 == 0)
    {
        frameClock += 0.5;
        channel3.tick();
        channel4.tick();


        if(frameClock == 3728.5 || frameClock == 11185.5)
            quarterTick();
        if(frameClock == 7456.5 || (fiveStepMode == false && frameClock == 14914.5) || frameClock == 18640.5)
            halfTick();


        if(frameClock == (fiveStepMode ?  18641 : 14915))
            frameClock = 0.0;
        
        if(pulseTimer)
        {
            channel1.timerTick();
            channel2.timerTick();
        }

        pulseTimer = !pulseTimer;
    }

    clock++;
}


void APU::writeToMemory(ADDRESS address,BYTE value)
{
    switch (address)
    {
        case 0x000:
                channel1.envelope.volume = value & 0x0F;
                channel1.envelope.constantVolume = (value & 0x10) ? true : false;
                channel1.lengthCounter.halt = (value & 0x20) ? true : false;
                channel1.envelope.loop = (value & 0x20) ? true : false;

                switch ((value & 0xC0) >> 6)
                {
                case 0:
                    channel1.sequence = 0x40;
                    channel1.dutyCycle = 0.125;
                    break;
                case 1:
                    channel1.sequence = 0x60;
                    channel1.dutyCycle = 0.250;
                    break;
                case 2:
                    channel1.sequence = 0x78;
                    channel1.dutyCycle = 0.500;
                    break;
                case 3:
                    channel1.sequence = 0x9F;
                    channel1.dutyCycle = 0.750;
                    break;
                }
                break;
        
        case 0x0001:
            channel1.shiftCount = value & 0x07;
            channel1.sweeperNegative = (value & 0x08) ? true : false;
            channel1.sweeperReload = true;
            channel1.sweeperEnabled = (value & 0x80) ? true : false;
            channel1.sweeperReloadTime = (value & 0x70) >> 4;
            break;
        case 0x0002:
            channel1.timerStart = 0xFF00 | value;
            break;
        case 0x0003:
            channel1.timerStart = (channel1.timerStart & 0x00FF) | (AUDIO)((value & 0x07) << 8);
            if(channel1.lengthCounter.enabled) channel1.lengthCounter.length =  LENGTH_COUNTER::LookUpTable[(value & 0xF8) >> 3];
            channel1.sequencer = channel1.sequence;
            channel1.envelope.start = true;
            break;
        case 0x0004:
                channel2.envelope.volume = value & 0x0F;
                channel2.envelope.constantVolume = (value & 0x10) ? true : false;
                channel2.lengthCounter.halt = (value & 0x20) ? true : false;
                channel2.envelope.loop = (value & 0x20) ? true : false;

                switch ((value & 0xC0) >> 6)
                {
                case 0:
                    channel2.sequence = 0x40;
                    channel2.dutyCycle = 0.125;
                    break;
                case 1:
                    channel2.sequence = 0x60;
                    channel2.dutyCycle = 0.250;
                    break;
                case 2:
                    channel2.sequence = 0x78;
                    channel2.dutyCycle = 0.500;
                    break;
                case 3:
                    channel2.sequence = 0x9F;
                    channel2.dutyCycle = 0.750;
                    break;
                }
                break;
        case 0x0005:
            channel2.shiftCount = value & 0x07;
            channel2.sweeperNegative = (value & 0x08) ? true : false;
            channel2.sweeperReload = true;
            channel2.sweeperEnabled = (value & 0x80) ? true : false;
            channel2.sweeperReloadTime = (value & 0x70) >> 4;
            break;
        case 0x0006:
            channel2.timerStart = 0xFF00 | value;
            break;
        case 0x0007:
            channel2.timerStart = (channel2.timerStart & 0x00FF) | (AUDIO)((value & 0x07) << 8);
            if(channel2.lengthCounter.enabled) channel2.lengthCounter.length =  LENGTH_COUNTER::LookUpTable[(value & 0xF8) >> 3];
            channel2.sequencer = channel2.sequence;
            channel2.envelope.start = true;
            break;
        case 0x0008:
            channel3.counterStart = value & 0x0F;
            channel3.counterControl = (value & 0x80) ? true : false;
            channel3.lengthCounter.halt = (value & 0x80) ? true : false;
            break;
        case 0x000A:
            channel3.timerStart = 0xFF00 | value;
            break;
        case 0x000B:
            channel3.timerStart = (channel3.timerStart & 0x00FF) | (AUDIO)((value & 0x07) << 8);
            if(channel3.lengthCounter.enabled) channel3.lengthCounter.length = LENGTH_COUNTER::LookUpTable[(value & 0xF8) >> 3];
            channel3.counterReload = true;
            break;
        case 0x000C:
            channel4.lengthCounter.halt = (value & 0x10) ? true : false;
            channel4.envelope.constantVolume = (value & 0x08) ? true : false;
            channel4.envelope.divider = value & 0x0F;
            break;
        case 0x000E:
            channel4.fastShift = (value & 0x80) ? true : false;
            channel4.timerStart = Noise::timerLookUpTable[value & 0x0F];
            break;
        case 0x000F:
            if(channel4.lengthCounter.enabled) channel4.lengthCounter.length = LENGTH_COUNTER::LookUpTable[(value & 0xF8) >> 3];
            channel4.envelope.start = true;
            break;
        case 0x0015:
            channel1.lengthCounter.enabled = (value & 0x01) ? true : false;
            channel2.lengthCounter.enabled = (value & 0x02) ? true : false;
            channel3.lengthCounter.enabled = (value & 0x04) ? true : false;
            channel4.lengthCounter.enabled = (value & 0x08) ? true : false;

            channel1.lengthCounter.length  = (value & 0x01) ? channel1.lengthCounter.length : 0x00;
            channel2.lengthCounter.length  = (value & 0x02) ? channel2.lengthCounter.length : 0x00;
            channel3.lengthCounter.length  = (value & 0x04) ? channel3.lengthCounter.length : 0x00;
            channel4.lengthCounter.length  = (value & 0x08) ? channel4.lengthCounter.length : 0x00;
            break;
        case 0x0017:
            fiveStepMode = (value & 0x80) ? true : false;
            break;
        default:
            break;
    }
}