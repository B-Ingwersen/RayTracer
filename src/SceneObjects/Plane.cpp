#include "SceneObjects.h"
#include "Scene.h"

// implementation of getIntersect for plane objects (see Scene_ObjectType)
bool plane_getIntersect(void * objectDataRaw, RTTypesRay * ray,
    RTTypesVector * returnPoint, RTTypesVector * returnNormal) {
    
    Plane_SceneObjectData * objectData = (Plane_SceneObjectData *)objectDataRaw;

    // if the ray is parallel with plane, return false (no intersect)
    RTTypesPlane * plane = &(objectData -> plane);
    float tDenominator = plane -> normalVector.dot(ray -> direction);
    if (tDenominator == 0) {
        return false;
    }

    // using the ray as the basis for a parametric equation, find the intersect
    // point with the plane
	float t = ( plane -> d - plane -> normalVector.dot(ray -> point) )
        / tDenominator;
	*returnPoint = ray -> point + ray -> direction * t;

    // the normal vector is always the same as the plane's normal vector
    *returnNormal = objectData -> plane.normalVector;
    return true;
}

// implementation of getIntersectNoSelf for plane objects (see Scene_ObjectType)
bool plane_getIntersectNoSelf(void * objectDataRaw, RTTypesRay * ray,
    RTTypesVector * returnPoint, RTTypesVector * returnNormal) {
    
    // a ray reflecting off of a plane can never directly intersect that same
    // plane again
    return false;
}

// implementation of getColor for plane objects (see Scene_ObjectType)
void plane_getColor(void * objectDataRaw, RTTypesVector * point,
    Color_Float * returnColor, float * returnReflection) {
    
    // the color/reflectivity accross the object is constant
    Plane_SceneObjectData * objectData = (Plane_SceneObjectData *)objectDataRaw;
    *returnColor = objectData -> color;
    *returnReflection = objectData -> reflection;
}

// Descriptor the plane scene object type
Scene_ObjectType plane_SceneObject = {
    "plane",
    plane_getIntersect,
    plane_getIntersectNoSelf,
    plane_getColor
};