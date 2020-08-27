#ifndef _SCENE_OBJECTS___H_
#define _SCENE_OBJECTS___H_

#include "RTTypes.h"
#include "Scene.h"

// Plane SceneObject data type
struct Plane_SceneObjectData {
    // the geometrical description of the plane -- note that the direction of
    // the normal vector must be out from the visible side of the floor
    RTTypesPlane plane;

    // reflection: a value from 0 (matte) to 1 (completely reflective)
    float reflection;

    // the color of the object
    Color_Float color;
};

// Sphere SceneObject data type
struct Sphere_SceneObjectData {
    // the geometric description of the sphere
    RTTypesSphere sphere;

    // reflection: a value from 0 (matte) to 1 (completely reflective)
    float reflection;

    // the color of the object
    Color_Float color;
};

// TileFloor SceneObject data
struct TileFloor_SceneObjectData {
    // the geometrical description of the plane of the tile floor -- note that
    // the direction of the normal must be out from the visible side of the
    // floor
    RTTypesPlane plane;

    // reflection: a value from 0 (matte) to 1 (completely reflective)
    float reflection;

    // the color of the object
    Color_Float color;
};

// the builtin scene object types -- defined in src/SceneObjects
extern Scene_ObjectType plane_SceneObject;
extern Scene_ObjectType sphere_SceneObject;
extern Scene_ObjectType tileFloor_SceneObject;

#endif // _SCENE_OBJECTS___H_