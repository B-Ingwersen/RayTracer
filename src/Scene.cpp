
#include "Scene.h"
#include <cfloat>

// Get the intersection of a ray with the scene. If an intersect is found,
// return true and store in the pointers passed the normal vector at the
// intersection point, the color of the object intersected, the reflectivity
// of the object at that point (see Scene_ObjectType::getColor for details),
// and the index of the object that was intersected. If no intersection is
// found, return false. The closest intersect along the positive direction
// of the ray is returned if there are multiple intersects found
// The ignore index specifies the object that the ray is starting at the
// intersection of; if set to a valid index, the rules described in
// Scene_ObjectType::getIntersectNoSelf will be applied; if an invalid
// index is used, this behavior will be ignored. It is recommended that -1
// is used as an invalid index
bool Scene_Descriptor::intersectRay(RTTypesRay * ray,
    RTTypesVector * returnPoint, RTTypesVector * returnNormal,
    Color_Float * returnColor, float * returnReflection, int ignoreIndex,
    int * returnIndex) {

    // variables used for storing the information of the best intersect found
    // so far
    *returnIndex = -1;
    RTTypesVector point;
    RTTypesVector normal;

    // the distance to the closest intersect currently found
    float minDistance = FLT_MAX;

    // iterate through each object in the scene
    for (int index = 0; index < this -> nObjects; index++) {

        // if the object matches the ignore index, use the no-self intersect
        // method; otherwise, ignore this behavior
        bool intersect;
        if (index == ignoreIndex) {
            intersect = objectsType[index].getIntersectNoSelf(
                objectsData[index], ray, &point, &normal);
        }
        else {
            intersect = objectsType[index].getIntersect(
                objectsData[index], ray, &point, &normal);
        }

        if (intersect) {
            // check if the intersect is in the positive direction of the ray;
            // if it is in the wrong direction, the intersect is discarded
            RTTypesVector diff = point - ray -> point;
            if (diff.dot(ray -> direction) < 0) {
                continue;
            }

            // check if the intersect is closer than the other intersects found;
            // if so, save it as the best intersect found
            float distance = diff.abs();
            if (distance < minDistance) {
                minDistance = distance;
                *returnIndex = index;
                *returnPoint = point;
                *returnNormal = normal;
            }
        }
    }

    // if an intersect was found, get the color and return true; otherwise,
    // return false
    if (*returnIndex != -1) {
        objectsType[*returnIndex].getColor(objectsData[*returnIndex],
            returnPoint, returnColor, returnReflection);
        return true;
    }
    return false;
}