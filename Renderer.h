void renderer_lensRender(uint32_t * pixelBuffer, unsigned int windowWidth, unsigned int windowHeight);

uint32_t * renderer_renderScene(int pixelWidth, int pixelHeight) {
    uint32_t * buffer = new uint32_t[pixelWidth * pixelHeight];

    //memset(buffer, 120, pixelWidth * pixelHeight * sizeof(uint32_t));
    renderer_lensRender(buffer, pixelWidth, pixelHeight);
    
    return buffer;
}

void renderer_lensRender(uint32_t * pixelBuffer, unsigned int windowWidth, unsigned int windowHeight) {

    int i, j;
	float FoVconst = 3;
	float distance = 5;

	float x1, y1, z1, radius;
	float lX, lY, lZ;

	//the point (xv,yv,zv) defines the location of the camera that looks into the scene towards the origin
	float xv = distance;
	float yv = 0;
	float zv = 0;

	float viewpoint[3] = { xv,yv,zv };

	for (j = 0; j < windowHeight; j++) {
		for (i = 0; i < windowWidth; i++) {

			//##########################	Code for point of view	  ###########################
			float Radius1 = .5;
			float Radius2 = -.5;

			//Spheres recorded in (x1,y1,z1,r) form; the intersection is the glass of the lens: the lens plane refers to the plane that their intersection lies on
			float sphere1[4] = { distance + float(.4),0,0,Radius1 };
			float sphere2[4] = { distance - float(.4),0,0,Radius2 };

			//Auto calculated values that can aid in focal distance calculatinns
			double glassN = 1.5;
			double lensWidth = fabs(Radius1 - Radius2) - sqrt((sphere1[0] - sphere2[0])*(sphere1[0] - sphere2[0]) + (sphere1[1] - sphere2[1])*(sphere1[1] - sphere2[1]) + (sphere1[2] - sphere2[2])*(sphere1[2] - sphere2[2]));

			//various calculation for focal distance:		
			//double focalDistance = 1 / ((glassN - 1)*(1 / Radius1 - 1 / Radius2 + (glassN - 1)*lensWidth / (glassN*Radius1*Radius2)));
			float focalDistance = .598;
			//double focalDistance = .617; //At distance 6
			//double focalDistance = .707; //At distance 2.57

			//the point that the ray is going to hit on the sensor: the ray is traced backwards from here, point "sensor"
			float Xsensor = distance + focalDistance;
			float Ysensor = -(2 * (double)(i) / windowWidth - 1) / 6;
			float Zsensor = -(windowHeight - 2 * (double)(j) )/ windowWidth / 6;

			int k = 0; //couting variable
			int RGBATotalColors[4] = { 0,0,0,0 }; //the overall color output of the program; it a total, not average of samples
			int iters = 1; //The number of samples in one dimension
			int numOfSamples = iters*iters; //The total number of samples on the 2 dimensional sensor

			//The computation for one sample is found ever interation of the while loop
			while (k < numOfSamples) {

				//the guiding 2d coordinates of each sample
				int w = (k) / iters - iters/2;
				int h = (k) % iters- iters/2;

				//The ray is aimed to this position of the "lens plane": point "LensPlane"
				float xLensPlane = distance;
				float yLensPlane = ((float)(w)*0.9781 + (float)(h)*0.2079)*0.06 / iters;
				float zLensPlane = ((float)(h)*0.9781 - (float)(w)*0.2079)*0.06 / iters;

				float ray[6]; //A variable that stores the information of the ray through various computations
				makeLine(ray, xLensPlane, yLensPlane, zLensPlane, Xsensor, Ysensor, Zsensor); //The ray is defined to go from "sensor" to "LensPlane"

				float sphere2Intersect[6];
				intersectSphereRay(sphere2Intersect, sphere2, ray);//finds the intersect of the current "ray" and "sphere 2"; stores result in "sphere2Intersect"

				//this ensures the closer intersect (further from the origin) is picked for the intersect
				if (sphere2Intersect[3] > sphere2Intersect[0]) {
					sphere2Intersect[0] = sphere2Intersect[3];
					sphere2Intersect[1] = sphere2Intersect[4];
					sphere2Intersect[2] = sphere2Intersect[5];
				}

				//redefines ray to be the line normal to sphere2 at "sphere2Intersect"
				makeLine(ray, sphere2Intersect[0], sphere2Intersect[1], sphere2Intersect[2], sphere2[0], sphere2[1], sphere2[2]);

				//"NormLensPlane" is the point where the normal line of Sphere 2 intersects the lens plane
				float t = (distance - ray[0]) / ray[3];
				float xNormLensPlane = distance;
				float yNormLensPlane = ray[1] + ray[4] * t;
				float zNormLensPlane = ray[2] + ray[5] * t;

				//A horizontal line on the x-axis is drawn from "sphere2Intersect" and intersects with the lens plane at "xHorizPlane"
				float yLensVector = yNormLensPlane - yLensPlane;
				float zLensVector = zNormLensPlane - zLensPlane;

				t = (yLensVector*(sphere2Intersect[1]-yNormLensPlane)+zLensVector*(sphere2Intersect[2]-zNormLensPlane))/(yLensVector*yLensVector+zLensVector*zLensVector);

				float xHorizPlane = distance;
				float yHorizPlane = yNormLensPlane + yLensVector*t;
				float zHorizPlane = zNormLensPlane + zLensVector*t;

				//The points have been calculated to the same plane, so the following is analyzed in two dimentions:
				float XDelta = hypot3d(xHorizPlane - sphere2Intersect[0], yHorizPlane - sphere2Intersect[1], zHorizPlane - sphere2Intersect[2]);//Distance of "HorizPlane" to "sphere2Intersec"
				float YDelta1 = hypot3d(xHorizPlane - xLensPlane, yHorizPlane - yLensPlane, zHorizPlane - zLensPlane);//distance of "HorizPlane" to "LensPlane"
				float YDelta2 = hypot3d(xHorizPlane - xNormLensPlane, yHorizPlane - yNormLensPlane, zHorizPlane - zNormLensPlane);//Distance of "HorizPlane" to "NormLensPlane
				float testDelta = hypot3d(xLensPlane - xNormLensPlane, yLensPlane - yNormLensPlane, zLensPlane - zNormLensPlane);

				float theta1 = atan(YDelta1 / XDelta);//angle of: "sphere2Intersect" to "HorizPlane" to "LensPlane"
				float theta2 = atan(YDelta2 / XDelta);//angle of: "sphere2Interesect" to "HorizPlane" to "NormLensPlane"
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
				float newX = xHorizPlane;
				float newY = yHorizPlane + newYDelta*(yLensPlane - yHorizPlane) / YDelta1;//"new" is the new value for "LensPlane" when it refracts through the glass
				float newZ = zHorizPlane + newYDelta*(zLensPlane - zHorizPlane) / YDelta1;

				makeLine(ray, sphere2Intersect[0], sphere2Intersect[1], sphere2Intersect[2], newX, newY, newZ);//The ray's path through the glass

				//Use [3,4,5]
				float sphere1Intersect[6];
				intersectSphereRay(sphere1Intersect, sphere1, ray);//The rays intersect with "sphere2" (where it exits the glass of the lens on the other side)

				if (sphere1Intersect[3] > sphere1Intersect[0]) {//Makes sure the intersect is part of the sphere that is part of the lens
					sphere1Intersect[3] = sphere1Intersect[0];
					sphere1Intersect[4] = sphere1Intersect[1];
					sphere1Intersect[5] = sphere1Intersect[2];
				}

				makeLine(ray, sphere1Intersect[3], sphere1Intersect[4], sphere1Intersect[5], sphere1[0], sphere1[1], sphere1[2]);//the normal line to the sphere at "sphere1Intersect"

				t = (distance - ray[0]) / ray[3];
				xNormLensPlane = distance;//"NormLensPlane" is where the normal line intersects the lens plane
				yNormLensPlane = ray[1] + ray[4] * t;
				zNormLensPlane = ray[2] + ray[5] * t;

				//calculations for xHoriz Plane
				yLensVector = yNormLensPlane - newY;
				zLensVector = zNormLensPlane - newZ;

				t = (yLensVector*(sphere1Intersect[4]-yNormLensPlane)+zLensVector*(sphere1Intersect[5]-zNormLensPlane))/(yLensVector*yLensVector+zLensVector*zLensVector);

				xHorizPlane = distance;//Used to simplify the calculation of a new ray to two dimensions.
				yHorizPlane = yNormLensPlane + yLensVector*t;
				zHorizPlane = zNormLensPlane + zLensVector*t;

				XDelta = hypot3d(xHorizPlane - sphere1Intersect[3], yHorizPlane - sphere1Intersect[4], zHorizPlane - sphere1Intersect[5]);//same as before
				YDelta1 = hypot3d(xHorizPlane - newX, yHorizPlane - newY, zHorizPlane - newZ);
				YDelta2 = hypot3d(xHorizPlane - xNormLensPlane, yHorizPlane - yNormLensPlane, zHorizPlane - zNormLensPlane);
				testDelta = hypot3d(newX - xNormLensPlane, newY - yNormLensPlane, newZ - zNormLensPlane);

				theta1 = atan(YDelta1 / XDelta);
				theta2 = atan(YDelta2 / XDelta);

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
				newX = xHorizPlane;//"new" is now a reference of where the eixiting ray would go it it went through the lens plane
				newY = yHorizPlane + newYDelta*(newY - yHorizPlane) / YDelta1;
				newZ = zHorizPlane + newYDelta*(newZ - zHorizPlane) / YDelta1;

				makeLine(ray, sphere1Intersect[3], sphere1Intersect[4], sphere1Intersect[5], newX, newY, newZ);//ray is now the exiting ray

				float checkPlane[4] = { 1,0,0,5 };//A plane that divides what is infront of the camera and what is behind; used to ensure behind objects are not counted.
				float origin[3] = { 0,0,0 };

				int RGBAColors[4];
				traceRay(RGBAColors, ray, viewpoint, 3, checkPlane, origin, -1, i, j);

				RGBATotalColors[0] += RGBAColors[0];
				RGBATotalColors[1] += RGBAColors[1];
				RGBATotalColors[2] += RGBAColors[2];
				RGBATotalColors[3] += RGBAColors[3];

				k += 1;
			}

			RGBATotalColors[0] /= numOfSamples;
			RGBATotalColors[1] /= numOfSamples;
			RGBATotalColors[2] /= numOfSamples;
			RGBATotalColors[3] /= numOfSamples;

			//##################	End of depth of field section	 ##############################

			//################	Non point of view code ########################

			/*float checkPlane[4] = { 1,0,0,5 };
			float origin[3] = { 0,0,0 };
			
			float ray[6];
			makeLine(ray, 5, 0, 0, 0, ((float)(2 * i) / (float)(windowWidth)-1) / FoVconst * distance, -((float)(2 * j - windowHeight) / (float)(windowWidth)) / FoVconst * distance);

			int RGBATotalColors[4];
			traceRay(RGBATotalColors, ray, viewpoint, 9, checkPlane, origin, -1, i ,j);*/

			//################	Non point of view code ########################

			pixelBuffer[windowWidth*j + i] = colorArraytoInt(RGBATotalColors);
		}
    }
}