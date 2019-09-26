
struct Plane_SceneObjectData {
    Objects_Plane plane;
    float reflection;
    Color_Float color;
};

bool plane_getIntersect(void * objectDataRaw, Objects_Ray * ray, Objects_Vector * returnPoint, Objects_Vector * returnNormal) {
    Plane_SceneObjectData * objectData = (Plane_SceneObjectData *)objectDataRaw;

    Objects_Plane * plane = &(objectData -> plane);
    float tDenominator = plane -> normalVector.dot(ray -> direction);
    if (tDenominator == 0) {
        return false;
    }
	float t = ( plane -> d - plane -> normalVector.dot(ray -> point) ) / tDenominator;
	*returnPoint = ray -> point + ray -> direction * t;
    *returnNormal = objectData -> plane.normalVector;
    return true;
}

bool plane_getIntersectNoSelf(void * objectDataRaw, Objects_Ray * ray, Objects_Vector * returnPoint, Objects_Vector * returnNormal) {
    return false;
}

void plane_getColor(void * objectDataRaw, Objects_Vector * point, Color_Float * returnColor, float * returnReflection) {
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