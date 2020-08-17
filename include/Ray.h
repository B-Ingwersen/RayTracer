#ifndef _RAY___H_
#define _RAY___H_

#include "RTTypes.h"
#include "Scene.h"

// find the intersect of a sphere and a ray; if an intersect is found, return
// it in *intersect and return true; otherwise return false
bool intersectSphereRay(RTTypesVector * intersect, RTTypesSphere * sphere,
	RTTypesRay * ray);

// find the intersect of a plane and a ray and record it in *IntersectOut
void intersectPlaneRay(RTTypesVector * IntersectOut, RTTypesPlane * plane,
	RTTypesRay * ray);

// calculate the ray that would be reflection if ray hits a surface with normal
// direction normalVec at point reflPoint; the calculated reflection ray is
// stored in *OutputRay
void findReflectedRay(RTTypesRay * OutputRay, RTTypesRay * ray,
	RTTypesVector * reflPoint, RTTypesVector * normalVec);

// calculate how ray will be refracted upon ecountering an object with normal
// vector normal, and where indexRatio is the ratio of the indexes of refraction
// of the source substance and the substance being entered; the refracted ray
// is stored in *returnRay -- WARNING: this function is only for where
// refraction is known occured; if it is a situation where internal reflection
// could occur, the function's behavior is undefined
void refractRay(RTTypesVector * ray, RTTypesVector * normal, float indexRatio,
	RTTypesVector * returnRay);

// trace a ray to determine the color that an observer would see looking in the
// ray's direction (this is the core of ray tracing)
//	ray: the ray being traced
//	recursion level: the number of additional levels of recursion that will be
// 	called; set to the desired number or reflection
//	returnColor: a pointer to where you want the results of the ray tracing to
//	be stored
//	scene: the scene to be rendered
//	ignoreObject: the index of the object that the ray just reflected from; set
//	 to -1 initially to designate no object
void traceRay(RTTypesRay * ray, int recursionLevel, Color_Int * returnColor,
	Scene_Descriptor * scene, int ignoreObject);

#endif // _RAY___H_