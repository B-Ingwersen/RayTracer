#include "Renderer.h"
#include "Ray.h"
#include <thread>

void renderer_lensRender(Renderer_Job * job, int pixelX, int pixelY) {

	RTTypesVector sensorLocation = {
		job -> focalDistance,
		float( -(2 * (double)(pixelX) / job -> windowWidth - 1) / 6 ),
		float( -(job -> windowHeight - 2 * (double)(pixelY) )
			/ job -> windowWidth / 6 )
	};

	Color_Int pixelColor = { 0,0,0,0 };
	int sample;
	int iters = 1;
	int numOfSamples = iters*iters;

	for (sample = 0; sample < numOfSamples; sample++) {

		int w = sample / iters - iters / 2;
		int h = sample % iters - iters / 2;
		RTTypesVector lensPlaneLocation = {
			0,
			float( ((float)(w)*0.9781 + (float)(h)*0.2079)*0.06 / iters ),
			float( ((float)(h)*0.9781 - (float)(w)*0.2079)*0.06 / iters )
		};

		RTTypesRay ray;
		ray.createFromPoints(&sensorLocation, &lensPlaneLocation );
		job -> lens -> passRayThrough(&ray);
		ray.point += *job -> camera;

		Color_Int RGBAColors;
		traceRay(&ray, 3, &RGBAColors, job -> scene, -1);

		pixelColor += RGBAColors;
	}

	pixelColor /= numOfSamples;

	job -> pixelBuffer[pixelY * job -> windowWidth + pixelX]
		= pixelColor.toUint32();
}

void renderer_executionThread(void * jobVoidPtr) {
	Renderer_Job * job = (Renderer_Job *)jobVoidPtr;

	while (true) {
		job -> lock.lock();
		int pixelNumber = job -> pixelJobIndex;
		if (pixelNumber >= job -> windowWidth * job -> windowHeight) {
			job -> lock.unlock();
			break;
		}
		else {
			job -> pixelJobIndex++;
		}
		job -> lock.unlock();
	
		int pixelX = pixelNumber % job -> windowWidth;
		int pixelY = pixelNumber / job -> windowWidth;
		renderer_lensRender(job, pixelX, pixelY);
	}
}

uint32_t * renderer_renderScene(Scene_Descriptor * scene, int pixelWidth,
	int pixelHeight) {
	
    uint32_t * buffer = new uint32_t[pixelWidth * pixelHeight];
	
	Lens_Descriptor lens = {
		{ float(.4), 0, 0, 0.5 },
		{ -float(.4), 0, 0, 0.5 },
		1.5
	};
	//float focalDistance = .598;
	float objectDistance = 4;
	float inverseFocalLength = (lens.glassN - 1) 
		* (1 / lens.sphere1.r + 1 / lens.sphere2.r - (lens.glassN - 1)*0.2 /
		(lens.glassN*lens.sphere1.r*lens.sphere2.r));
	float focalDistance = 1 / (inverseFocalLength - 1 / objectDistance);
    RTTypesVector camera = {5, 0, 0};

	Renderer_Job job = {
		scene,
		buffer,
		pixelWidth,
		pixelHeight,
		0,

		&camera,
		&lens,
		focalDistance
	};
	job.lock.unlock();

	std::thread executionThreads[nExecutionThreads];
	int threadNumber;
	for (threadNumber = 0; threadNumber < nExecutionThreads; threadNumber++) {
		executionThreads[threadNumber] =
			std::thread(renderer_executionThread, &job);
	}
	for (threadNumber = 0; threadNumber < nExecutionThreads; threadNumber++) {
		executionThreads[threadNumber].join();
	}
    
    return buffer;
}
