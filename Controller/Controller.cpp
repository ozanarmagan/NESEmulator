#include "Controller.h"

Controller::Controller(Bus* bus) : bus(bus) 
{  
    for(bool key : keys) 
        key = false; 
}


void Controller::handleInput()
{
    BYTE controllerStatus[2] = {0x00, 0x00};
    controllerStatus[0] |= keys[SDL_SCANCODE_J] ?  0x80 : 0x00;  // A BUTTON
    controllerStatus[0] |= keys[SDL_SCANCODE_K] ?  0x40 : 0x00;  // B BUTTON
    controllerStatus[0] |= keys[SDL_SCANCODE_SPACE] ?  0x20 : 0x00;  // SELECT BUTTON
    controllerStatus[0] |= keys[SDL_SCANCODE_LCTRL] ?  0x10 : 0x00;  // START BUTTON
    controllerStatus[0] |= keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP] ?  0x08 : 0x00;  // UP
    controllerStatus[0] |= keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN] ?  0x04 : 0x00;  // DOWN 
    controllerStatus[0] |= keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT] ?  0x02 : 0x00;  // LEFT
    controllerStatus[0] |= keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT] ?  0x01 : 0x00;  // RIGHT

    bus->setControllerMemory(controllerStatus);
}