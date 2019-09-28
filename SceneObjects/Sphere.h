struct Sphere_SceneObjectData {
    RTTypesSphere sphere;
    float reflection;
    Color_Float color;
};

bool sphere_getIntersect(void * objectDataRaw, RTTypesRay * ray, RTTypesVector * returnPoint, RTTypesVector * returnNormal) {
    Sphere_SceneObjectData * objectData = (Sphere_SceneObjectData *)objectDataRaw;
    RTTypesSphere * sphere = &(objectData -> sphere);

	//A,B, and C are defined as the coefficients of a quadratic formula for the intersection of the ray and the sphere
	float A = (ray -> direction).dot(ray -> direction);
	float B = 2 * ( (ray -> direction).dot(ray -> point) - (ray -> direction).dot(sphere -> center) );
	float C = (sphere -> center).dot(sphere -> center) + (ray -> point).dot(ray -> point) - 2 * (sphere -> center).dot(ray -> point) - sphere -> r * sphere -> r;
	float det = B*B - 4 * A*C;
    
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

		*returnPoint = ray -> point + ray -> direction * t;
		*returnNormal = *returnPoint - sphere -> center;
		return true;
	}
	else {
		return false;
	}
}

bool sphere_getIntersectNoSelf(void * objectDataRaw, RTTypesRay * ray, RTTypesVector * returnPoint, RTTypesVector * returnNormal) {
    Sphere_SceneObjectData * objectData = (Sphere_SceneObjectData *)objectDataRaw;
    RTTypesSphere * sphere = &(objectData -> sphere);

    // quick test for no intersection
    RTTypesVector normalVector = ray -> point - sphere -> center;
	float dotProduct = normalVector.dot(ray -> direction);
	if (dotProduct >= 0) {
		return false;
	}

	//A,B, and C are defined as the coefficients of a quadratic formula for the intersection of the ray and the sphere
	float A = (ray -> direction).dot(ray -> direction);
	float B = 2 * ( (ray -> direction).dot(ray -> point) - (ray -> direction).dot(sphere -> center) );
	float C = (sphere -> center).dot(sphere -> center) + (ray -> point).dot(ray -> point) - 2 * (sphere -> center).dot(ray -> point) - sphere -> r * sphere -> r;
	float det = B*B - 4 * A*C;
    
	if (det >= 0) {
		det = sqrt(det);
		float t1 = (-B + det) / (2 * A);
		float t2 = (-B - det) / (2 * A);
        float t;
        if (abs(t1) > abs(t2)) {
            t = t1;
        }
        else {
            t = t2;
        }

        if (t <= 0) {
            return false;
        }

		*returnPoint = ray -> point + ray -> direction * t;
		*returnNormal = *returnPoint - sphere -> center;
		return true;
	}
	else {
		return false;
	}
}

void sphere_getColor(void * objectDataRaw, RTTypesVector * point, Color_Float * returnColor, float * returnReflection) {
    Sphere_SceneObjectData * objectData = (Sphere_SceneObjectData *)objectDataRaw;

    *returnColor = objectData -> color;
	*returnReflection = objectData -> reflection;
}

Scene_ObjectType sphere_SceneObject = {
    "sphere",
    sphere_getIntersect,
    sphere_getIntersectNoSelf,
    sphere_getColor
};