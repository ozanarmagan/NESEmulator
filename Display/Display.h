#ifndef DISPLAY_H
#define DISPLAY_H
#define SDL_MAIN_HANDLED
#include "../Utils/handler.h"
#include "../Controller/Controller.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <stdio.h>

#define DEBUG

namespace nesemulator
{
    /**
     * Display class to store pixels of frames and display it with SDL2 library utilities
     * 
     */
    class Display
    {
        public:
            Display(SDL_Event* event,Controller& controller);
            ~Display() { SDL_DestroyTexture(texture); SDL_DestroyWindow(window); SDL_DestroyRenderer(renderer); };
            void init(); //Init display
            void initDebug(); // Init debug window
            void setPixel(int x, int y, PIXEL_RGB pixelColors); // Set pixel in the pixel matrix
            void frameDone(); // Transfer next frame to current frame 
            void renderFrame(); // Render current frame
    #ifdef DEBUG
            void renderDebugFrame(); // Render Debug Frame
            void setPixelDebug(int x, int y, PIXEL_RGB colors); // Set pixel for debug window
    #endif
        private:
            PIXEL currentFrame[RENDER_WIDTH * RENDER_HEIGHT]; // Pixel array of current frame
            PIXEL nextFrame[RENDER_WIDTH * RENDER_HEIGHT]; // Pixel array of next frame
    #ifdef DEBUG
            PIXEL debug[DEBUG_WIDTH * DEBUG_HEIGHT]; // Pixel array of debug frame
    #endif
            SDL_Window *window;
            SDL_Renderer *renderer;
            SDL_Texture *texture;
            SDL_Window *debugWindow;
            SDL_Renderer *Drenderer;
            SDL_Texture *Dtexture;
            SDL_Event *eventPtr;
            Controller& controller;
            TIMER interval0 = 0,interval1 = 0; //Interval between frames to track FPS
            int pitch = RENDER_WIDTH * sizeof(PIXEL);
    };
}

#endif