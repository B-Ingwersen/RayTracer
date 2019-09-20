int colorArraytoInt(int colors[4]) {
	return 256 * 256 * colors[0] + 256 * colors[1] + colors[2];
}

float angleBetweenVectors(float x1, float y1, float z1, float x2, float y2, float z2) {
	float val = (x1*x2 + y1*y2 + z1*z2) / sqrt((x1*x1 + y1*y1 + z1*z1)*(x2*x2 + y2*y2 + z2*z2));
	return val;
}

float hypot3d(float x, float y, float z) {
	return sqrt(x*x + y*y + z*z);
}
double doubleHypot3d(double x, double y, double z) {
	return sqrt(x*x + y*y + z*z);
}
float distBetweenPoints(float point1[3], float point2[3]) {
	return hypot3d(point1[0] - point2[0], point1[1] - point2[1], point1[2] - point2[2]);
}

void makeLine(float outputLine[6], float x1, float y1, float z1, float x2, float y2, float z2) {
	float Vx = x2 - x1;
	float Vy = y2 - y1;
	float Vz = z2 - z1;
	//float D = hypot3d(Vx, Vy, Vz);
	outputLine[0] = x1;
	outputLine[1] = y1;
	outputLine[2] = z1;
	/*outputLine[3] = Vx / D;
	outputLine[4] = Vy / D;
	outputLine[5] = Vz / D;*/
	outputLine[3] = Vx;
	outputLine[4] = Vy;
	outputLine[5] = Vz;
}

bool intersectSphereRay(float IntersectOut[6], float sphere[4], float ray[6]) {
	float x1 = ray[0];
	float y1 = ray[1];
	float z1 = ray[2];
	float X = ray[3];
	float Y = ray[4];
	float Z = ray[5];

	float X1 = sphere[0];
	float Y1 = sphere[1];
	float Z1 = sphere[2];
	float R = sphere[3];

	//A,B, and C are defined as the coefficients of a quadratic formula for the intersection of the ray and the sphere
	float A = X*X + Y*Y + Z*Z;
	float B = 2 * (X*x1 + Y*y1 + Z*z1 - X*X1 - Y*Y1 - Z*Z1);
	float C = X1*X1 + x1*x1 + Y1*Y1 + y1*y1 + Z1*Z1 + z1*z1 - 2 * X1*x1 - 2 * Y1*y1 - 2 * Z1*z1 - R*R;
	float det = B*B - 4 * A*C;
	//The determinant of the quad form is checked; negative result means that the objects don't intersect and this step is skipped
	if (det >= 0) {
		//The two intersects are computed and stored as (X1,Y1,Z1) and (X2,Y2,Z2)
		det = sqrt(det);
		float t = (-B + det) / (2 * A);

		float X1 = x1 + X*t;
		float Y1 = y1 + Y*t;
		float Z1 = z1 + Z*t;

		t = (-B - det) / (2 * A);

		float X2 = x1 + X*t;
		float Y2 = y1 + Y*t;
		float Z2 = z1 + Z*t;

		IntersectOut[0] = X1;
		IntersectOut[1] = Y1;
		IntersectOut[2] = Z1;
		IntersectOut[3] = X2;
		IntersectOut[4] = Y2;
		IntersectOut[5] = Z2;

		return true;
	}

	else {
		return false;
	}
}

void intersectPlaneRay(float IntersectOut[3], float plane[4], float ray[6]) {
	float x1 = ray[0];
	float y1 = ray[1];
	float z1 = ray[2];
	float X = ray[3];
	float Y = ray[4];
	float Z = ray[5];
	
	float A = plane[0];
	float B = plane[1];
	float C = plane[2];
	float D = plane[3];

	float t = (D - (A*x1 + B*y1 + C*z1)) / (A*X + B*Y + C*Z);

	IntersectOut[0] = x1 + X*t;
	IntersectOut[1] = y1 + Y*t;
	IntersectOut[2] = z1 + Z*t;
}

bool intersectHCyl(float IntersectOut[6], float Y1, float Z1, float R, float ray[6]) {
	float x1 = ray[0];
	float y1 = ray[1];
	float z1 = ray[2];
	float X = ray[3];
	float Y = ray[4];
	float Z = ray[5];

	//A,B, and C are defined as the coefficients of a quadratic formula for the intersection of the ray and the sphere
	float A = Y*Y + Z*Z;
	float B = 2 * (Y*y1 + Z*z1 - Y*Y1 - Z*Z1);
	float C = Y1*Y1 + y1*y1 + Z1*Z1 + z1*z1 - 2 * Y1*y1 - 2 * Z1*z1 - R*R;

	float det = B*B - 4 * A*C;
	//The determinant of the quad form is checked; negative result means that the objects don't intersect and this step is skipped
	if (det >= 0) {
		//The two intersects are computed and stored as (X1,Y1,Z1) and (X2,Y2,Z2)
		det = sqrt(det);
		float t = (-B + det) / (2 * A);

		float X1 = x1 + X*t;
		float Y1 = y1 + Y*t;
		float Z1 = z1 + Z*t;

		t = (-B - det) / (2 * A);

		float X2 = x1 + X*t;
		float Y2 = y1 + Y*t;
		float Z2 = z1 + Z*t;

		IntersectOut[0] = X1;
		IntersectOut[1] = Y1;
		IntersectOut[2] = Z1;
		IntersectOut[3] = X2;
		IntersectOut[4] = Y2;
		IntersectOut[5] = Z2;

		return true;
	}

	else {
		return false;
	}
}

bool intersectWCyl(float IntersectOut[6], float X1, float Z1, float R, float ray[6]) {
	float x1 = ray[0];
	float y1 = ray[1];
	float z1 = ray[2];
	float X = ray[3];
	float Y = ray[4];
	float Z = ray[5];

	//A,B, and C are defined as the coefficients of a quadratic formula for the intersection of the ray and the sphere
	float A = X*X + Z*Z;
	float B = 2 * (X*x1 + Z*z1 - X*X1 - Z*Z1);
	float C = X1*X1 + x1*x1 + Z1*Z1 + z1*z1 - 2 * X1*x1 - 2 * Z1*z1 - R*R;

	float det = B*B - 4 * A*C;
	//The determinant of the quad form is checked; negative result means that the objects don't intersect and this step is skipped
	if (det >= 0) {
		//The two intersects are computed and stored as (X1,Y1,Z1) and (X2,Y2,Z2)
		det = sqrt(det);
		float t = (-B + det) / (2 * A);

		float X1 = x1 + X*t;
		float Y1 = y1 + Y*t;
		float Z1 = z1 + Z*t;

		t = (-B - det) / (2 * A);

		float X2 = x1 + X*t;
		float Y2 = y1 + Y*t;
		float Z2 = z1 + Z*t;

		IntersectOut[0] = X1;
		IntersectOut[1] = Y1;
		IntersectOut[2] = Z1;
		IntersectOut[3] = X2;
		IntersectOut[4] = Y2;
		IntersectOut[5] = Z2;

		return true;
	}

	else {
		return false;
	}
}

int intersectTiledFloor(float IntersectOut[24], float NormalVecOut[24], float plane[4], float ray[6], int i, int j) {
	//Plane Intersect
	float x1 = ray[0];
	float y1 = ray[1];
	float z1 = ray[2];
	float X = ray[3];
	float Y = ray[4];
	float Z = ray[5];

	float A = plane[0];
	float B = plane[1];
	float C = plane[2];
	float D = plane[3];

	float t = (D - (A*x1 + B*y1 + C*z1)) / (A*X + B*Y + C*Z);

	//Intersect of the upper plane with the ray
	IntersectOut[0] = x1 + X*t;
	IntersectOut[1] = y1 + Y*t;
	IntersectOut[2] = z1 + Z*t;

	float GapWidth = 0.03;//half the width between top of tiles
	float lipRadius = 0.03;//depth between the tiles; should be identical to "GapWidth"
	float TileWidth = 0.25;

	//WLoc and Hloc are the "grid coordinates" of the closest point.
	float Wloc = (float)(round(IntersectOut[0]/TileWidth))*TileWidth;
	float Hloc = (float)(round(IntersectOut[1]/TileWidth))*TileWidth;
	float W = fmodf(fabs(IntersectOut[0]), TileWidth);//modulo of the location of the intersect
	float H = fmodf(fabs(IntersectOut[1]), TileWidth);

	int numOfIntersects = 0;//counter of number of intersects indentified

	if ((H >= GapWidth && H <= (TileWidth - GapWidth)) && (W >= GapWidth && W <= (TileWidth - GapWidth))) {
		numOfIntersects = 3;
		NormalVecOut[0] = plane[0];
		NormalVecOut[1] = plane[1];
		NormalVecOut[2] = plane[2];
	}
	else {
		if (H <= GapWidth || H >= (TileWidth - GapWidth)) { //determine if within GapWidth of grid lines		
		float edgeIntersects[6];
		bool intersect = intersectHCyl(edgeIntersects, Hloc - GapWidth, plane[3] - lipRadius, lipRadius, ray);//intersect cylinder parallel to x axis
		if (intersect) {
			if (edgeIntersects[1] >= Hloc - GapWidth && edgeIntersects[2] >= plane[3] - lipRadius) {//checks 1st intersect to be in quarter of cylinder that is visible
				float myW = fmodf(fabs(edgeIntersects[0]), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }//distance from grid of intersects
				float myH = fmodf(fabs(edgeIntersects[1]), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
				if (myW >= myH) {
					IntersectOut[numOfIntersects] = edgeIntersects[0];
					IntersectOut[numOfIntersects + 1] = edgeIntersects[1];
					IntersectOut[numOfIntersects + 2] = edgeIntersects[2];
					NormalVecOut[numOfIntersects] = 0;
					NormalVecOut[numOfIntersects + 1] = edgeIntersects[1] - (Hloc - GapWidth);
					NormalVecOut[numOfIntersects + 2] = edgeIntersects[2] - (plane[3] - lipRadius);
					numOfIntersects += 3;
				}
			}
			if (edgeIntersects[4] >= Hloc - GapWidth && edgeIntersects[5] >= plane[3] - lipRadius) {//checks 2nd intersect to be in quarter of cylinder that is visible
				float myW = fmodf(fabs(edgeIntersects[3]), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }
				float myH = fmodf(fabs(edgeIntersects[4]), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
				if (myW >= myH) {
					IntersectOut[numOfIntersects] = edgeIntersects[3];
					IntersectOut[numOfIntersects + 1] = edgeIntersects[4];
					IntersectOut[numOfIntersects + 2] = edgeIntersects[5];
					NormalVecOut[numOfIntersects] = 0;
					NormalVecOut[numOfIntersects + 1] = edgeIntersects[4] - (Hloc - GapWidth);
					NormalVecOut[numOfIntersects + 2] = edgeIntersects[5] - (plane[3] - lipRadius);
					numOfIntersects += 3;
				}
			}
		}
		intersect = intersectHCyl(edgeIntersects, Hloc + GapWidth, plane[3] - lipRadius, lipRadius, ray);
		if (intersect) {
			if (edgeIntersects[1] <= Hloc + GapWidth && edgeIntersects[2] >= plane[3] - lipRadius) {
				float myW = fmodf(fabs(edgeIntersects[0]), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }
				float myH = fmodf(fabs(edgeIntersects[1]), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
				if (myW >= myH) {
					IntersectOut[numOfIntersects] = edgeIntersects[0];
					IntersectOut[numOfIntersects + 1] = edgeIntersects[1];
					IntersectOut[numOfIntersects + 2] = edgeIntersects[2];
					NormalVecOut[numOfIntersects] = 0;
					NormalVecOut[numOfIntersects + 1] = edgeIntersects[1] - (Hloc + GapWidth);
					NormalVecOut[numOfIntersects + 2] = edgeIntersects[2] - (plane[3] - lipRadius);
					numOfIntersects += 3;
				}
			}
			if (edgeIntersects[4] <= Hloc + GapWidth && edgeIntersects[5] >= plane[3] - lipRadius) {
				float myW = fmodf(fabs(edgeIntersects[3]), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }
				float myH = fmodf(fabs(edgeIntersects[4]), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
				if (myW >= myH) {
					IntersectOut[numOfIntersects] = edgeIntersects[3];
					IntersectOut[numOfIntersects + 1] = edgeIntersects[4];
					IntersectOut[numOfIntersects + 2] = edgeIntersects[5];
					NormalVecOut[numOfIntersects] = 0;
					NormalVecOut[numOfIntersects + 1] = edgeIntersects[4] - (Hloc + GapWidth);
					NormalVecOut[numOfIntersects + 2] = edgeIntersects[5] - (plane[3] - lipRadius);
					numOfIntersects += 3;
				}
			}
		}
		}
	
		float edgeIntersects[6];
		bool intersect = intersectWCyl(edgeIntersects, Wloc - GapWidth, plane[3] - lipRadius, lipRadius, ray);
		if (intersect) {
			if (edgeIntersects[0] >= Wloc - GapWidth && edgeIntersects[2] >= plane[3] - lipRadius) {
				float myW = fmodf(fabs(edgeIntersects[0]), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }
				float myH = fmodf(fabs(edgeIntersects[1]), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
				if (myH >= myW) {
					IntersectOut[numOfIntersects] = edgeIntersects[0];
					IntersectOut[numOfIntersects + 1] = edgeIntersects[1];
					IntersectOut[numOfIntersects + 2] = edgeIntersects[2];
					NormalVecOut[numOfIntersects] = edgeIntersects[0] - (Wloc - GapWidth);
					NormalVecOut[numOfIntersects + 1] = 0;
					NormalVecOut[numOfIntersects + 2] = edgeIntersects[2] - (plane[3] - lipRadius);
					numOfIntersects += 3;
				}
			}
			if (edgeIntersects[3] >= Wloc - GapWidth && edgeIntersects[5] >= plane[3] - lipRadius) {
				float myW = fmodf(fabs(edgeIntersects[3]), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }
				float myH = fmodf(fabs(edgeIntersects[4]), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
				if (myH >= myW) {
					IntersectOut[numOfIntersects] = edgeIntersects[3];
					IntersectOut[numOfIntersects + 1] = edgeIntersects[4];
					IntersectOut[numOfIntersects + 2] = edgeIntersects[5];
					NormalVecOut[numOfIntersects] = edgeIntersects[3] - (Wloc - GapWidth);;
					NormalVecOut[numOfIntersects + 1] = 0;
					NormalVecOut[numOfIntersects + 2] = edgeIntersects[5] - (plane[3] - lipRadius);
					numOfIntersects += 3;
				}
			}
		}

		intersect = intersectWCyl(edgeIntersects, Wloc + GapWidth, plane[3] - lipRadius, lipRadius, ray);
		if (intersect) {
			if (edgeIntersects[0] <= Wloc + GapWidth && edgeIntersects[2] >= plane[3] - lipRadius) {
				float myW = fmodf(fabs(edgeIntersects[0]), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }
				float myH = fmodf(fabs(edgeIntersects[1]), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
				if (myH >= myW) {
					IntersectOut[numOfIntersects] = edgeIntersects[0];
					IntersectOut[numOfIntersects + 1] = edgeIntersects[1];
					IntersectOut[numOfIntersects + 2] = edgeIntersects[2];
					NormalVecOut[numOfIntersects] = edgeIntersects[0] - (Wloc + GapWidth);
					NormalVecOut[numOfIntersects + 1] = 0;
					NormalVecOut[numOfIntersects + 2] = edgeIntersects[2] - (plane[3] - lipRadius);
					numOfIntersects += 3;
				}
			}
			if (edgeIntersects[3] <= Wloc + GapWidth && edgeIntersects[5] >= plane[3] - lipRadius) {
				float myW = fmodf(fabs(edgeIntersects[3]), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }
				float myH = fmodf(fabs(edgeIntersects[4]), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
				if (myH >= myW) {
					IntersectOut[numOfIntersects] = edgeIntersects[3];
					IntersectOut[numOfIntersects + 1] = edgeIntersects[4];
					IntersectOut[numOfIntersects + 2] = edgeIntersects[5];
					NormalVecOut[numOfIntersects] = edgeIntersects[3] - (Wloc + GapWidth);
					NormalVecOut[numOfIntersects + 1] = 0;
					NormalVecOut[numOfIntersects + 2] = edgeIntersects[5] - (plane[3] - lipRadius);
					numOfIntersects += 3;
				}
			}
		}
	}

	return numOfIntersects;
}

bool findNearIntersect(float IntersectOut[3], float NormalVector[3], float ColorReference[4], int objectCountOut[1], float ray[4], float objects[], float refPoint[3], float checkPlane[4], float checkPoint[3], int ObjectIndex, int I, int J) {

	float sphereRefl = 3;
	float planeRefl = 1;

	int checkSign;
	if (checkPlane[0] * checkPoint[0] + checkPlane[1] * checkPoint[1] + checkPlane[2] * checkPoint[2] > checkPlane[3]) {
		checkSign = 1;
	}
	else { checkSign = -1; }

	float xr = refPoint[0];
	float yr = refPoint[1];
	float zr = refPoint[2];

	float minDistance = FLT_MAX;
	bool foundIntersect = false;

	bool Continue = true;
	int i = 0;
	int objectCounter = 0;

	while (Continue) {
		//0: EXIT
		if (objects[i] == 0) {Continue = false;}

		//1:PLANE
		else if (objects[i] == 1) {
			float PlaneIntersect[3];
			float plane[4] = { objects[i + 1],objects[i + 2],objects[i + 3],objects[i + 4] };
			intersectPlaneRay(PlaneIntersect, plane, ray);

			float distance = distBetweenPoints(PlaneIntersect, refPoint);
			if (distance < minDistance && objectCounter != ObjectIndex) {
				if (checkSign*(checkPlane[0] * PlaneIntersect[0] + checkPlane[1] * PlaneIntersect[1] + checkPlane[2] * PlaneIntersect[2]) > checkPlane[3] * checkSign && objectCounter != ObjectIndex) {
					minDistance = distance;
					IntersectOut[0] = PlaneIntersect[0];
					IntersectOut[1] = PlaneIntersect[1];
					IntersectOut[2] = PlaneIntersect[2];

					NormalVector[0] = objects[i + 1];
					NormalVector[1] = objects[i + 2];
					NormalVector[2] = objects[i + 3];

					if (plane[3] > -0.33 && plane[3] < -0.31) {
						ColorReference[0] = .6;
						ColorReference[1] = .6;
						ColorReference[2] = .6;
						ColorReference[3] = planeRefl;
					}
					else {
						ColorReference[0] = .9;
						ColorReference[1] = .9;
						ColorReference[2] = .9;
						ColorReference[3] = planeRefl;
					}
	
					foundIntersect = true;
					objectCountOut[0] = objectCounter;
				}
			}
			i += 5;
			objectCounter += 1;
		}

		//2:SPHERE
		else if (objects[i] == 2) {
			float SphereIntersects[6];
			float sphere[4] = { objects[i + 1], objects[i + 2], objects[i + 3], objects[i + 4] };
			bool intersect;
			intersect = intersectSphereRay(SphereIntersects, sphere, ray);
			if (intersect) {

				if (objectCounter == ObjectIndex) {
					float distance1 = hypot3d(xr - SphereIntersects[0], yr - SphereIntersects[1], zr - SphereIntersects[2]);
					float distance2 = hypot3d(xr - SphereIntersects[3], yr - SphereIntersects[4], zr - SphereIntersects[5]);

					if (distance2 < distance1 && distance1 < minDistance) {
						if (checkSign*(checkPlane[0] * SphereIntersects[0] + checkPlane[1] * SphereIntersects[1] + checkPlane[2] * SphereIntersects[2]) > checkPlane[3] * checkSign) {
							minDistance = distance1;
							IntersectOut[0] = SphereIntersects[0];
							IntersectOut[1] = SphereIntersects[1];
							IntersectOut[2] = SphereIntersects[2];

							NormalVector[0] = IntersectOut[0] - objects[i + 1];
							NormalVector[1] = IntersectOut[1] - objects[i + 2];
							NormalVector[2] = IntersectOut[2] - objects[i + 3];

							ColorReference[0] = 1;
							ColorReference[1] = 0;
							ColorReference[2] = 0;
							ColorReference[3] = sphereRefl;

							foundIntersect = true;
							objectCountOut[0] = objectCounter;
						}
					}

					else if (distance1 < distance2 && distance2 < minDistance) {
						if (checkSign*(checkPlane[0] * SphereIntersects[3] + checkPlane[1] * SphereIntersects[4] + checkPlane[2] * SphereIntersects[5]) > checkPlane[3] * checkSign) {
							minDistance = distance2;
							IntersectOut[0] = SphereIntersects[3];
							IntersectOut[1] = SphereIntersects[4];
							IntersectOut[2] = SphereIntersects[5];

							NormalVector[0] = IntersectOut[0] - objects[i + 1];
							NormalVector[1] = IntersectOut[1] - objects[i + 2];
							NormalVector[2] = IntersectOut[2] - objects[i + 3];

							ColorReference[0] = 1;
							ColorReference[1] = 0;
							ColorReference[2] = 0;
							ColorReference[3] = sphereRefl;

							foundIntersect = true;
							objectCountOut[0] = objectCounter;
						}
					}

				}
				else {
					float distance = hypot3d(xr - SphereIntersects[0], yr - SphereIntersects[1], zr - SphereIntersects[2]);
					if (distance < minDistance && distance > 0.0001) {
						if (checkSign*(checkPlane[0] * SphereIntersects[0] + checkPlane[1] * SphereIntersects[1] + checkPlane[2] * SphereIntersects[2]) > checkPlane[3] * checkSign) {
							minDistance = distance;
							IntersectOut[0] = SphereIntersects[0];
							IntersectOut[1] = SphereIntersects[1];
							IntersectOut[2] = SphereIntersects[2];

							NormalVector[0] = IntersectOut[0] - objects[i + 1];
							NormalVector[1] = IntersectOut[1] - objects[i + 2];
							NormalVector[2] = IntersectOut[2] - objects[i + 3];

							ColorReference[0] = 1;
							ColorReference[1] = 0;
							ColorReference[2] = 0;
							ColorReference[3] = sphereRefl;

							foundIntersect = true;
							objectCountOut[0] = objectCounter;
						}
					}

					distance = hypot3d(xr - SphereIntersects[3], yr - SphereIntersects[4], zr - SphereIntersects[5]);
					if (distance < minDistance && distance > 0.0001) {
						if (checkSign*(checkPlane[0] * SphereIntersects[3] + checkPlane[1] * SphereIntersects[4] + checkPlane[2] * SphereIntersects[5]) > checkPlane[3] * checkSign) {
							minDistance = distance;
							IntersectOut[0] = SphereIntersects[3];
							IntersectOut[1] = SphereIntersects[4];
							IntersectOut[2] = SphereIntersects[5];

							NormalVector[0] = IntersectOut[0] - objects[i + 1];
							NormalVector[1] = IntersectOut[1] - objects[i + 2];
							NormalVector[2] = IntersectOut[2] - objects[i + 3];

							ColorReference[0] = 1;
							ColorReference[1] = 0;
							ColorReference[2] = 0;
							ColorReference[3] = sphereRefl;

							foundIntersect = true;
							objectCountOut[0] = objectCounter;
						}
					}
				}
			}
			i += 5;
			objectCounter += 1;
		}

		//3:TILED FLOOR
		else if (objects[i] == 3) {
			float plane[4] = { objects[i + 1],objects[i + 2],objects[i + 3],objects[i + 4] };

			float tileIntersects[24];
			float tileNormVecs[24];

			int numOfIntersects = intersectTiledFloor(tileIntersects, tileNormVecs, plane, ray,I,J);
			int j = 0;

			float minCheckDistance = FLT_MAX;
			int bannedIndex = -1;

			if (objectCounter == ObjectIndex) {
				while (j < numOfIntersects) {
					float distance = hypot3d(xr - tileIntersects[j], yr - tileIntersects[j + 1], zr - tileIntersects[j + 2]);
					if (distance < minCheckDistance) {
						minCheckDistance = distance;
						bannedIndex = j;
					}
					j += 3;
				}
			}

			j = 0;

			while (j < numOfIntersects) {
				float distance = hypot3d(xr - tileIntersects[j], yr - tileIntersects[j + 1], zr - tileIntersects[j + 2]);
				if (distance < minDistance && j != bannedIndex) {
					if (checkSign*(checkPlane[0] * tileIntersects[j] + checkPlane[1] * tileIntersects[j + 1] + checkPlane[2] * tileIntersects[j + 2]) > checkPlane[3] * checkSign) {
						minDistance = distance;

						IntersectOut[0] = tileIntersects[j];
						IntersectOut[1] = tileIntersects[j + 1];
						IntersectOut[2] = tileIntersects[j + 2];

						NormalVector[0] = tileNormVecs[j];
						NormalVector[1] = tileNormVecs[j + 1];
						NormalVector[2] = tileNormVecs[j + 2];

						ColorReference[0] = 0.95;
						ColorReference[1] = 0.95;
						ColorReference[2] = 0.95;
						ColorReference[3] = 2;

						foundIntersect = true;
						objectCountOut[0] = objectCounter;
					}
				}
				j += 3;
			}

			i += 5;
			objectCounter += 1;
		}
	}

	return foundIntersect;
}

bool findIfIntersect2(float objects[], float refPoint[3], float toPoint[3], int ObjectIndex) {

	int comparisonIndex;
	if (refPoint[0] != toPoint[0]) {
		comparisonIndex = 0;
	}
	else if (refPoint[1] != toPoint[1]) {
		comparisonIndex = 0;
	}
	else if (refPoint[2] != toPoint[2]) {
		comparisonIndex = 2;
	}

	/*float a = (refPoint[1] - toPoint[1]) / (refPoint[0] - toPoint[0]);
	float b = refPoint[1] - refPoint[0] * a;
	float c = (refPoint[2] - toPoint[2]) / (refPoint[0] - toPoint[0]);
	float d = refPoint[2] - refPoint[0] * c;
	float ray[4] = { a,b,c,d };*/

	float ray[6];
	makeLine(ray, refPoint[0], refPoint[1], refPoint[2], toPoint[0], toPoint[1], toPoint[2]);

	float xr = refPoint[0];
	float yr = refPoint[1];
	float zr = refPoint[2];

	bool Continue = true;
	int i = 0;
	int objectCounter = 0;

	while (Continue) {
		//0: EXIT
		if (objects[i] == 0) { Continue = false; }

		//1:PLANE
		else if (objects[i] == 1) {
			float PlaneIntersect[3];
			float plane[4] = { objects[i + 1],objects[i + 2],objects[i + 3],objects[i + 4] };
			intersectPlaneRay(PlaneIntersect, plane, ray);

			float distance = distBetweenPoints(PlaneIntersect, refPoint);
			if (objectCounter != ObjectIndex) {
				if (PlaneIntersect[comparisonIndex] < refPoint[comparisonIndex] && PlaneIntersect[comparisonIndex] > toPoint[comparisonIndex]) {
					return true;
				}
				else if (PlaneIntersect[comparisonIndex] > refPoint[comparisonIndex] && PlaneIntersect[comparisonIndex] < toPoint[comparisonIndex]) {
					return true;
				}
			}

			i += 5;
			objectCounter += 1;
		}

		//2:SPHERE
		else if (objects[i] == 2) {
			float SphereIntersects[6];
			float sphere[4] = { objects[i + 1], objects[i + 2], objects[i + 3], objects[i + 4] };
			bool intersect;
			intersect = intersectSphereRay(SphereIntersects, sphere, ray);
			if (intersect) {
				if (objectCounter == ObjectIndex) {
					float distance1 = hypot3d(xr - SphereIntersects[0], yr - SphereIntersects[1], zr - SphereIntersects[2]);
					float distance2 = hypot3d(xr - SphereIntersects[3], yr - SphereIntersects[4], zr - SphereIntersects[5]);

					if (distance2 < distance1) {
						if (SphereIntersects[comparisonIndex] < refPoint[comparisonIndex] && SphereIntersects[comparisonIndex] > toPoint[comparisonIndex]) {
							return true;
						}
						else if (SphereIntersects[comparisonIndex] > refPoint[comparisonIndex] && SphereIntersects[comparisonIndex] < toPoint[comparisonIndex]) {
							return true;
						}
					}

					else if (distance1 < distance2) {
						if (SphereIntersects[comparisonIndex + 3] < refPoint[comparisonIndex] && SphereIntersects[comparisonIndex + 3] > toPoint[comparisonIndex]) {
							return true;
						}
						else if (SphereIntersects[comparisonIndex + 3] > refPoint[comparisonIndex] && SphereIntersects[comparisonIndex + 3] < toPoint[comparisonIndex]) {
							return true;
						}
					}

				}
				else {
					if (SphereIntersects[comparisonIndex] < refPoint[comparisonIndex] && SphereIntersects[comparisonIndex] > toPoint[comparisonIndex]) {
						return true;
					}
					else if (SphereIntersects[comparisonIndex] > refPoint[comparisonIndex] && SphereIntersects[comparisonIndex] < toPoint[comparisonIndex]) {
						return true;
					}

					if (SphereIntersects[comparisonIndex + 3] < refPoint[comparisonIndex] && SphereIntersects[comparisonIndex + 3] > toPoint[comparisonIndex]) {
						return true;
					}
					else if (SphereIntersects[comparisonIndex + 3] > refPoint[comparisonIndex] && SphereIntersects[comparisonIndex + 3] < toPoint[comparisonIndex]) {
						return true;
					}
				}
			}
			i += 5;
			objectCounter += 1;
		}

		//3:TILED FLOOR
		else if (objects[i] == 3) {
			float plane[4] = { objects[i + 1],objects[i + 2],objects[i + 3],objects[i + 4] };

			float tileIntersects[24];
			float tileNormVecs[24];

			int numOfIntersects = intersectTiledFloor(tileIntersects, tileNormVecs, plane, ray, 0, 0);
			int j = 0;

			float minCheckDistance = FLT_MAX;
			int bannedIndex = -1;

			if (objectCounter == ObjectIndex) {
				while (j < numOfIntersects) {
					float distance = hypot3d(xr - tileIntersects[j], yr - tileIntersects[j + 1], zr - tileIntersects[j + 2]);
					if (distance < minCheckDistance) {
						minCheckDistance = distance;
						bannedIndex = j;
					}
					j += 3;
				}
			}

			j = 0;

			while (j < numOfIntersects) {
				if (j != bannedIndex) {
					if (tileIntersects[comparisonIndex + j] < refPoint[comparisonIndex] && tileIntersects[comparisonIndex + j] > toPoint[comparisonIndex]) {
						return true;
					}
					else if (tileIntersects[comparisonIndex + j] > refPoint[comparisonIndex] && tileIntersects[comparisonIndex + j] < toPoint[comparisonIndex]) {
						return true;
					}
				}
				j += 3;
			}
			i += 5;
			objectCounter += 1;
		}
	}
	return false;
}

void findReflectedRay(float OutputRay[6], float ray[6], float reflPoint[3], float normalVec[3]) {

	float a = ray[0];
	float b = ray[1];
	float c = ray[2];
	float d = ray[3];

	//{Ap,Bp,Cp} is a vector normal to the plane tangent to the sphere at (X1,Y1,Z1)
	//Ap*x+Bp*y+Cp*z = Dp is a plane that is:
	//	-parallel to the one tangent to the sphere at (X1,Y1,Z1)
	//	-through the point (x1,y1,z1)
	float Ap = normalVec[0];
	float Bp = normalVec[1];
	float Cp = normalVec[2];
	float Dp = Ap*(reflPoint[0]+normalVec[0]) + Bp*(reflPoint[1] + normalVec[1]) + Cp*(reflPoint[2] + normalVec[2]);
	float plane[4] = { Ap,Bp,Cp,Dp };

	//(x,y,z) is the intersect betweent the previously defined plane and the original ray
	float newInt[3];
	intersectPlaneRay(newInt, plane, ray);
	/*float x = (Dp - Bp*b - Cp*d) / (Ap + Bp*a + Cp*c);
	float y = a*x + b;
	float z = c*x + d;*/

	//(newX,newY,newZ) is a point opposite of (x,y,z) from (x1,y1,z1)
	float newX = 2 * (reflPoint[0] + normalVec[0]) - newInt[0];
	float newY = 2 * (reflPoint[1] + normalVec[1]) - newInt[1];
	float newZ = 2 * (reflPoint[2] + normalVec[2]) - newInt[2];

	makeLine(OutputRay, reflPoint[0], reflPoint[1], reflPoint[2], newX, newY, newZ);
}

void traceRay(int outputColors[4], float ray[6], float refPoint[3], int recursionLevel, float checkPlane[4], float checkPoint[3], int ObjectIndexIn, int i, int j) {

	// The objects in the scene are defined in this array
	// 0: Termination signal
	// 1: Plane { 1, A, B, C, D} where A*x+By+Cz=D
	// 2: Sphere { 2, xCenter, yCenter, zCenter, radius}
	// 3: Tiled Floor { 3, A, B, C, D} where A*x+By+Cz=D
	//float Objects[] = { 2,2.5,.6,0,0.3, 2,-1,0,0,0.3, 2,-4.5,-.6,0,0.3, 3,0,0,1,-0.3, 1,0,0,1,-0.32, 0 };
	//float Objects[] = { 2,2.5,.6,0,0.3, 2,-1,0,0,0.3, 2,-4.5,-.6,0,0.3, 1,0,0,1,-0.3, 0 };
	//float Objects[] = { 2,0,0,0, .5, 1,0,0,1,-0.5, 0 };
	//float Objects[] = { 2,-2,-1,0,0.5,  2,-1,1,0,0.5, 1,0,0,1,-0.5, 1,0,0,-1,-1.5, 1,-1,0,0,-6, 1,1,0,0,-3.5, 1,0,1,0,-2, 1,0,-1,0,-1.5, 0};
	float Objects[] = { 2,2.5,-.6,0,0.3, 2,-1,0,0,0.3, 2,-4.5,1.2,0,0.3, 3,0,0,1,-0.3, 1,0,0,1,-0.31, 1,0,0,-1,-1.2, 1,-1,0,0,-6, 1,1,0,0,-4.8, 1,0,1,0,-1.5, 1,0,-1,0,-1.5, 0};

	//These three values define the vector {lX,lY,lZ} which is the directional vector of light into the scene
	float light[3] = { 2,1,.75 };
	//float light[3] = { 3.5,1,1 };
	//float light[] = { 3,-3,3 };

	//default color for no hit
	outputColors[0] = 200;
	outputColors[1] = 200;
	outputColors[2] = 255;
	outputColors[3] = 0;
	
	//finds the first hit of the ray; finds the intersect, color of the object, and the normal vector to the surface
	float Intersects[3];
	float NormalVector[3];
	float ColorReference[4];
	int ObjectIndexOut[1];
	bool intersect = findNearIntersect(Intersects, NormalVector, ColorReference, ObjectIndexOut, ray, Objects, refPoint, checkPlane, checkPoint, ObjectIndexIn, i, j);

	/*if (i == 765 && j == 845) {
		int stop = 1;
	}*/
	
	//if a hit is found, it procedes to find the lighting at that point
	if (intersect) {
		float colVal;

		//DIFFUSE LIGHTING
		bool clearPathToLight = findIfIntersect2(Objects, Intersects, light, ObjectIndexOut[0]);
		if (!clearPathToLight) {
			float angle = fabs(angleBetweenVectors(NormalVector[0], NormalVector[1], NormalVector[2], light[0] - Intersects[0], light[1] - Intersects[1], light[2] - Intersects[2]));
			
			//colVal is set to 50 as default for shadow lighting; if it is not in the shadow, angle is used to compute the color of diffuse lighting
			colVal = 50 + (angle) * 200;
		}
		else { colVal = 50; }

		//rgb components are set based on colVal
		float red = (colVal * ColorReference[0]);
		float green = (colVal * ColorReference[1]);
		float blue = (colVal * ColorReference[2]);

		//SPECULAR LIGHTING
		float reflectedRay[6];
		findReflectedRay(reflectedRay, ray, Intersects, NormalVector);

		float lightPlane[4] = { light[0], light[1], light[2], light[0] * light[0] + light[1] * light[1] + light[2] * light[2] };
		float lightPlaneIntersect[3];
		intersectPlaneRay(lightPlaneIntersect, lightPlane, reflectedRay);

		if (recursionLevel != 0) {
			float CheckPlane[4];

			CheckPlane[0] = NormalVector[0];
			CheckPlane[1] = NormalVector[1];
			CheckPlane[2] = NormalVector[2];
			CheckPlane[3] = CheckPlane[0] * Intersects[0] + CheckPlane[1] * Intersects[1] + CheckPlane[2] * Intersects[2];

			float CheckPoint[3];
			CheckPoint[0] = Intersects[0] + NormalVector[0];
			CheckPoint[1] = Intersects[1] + NormalVector[1];
			CheckPoint[2] = Intersects[2] + NormalVector[2];

			int reflectedColor[4];
			traceRay(reflectedColor, reflectedRay, Intersects, recursionLevel-1, CheckPlane, CheckPoint, ObjectIndexOut[0], i, j);

			int refl = ColorReference[3];
			int matte = 10 - refl;

			red = (matte*red + refl*reflectedColor[0]) / 10;
			green = (matte*green + refl*reflectedColor[1]) / 10;
			blue = (matte*blue + refl*reflectedColor[2]) / 10;
		}

		if (!clearPathToLight) {
			float distance = distBetweenPoints(light, lightPlaneIntersect);
			if (distance < 1) {
				float brightConst = sqrt(1 - distance*distance);
				int redDiff = 255 - red;
				int greenDiff = 255 - green;
				int blueDiff = 255 - blue;
				red += brightConst*redDiff;
				green += brightConst*greenDiff;
				blue += brightConst*blueDiff;
			}
		}

		int redI = red;
		int greenI = green;
		int blueI = blue;

		if (redI > 255) { redI = 255; }
		if (greenI > 255) { greenI = 255; }
		if (blueI > 255) { blueI = 255; }

		outputColors[0] = redI;
		outputColors[1] = greenI;
		outputColors[2] = blueI;
		outputColors[3] = 0;
	}
}