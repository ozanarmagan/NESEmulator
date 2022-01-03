#ifndef DISPLAY_H
#define DISPLAY_H
#define SDL_MAIN_HANDLED
#include "../Utils/handler.h"
#include "../Audio/Audio.h"
#include "../Controller/Controller.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <stdio.h>

//#define DEBUG

class Display
{
    public:
        Display(SDL_Event* event,Controller& controller,Audio& audio);
        void init();
        void initDebug();
        void setPixel(int x, int y, PIXEL_RGB pixelColors);
        void renderFrame();
#ifdef DEBUG
        void renderDebugFrame();
        void setPixelDebug(int x, int y, PIXEL_RGB colors);
#endif
    private:
        PIXEL pixels[RENDER_WIDTH * RENDER_HEIGHT];
#ifdef DEBUG
        PIXEL debug[DEBUG_WIDTH * DEBUG_HEIGHT];
#endif
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        SDL_Window *debugWindow;
        SDL_Renderer *Drenderer;
        SDL_Texture *Dtexture;
        SDL_Event *eventPtr;
        Controller& controller;
        Audio& audio;
        TIMER interval0 = 0,interval1 = 0;
};

#endif