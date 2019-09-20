void renderer_lensRender(uint32_t * pixelBuffer, unsigned int windowWidth, unsigned int windowHeight);

uint32_t * renderer_renderScene(int pixelWidth, int pixelHeight) {
    uint32_t * buffer = new uint32_t[pixelWidth * pixelHeight];
	
    renderer_lensRender(buffer, pixelWidth, pixelHeight);
    
    return buffer;
}

void renderer_lensRender(uint32_t * pixelBuffer, unsigned int windowWidth, unsigned int windowHeight) {

    int pixelX, pixelY;
	float FoVconst = 3;
	float distance = 5;

	float x1, y1, z1, radius;
	float lX, lY, lZ;

	//the point (xv,yv,zv) defines the location of the camera that looks into the scene towards the origin
    Objects_Vector camera = {distance, 0, 0};
    Objects_Vector viewpoint = camera;

	for (pixelY = 0; pixelY < windowHeight; pixelY++) {
		for (pixelX = 0; pixelX < windowWidth; pixelX++) {

			//##########################	Code for point of view	  ###########################
			float Radius1 = .5;
			float Radius2 = -.5;

			//Spheres recorded in (x1,y1,z1,r) form; the intersection is the glass of the lens: the lens plane refers to the plane that their intersection lies on
            Objects_Sphere sphere1 = { distance + float(.4), 0, 0, Radius1 };
            Objects_Sphere sphere2 = { distance - float(.4), 0, 0, Radius2 };


			//Auto calculated values that can aid in focal distance calculatinns
			double glassN = 1.5;
			double lensWidth = fabs(Radius1 - Radius2) - sphere1.center.dist(sphere2.center);

			//various calculation for focal distance:		
			//double focalDistance = 1 / ((glassN - 1)*(1 / Radius1 - 1 / Radius2 + (glassN - 1)*lensWidth / (glassN*Radius1*Radius2)));
			float focalDistance = .598;
			//double focalDistance = .617; //At distance 6
			//double focalDistance = .707; //At distance 2.57

			//the point that the ray is going to hit on the sensor: the ray is traced backwards from here, point "sensor"
            Objects_Vector sensor = {
                distance + focalDistance,
                float( -(2 * (double)(pixelX) / windowWidth - 1) / 6 ),
                float( -(windowHeight - 2 * (double)(pixelY) )/ windowWidth / 6 )
            };

			int sample; //couting variable
			Color RGBATotalColors = { 0,0,0,0 }; //the overall color output of the program; it a total, not average of samples
			int iters = 1; //The number of samples in one dimension
			int numOfSamples = iters*iters; //The total number of samples on the 2 dimensional sensor

			//The computation for one sample is found ever interation of the while loop
			for (sample = 0; sample < numOfSamples; sample++) {

				//the guiding 2d coordinates of each sample
				int w = (sample) / iters - iters/2;
				int h = (sample) % iters- iters/2;

				//The ray is aimed to this position of the "lens plane": point "LensPlane"
                Objects_Vector lensPlane = {
                    distance,
                    float( ((float)(w)*0.9781 + (float)(h)*0.2079)*0.06 / iters ),
                    float( ((float)(h)*0.9781 - (float)(w)*0.2079)*0.06 / iters )
                };

				Objects_Ray ray;
				ray.createFromPoints(&lensPlane, &sensor);

				Objects_Vector sphere2Intersect;
				intersectSphereRay(&sphere2Intersect, &sphere2, &ray);//finds the intersect of the current "ray" and "sphere 2"; stores result in "sphere2Intersect"

				ray.createFromPoints(&sphere2Intersect, &(sphere2.center));

				//"NormLensPlane" is the point where the normal line of Sphere 2 intersects the lens plane
				float t = (distance - ray.point.x) / ray.direction.x;
				Objects_Vector normLensPlane = ray.point + ray.direction * t;

				//A horizontal line on the x-axis is drawn from "sphere2Intersect" and intersects with the lens plane at "horizPlane.x"
				Objects_Vector lensVector = normLensPlane - lensPlane;

				t = lensVector.dot(sphere2Intersect - normLensPlane);
				Objects_Vector horizPlane = normLensPlane + lensVector * t;

				//The points have been calculated to the same plane, so the following is analyzed in two dimentions:
				float XDelta = horizPlane.dist(sphere2Intersect);
				float YDelta1 = horizPlane.dist(lensPlane);
				float YDelta2 = horizPlane.dist(normLensPlane);
				float testDelta = lensPlane.dist(normLensPlane);

				float theta1 = atan2(YDelta1, XDelta);//angle of: "sphere2Intersect" to "HorizPlane" to "LensPlane"
				float theta2 = atan2(YDelta2, XDelta);//angle of: "sphere2Interesect" to "HorizPlane" to "NormLensPlane"
				float theta3/*angle of "LensPlane" to "HorizPlane" to "NormLensPlane"*/, theta4, theta5;

				if (YDelta1 < testDelta && YDelta2 < testDelta) {//is theta1 negative or positive (distance gives abs value)
					theta3 = theta1 + theta2;
					theta4 = theta3 / 1.5;//angle in glass adjusted for Refraction of glass
					theta5 = theta4 - theta2;//new angle of theta adjusted to horizontal line in 2-d reference frame
				}
				else {
					theta3 = theta1 - theta2;
					theta4 = theta3 / 1.5;
					theta5 = theta4 + theta2;
				}

				float newYDelta = tan(theta5)*XDelta;
				Objects_Vector newPoint = horizPlane + (lensPlane - horizPlane) * (newYDelta / YDelta1);
				ray.createFromPoints(&sphere2Intersect, &newPoint);

				Objects_Vector sphere1Intersect;
				intersectSphereRay(&sphere1Intersect, &sphere1, &ray);//The rays intersect with "sphere2" (where it exits the glass of the lens on the other side)
				ray.createFromPoints(&sphere1Intersect, &(sphere1.center) );

				t = (distance - ray.point.x) / ray.direction.x;
				normLensPlane = ray.point + ray.direction * t;

				//calculations for xHoriz Plane
				lensVector = normLensPlane - newPoint;

				//t = (lensVector.y*(sphere1Intersect[4]-normLensPlane.y)+lensVector.z*(sphere1Intersect[5]-normLensPlane.z))/(lensVector.y*lensVector.y+lensVector.z*lensVector.z);
				t = lensVector.dot(sphere1Intersect - normLensPlane)/(lensVector.dot(lensVector));
				horizPlane = normLensPlane + lensVector * t;

				XDelta = horizPlane.dist(sphere1Intersect);
				YDelta1 = horizPlane.dist(lensPlane);
				YDelta2 = horizPlane.dist(normLensPlane);
				testDelta = lensPlane.dist(normLensPlane);

				theta1 = atan2(YDelta1, XDelta);
				theta2 = atan2(YDelta2, XDelta);

				if (YDelta1 < testDelta && YDelta2 < testDelta) {
					theta3 = theta1 + theta2;
					theta4 = theta3 * 1.5;//The angle increases because it is exiting the glass now
					theta5 = theta4 - theta2;
				}
				else {
					theta3 = theta1 - theta2;
					theta4 = theta3 * 1.5;
					theta5 = theta4 + theta2;
				}

				newYDelta = tan(theta5)*XDelta;
				newPoint = horizPlane + (newPoint - horizPlane) * (newYDelta / YDelta1);
				ray.createFromPoints(&newPoint, &sphere1Intersect);

				//float checkPlane[4] = { 1,0,0,5 };//A plane that divides what is infront of the camera and what is behind; used to ensure behind objects are not counted.
				//float origin[3] = { 0,0,0 };
				Objects_Plane checkPlane = { {1,0,0}, 5};
				Objects_Vector origin = { 0, 0, 0 };

				Color RGBAColors;
				traceRay(&RGBAColors, &ray, &viewpoint, 3, &checkPlane, &origin, -1, pixelX, pixelY);

				RGBATotalColors += RGBAColors;
			}

			RGBATotalColors /= numOfSamples;

			//##################	End of depth of field section	 ##############################

			//################	Non point of view code ########################

			/*float checkPlane[4] = { 1,0,0,5 };
			float origin[3] = { 0,0,0 };
			
			float ray[6];
			makeRay(ray, 5, 0, 0, 0, ((float)(2 * i) / (float)(windowWidth)-1) / FoVconst * distance, -((float)(2 * j - windowHeight) / (float)(windowWidth)) / FoVconst * distance);

			int RGBATotalColors[4];
			traceRay(RGBATotalColors, ray, viewpoint, 9, checkPlane, origin, -1, i ,j);*/

			//################	Non point of view code ########################

			pixelBuffer[windowWidth * pixelY + pixelX] = RGBATotalColors.u32();
		}
    }
}