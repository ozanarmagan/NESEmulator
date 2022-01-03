#include "Audio.h"


void Audio::addToQueue(float sample)
{
    SDL_QueueAudio(1,&sample,sizeof(float));
}