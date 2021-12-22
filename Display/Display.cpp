#include "Display.h"

Display::Display()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        std::cout << "ERROR: SDL Couldn't initialized!\n";
    init();
}

void Display::init()
{
    window = SDL_CreateWindow("NES Emulator - OzanArmagan", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);

    if(!window)
    {
        std::cout << "Failed to create window\n";
        std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
        return;
    }
    else
        std::cout << "SDL Window created!\n",
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, RENDER_WIDTH, RENDER_HEIGHT);

    SDL_RenderSetLogicalSize(renderer, RENDER_WIDTH, RENDER_HEIGHT);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  
}


void Display::setPixel(int x, int y,PIXEL_RGB pixelColors)
{
    PIXEL pixel = 0;
    
    pixel |= 255 << 24;
    pixel |= (PIXEL)pixelColors.r << 16;
    pixel |= (PIXEL)pixelColors.g << 8;
    pixel |= (PIXEL)pixelColors.b << 0;

    pixels[x + y * RENDER_WIDTH - 1] = pixel;
}

void Display::renderFrame()
{
    SDL_UpdateTexture(texture, NULL, pixels, RENDER_WIDTH * sizeof(PIXEL));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Delay(1000/60);
}