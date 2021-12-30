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

void Controller::handleInput()
{



    getKeyState();
    controllerStatus[0] |= keys[SDL_SCANCODE_J] ?  0x80 : 0x00;  // A BUTTON
    controllerStatus[0] |= keys[SDL_SCANCODE_K] ?  0x40 : 0x00;  // B BUTTON
    controllerStatus[0] |= keys[SDL_SCANCODE_SPACE] ?  0x20 : 0x00;  // SELECT BUTTON
    controllerStatus[0] |= keys[SDL_SCANCODE_LCTRL] ?  0x10 : 0x00;  // START BUTTON
    controllerStatus[0] |= (keys[SDL_SCANCODE_W] | keys[SDL_SCANCODE_UP]) ?  0x08 : 0x00;  // UP
    controllerStatus[0] |= (keys[SDL_SCANCODE_S] | keys[SDL_SCANCODE_DOWN]) ?  0x04 : 0x00;  // DOWN 
    controllerStatus[0] |= (keys[SDL_SCANCODE_A] | keys[SDL_SCANCODE_LEFT]) ?  0x02 : 0x00;  // LEFT
    controllerStatus[0] |= (keys[SDL_SCANCODE_D] | keys[SDL_SCANCODE_RIGHT]) ?  0x01 : 0x00;  // RIGHT

    // auto numOfGamePads = SDL_NumGameControllers();



    // if(contorller1)
    // {
    //        BYTE control1 = 0x00;
    //        control1 |= SDL_GameControllerGetButton(contorller1,SDL_CONTROLLER_BUTTON_A) << 0;
    //        control1 |= SDL_GameControllerGetButton(contorller1,SDL_CONTROLLER_BUTTON_B) << 1;
    //        control1 |= SDL_GameControllerGetButton(contorller1,SDL_CONTROLLER_BUTTON_BACK) << 2;
    //        control1 |= SDL_GameControllerGetButton(contorller1,SDL_CONTROLLER_BUTTON_START) << 3;
    //        control1 |= SDL_GameControllerGetButton(contorller1,SDL_CONTROLLER_BUTTON_DPAD_UP) << 4;
    //        control1 |= SDL_GameControllerGetButton(contorller1,SDL_CONTROLLER_BUTTON_DPAD_DOWN) << 5;
    //        control1 |= SDL_GameControllerGetButton(contorller1,SDL_CONTROLLER_BUTTON_DPAD_LEFT) << 6;
    //        control1 |= SDL_GameControllerGetButton(contorller1,SDL_CONTROLLER_BUTTON_DPAD_RIGHT) << 7;
    //        controllerStatus[0] |= control1;
    // }

    // if(controller2)
    // {
    //     if(SDL_GameControllerGetButton(controller2,SDL_CONTROLLER_BUTTON_A)) controllerStatus[1] |= 0x80;
    //     if(SDL_GameControllerGetButton(controller2,SDL_CONTROLLER_BUTTON_B)) controllerStatus[1] |= 0x40;
    //     if(SDL_GameControllerGetButton(controller2,SDL_CONTROLLER_BUTTON_BACK)) controllerStatus[1] |= 0x20;
    //     if(SDL_GameControllerGetButton(controller2,SDL_CONTROLLER_BUTTON_START)) controllerStatus[1] |= 0x10;
    //     if(SDL_GameControllerGetButton(controller2,SDL_CONTROLLER_BUTTON_DPAD_UP) || SDL_GameControllerGetAxis(controller2,SDL_CONTROLLER_AXIS_RIGHTY)) controllerStatus[1] |= 0x80;
    //     if(SDL_GameControllerGetButton(controller2,SDL_CONTROLLER_BUTTON_DPAD_DOWN) || SDL_GameControllerGetAxis(controller2,SDL_CONTROLLER_AXIS_LEFTY)) controllerStatus[1] |= 0x40;
    //     if(SDL_GameControllerGetButton(controller2,SDL_CONTROLLER_BUTTON_DPAD_LEFT) || SDL_GameControllerGetAxis(controller2,SDL_CONTROLLER_AXIS_LEFTX)) controllerStatus[1] |= 0x20;
    //     if(SDL_GameControllerGetButton(controller2,SDL_CONTROLLER_BUTTON_DPAD_RIGHT) || SDL_GameControllerGetAxis(controller2,SDL_CONTROLLER_AXIS_RIGHTX)) controllerStatus[1] |= 0x10;
    // }

    bus->setControllerMemory(controllerStatus);
    controllerStatus[0] = 0x00;

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