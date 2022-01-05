#include "Audio.h"


void Audio::addToQueue(float sample)
{
    //int16_t sam = sample * INT16_MAX;
    SDL_QueueAudio(1,&sample,sizeof(float));
}