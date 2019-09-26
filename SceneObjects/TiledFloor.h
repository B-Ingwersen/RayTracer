
struct TileFloor_SceneObjectData {
    Objects_Plane plane;
    float reflection;
    Color_Float color;
};

bool tileFloor_intersectHCyl(Objects_Vector * IntersectOut, float Y1, float Z1, float R, Objects_Ray * ray) {
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

bool tileFloor_intersectWCyl(Objects_Vector * IntersectOut, float X1, float Z1, float R, Objects_Ray * ray) {
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

int tileFloor_getAllIntersect(void * objectDataRaw, Objects_Ray * ray, Objects_Vector * IntersectOut, Objects_Vector * NormalVecOut) {
    TileFloor_SceneObjectData * objectData = (TileFloor_SceneObjectData *)objectDataRaw;
    Objects_Plane * plane = &(objectData -> plane);

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
			bool intersect = tileFloor_intersectHCyl(edgeIntersects, Hloc - GapWidth, plane -> d - lipRadius, lipRadius, ray);//intersect cylinder parallel to x axis
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
			intersect = tileFloor_intersectHCyl(edgeIntersects, Hloc + GapWidth, plane -> d - lipRadius, lipRadius, ray);
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
		bool intersect = tileFloor_intersectWCyl(edgeIntersects, Wloc - GapWidth, plane -> d - lipRadius, lipRadius, ray);
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

		intersect = tileFloor_intersectWCyl(edgeIntersects, Wloc + GapWidth, plane -> d - lipRadius, lipRadius, ray);
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

bool tileFloor_getIntersect(void * objectDataRaw, Objects_Ray * ray, Objects_Vector * returnPoint, Objects_Vector * returnNormal) {
	Objects_Vector tileIntersects[8];
	Objects_Vector tileNormVecs[8];

	int numOfIntersects = tileFloor_getAllIntersect(objectDataRaw, ray, tileIntersects, tileNormVecs);
	int j = 0;

	float minDistance = FLT_MAX;
	bool foundIntersect = false;

	j = 0;

	while (j < numOfIntersects) {
		float distance = ray -> point.dist(tileIntersects[j]);
		if (distance < minDistance && ray -> direction.dot(tileIntersects[j] - ray -> point) > 0) {
			minDistance = distance;

			*returnPoint = tileIntersects[j];
			*returnNormal = tileNormVecs[j];

			foundIntersect = true;
		}
		j++;
	}

	return foundIntersect;
}

bool tileFloor_getIntersectNoSelf(void * objectDataRaw, Objects_Ray * ray, Objects_Vector * returnPoint, Objects_Vector * returnNormal) {
	Objects_Vector tileIntersects[8];
	Objects_Vector tileNormVecs[8];

	int numOfIntersects = tileFloor_getAllIntersect(objectDataRaw, ray, tileIntersects, tileNormVecs);
	int j = 0;

	float minCheckDistance = FLT_MAX;
	float minDistance = FLT_MAX;
	bool foundIntersect = false;
	int bannedIndex = -1;

	while (j < numOfIntersects) {
		float distance = ray -> point.dist(tileIntersects[j]);
		if (distance < minCheckDistance) {
			minCheckDistance = distance;
			bannedIndex = j;
		}
		j++;
	}

	j = 0;

	while (j < numOfIntersects) {
		if (j == bannedIndex) {
			j++;
			continue;
		}

		float distance = ray -> point.dist(tileIntersects[j]);
		if (distance < minDistance && ray -> direction.dot(tileIntersects[j] - ray -> point) > 0) {
			minDistance = distance;

			*returnPoint = tileIntersects[j];
			*returnNormal = tileNormVecs[j];

			foundIntersect = true;
		}
		j++;
	}

	return foundIntersect;
}

void tileFloor_getColor(void * objectDataRaw, Objects_Vector * point, Color_Float * returnColor, float * returnReflection) {
    TileFloor_SceneObjectData * objectData = (TileFloor_SceneObjectData *)objectDataRaw;

    *returnColor = objectData -> color;
	*returnReflection = objectData -> reflection;
}

Scene_ObjectType tileFloor_SceneObject = {
    "tileFloor",
    tileFloor_getIntersect,
    tileFloor_getIntersectNoSelf,
    tileFloor_getColor
};