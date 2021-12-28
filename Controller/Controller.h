#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <SDL2/SDL.h>
#include "../Utils/handler.h"
#include "../Bus/Bus.h"

class Controller
{
    public: 
        Controller(Bus* bus) : bus(bus) {   };
        bool getKeyStatus(int index) { return keys[index]; };
        void setKeys(const BYTE* keys_) { keys = keys_; };
        void handleInput();
    private:
        void getKeyState() {const BYTE* keys_ = SDL_GetKeyboardState(NULL);keys = keys_;};
        const BYTE *keys; //SDL Keys
        Bus* bus;
};


#endif