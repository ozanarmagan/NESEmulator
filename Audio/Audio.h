#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include <SDL2/SDL_audio.h>

#include "../Utils/handler.h"
#include "../Utils/Queue.h"

class Audio
{
    public:
        void addToQueue(float sample);
    private:
};



#endif