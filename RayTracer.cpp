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
#include <thread>
#include <mutex>

using namespace std;

#include "Graphical.h"
#include "RTTypes.h"
#include "Scene.h"
#include "Ray.h"
#include "Lens.h"
#include "Renderer.h"
#include "PostProcessing.h"

int main(int argc, char ** args) {
    graphical_init();

    // START TEST SCENE OBJECT
		Sphere_SceneObjectData Object1 = { {{2.5,-0.6,0}, 0.3}, 0.3, {1.0,0.0,0.0} };
		Sphere_SceneObjectData Object2 = { {{-1,0,0}, 0.3}, 0.3, {1.0,0.0,0.0} };
		Sphere_SceneObjectData Object3 = { {{-4.5,1.2,0}, 0.3}, 0.3, {1.0,0.0,0.0} };
		TileFloor_SceneObjectData Object4 = { {{0,0,1}, -0.3}, 0.2, {0.95,0.95,0.95} };
		Plane_SceneObjectData Object5 = { {{0,0,1}, -0.31}, 0.1, {0.6,0.6,0.6} };
		Plane_SceneObjectData Object6 = { {{0,0,-1}, -1.2}, 0.1, {0.9,0.9,0.9} };
		Plane_SceneObjectData Object7 = { {{-1,0,0}, -6}, 0.1, {0.9,0.9,0.9} };
		Plane_SceneObjectData Object8 = { {{1,0,0}, -4.8}, 0.1, {0.9,0.9,0.9} };
		Plane_SceneObjectData Object9 = { {{0,1,0}, -1.5}, 0.1, {0.9,0.9,0.9} };
		Plane_SceneObjectData Object10 = { {{0,-1,0}, -1.5}, 0.1, {0.9,0.9,0.9} };

		Scene_ObjectType objectsType[] = {
			sphere_SceneObject,
			sphere_SceneObject,
			sphere_SceneObject,
			tileFloor_SceneObject,
			plane_SceneObject,
			plane_SceneObject,
			plane_SceneObject,
			plane_SceneObject,
			plane_SceneObject,
			plane_SceneObject
		};
		void * objectsData[] = {
			&Object1,
			&Object2,
			&Object3,
			&Object4,
			&Object5,
			&Object6,
			&Object7,
			&Object8,
			&Object9,
			&Object10
		};
        RTTypesVector light = { 2,1,.75 };
		Scene_Descriptor scene = {
			10,
			objectsType,
			objectsData,
            &light
		};
	// END TEST SCENE OBJECT

    uint32_t * buffer = renderer_renderScene(&scene, graphical_data.windowWidth, graphical_data.windowHeight);
    //postProcessing_3x3GuassianBlur(buffer, graphical_data.windowWidth, graphical_data.windowHeight);
    graphical_display(buffer);

    graphical_wait();
    graphical_quit();

    return 0;
}