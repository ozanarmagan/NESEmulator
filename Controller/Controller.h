#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <SDL2/SDL.h>
#include "../Utils/handler.h"
#include "../Bus/Bus.h"

namespace nesemulator
{
    /**
     * Controller class to store current controller inputs
     * 
     */
    class Controller
    {
        public: 
            Controller(Bus* bus);
            bool getKeyStatus(int index) { return keys[index]; };
            void setKeys(const BYTE* keys_) { keys = keys_; };
            void setJoyButton(int id,int which);
            void clearJoyButton(int id,int which);
            void setJoyAxis(int id,int value,int which);
            const bool& getSaveButtonStatus() { return saveStateButton; }
            const bool& getLoadButtonStatus() { return loadStateButton; }
            void setKey(int id);
            void clearKey(int id);
            void handleInput();
        private:
            void getKeyState() {const BYTE* keys_ = SDL_GetKeyboardState(NULL);keys = keys_;};
            friend class NES;
            const BYTE *keys; //SDL Keys
            Bus* bus;
            BYTE controllerStatus[2] = {0x00, 0x00};
            bool saveStateButton;
            bool loadStateButton;
            SDL_GameController *contorller1 = nullptr;
            SDL_GameController *contorller2 = nullptr;
    };

}

#endif