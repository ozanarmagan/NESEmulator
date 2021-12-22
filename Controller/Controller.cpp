#include "Controller.h"

Controller::Controller(Bus& bus) : bus(bus) 
{  
    for(bool key : keys) 
        key = false; 
}


void Controller::handleInput()
{
    BYTE* controllerStatus = bus.getControllerMemory();
    controllerStatus[0] |= keys[SDLK_j] ?  0x80 : 0x00;  // A BUTTON
    controllerStatus[0] |= keys[SDLK_k] ?  0x40 : 0x00;  // B BUTTON
    controllerStatus[0] |= keys[SDLK_SPACE] ?  0x20 : 0x00;  // SELECT BUTTON
    controllerStatus[0] |= keys[SDLK_RCTRL] ?  0x10 : 0x00;  // START BUTTON
    controllerStatus[0] |= keys[SDLK_w] || keys[SDLK_UP] ?  0x08 : 0x00;  // UP
    controllerStatus[0] |= keys[SDLK_s] || keys[SDLK_DOWN] ?  0x04 : 0x00;  // DOWN 
    controllerStatus[0] |= keys[SDLK_a] || keys[SDLK_LEFT] ?  0x02 : 0x00;  // LEFT
    controllerStatus[0] |= keys[SDLK_d] || keys[SDLK_RIGHT] ?  0x01 : 0x00;  // RIGHT
}