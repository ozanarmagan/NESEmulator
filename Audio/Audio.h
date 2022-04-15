#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include <SDL2/SDL_audio.h>


namespace nesemulator
{
    namespace Audio
    {
        inline void addToQueue(float sample) { SDL_QueueAudio(1,&sample,sizeof(float));    };
    }
}



#endif