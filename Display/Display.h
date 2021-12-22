#ifndef DISPLAY_H
#define DISPLAY_H
#define SDL_MAIN_HANDLED
#include "../Utils/handler.h"
#include <SDL2/SDL.h>


class Display
{
    public:
        Display();
        void init();
        void setPixel(int x, int y, PIXEL_RGB pixelColors);
        void renderFrame();
    private:
        PIXEL pixels[RENDER_WIDTH * RENDER_HEIGHT];
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
};

#endif