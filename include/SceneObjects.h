#ifndef _SCENE_OBJECTS___H_
#define _SCENE_OBJECTS___H_

#include "RTTypes.h"
#include "Scene.h"

struct Plane_SceneObjectData {
    RTTypesPlane plane;
    float reflection;
    Color_Float color;
};

struct Sphere_SceneObjectData {
    RTTypesSphere sphere;
    float reflection;
    Color_Float color;
};

struct TileFloor_SceneObjectData {
    RTTypesPlane plane;
    float reflection;
    Color_Float color;
};

extern Scene_ObjectType plane_SceneObject;
extern Scene_ObjectType sphere_SceneObject;
extern Scene_ObjectType tileFloor_SceneObject;

#endif // _SCENE_OBJECTS___H_