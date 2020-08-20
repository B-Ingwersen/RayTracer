#ifndef _SCENE___H_
#define _SCENE___H_

#include "RTTypes.h"

// Scene Object Types describe how to handle a particular geometry; they must
// be able to interpret a custom data format for that particular geometry
// (which is passed as void * objectData). This descriptor is an interface
// specifying how to intersect the geometry and get information about that
// point of intersection
struct Scene_ObjectType {
    // a null-terminated string specifying the name of the object type
    // eg "sphere"
    const char * name;

    // 3 functions that must be implmented for each geometry in order to use
    // them in a ray traced scene
    
    // try to intersect the object with a ray; if the ray does intersect, return
    // true and record the point of intersection and the normal vector at that
    // point through the pointers passed to the function; if there is no
    // intersect, return false
    bool (*getIntersect)(void * objectData, RTTypesRay * ray,
        RTTypesVector * returnPoint, RTTypesVector * returnNormal);
    
    // try and get the intersect as described above, except that the ray passed
    // has its origin at point of intersection on the object; the function
    // should ensure that it does NOT return this same intersect. It may return
    // an intersect of the same object if it is a distinctly different intersect
    bool (*getIntersectNoSelf)(void * objectData, RTTypesRay * ray,
        RTTypesVector * returnPoint, RTTypesVector * returnNormal);
    
    // get the color of an object at the specified point; store the following
    // return data:
    //  *returnColor: the Color_Float containing the color
    //  *returnReflection: a float from 0 to 1 specifying the reflectivity of
    //      the object at that point; 0 is completely reflective, 1 is
    //      completely matte
    void (*getColor)(void * objectData, RTTypesVector * point,
        Color_Float * returnColor, float * returnReflection);
};

// A descriptor of all the elements of a scene, including all the objects
// and the light source
struct Scene_Descriptor {
    int nObjects; // number of objects
    Scene_ObjectType * objectsType; // an array of the types of each object
    void ** objectsData; // an array of the object data
    
    // a point specifying the location of the light source (currently the ray
    // tracer only supports a single, fixed format light souce)
    RTTypesVector * light;

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
    bool intersectRay(RTTypesRay * ray, RTTypesVector * returnPoint,
        RTTypesVector * returnNormal, Color_Float * returnColor,
        float * returnReflection, int ignoreIndex, int * returnIndex);
};

#endif // _SCENE___H_