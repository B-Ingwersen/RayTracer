/*
    A basic ray tracer that uses SDL2 as a backend

    To compile on Linux, run:
        g++ RayTracer.cpp -lpthread -lSDL2 -o RayTracer

        Note: This obviously requires SDL2 development package as a dependency.  On Ubuntu, you can just get the package libsdl2-dev
        Note: On some desktop environments & graphics cards, SDL2 software rendering can be a bit buggy
*/

#include "SDL2/SDL.h"
#include <iostream>
#include <math.h>
#include <cfloat>
#include <cctype>
#include <cstring>

using namespace std;

#include "Graphical.h"
#include "Ray.h"
#include "Renderer.h"

int main(int argc, char ** args) {
    graphical_init();

    uint32_t * buffer = renderer_renderScene(graphical_data.windowWidth, graphical_data.windowHeight);
    graphical_display(buffer);

    graphical_wait();
    graphical_quit();

    return 0;
}