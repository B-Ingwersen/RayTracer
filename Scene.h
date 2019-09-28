struct Scene_ObjectType {
    const char * name;
    bool (*getIntersect)(void * objectData, RTTypesRay * ray, RTTypesVector * returnPoint, RTTypesVector * returnNormal);
    bool (*getIntersectNoSelf)(void * objectData, RTTypesRay * ray, RTTypesVector * returnPoint, RTTypesVector * returnNormal);
    void (*getColor)(void * objectData, RTTypesVector * point, Color_Float * returnColor, float * returnReflection);
};

#include "SceneObjects/Plane.h"
#include "SceneObjects/Sphere.h"
#include "SceneObjects/TiledFloor.h"

struct Scene_Descriptor {
    int nObjects;
    Scene_ObjectType * objectsType;
    void ** objectsData;
    RTTypesVector * light;

    bool intersectRay(RTTypesRay * ray, RTTypesVector * returnPoint, RTTypesVector * returnNormal, Color_Float * returnColor, float * returnReflection, int ignoreIndex, int * returnIndex) {
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
};