#include <vector>
#include <SDL2/SDL_gamecontroller.h>

#include "Controller.h"

Controller::Controller(Bus* bus) : bus(bus) { 
    if(SDL_NumJoysticks() > 0) 
    {
        contorller1 = SDL_GameControllerOpen(0);
        std::cout << "Found GameController:\n";
        std::cout << "Name: " << SDL_GameControllerNameForIndex(0) << std::endl;

    } 
    if(SDL_NumJoysticks() > 1) 
    {
        contorller2 = SDL_GameControllerOpen(1);
        std::cout << "Found GameController:\n";
        std::cout << "Name: " << SDL_GameControllerNameForIndex(1) << std::endl;
    } 
}



void Controller::setJoyButton(int id,int which) 
{
    switch(id)
    {
        case SDL_CONTROLLER_BUTTON_A:  
            controllerStatus[which] |= 0x80;
            break;
        case SDL_CONTROLLER_BUTTON_B:  
            controllerStatus[which] |= 0x40;
            break;
        case SDL_CONTROLLER_BUTTON_BACK:   
            controllerStatus[which] |= 0x20;
            break;
        case SDL_CONTROLLER_BUTTON_START:   
            controllerStatus[which] |= 0x10;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:   
            controllerStatus[which] |= 0x08;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:   
            controllerStatus[which] |= 0x04;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT: 
            controllerStatus[which] |= 0x02;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:   
            controllerStatus[which] |= 0x01;
            break;
    }
    bus->setControllerMemory(controllerStatus);
}

void Controller::clearJoyButton(int id,int which)
{
    switch(id)
    {
        case SDL_CONTROLLER_BUTTON_A:   
            controllerStatus[which] &= 0x7F;
            break;
        case SDL_CONTROLLER_BUTTON_B:   
            controllerStatus[which] &= 0xBF;
            break;
        case SDL_CONTROLLER_BUTTON_BACK:   
            controllerStatus[which] &= 0xD0;
            break;
        case SDL_CONTROLLER_BUTTON_START:   
            controllerStatus[which] &= 0xE0;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:  
            controllerStatus[which] &= 0xF7;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:   
            controllerStatus[which] &= 0xFB;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:   
            controllerStatus[which] &= 0xFD;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:   
            controllerStatus[which] &= 0xFE;
            break;
    }
    bus->setControllerMemory(controllerStatus);
}

void Controller::setJoyAxis(int id,int value,int which)
{
    switch(id)
    {
        case SDL_CONTROLLER_AXIS_LEFTY:  
            break;
        case SDL_CONTROLLER_AXIS_LEFTX:   
            controllerStatus[which] |= (value >  20000) ?  0x02 : 0x00;
            controllerStatus[which] |= (value < -20000) ?  0x01 : 0x00;
            break;
        default:
            break;
    }
    bus->setControllerMemory(controllerStatus);
}

void Controller::setKey(int id)
{
    switch(id)
    {
        case SDL_SCANCODE_X:  
            controllerStatus[0] |= 0x80;
            break;
        case SDL_SCANCODE_C:  
            controllerStatus[0] |= 0x40;
            break;
        case SDL_SCANCODE_A:   
            controllerStatus[0] |= 0x20;
            break;
        case SDL_SCANCODE_S:   
            controllerStatus[0] |= 0x10;
            break;
        case SDL_SCANCODE_UP:   
            controllerStatus[0] |= 0x08;
            break;
        case SDL_SCANCODE_DOWN:   
            controllerStatus[0] |= 0x04;
            break;
        case SDL_SCANCODE_LEFT: 
            controllerStatus[0] |= 0x02;
            break;
        case SDL_SCANCODE_RIGHT:   
            controllerStatus[0] |= 0x01;
            break;
    }
    bus->setControllerMemory(controllerStatus);
}

void Controller::clearKey(int id)
{
    switch(id)
    {
        case SDL_SCANCODE_X:  
            controllerStatus[0] &= 0x7F;
            break;
        case SDL_SCANCODE_C:  
            controllerStatus[0] &= 0xBF;
            break;
        case SDL_SCANCODE_A:   
            controllerStatus[0] &= 0xD0;
            break;
        case SDL_SCANCODE_S:   
            controllerStatus[0] &= 0xE0;
            break;
        case SDL_SCANCODE_UP:   
            controllerStatus[0] &= 0xF7;
            break;
        case SDL_SCANCODE_DOWN:   
            controllerStatus[0] &= 0xFB;
            break;
        case SDL_SCANCODE_LEFT: 
            controllerStatus[0] &= 0xFD;
            break;
        case SDL_SCANCODE_RIGHT:   
            controllerStatus[0] &= 0xFE;
            break;
    }
    bus->setControllerMemory(controllerStatus);
}