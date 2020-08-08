#ifndef _RENDERER___H_
#define _RENDERER___H_

#include "RTTypes.h"
#include "Scene.h"
#include "Lens.h"
#include <mutex>

#define nExecutionThreads 8

struct Renderer_Job {
	Scene_Descriptor * scene;
	uint32_t * pixelBuffer;
	int windowWidth;
	int windowHeight;
	volatile int pixelJobIndex;

	RTTypesVector * camera;
    Lens_Descriptor * lens;
	float focalDistance;

	std::mutex lock;
};

void renderer_lensRender(Renderer_Job * job, int pixelX, int pixelY);

void renderer_executionThread(void * jobVoidPtr);

uint32_t * renderer_renderScene(Scene_Descriptor * scene, int pixelWidth,
	int pixelHeight);

#endif // _RENDERER___H_