#ifndef _RAY___H_
#define _RAY___H_

#include "RTTypes.h"
#include "Scene.h"

bool intersectSphereRay(RTTypesVector * intersect, RTTypesSphere * sphere,
	RTTypesRay * ray);

void intersectPlaneRay(RTTypesVector * IntersectOut, RTTypesPlane * plane,
	RTTypesRay * ray);

void findReflectedRay(RTTypesRay * OutputRay, RTTypesRay * ray,
	RTTypesVector * reflPoint, RTTypesVector * normalVec);

void refractRay(RTTypesVector * ray, RTTypesVector * normal, float indexRatio,
	RTTypesVector * returnRay);

void traceRay(RTTypesRay * ray, int recursionLevel, Color_Int * returnColor,
	Scene_Descriptor * scene, int ignoreObject);

#endif // _RAY___H_