#ifndef AUDIO_H
#define AUDIO_H

#include <iostream>
#include <SDL2/SDL_audio.h>

#include "../Utils/handler.h"
#include "../Utils/Queue.h"

class Audio
{
    public:
        Audio(Queue<AUDIO>* queuePtr) {audioQueue = queuePtr; };
        void init();
        void audioCallback(BYTE* stream,int len);
        static void staticCallback(void* userData,BYTE* stream,int len);
    private:
        Queue<AUDIO>* audioQueue = nullptr;
};



#endif