
struct {
    int windowWidth;
    int windowHeight;
    SDL_Window * sdlWindow;
    SDL_Surface * sdlSurface;
    uint32_t * frameBuffer;
} graphical_data = {
    1600,
    900,
    NULL,
    NULL,
    NULL
};

void graphical_init() {
    SDL_Init(SDL_INIT_EVERYTHING);
    graphical_data.sdlWindow = SDL_CreateWindow("RayTracer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        graphical_data.windowWidth, 
        graphical_data.windowHeight,
        SDL_WINDOW_RESIZABLE);
    
    if (graphical_data.sdlWindow == NULL) {
        cout << "SDL Initialization Error: " << SDL_GetError() << endl;
        abort();
    }

    graphical_data.sdlSurface = SDL_GetWindowSurface( graphical_data.sdlWindow );
    graphical_data.frameBuffer = (uint32_t *)graphical_data.sdlSurface -> pixels;
}

void graphical_display(uint32_t * pixelData) {
    SDL_LockSurface(graphical_data.sdlSurface);

    memcpy(graphical_data.frameBuffer, pixelData, graphical_data.windowHeight * graphical_data.windowWidth * sizeof(uint32_t));

    SDL_UnlockSurface(graphical_data.sdlSurface);
    SDL_UpdateWindowSurface(graphical_data.sdlWindow);
}

void graphical_wait() {
    SDL_Event event;
    bool quit = false;
    while (!quit) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT) {
            quit = true;
        }
    }
}

void graphical_quit() {
    SDL_FreeSurface(graphical_data.sdlSurface);
    SDL_DestroyWindow(graphical_data.sdlWindow);
    SDL_Quit();
}