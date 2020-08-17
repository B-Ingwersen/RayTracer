#ifndef _GRAPHICAL___H_
#define _GRAPHICAL___H_

#include "SDL2/SDL.h"
#include <cstdint>

/* A container for all the global variables needed for rendering and display
to the screen */
struct Graphical_Data {
    int windowWidth;
    int windowHeight;
    SDL_Window * sdlWindow;
    SDL_Surface * sdlSurface;
    uint32_t * frameBuffer;
};

// The single instance of a Graphical_Data object that should be used
extern Graphical_Data graphical_data;

// Initialize the graphical output system; abort the program if there is an
// error
void graphical_init();

// Copy pixelData to the screen; pixelData must point to an array with the same
// dimensions set in graphical_data
void graphical_display(uint32_t * pixelData);

// A wait loop until the user closes the output window
void graphical_wait();

// Quit the graphical program and free the associated resources
void graphical_quit();

#endif // _GRAPHICAL___H_