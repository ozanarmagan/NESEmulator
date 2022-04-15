#ifndef DISPLAY_H
#define DISPLAY_H
#define SDL_MAIN_HANDLED
#include "../Utils/handler.h"
#include "../Controller/Controller.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <stdio.h>

//#define DEBUG

namespace nesemulator
{
    class Display
    {
        public:
            Display(SDL_Event* event,Controller& controller);
            ~Display() { SDL_DestroyTexture(texture); SDL_DestroyWindow(window); SDL_DestroyRenderer(renderer); };
            void init();
            void initDebug();
            void setPixel(int x, int y, PIXEL_RGB pixelColors);
            void frameDone();
            void renderFrame();
    #ifdef DEBUG
            void renderDebugFrame();
            void setPixelDebug(int x, int y, PIXEL_RGB colors);
    #endif
        private:
            PIXEL currentFrame[RENDER_WIDTH * RENDER_HEIGHT];
            PIXEL nextFrame[RENDER_WIDTH * RENDER_HEIGHT];
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
            TIMER interval0 = 0,interval1 = 0;
            int pitch = RENDER_WIDTH * sizeof(PIXEL);
    };
}

#endif