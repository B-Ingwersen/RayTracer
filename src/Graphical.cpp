#include "Graphical.h"
#include <iostream>

// The single instance of a Graphical_Data object that should be used
// The default window dimensions are 1600 x 900 as defined here
// graphical_data cannot be used validly until graphical_init is called
Graphical_Data graphical_data = {
    1600,
    900,
    NULL,
    NULL,
    NULL
};

// Initialize the graphical output system; abort the program if there is an
// error
void graphical_init() {
    // attempt to initilize an SDL window
    SDL_Init(SDL_INIT_EVERYTHING);
    graphical_data.sdlWindow = SDL_CreateWindow("RayTracer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        graphical_data.windowWidth, 
        graphical_data.windowHeight,
        SDL_WINDOW_RESIZABLE);
    
    // abort if an error occured in SDL
    if (graphical_data.sdlWindow == NULL) {
        std::cout << "SDL Initialization Error: " << SDL_GetError()
            << std::endl;
        abort();
    }

    // save the SDL surface and framebuffer
    graphical_data.sdlSurface = SDL_GetWindowSurface(graphical_data.sdlWindow);
    graphical_data.frameBuffer =
        (uint32_t *)graphical_data.sdlSurface -> pixels;
}

// Copy pixelData to the screen; pixelData must point to an array with the same
// dimensions set in graphical_data
void graphical_display(uint32_t * pixelData) {
    SDL_LockSurface(graphical_data.sdlSurface);

    memcpy(graphical_data.frameBuffer, pixelData, graphical_data.windowHeight
        * graphical_data.windowWidth * sizeof(uint32_t));

    SDL_UnlockSurface(graphical_data.sdlSurface);
    SDL_UpdateWindowSurface(graphical_data.sdlWindow);
}

// A wait loop until the user closes the output window
void graphical_wait() {
    SDL_Event event;
    bool quit = false;
    while (!quit) {
        SDL_WaitEvent(&event);

        // quit upon an SDL_QUIT event; this is generally triggered by the user
        // closing the window
        if (event.type == SDL_QUIT) {
            quit = true;
        }
    }
}

// Quit the graphical program and free the associated resources
void graphical_quit() {
    SDL_FreeSurface(graphical_data.sdlSurface);
    SDL_DestroyWindow(graphical_data.sdlWindow);
    SDL_Quit();
}