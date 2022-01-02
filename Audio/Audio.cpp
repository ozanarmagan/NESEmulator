#include "Audio.h"



void Audio::init()
{

}

void Audio::staticCallback(void* userData,BYTE* stream,int len)
{
    std::cout << "callback" << std::endl;
    static_cast<Audio*>(userData)->audioCallback(stream, len);
}


void Audio::audioCallback(BYTE* stream,int len)
{
    int writeCount = 0;
    for(;writeCount < len / 2 && audioQueue->size() > 0;writeCount++)
    {
        reinterpret_cast<AUDIO*>(stream)[writeCount] = audioQueue->pop();
    }

    while(writeCount < len / 2)
        reinterpret_cast<AUDIO*>(stream)[writeCount++] = 0;
}
