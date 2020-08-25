#ifndef _RENDERER___H_
#define _RENDERER___H_

#include "RTTypes.h"
#include "Scene.h"
#include "Lens.h"
#include <mutex>

// the number of threads to run in the renderer
#define nExecutionThreads 8

// The information for a render job
struct Renderer_Job {
	Scene_Descriptor * scene; // scene objects

	// the pixel buffer and dimensions to render the scene to
	uint32_t * pixelBuffer;
	int windowWidth;
	int windowHeight;

	// the location of the next pixel to be rendered
	volatile int pixelJobIndex;

	RTTypesVector * camera; // camera location
    Lens_Descriptor * lens; // camera lens
	
	// distance of imaginary camera sensor from the lens; this should match the
	// optics of the lens if depth of field rendering is being used
	float focalDistance;

	std::mutex lock;
};

// render a specific pixel of a renderer job -- this is called by
// renderer_executionThread. It is not meant to be called directly elsewhere
void renderer_lensRender(Renderer_Job * job, int pixelX, int pixelY);

// the function a renderer thread executes
void renderer_executionThread(void * jobVoidPtr);

// render a scene with default camera parameters; specify the dimensions of
// the desired output buffer, and a pointer to the buffer will be returned
uint32_t * renderer_renderScene(Scene_Descriptor * scene, int pixelWidth,
	int pixelHeight);

#endif // _RENDERER___H_