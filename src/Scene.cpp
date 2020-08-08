
#include "Scene.h"
#include <cfloat>

bool Scene_Descriptor::intersectRay(RTTypesRay * ray,
    RTTypesVector * returnPoint, RTTypesVector * returnNormal,
    Color_Float * returnColor, float * returnReflection, int ignoreIndex,
    int * returnIndex) {
    
    int index;

    *returnIndex = -1;
    RTTypesVector point;
    RTTypesVector normal;
    float minDistance = FLT_MAX;
    for (index = 0; index < nObjects; index++) {
        bool intersect;
        if (index == ignoreIndex) {
            intersect = objectsType[index].getIntersectNoSelf(objectsData[index], ray, &point, &normal);
        }
        else {
            intersect = objectsType[index].getIntersect(objectsData[index], ray, &point, &normal);
        }

        if (intersect) {
            RTTypesVector diff = point - ray -> point;
            if (diff.dot(ray -> direction) < 0) {
                continue;
            }
            float distance = diff.abs();
            if (distance < minDistance) {
                minDistance = distance;
                *returnIndex = index;
                *returnPoint = point;
                *returnNormal = normal;
            }
        }
    }

    if (*returnIndex != -1) {
        objectsType[*returnIndex].getColor(objectsData[*returnIndex], returnPoint, returnColor, returnReflection);
        return true;
    }
    return false;
}