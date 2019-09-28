bool intersectSphereRay(RTTypesVector * intersect, RTTypesSphere * sphere, RTTypesRay * ray) {
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

void intersectPlaneRay(RTTypesVector * IntersectOut, RTTypesPlane * plane, RTTypesRay * ray) {
	float t = ( plane -> d - plane -> normalVector.dot(ray -> point) ) / plane -> normalVector.dot(ray -> direction);
	*IntersectOut = ray -> point + ray -> direction * t;
}

void findReflectedRay(RTTypesRay * OutputRay, RTTypesRay * ray, RTTypesVector * reflPoint, RTTypesVector * normalVec) {

	RTTypesPlane reflectionPlane;
	reflectionPlane.normalVector = *normalVec;
	reflectionPlane.d = normalVec -> dot(*reflPoint + *normalVec);

	RTTypesVector newInt;
	intersectPlaneRay(&newInt, &reflectionPlane, ray);

	RTTypesVector newPoint = (*reflPoint + *normalVec) * 2.0 - newInt;

	(*OutputRay).createFromPoints(reflPoint, &newPoint);
}

void refractRay(RTTypesVector * ray, RTTypesVector * normal, float indexRatio, RTTypesVector * returnRay) {
	if (ray -> dot(*normal) < 0) {
		normal -> invert();
	}
	float r = ray -> abs();
	float k = normal -> abs();
	float theta1 = normal -> angleBetween(*ray);
	float theta2 = theta1 * indexRatio;

	float ratio1;
	if (fabs(theta1) > 0.05) {
		ratio1 = sin(theta2) / sin(theta1);
	}
	else {
		// 3rd degree Taylor approximation of sin(theta1 * indexRatio) / sin(theta1) for theta1 close to zero
		ratio1 = indexRatio + theta1 * theta1 * (indexRatio - indexRatio * indexRatio * indexRatio) / 6.0;
	}

	float a1 = r / k * (cos(theta2) - cos(theta1) * ratio1);
	float a2 = ratio1;
	*returnRay = *normal * a1 + *ray * a2;
}

void traceRay(RTTypesRay * ray, int recursionLevel, Color * returnColor, Scene_Descriptor * scene, int ignoreObject) {
	RTTypesVector light = { 2,1,.75 };
	*returnColor = {200, 200, 255, 0};

	RTTypesVector point;
	RTTypesVector normal;
	Color_Float color;
	float reflection;
	int objectIndex;

	bool intersect = scene -> intersectRay(ray, &point, &normal, &color, &reflection, ignoreObject, &objectIndex);

	if (intersect) {
		float scaleColor = 50;

		RTTypesRay pathToLight;
		pathToLight.createFromPoints(&point, &light);

		RTTypesVector lightIntersect;
		RTTypesVector unused2;
		Color_Float unused3;
		float unused4;
		int unused5;

		bool clearPathToLight = scene -> intersectRay(&pathToLight, &lightIntersect, &unused2, &unused3, &unused4, objectIndex, &unused5);
		if (clearPathToLight && pathToLight.point.dist(lightIntersect) > pathToLight.point.dist(light)) {
			clearPathToLight = false;
		}
		if (!clearPathToLight) {
			scaleColor += 200 * fabs( normal.cosineOfAngleBetween(pathToLight.direction) );
		}

		color *= scaleColor;

		RTTypesRay reflectedRay;
		findReflectedRay(&reflectedRay, ray, &point, &normal);

		if (recursionLevel > 0) {
			Color reflectedColor;
			traceRay(&reflectedRay, recursionLevel - 1, &reflectedColor, scene, objectIndex);

			float matte = 1.0 - reflection;
			color *= matte;
			color.r += reflectedColor.r * reflection;
			color.g += reflectedColor.g * reflection;
			color.b += reflectedColor.b * reflection;
		}

		if (!clearPathToLight) {
			RTTypesPlane lightPlane = {light, light.dot(light)};
			RTTypesVector lightPlaneIntersect;
			intersectPlaneRay(&lightPlaneIntersect, &lightPlane, &reflectedRay);
			float distance = light.dist(lightPlaneIntersect);
			if (distance < 1) {
				float brightness = sqrt(1 - distance * distance);
				color.r += (255 - color.r) * brightness;
				color.g += (255 - color.g) * brightness;
				color.b += (255 - color.b) * brightness;
			}
		}

		if (color.r > 255) {color.r = 255;}
		if (color.g > 255) {color.g = 255;}
		if (color.b > 255) {color.b = 255;}
		returnColor -> r = (uint8_t)color.r;
		returnColor -> g = (uint8_t)color.g;
		returnColor -> b = (uint8_t)color.b;
	}
}