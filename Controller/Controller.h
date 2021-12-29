#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <SDL2/SDL.h>
#include "../Utils/handler.h"
#include "../Bus/Bus.h"

class Controller
{
    public: 
        Controller(Bus* bus,SDL_Event* event);
        bool getKeyStatus(int index) { return keys[index]; };
        void setKeys(const BYTE* keys_) { keys = keys_; };
        void setJoyButton(int id,int which);
        void clearJoyButton(int id,int which);
        void setJoyAxis(int id,int value,int which);
        void handleInput();
    private:
        void getKeyState() {const BYTE* keys_ = SDL_GetKeyboardState(NULL);keys = keys_;};

        const BYTE *keys; //SDL Keys
        Bus* bus;
        SDL_Event* eventPtr;
        BYTE controllerStatus[2] = {0x00, 0x00};
        SDL_GameController *contorller1 = nullptr;
        SDL_GameController *contorller2 = nullptr;
};


#endif