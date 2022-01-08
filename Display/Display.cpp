#include "Display.h"



Display::Display(SDL_Event* event,Controller& controller,Audio& audio) : eventPtr(event),controller(controller),audio(audio)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
        std::cout << "ERROR: SDL Couldn't initialized!\n";
    init();

    SDL_AudioSpec audiospec;

    audiospec.freq     = 44100;
    audiospec.format   = AUDIO_F32SYS;
    audiospec.samples  = 735;
    audiospec.channels = 1;
    audiospec.callback = NULL;


    auto audio_open = SDL_OpenAudio(&audiospec,NULL);

    if(audio_open < 0)
        std::cout << "ERROR! " << SDL_GetError() << std::endl;

    std::cout << "Audio Device Initialized\n";
    std::cout << "Device Name: " << SDL_GetAudioDeviceName(audio_open,0) << std::endl;
    std::cout << "Frequency: " << audiospec.freq << std::endl;
    std::cout << "Samples: " << audiospec.samples << std::endl;
    std::cout << "Channels: " << (int)audiospec.channels  << std::endl;
    
    SDL_PauseAudio(0);

#ifdef DEBUG
    initDebug();
#endif

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

    //SDL_SetWindowBordered(window,SDL_FALSE);
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

    nextFrame[x + y * RENDER_WIDTH - 1] = pixel;
}

void Display::renderFrame()
{
    SDL_UpdateTexture(texture, NULL, currentFrame, RENDER_WIDTH * sizeof(PIXEL));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    //SDL_DestroyTexture(texture);

    interval1 = interval0;
    interval0 = SDL_GetTicks();

    SDL_SetWindowTitle(window,(string("NES Emulator FPS: " + std::to_string(1000.0 / (interval0 - interval1)))).c_str());

    while(SDL_PollEvent(eventPtr))
    {
        switch (eventPtr->type)
            {
            case SDL_QUIT:
                std::cout << "LOG: Quit has called! Good bye!\n";
                exit(1);
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                controller.setJoyButton(eventPtr->cbutton.button,eventPtr->cbutton.which);
                break;
            case SDL_CONTROLLERBUTTONUP:
                controller.clearJoyButton(eventPtr->cbutton.button,eventPtr->cbutton.which);
                break;
            case SDL_KEYDOWN:
                controller.setKey(eventPtr->key.keysym.scancode);
                break;
            case SDL_KEYUP:
                controller.clearKey(eventPtr->key.keysym.scancode);
                break;
            default:
                break;
            }
    }


}

void Display::frameDone()
{
    for(int i = 0;i < RENDER_WIDTH * RENDER_HEIGHT;i++)
        currentFrame[i] = nextFrame[i];
}


#ifdef DEBUG 
void Display::initDebug()
{
    debugWindow = SDL_CreateWindow("DEBUG WINDOW", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEBUG_WIDTH * 2 , DEBUG_HEIGHT * 2 , 0);

    if(!debugWindow)
    {
        std::cout << "Failed to create debug window\n";
        std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
        return;
    }
    else
        std::cout << "Debug Window created!\n",
    Drenderer = SDL_CreateRenderer(debugWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    Dtexture = SDL_CreateTexture(Drenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, DEBUG_WIDTH, DEBUG_HEIGHT);

    SDL_RenderSetLogicalSize(Drenderer, DEBUG_WIDTH, DEBUG_HEIGHT);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");


    for(int i = 0;i < DEBUG_HEIGHT;i++)
        for(int j = 0;j < DEBUG_WIDTH;j++)
            setPixelDebug(i,j,{84,84,84});
}

#endif

#ifdef DEBUG
void Display::renderDebugFrame()
{
    SDL_UpdateTexture(Dtexture, NULL, debug, DEBUG_WIDTH * sizeof(PIXEL));
    SDL_RenderCopy(Drenderer, Dtexture, NULL, NULL);
    SDL_RenderPresent(Drenderer);

}

void Display::setPixelDebug(int x, int y,PIXEL_RGB colors)
{
    PIXEL pixel = 0;
    
    pixel |= 255 << 24;
    pixel |= (PIXEL)colors.r << 16;
    pixel |= (PIXEL)colors.g << 8;
    pixel |= (PIXEL)colors.b << 0;

    debug[x + y * DEBUG_WIDTH] = pixel;
}



#endif