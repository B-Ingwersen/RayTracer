#include "SceneObjects.h"
#include "Scene.h"

// implementation of getIntersect for sphere objects (see Scene_ObjectType)
bool sphere_getIntersect(void * objectDataRaw, RTTypesRay * ray,
	RTTypesVector * returnPoint, RTTypesVector * returnNormal) {
	
    Sphere_SceneObjectData * objectData =
		(Sphere_SceneObjectData *)objectDataRaw;
    RTTypesSphere * sphere = &(objectData -> sphere);

	// A,B, and C are defined as the coefficients of a quadratic formula for the
	// intersection of the ray and the sphere; these solutions are for the value
	// t from parametric equation derived from the ray
	float A = (ray -> direction).dot(ray -> direction);
	float B = 2 * ( (ray -> direction).dot(ray -> point)
		- (ray -> direction).dot(sphere -> center) );
	float C = (sphere -> center).dot(sphere -> center)
		+ (ray -> point).dot(ray -> point)
		- 2 * (sphere -> center).dot(ray -> point) - sphere -> r * sphere -> r;
	float det = B*B - 4 * A*C;
    
	// check if the quadratic formula even has real solutions to see if there
	// are possible intersects
	if (det >= 0) {
		// find both solutions to the quadratic equation
		det = sqrt(det);
		float t1 = (-B + det) / (2 * A);
		float t2 = (-B - det) / (2 * A);

		// the values of t must be positive to be valid (since a ray only
		// in one direction from its origin). Either return that there is no
		// positive intersect, or return the closest positive intersect
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

		// use the selected value of t to calculate the actual intersect point
		*returnPoint = ray -> point + ray -> direction * t;
		*returnNormal = *returnPoint - sphere -> center;
		return true;
	}
	else {
		return false;
	}
}

// implementation of getIntersectNoSelf for sphere objects
// (see Scene_ObjectType)
bool sphere_getIntersectNoSelf(void * objectDataRaw, RTTypesRay * ray,
	RTTypesVector * returnPoint, RTTypesVector * returnNormal) {
	
    Sphere_SceneObjectData * objectData =
		(Sphere_SceneObjectData *)objectDataRaw;
    RTTypesSphere * sphere = &(objectData -> sphere);

    // If the ray is traveling away from the surface of the sphere, there can
	// be no intersect
    RTTypesVector normalVector = ray -> point - sphere -> center;
	float dotProduct = normalVector.dot(ray -> direction);
	if (dotProduct >= 0) {
		return false;
	}

	// A,B, and C are defined as the coefficients of a quadratic formula for the
	// intersection of the ray and the sphere; these solutions are for the value
	// t from parametric equation derived from the ray
	float A = (ray -> direction).dot(ray -> direction);
	float B = 2 * ( (ray -> direction).dot(ray -> point)
		- (ray -> direction).dot(sphere -> center) );
	float C = (sphere -> center).dot(sphere -> center)
		+ (ray -> point).dot(ray -> point)
		- 2 * (sphere -> center).dot(ray -> point) - sphere -> r * sphere -> r;
	float det = B*B - 4 * A*C;
    
	// check if the quadratic formula even has real solutions to see if there
	// are possible intersects
	if (det >= 0) {

		// find both solutions to the quadratic equation
		det = sqrt(det);
		float t1 = (-B + det) / (2 * A);
		float t2 = (-B - det) / (2 * A);

		// select the further intersect since the nearer intersect is assumed to
		// be the point of reflection that the ray just came from
        float t;
        if (abs(t1) > abs(t2)) {
            t = t1;
        }
        else {
            t = t2;
        }

		// take negative t values as no intersect because rays only travel in
		// one direction from their origin
        if (t <= 0) {
            return false;
        }

		// use the selected value of t to calculate the actual intersect point
		*returnPoint = ray -> point + ray -> direction * t;
		*returnNormal = *returnPoint - sphere -> center;
		return true;
	}
	else {
		return false;
	}
}

// implementation of getColor for sphere objects (see Scene_ObjectType)
void sphere_getColor(void * objectDataRaw, RTTypesVector * point,
	Color_Float * returnColor, float * returnReflection) {
	
	// the color/reflectivity accross the object is constant
    Sphere_SceneObjectData * objectData =
		(Sphere_SceneObjectData *)objectDataRaw;
    *returnColor = objectData -> color;
	*returnReflection = objectData -> reflection;
}

// Descriptor the sphere scene object type
Scene_ObjectType sphere_SceneObject = {
    "sphere",
    sphere_getIntersect,
    sphere_getIntersectNoSelf,
    sphere_getColor
};