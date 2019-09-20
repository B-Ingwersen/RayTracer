bool intersectSphereRay(Objects_Vector * intersect, Objects_Sphere * sphere, Objects_Ray * ray) {
	float x1 = ray -> point.x;
	float y1 = ray -> point.y;
	float z1 = ray -> point.z;
	float X = ray -> direction.x;
	float Y = ray -> direction.y;
	float Z = ray -> direction.z;

	float X1 = sphere -> center.x;
	float Y1 = sphere -> center.y;
	float Z1 = sphere -> center.z;
	float R = sphere -> r;

	//A,B, and C are defined as the coefficients of a quadratic formula for the intersection of the ray and the sphere
	//float A = X*X + Y*Y + Z*Z;
	float A = (ray -> direction).dot(ray -> direction);
	//float B = 2 * (X*x1 + Y*y1 + Z*z1 - X*X1 - Y*Y1 - Z*Z1);
	float B = 2 * ( (ray -> direction).dot(ray -> point) - (ray -> direction).dot(sphere -> center) );
	//float C = X1*X1 + x1*x1 + Y1*Y1 + y1*y1 + Z1*Z1 + z1*z1 - 2 * X1*x1 - 2 * Y1*y1 - 2 * Z1*z1 - R*R;
	float C = (sphere -> center).dot(sphere -> center) + (ray -> point).dot(ray -> point) - 2 * (sphere -> center).dot(ray -> point) - R*R;
	float det = B*B - 4 * A*C;
	//The determinant of the quad form is checked; negative result means that the objects don't intersect and this step is skipped
	if (det >= 0) {
		//The two intersects are computed and stored as (X1,Y1,Z1) and (X2,Y2,Z2)
		/*det = sqrt(det);
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
		IntersectOut[5] = Z2;*/

		det = sqrt(det);
		float t1 = (-B + det) / (2 * A);
		float t2 = (-B - det) / (2 * A);
		float t;
		if (t1 <= 0 && t2 <= 0) {
			return false;
		}
		else if (t1 <= 0 || (t2 > 0 && t2 <= t1)) {
			t = t2;
		}
		else {
			t = t1;
		}
		*intersect = ray -> point + ray -> direction * t;

		return true;
	}

	else {
		return false;
	}
}

bool sphereIntersectNotSelf(Objects_Sphere * sphere, Objects_Ray * ray) {
	Objects_Vector normalVector = ray -> point - sphere -> center;
	float dotProduct = normalVector.dot(ray -> direction);
	if (dotProduct < 0) {
		return true;
	}
	else {
		return false;
	}
}

void intersectPlaneRay(Objects_Vector * IntersectOut, Objects_Plane * plane, Objects_Ray * ray) {
	float t = ( plane -> d - plane -> normalVector.dot(ray -> point) ) / plane -> normalVector.dot(ray -> direction);
	*IntersectOut = ray -> point + ray -> direction * t;
}

bool intersectHCyl(Objects_Vector * IntersectOut, float Y1, float Z1, float R, Objects_Ray * ray) {
	float x1 = ray -> point.x;
	float y1 = ray -> point.y;
	float z1 = ray -> point.z;
	float X = ray -> direction.x;
	float Y = ray -> direction.y;
	float Z = ray -> direction.z;

	//A,B, and C are defined as the coefficients of a quadratic formula for the intersection of the ray and the sphere
	float A = Y*Y + Z*Z;
	float B = 2 * (Y*y1 + Z*z1 - Y*Y1 - Z*Z1);
	float C = Y1*Y1 + y1*y1 + Z1*Z1 + z1*z1 - 2 * (Y1*y1 + Z1*z1) - R*R;

	float det = B*B - 4 * A*C;
	//The determinant of the quad form is checked; negative result means that the objects don't intersect and this step is skipped
	if (det >= 0) {
		det = sqrt(det);
		float t1 = (-B + det) / (2 * A);
		float t2 = (-B - det) / (2 * A);
		IntersectOut[0] = ray -> point + ray -> direction * t1;
		IntersectOut[1] = ray -> point + ray -> direction * t2;

		return true;
	}

	else {
		return false;
	}
}

bool intersectWCyl(Objects_Vector IntersectOut[2], float X1, float Z1, float R, Objects_Ray * ray) {
	float x1 = ray -> point.x;
	float y1 = ray -> point.y;
	float z1 = ray -> point.z;
	float X = ray -> direction.x;
	float Y = ray -> direction.y;
	float Z = ray -> direction.z;

	//A,B, and C are defined as the coefficients of a quadratic formula for the intersection of the ray and the sphere
	float A = X*X + Z*Z;
	float B = 2 * (X*x1 + Z*z1 - X*X1 - Z*Z1);
	float C = X1*X1 + x1*x1 + Z1*Z1 + z1*z1 - 2 * X1*x1 - 2 * Z1*z1 - R*R;

	float det = B*B - 4 * A*C;
	//The determinant of the quad form is checked; negative result means that the objects don't intersect and this step is skipped
	if (det >= 0) {
		det = sqrt(det);
		float t1 = (-B + det) / (2 * A);
		float t2 = (-B - det) / (2 * A);
		IntersectOut[0] = ray -> point + ray -> direction * t1;
		IntersectOut[1] = ray -> point + ray -> direction * t2;

		return true;
	}

	else {
		return false;
	}
}

int intersectTiledFloor(Objects_Vector * IntersectOut, Objects_Vector * NormalVecOut, Objects_Plane * plane, Objects_Ray * ray, int i, int j) {
	float t = (plane -> d - plane -> normalVector.dot(ray -> point)) / plane -> normalVector.dot(ray -> direction);

	//Intersect of the upper plane with the ray
	IntersectOut[0] = ray -> point + ray -> direction * t;

	float GapWidth = 0.03;//half the width between top of tiles
	float lipRadius = 0.03;//depth between the tiles; should be identical to "GapWidth"
	float TileWidth = 0.25;

	//WLoc and Hloc are the "grid coordinates" of the closest point.
	float Wloc = (float)(round(IntersectOut[0].x/TileWidth))*TileWidth;
	float Hloc = (float)(round(IntersectOut[0].y/TileWidth))*TileWidth;
	float W = fmodf(fabs(IntersectOut[0].x), TileWidth);//modulo of the location of the intersect
	float H = fmodf(fabs(IntersectOut[0].y), TileWidth);

	int numOfIntersects = 0;//counter of number of intersects indentified

	if ((H >= GapWidth && H <= (TileWidth - GapWidth)) && (W >= GapWidth && W <= (TileWidth - GapWidth))) {
		numOfIntersects = 1;
		NormalVecOut[0] = plane -> normalVector;
	}
	else {
		if (H <= GapWidth || H >= (TileWidth - GapWidth)) { //determine if within GapWidth of grid lines		
			//float edgeIntersects[6];
			Objects_Vector edgeIntersects[2];
			bool intersect = intersectHCyl(edgeIntersects, Hloc - GapWidth, plane -> d - lipRadius, lipRadius, ray);//intersect cylinder parallel to x axis
			if (intersect) {
				if (edgeIntersects[0].y >= Hloc - GapWidth && edgeIntersects[0].z >= plane -> d - lipRadius) {//checks 1st intersect to be in quarter of cylinder that is visible
					float myW = fmodf(fabs(edgeIntersects[0].x), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }//distance from grid of intersects
					float myH = fmodf(fabs(edgeIntersects[0].y), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
					if (myW >= myH) {
						IntersectOut[numOfIntersects] = edgeIntersects[0];
						NormalVecOut[numOfIntersects].x = 0;
						NormalVecOut[numOfIntersects].y = edgeIntersects[0].y - (Hloc - GapWidth);
						NormalVecOut[numOfIntersects].z = edgeIntersects[0].z - (plane -> d - lipRadius);
						numOfIntersects++;
					}
				}
				if (edgeIntersects[1].y >= Hloc - GapWidth && edgeIntersects[1].z >= plane -> d - lipRadius) {//checks 2nd intersect to be in quarter of cylinder that is visible
					float myW = fmodf(fabs(edgeIntersects[1].x), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }
					float myH = fmodf(fabs(edgeIntersects[1].y), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
					if (myW >= myH) {
						IntersectOut[numOfIntersects] = edgeIntersects[1];
						NormalVecOut[numOfIntersects].x = 0;
						NormalVecOut[numOfIntersects].y = edgeIntersects[1].y - (Hloc - GapWidth);
						NormalVecOut[numOfIntersects].z = edgeIntersects[1].z - (plane -> d - lipRadius);
						numOfIntersects++;
					}
				}
			}
			intersect = intersectHCyl(edgeIntersects, Hloc + GapWidth, plane -> d - lipRadius, lipRadius, ray);
			if (intersect) {
				if (edgeIntersects[0].y <= Hloc + GapWidth && edgeIntersects[0].z >= plane -> d - lipRadius) {
					float myW = fmodf(fabs(edgeIntersects[0].x), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }
					float myH = fmodf(fabs(edgeIntersects[0].y), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
					if (myW >= myH) {
						IntersectOut[numOfIntersects] = edgeIntersects[0];
						NormalVecOut[numOfIntersects].x = 0;
						NormalVecOut[numOfIntersects].y = edgeIntersects[0].y - (Hloc + GapWidth);
						NormalVecOut[numOfIntersects].z = edgeIntersects[0].z - (plane -> d - lipRadius);
						numOfIntersects++;
					}
				}
				if (edgeIntersects[1].y <= Hloc + GapWidth && edgeIntersects[1].z >= plane -> d - lipRadius) {
					float myW = fmodf(fabs(edgeIntersects[1].x), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }
					float myH = fmodf(fabs(edgeIntersects[1].y), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
					if (myW >= myH) {
						IntersectOut[numOfIntersects] = edgeIntersects[1];
						NormalVecOut[numOfIntersects].x = 0;
						NormalVecOut[numOfIntersects].y = edgeIntersects[1].y - (Hloc + GapWidth);
						NormalVecOut[numOfIntersects].z = edgeIntersects[1].z - (plane -> d - lipRadius);
						numOfIntersects++;
					}
				}
			}
		}
	
		Objects_Vector edgeIntersects[2];
		bool intersect = intersectWCyl(edgeIntersects, Wloc - GapWidth, plane -> d - lipRadius, lipRadius, ray);
		if (intersect) {
			if (edgeIntersects[0].x >= Wloc - GapWidth && edgeIntersects[0].z >= plane -> d - lipRadius) {
				float myW = fmodf(fabs(edgeIntersects[0].x), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }
				float myH = fmodf(fabs(edgeIntersects[0].y), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
				if (myH >= myW) {
					IntersectOut[numOfIntersects] = edgeIntersects[0];
					NormalVecOut[numOfIntersects].x = edgeIntersects[0].x - (Wloc - GapWidth);
					NormalVecOut[numOfIntersects].y = 0;
					NormalVecOut[numOfIntersects].z = edgeIntersects[0].z - (plane -> d - lipRadius);
					numOfIntersects++;
				}
			}
			if (edgeIntersects[1].x >= Wloc - GapWidth && edgeIntersects[1].z >= plane -> d - lipRadius) {
				float myW = fmodf(fabs(edgeIntersects[1].x), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }
				float myH = fmodf(fabs(edgeIntersects[1].y), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
				if (myH >= myW) {
					IntersectOut[numOfIntersects] = edgeIntersects[1];
					NormalVecOut[numOfIntersects].x = edgeIntersects[1].x - (Wloc - GapWidth);
					NormalVecOut[numOfIntersects].y = 0;
					NormalVecOut[numOfIntersects].z = edgeIntersects[1].z - (plane -> d - lipRadius);
					numOfIntersects++;
				}
			}
		}

		intersect = intersectWCyl(edgeIntersects, Wloc + GapWidth, plane -> d - lipRadius, lipRadius, ray);
		if (intersect) {
			if (edgeIntersects[0].x <= Wloc + GapWidth && edgeIntersects[0].z >= plane -> d - lipRadius) {
				float myW = fmodf(fabs(edgeIntersects[0].x), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }
				float myH = fmodf(fabs(edgeIntersects[0].y), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
				if (myH >= myW) {
					IntersectOut[numOfIntersects] = edgeIntersects[0];
					NormalVecOut[numOfIntersects].x = edgeIntersects[0].x - (Wloc + GapWidth);
					NormalVecOut[numOfIntersects].y = 0;
					NormalVecOut[numOfIntersects].z = edgeIntersects[0].z - (plane -> d - lipRadius);
					numOfIntersects++;
				}
			}
			if (edgeIntersects[1].x <= Wloc + GapWidth && edgeIntersects[1].z >= plane -> d - lipRadius) {
				float myW = fmodf(fabs(edgeIntersects[1].x), TileWidth); if (myW > TileWidth/2) { myW = TileWidth - myW; }
				float myH = fmodf(fabs(edgeIntersects[1].y), TileWidth); if (myH > TileWidth/2) { myH = TileWidth - myH; }
				if (myH >= myW) {
					IntersectOut[numOfIntersects] = edgeIntersects[1];
					NormalVecOut[numOfIntersects].x = edgeIntersects[1].x - (Wloc + GapWidth);
					NormalVecOut[numOfIntersects].y = 0;
					NormalVecOut[numOfIntersects].z = edgeIntersects[1].z - (plane -> d - lipRadius);
					numOfIntersects++;
				}
			}
		}
	}

	return numOfIntersects;
}

bool findNearIntersect(Objects_Vector * IntersectOut, Objects_Vector * NormalVector, Color_Float * ColorReference, int * objectCountOut, Objects_Ray * ray, float objects[], Objects_Vector * refPoint, Objects_Plane * checkPlane, Objects_Vector * checkPoint, int ObjectIndex, int I, int J) {

	float sphereRefl = 3;
	float planeRefl = 1;

	int checkSign;
	if (checkPlane -> normalVector.dot(*checkPoint) > checkPlane -> d) {
		checkSign = 1;
	}
	else { checkSign = -1; }

	float xr = refPoint -> x;
	float yr = refPoint -> y;
	float zr = refPoint -> z;

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
            Objects_Vector PlaneIntersect;
			Objects_Plane plane = { {objects[i + 1], objects[i + 2], objects[i + 3]}, objects[i + 4] };
			intersectPlaneRay(&PlaneIntersect, &plane, ray);

			float distance = PlaneIntersect.dist(*refPoint);
			if (distance < minDistance && objectCounter != ObjectIndex) {
				if (checkSign * checkPlane -> normalVector.dot(PlaneIntersect) > checkPlane -> d * checkSign && objectCounter != ObjectIndex) {
					minDistance = distance;
					*IntersectOut = PlaneIntersect;
					*NormalVector = plane.normalVector;

					if (plane.d > -0.33 && plane.d < -0.31) {
						*ColorReference = {0.6, 0.6, 0.6, planeRefl};
					}
					else {
						*ColorReference = {0.9, 0.9, 0.9, planeRefl};
					}
	
					foundIntersect = true;
					*objectCountOut = objectCounter;
				}
			}
			i += 5;
			objectCounter += 1;
		}

		//2:SPHERE
		else if (objects[i] == 2) {
			Objects_Vector SphereIntersects;
			Objects_Sphere sphere = {
                objects[i + 1],
                objects[i + 2],
                objects[i + 3],
                objects[i + 4]
            };
			bool intersect;
			intersect = intersectSphereRay(&SphereIntersects, &sphere, ray);
			if (intersect) {

				if (objectCounter == ObjectIndex) {
					float distance1 = refPoint -> dist(SphereIntersects);
					intersect = sphereIntersectNotSelf(&sphere, ray);
					if (intersect) {
						if (checkSign*(checkPlane -> normalVector.dot(SphereIntersects)) > checkPlane -> d * checkSign) {
							minDistance = distance1;
							*IntersectOut = SphereIntersects;

							NormalVector -> x = IntersectOut -> x - objects[i + 1];
							NormalVector -> y = IntersectOut -> y - objects[i + 2];
							NormalVector -> z = IntersectOut -> z - objects[i + 3];

							*ColorReference = {1, 0, 0, sphereRefl};

							foundIntersect = true;
							*objectCountOut = objectCounter;
						}
					}
				}
				else {
					float distance = refPoint -> dist(SphereIntersects);
					if (distance < minDistance && distance > 0.0001) {
						if (checkSign * (checkPlane -> normalVector.dot(SphereIntersects)) > checkPlane -> d * checkSign) {
							minDistance = distance;
							*IntersectOut = SphereIntersects;

							NormalVector -> x = IntersectOut -> x - objects[i + 1];
							NormalVector -> y = IntersectOut -> y - objects[i + 2];
							NormalVector -> z = IntersectOut -> z - objects[i + 3];

							*ColorReference = {1,0,0,sphereRefl};

							foundIntersect = true;
							*objectCountOut = objectCounter;
						}
					}
				}
			}
			i += 5;
			objectCounter += 1;
		}

		//3:TILED FLOOR
		else if (objects[i] == 3) {
			Objects_Plane plane = { {objects[i + 1], objects[i + 2], objects[i + 3]}, objects[i + 4] };
			Objects_Vector tileIntersects[8];
			Objects_Vector tileNormVecs[8];

			int numOfIntersects = intersectTiledFloor(tileIntersects, tileNormVecs, &plane, ray,I,J);
			int j = 0;

			float minCheckDistance = FLT_MAX;
			int bannedIndex = -1;

			if (objectCounter == ObjectIndex) {
				while (j < numOfIntersects) {
					float distance = refPoint -> dist(tileIntersects[j]);
					if (distance < minCheckDistance) {
						minCheckDistance = distance;
						bannedIndex = j;
					}
					j += 3;
				}
			}

			j = 0;

			while (j < numOfIntersects) {
				float distance = refPoint -> dist(tileIntersects[j]);
				if (distance < minDistance && j != bannedIndex) {
					if (checkSign*(checkPlane -> normalVector.x * tileIntersects[j].x + checkPlane -> normalVector.y * tileIntersects[j].y + checkPlane -> normalVector.z * tileIntersects[j].z) > checkPlane -> d * checkSign) {
						minDistance = distance;

						*IntersectOut = tileIntersects[j];
						*NormalVector = tileNormVecs[j];

						*ColorReference = {0.95, 0.95, 0.95, 2};

						foundIntersect = true;
						*objectCountOut = objectCounter;
					}
				}
				j++;
			}

			i += 5;
			objectCounter += 1;
		}
	}

	return foundIntersect;
}

bool findIfIntersect2(float objects[], Objects_Vector * refPoint, Objects_Vector * toPoint, int ObjectIndex) {

	int comparisonIndex;
	if (refPoint -> x != toPoint -> x) {
		comparisonIndex = 0;
	}
	else if (refPoint -> y != toPoint -> y) {
		comparisonIndex = 1;
	}
	else if (refPoint -> z != toPoint -> z) {
		comparisonIndex = 2;
	}

	Objects_Ray ray;
	ray.createFromPoints(refPoint, toPoint);

	bool Continue = true;
	int i = 0;
	int objectCounter = 0;

	while (Continue) {
		//0: EXIT
		if (objects[i] == 0) { Continue = false; }

		//1:PLANE
		else if (objects[i] == 1) {
			Objects_Vector PlaneIntersect;
			Objects_Plane plane = { {objects[i + 1],objects[i + 2],objects[i + 3]}, objects[i + 4] };
			intersectPlaneRay(&PlaneIntersect, &plane, &ray);

			float distance = PlaneIntersect.dist(*refPoint);
			if (objectCounter != ObjectIndex) {
				if (PlaneIntersect[comparisonIndex] < ((float*)refPoint)[comparisonIndex] && PlaneIntersect[comparisonIndex] > ((float*)toPoint)[comparisonIndex]) {
					return true;
				}
				else if (PlaneIntersect[comparisonIndex] > ((float*)refPoint)[comparisonIndex] && PlaneIntersect[comparisonIndex] < ((float*)toPoint)[comparisonIndex]) {
					return true;
				}
			}

			i += 5;
			objectCounter += 1;
		}

		//2:SPHERE
		else if (objects[i] == 2) {
			Objects_Vector SphereIntersects;
			Objects_Sphere sphere = {
                objects[i + 1],
                objects[i + 2],
                objects[i + 3],
                objects[i + 4]
            };
			bool intersect;
			intersect = intersectSphereRay(&SphereIntersects, &sphere, &ray);
			if (intersect) {
				if (objectCounter == ObjectIndex) {
					float distance1 = refPoint -> dist(SphereIntersects);
					if (sphereIntersectNotSelf(&sphere, &ray)) {
						return true;
					}
				}
				else {
					if (SphereIntersects[comparisonIndex] < ((float*)refPoint)[comparisonIndex] && SphereIntersects[comparisonIndex] > ((float*)toPoint)[comparisonIndex]) {
						return true;
					}
					else if (SphereIntersects[comparisonIndex] > ((float*)refPoint)[comparisonIndex] && SphereIntersects[comparisonIndex] < ((float*)toPoint)[comparisonIndex]) {
						return true;
					}
				}
			}
			i += 5;
			objectCounter += 1;
		}

		//3:TILED FLOOR
		else if (objects[i] == 3) {
			Objects_Plane plane = { {objects[i + 1], objects[i + 2], objects[i + 3]}, objects[i + 4] };
			Objects_Vector tileIntersects[8];
			Objects_Vector tileNormVecs[8];

			int numOfIntersects = intersectTiledFloor(tileIntersects, tileNormVecs, &plane, &ray, 0, 0);
			int j = 0;

			float minCheckDistance = FLT_MAX;
			int bannedIndex = -1;

			if (objectCounter == ObjectIndex) {
				while (j < numOfIntersects) {
					float distance = refPoint -> dist(tileIntersects[j]);
					if (distance < minCheckDistance) {
						minCheckDistance = distance;
						bannedIndex = j;
					}
					j++;
				}
			}

			j = 0;

			while (j < numOfIntersects) {
				if (j != bannedIndex) {
					if (tileIntersects[j][comparisonIndex] < ((float*)refPoint)[comparisonIndex] && tileIntersects[j][comparisonIndex] > ((float*)toPoint)[comparisonIndex]) {
						return true;
					}
					else if (tileIntersects[j][comparisonIndex] > ((float*)refPoint)[comparisonIndex] && tileIntersects[j][comparisonIndex] < ((float*)toPoint)[comparisonIndex]) {
						return true;
					}
				}
				j++;
			}
			i += 5;
			objectCounter += 1;
		}
	}
	return false;
}

void findReflectedRay(Objects_Ray * OutputRay, Objects_Ray * ray, Objects_Vector * reflPoint, Objects_Vector * normalVec) {

	Objects_Plane reflectionPlane;
	reflectionPlane.normalVector = *normalVec;
	reflectionPlane.d = normalVec -> dot(*reflPoint + *normalVec);

	Objects_Vector newInt;
	intersectPlaneRay(&newInt, &reflectionPlane, ray);

	Objects_Vector newPoint = (*reflPoint + *normalVec) * 2.0 - newInt;

	(*OutputRay).createFromPoints(reflPoint, &newPoint);
}

void traceRay(Color * outputColors, Objects_Ray * ray, Objects_Vector * refPoint, int recursionLevel, Objects_Plane * checkPlane, Objects_Vector * checkPoint, int ObjectIndexIn, int i, int j) {

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
	Objects_Vector light = { 2,1,.75 };
	//float light[3] = { 3.5,1,1 };
	//float light[] = { 3,-3,3 };

	//default color for no hit
	*outputColors = {200, 200, 255, 0};
	
	//finds the first hit of the ray; finds the intersect, color of the object, and the normal vector to the surface
	Objects_Vector Intersects;
	Objects_Vector NormalVector;
	Color_Float ColorReference;
	int ObjectIndexOut;
	bool intersect = findNearIntersect(&Intersects, &NormalVector, &ColorReference, &ObjectIndexOut, ray, Objects, refPoint, checkPlane, checkPoint, ObjectIndexIn, i, j);

	/*if (i == 765 && j == 845) {
		int stop = 1;
	}*/
	
	//if a hit is found, it procedes to find the lighting at that point
	if (intersect) {
		float colVal;

		//DIFFUSE LIGHTING
		bool clearPathToLight = findIfIntersect2(Objects, &Intersects, &light, ObjectIndexOut);
		if (!clearPathToLight) {
			float cosAngle = fabs( NormalVector.cosineOfAngleBetween(light - Intersects));
			
			//colVal is set to 50 as default for shadow lighting; if it is not in the shadow, angle is used to compute the color of diffuse lighting
			colVal = 50 + (cosAngle) * 200;
		}
		else { colVal = 50; }

		//rgb components are set based on colVal
		float red = (colVal * ColorReference.r);
		float green = (colVal * ColorReference.g);
		float blue = (colVal * ColorReference.b);

		//SPECULAR LIGHTING
		Objects_Ray reflectedRay;
		findReflectedRay(&reflectedRay, ray, &Intersects, &NormalVector);

		Objects_Plane lightPlane = {light, light.dot(light)};
		Objects_Vector lightPlaneIntersect;
		intersectPlaneRay(&lightPlaneIntersect, &lightPlane, &reflectedRay);

		if (recursionLevel != 0) {
			Objects_Plane CheckPlane;

			CheckPlane.normalVector = NormalVector;
			CheckPlane.d = CheckPlane.normalVector.dot(Intersects);

			Objects_Vector CheckPoint = Intersects + NormalVector;

			Color reflectedColor;
			traceRay(&reflectedColor, &reflectedRay, &Intersects, recursionLevel-1, &CheckPlane, &CheckPoint, ObjectIndexOut, i, j);

			int refl = ColorReference.a;
			int matte = 10 - refl;

			red = (matte*red + refl*reflectedColor.r) / 10;
			green = (matte*green + refl*reflectedColor.g) / 10;
			blue = (matte*blue + refl*reflectedColor.b) / 10;
		}

		if (!clearPathToLight) {
			float distance = light.dist(lightPlaneIntersect);
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

		outputColors -> r = redI;
		outputColors -> g = greenI;
		outputColors -> b = blueI;
		outputColors -> a = 0;
	}
}