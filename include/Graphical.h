#ifndef _GRAPHICAL___H_
#define _GRAPHICAL___H_

#include "SDL2/SDL.h"
#include <cstdint>

struct Graphical_Data {
    int windowWidth;
    int windowHeight;
    SDL_Window * sdlWindow;
    SDL_Surface * sdlSurface;
    uint32_t * frameBuffer;
};
extern Graphical_Data graphical_data;

void graphical_init();

void graphical_display(uint32_t * pixelData);

void graphical_wait();

void graphical_quit();

#endif // _GRAPHICAL___H_