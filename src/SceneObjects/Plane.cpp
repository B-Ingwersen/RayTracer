#include "SceneObjects.h"
#include "Scene.h"

bool plane_getIntersect(void * objectDataRaw, RTTypesRay * ray,
    RTTypesVector * returnPoint, RTTypesVector * returnNormal) {
    
    Plane_SceneObjectData * objectData = (Plane_SceneObjectData *)objectDataRaw;

    RTTypesPlane * plane = &(objectData -> plane);
    float tDenominator = plane -> normalVector.dot(ray -> direction);
    if (tDenominator == 0) {
        return false;
    }
	float t = ( plane -> d - plane -> normalVector.dot(ray -> point) )
        / tDenominator;
	*returnPoint = ray -> point + ray -> direction * t;
    *returnNormal = objectData -> plane.normalVector;
    return true;
}

bool plane_getIntersectNoSelf(void * objectDataRaw, RTTypesRay * ray,
    RTTypesVector * returnPoint, RTTypesVector * returnNormal) {

    return false;
}

void plane_getColor(void * objectDataRaw, RTTypesVector * point,
    Color_Float * returnColor, float * returnReflection) {
    
    Plane_SceneObjectData * objectData = (Plane_SceneObjectData *)objectDataRaw;

    *returnColor = objectData -> color;
    *returnReflection = objectData -> reflection;
}

Scene_ObjectType plane_SceneObject = {
    "plane",
    plane_getIntersect,
    plane_getIntersectNoSelf,
    plane_getColor
};