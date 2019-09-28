void renderer_lensRender(Scene_Descriptor * scene, uint32_t * pixelBuffer, unsigned int windowWidth, unsigned int windowHeight) {

	Lens_Descriptor lens = {
		{ float(.4), 0, 0, 0.5 },
		{ -float(.4), 0, 0, 0.5 },
		1.5
	};
	float focalDistance = .598;
    RTTypesVector camera = {5, 0, 0};

	int pixelX, pixelY;
	for (pixelY = 0; pixelY < windowHeight; pixelY++) {
		for (pixelX = 0; pixelX < windowWidth; pixelX++) {

			RTTypesVector sensorLocation = {
				focalDistance,
				float( -(2 * (double)(pixelX) / windowWidth - 1) / 6 ),
				float( -(windowHeight - 2 * (double)(pixelY) )/ windowWidth / 6 )
			};

			Color pixelColor = { 0,0,0,0 };
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
				lens.passRayThrough(&ray);
				ray.point += camera;

				Color RGBAColors;
				traceRay(&ray, 3, &RGBAColors, scene, -1);

				pixelColor += RGBAColors;
			}

			pixelColor /= numOfSamples;

			pixelBuffer[windowWidth * pixelY + pixelX] = pixelColor.u32();
		}
    }
}

uint32_t * renderer_renderScene(Scene_Descriptor * scene, int pixelWidth, int pixelHeight) {
    uint32_t * buffer = new uint32_t[pixelWidth * pixelHeight];
	
    renderer_lensRender(scene, buffer, pixelWidth, pixelHeight);
    
    return buffer;
}