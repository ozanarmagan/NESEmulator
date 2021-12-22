#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <SDL2/SDL.h>
#include "../Utils/handler.h"
#include "../Bus/Bus.h"

class Controller
{
    public: 
        Controller(Bus& bus);
        void setKeyStatus(int index) { keys[index] = true; };
        void clearKeyStatus(int index) { keys[index] = false; };
        bool getKeyStatus(int index) { return keys[index]; };
        void handleInput();
    private:
        bool keys[322]; //SDL Keys
        Bus& bus;
};


#endif