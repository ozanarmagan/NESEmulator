#include "RAM.h"
#include <iostream>

RAM::RAM()
{
    //memory = new uint8_t[64 * 1024](); // Allocate 64KB memory for RAM of NES 
}

uint8_t RAM::readFromMemory(uint16_t address) const
{
    return memory[address];
}

void RAM::writeToMemory(uint16_t address,uint8_t value) 
{
    memory[address] = value;
}

void RAM::clearMemoryBlock(uint16_t start,uint16_t end)
{
    for(uint16_t i = start;i < end + 1;i++)
        memory[i] = 0x00;
}