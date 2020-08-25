#include "Renderer.h"
#include "Ray.h"
#include <thread>

void renderer_lensRender(Renderer_Job * job, int pixelX, int pixelY) {

	// specify the point on the sensor corresponding to the pixel to be rendered
	RTTypesVector sensorLocation = {
		job -> focalDistance,
		float( -(2 * (double)(pixelX) / job -> windowWidth - 1) / 6 ),
		float( -(job -> windowHeight - 2 * (double)(pixelY) )
			/ job -> windowWidth / 6 )
	};

	// 
	Color_Int pixelColor = { 0,0,0,0 };
	int sample;

	// the number of samples to take in each direction; higher numbers create
	// higher quality depth of field renders; set to 1 to include no depth of
	// field effects
	// TODO -- move to a parameter in Renderer_Job
	int iters = 1;

	// the total number of samples used to calculate the pixel color
	int numOfSamples = iters*iters;

	// iterate over each sample
	for (sample = 0; sample < numOfSamples; sample++) {

		// based on the sample #, determine where on the lens to target
		int w = sample / iters - iters / 2;
		int h = sample % iters - iters / 2;
		RTTypesVector lensPlaneLocation = {
			0,
			float( ((float)(w)*0.9781 + (float)(h)*0.2079)*0.06 / iters ),
			float( ((float)(h)*0.9781 - (float)(w)*0.2079)*0.06 / iters )
		};

		// create the ray and send it through the lens
		RTTypesRay ray;
		ray.createFromPoints(&sensorLocation, &lensPlaneLocation );
		job -> lens -> passRayThrough(&ray);
		ray.point += *job -> camera;

		// trace the ray through the scene
		Color_Int RGBAColors;
		traceRay(&ray, 3, &RGBAColors, job -> scene, -1);

		// accumulate the calculated color
		pixelColor += RGBAColors;
	}

	// divide to finish averaging all the pixels
	pixelColor /= numOfSamples;

	// record the result in the pixel buffer
	job -> pixelBuffer[pixelY * job -> windowWidth + pixelX]
		= pixelColor.toUint32();
}

void renderer_executionThread(void * jobVoidPtr) {
	Renderer_Job * job = (Renderer_Job *)jobVoidPtr;

	// keep attempting to obtain render jobs until all the pixels are covered
	while (true) {

		// obtain exculsive access to the job and try to obtain a job index; if
		// there are none left, exit the loop (an the thread)
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
	
		// convert the job index to a pixel and render it
		int pixelX = pixelNumber % job -> windowWidth;
		int pixelY = pixelNumber / job -> windowWidth;
		renderer_lensRender(job, pixelX, pixelY);
	}
}

uint32_t * renderer_renderScene(Scene_Descriptor * scene, int pixelWidth,
	int pixelHeight) {
	
	// generate the pixel buffer
    uint32_t * buffer = new uint32_t[pixelWidth * pixelHeight];
	
	// default lens descriptor
	Lens_Descriptor lens = {
		{ float(.4), 0, 0, 0.5 },
		{ -float(.4), 0, 0, 0.5 },
		1.5
	};

	// calculate the focal distance base on optics lens formulas, taking that
	// an object objectDistance away should be in focus
	float objectDistance = 4;
	float inverseFocalLength = (lens.glassN - 1) 
		* (1 / lens.sphere1.r + 1 / lens.sphere2.r - (lens.glassN - 1)*0.2 /
		(lens.glassN*lens.sphere1.r*lens.sphere2.r));
	float focalDistance = 1 / (inverseFocalLength - 1 / objectDistance);
    RTTypesVector camera = {5, 0, 0};

	// build a render job
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

	// create the execution threads and have them all begin rendering
	std::thread executionThreads[nExecutionThreads];
	int threadNumber;
	for (threadNumber = 0; threadNumber < nExecutionThreads; threadNumber++) {
		executionThreads[threadNumber] =
			std::thread(renderer_executionThread, &job);
	}

	// wait for all the threads to finish
	for (threadNumber = 0; threadNumber < nExecutionThreads; threadNumber++) {
		executionThreads[threadNumber].join();
	}
    
    return buffer;
}
