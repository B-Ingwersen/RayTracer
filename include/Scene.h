#ifndef _SCENE___H_
#define _SCENE___H_

#include "RTTypes.h"

struct Scene_ObjectType {
    const char * name;
    bool (*getIntersect)(void * objectData, RTTypesRay * ray,
        RTTypesVector * returnPoint, RTTypesVector * returnNormal);
    bool (*getIntersectNoSelf)(void * objectData, RTTypesRay * ray,
        RTTypesVector * returnPoint, RTTypesVector * returnNormal);
    void (*getColor)(void * objectData, RTTypesVector * point,
        Color_Float * returnColor, float * returnReflection);
};

struct Scene_Descriptor {
    int nObjects;
    Scene_ObjectType * objectsType;
    void ** objectsData;
    RTTypesVector * light;

    bool intersectRay(RTTypesRay * ray, RTTypesVector * returnPoint,
        RTTypesVector * returnNormal, Color_Float * returnColor,
        float * returnReflection, int ignoreIndex, int * returnIndex);
};

#endif // _SCENE___H_