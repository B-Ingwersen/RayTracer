#include "Ray.h"

// find the intersect of a sphere and a ray; if an intersect is found, return
// it in *intersect and return true; otherwise return false
bool intersectSphereRay(RTTypesVector * intersect, RTTypesSphere * sphere,
	RTTypesRay * ray) {

	//A,B, and C are defined as the coefficients of a quadratic formula for the
	// intersection of the ray and the sphere
	float A = (ray -> direction).dot(ray -> direction);
	float B = 2 * ( (ray -> direction).dot(ray -> point)
		- (ray -> direction).dot(sphere -> center) );
	float C = (sphere -> center).dot(sphere -> center)
		+ (ray -> point).dot(ray -> point)
		- 2 * (sphere -> center).dot(ray -> point) - sphere -> r * sphere -> r;
	float det = B*B - 4 * A*C;

	//The determinant of the quad form is checked; negative result means that
	// the objects don't intersect and this step is skipped
	if (det >= 0) {
		det = sqrt(det);
		float t1 = (-B + det) / (2 * A);
		float t2 = (-B - det) / (2 * A);
		float t;

		// the ray is used to create the parametric equation for a line, and
		// the quadratic formula has yielded the two t values for the two
		// intersects of the line and the sphere; since a ray only extends one
		// direction from its origin though, a negative t value means there is
		// no intersect. Therefore, the smallest positive t-value is used, or
		// both are negative, no intersect is recorded
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

// find the intersect of a plane and a ray; if an intersect is found, return
// it in *intersect and return true; otherwise return false
void intersectPlaneRay(RTTypesVector * IntersectOut, RTTypesPlane * plane,
	RTTypesRay * ray) {
	
	float t = ( plane -> d - plane -> normalVector.dot(ray -> point) )
		/ plane -> normalVector.dot(ray -> direction);
	*IntersectOut = ray -> point + ray -> direction * t;
}

// calculate the ray that would be reflection if ray hits a surface with normal
// direction normalVec at point reflPoint; the calculated reflection ray is
// stored in *OutputRay
void findReflectedRay(RTTypesRay * OutputRay, RTTypesRay * ray,
	RTTypesVector * reflPoint, RTTypesVector * normalVec) {

	// construct a plane normal to normalVec, but moved normalVec away from the
	// reflection point
	RTTypesPlane reflectionPlane;
	reflectionPlane.normalVector = *normalVec;
	reflectionPlane.d = normalVec -> dot(*reflPoint + *normalVec);

	// find where ray intersects the newly constructed plane
	RTTypesVector newInt;
	intersectPlaneRay(&newInt, &reflectionPlane, ray);

	// *reflPoint + *normalVec is where the normal vector will intersect
	// reflection plane; we can calculate where the reflected ray will intersect
	// the reflection plane since we know that the normal vector will intersect
	// it directly in between where the original ray and the reflected ray will
	RTTypesVector newPoint = (*reflPoint + *normalVec) * 2.0 - newInt;

	// use the intersect point to construct the output ray
	OutputRay -> createFromPoints(reflPoint, &newPoint);
}

// calculate how ray will be refracted upon ecountering an object with normal
// vector normal, and where indexRatio is the ratio of the indexes of refraction
// of the source substance and the substance being entered; the refracted ray
// is stored in *returnRay -- WARNING: this function is only for where
// refraction is known occured; if it is a situation where internal reflection
// could occur, the function's behavior is undefined
void refractRay(RTTypesVector * ray, RTTypesVector * normal, float indexRatio,
	RTTypesVector * returnRay) {
	
	// make sure that ray and normal point in the same general direction (<90
	// degrees apart)
	if (ray -> dot(*normal) < 0) {
		normal -> invert();
	}

	// alias some vector magnitudes
	float r = ray -> abs();
	float k = normal -> abs();

	// determine the angle of incidence of the ray hitting the surface, and use
	// that to determine the angle at which the ray will will travel after 
	// refracting
	float theta1 = normal -> angleBetween(*ray);
	float theta2 = theta1 * indexRatio;

	// caltulate the ratio sin(theta2) / sin(theta1), guarding against errors
	// at angles of incidence near zero
	float ratio1;
	if (fabs(theta1) > 0.05) {
		ratio1 = sin(theta2) / sin(theta1);
	}
	else {
		// 3rd degree Taylor approximation of
		// sin(theta1 * indexRatio) / sin(theta1) for theta1 close to zero
		ratio1 = indexRatio + theta1 * theta1
			* (indexRatio - indexRatio * indexRatio * indexRatio) / 6.0;
	}

	// using the normal vector and the original ray to define a coordinate
	// system, find the coordinates of the return ray, and then convert them
	// back into cartesian coordinates to calculate the refracted ray
	float a1 = r / k * (cos(theta2) - cos(theta1) * ratio1);
	float a2 = ratio1;
	*returnRay = *normal * a1 + *ray * a2;
}

// trace a ray to determine the color that an observer would see looking in the
// ray's direction (this is the core of ray tracing)
//	ray: the ray being traced
//	recursion level: the number of additional levels of recursion that will be
// 	called; set to the desired number or reflection
//	has been reached; set to 0 on the initial call
//	returnColor: a pointer to where you want the results of the ray tracing to
//	be stored
//	scene: the scene to be rendered
//	ignoreObject: the index of the object that the ray just reflected from; set
//	 to -1 initially to designate no object
void traceRay(RTTypesRay * ray, int recursionLevel, Color_Int * returnColor,
	Scene_Descriptor * scene, int ignoreObject) {
	
	// the default background color of light blue -- this is what gets returned
	// if the ray intersects nothing
	*returnColor = {200, 200, 255, 0};

	RTTypesVector point;
	RTTypesVector normal;
	Color_Float color;
	float reflection;
	int objectIndex;

	// see if the ray intersects any object in the scene; if it intersects, get
	// the point of intersection, the normal vector at the intersect, the color
	// at the intersect, and the index of the object that was intersected
	bool intersect = scene -> intersectRay(ray, &point, &normal, &color,
		&reflection, ignoreObject, &objectIndex);

	// if there was an intersect, determine the lighting characteristics of the
	// object that was hit to modify the return color
	if (intersect) {
		float scaleColor = 50;

		RTTypesRay pathToLight;
		pathToLight.createFromPoints(&point, scene -> light);

		RTTypesVector lightIntersect;
		// discarded parameters in next function call
		RTTypesVector unused2;
		Color_Float unused3;
		float unused4;
		int unused5;

		// determine if the there is anything obstructing light from getting to
		// the point of intersect
		bool blockedPathToLight = scene -> intersectRay(&pathToLight,
			&lightIntersect, &unused2, &unused3, &unused4, objectIndex,
			&unused5);
		
		// if the obstruction is further away than the light source, then it is
		// not an actual obstruction
		if (blockedPathToLight && pathToLight.point.dist(lightIntersect)
			> pathToLight.point.dist(*scene -> light)) {
			
			blockedPathToLight = false;
		}

		// if the light source is not blocked, factor in difuse lighting
		if (!blockedPathToLight) {
			scaleColor += 200
				* fabs(normal.cosineOfAngleBetween(pathToLight.direction));
		}

		// amplify the color of the intersected object based on the diffuse
		// lighting calculation
		color *= scaleColor;

		// determine the ray that is reflected off of the intersected object
		RTTypesRay reflectedRay;
		findReflectedRay(&reflectedRay, ray, &point, &normal);

		// if we have not hit the recursion limit, ray trace the reflected ray
		// and incorperate the reflection into the computed color
		if (recursionLevel > 0) {
			Color_Int reflectedColor;
			traceRay(&reflectedRay, recursionLevel - 1, &reflectedColor, scene,
				objectIndex);

			// modify the color based on the trace of the reflected ray and how
			// reflective the object is
			float matte = 1.0 - reflection;
			color *= matte;
			color.r += reflectedColor.r * reflection;
			color.g += reflectedColor.g * reflection;
			color.b += reflectedColor.b * reflection;
		}

		// check for a specular reflection
		if (!blockedPathToLight) {
			RTTypesPlane lightPlane = {
				*scene -> light,
				(scene -> light) -> dot(*scene -> light)
			};

			// if the reflected ray hits the light source (currently hard-coded
			// as 1 unit radius spheres, then incorperate a specular component
			RTTypesVector lightPlaneIntersect;
			intersectPlaneRay(&lightPlaneIntersect, &lightPlane, &reflectedRay);
			float distance = scene -> light -> dist(lightPlaneIntersect);
			if (distance < 1) {
				// the light source is made brightest at its center and falls
				// off at the edges
				float brightness = sqrt(1 - distance * distance);

				// incorperate the specular reflection into the color
				color.r += (255 - color.r) * brightness;
				color.g += (255 - color.g) * brightness;
				color.b += (255 - color.b) * brightness;
			}
		}

		// clip each color component to be <= 255
		if (color.r > 255) {color.r = 255;}
		if (color.g > 255) {color.g = 255;}
		if (color.b > 255) {color.b = 255;}
		returnColor -> r = (uint32_t)color.r;
		returnColor -> g = (uint32_t)color.g;
		returnColor -> b = (uint32_t)color.b;
	}
}