#ifndef _LENS___H_
#define _LENS___H_

#include "RTTypes.h"

// an object that defines a simple lens; it includes the two spheres that make
// its surface and the refactive index of the material; Lenses are optimized
// for being connected to the camera and CANNOT be used as general objects;
// sphere2 must include the face closer to the camera for the lens descriptor
// to be valid
struct Lens_Descriptor {
	// spheres that define the surface of the lens
	RTTypesSphere sphere1;
	RTTypesSphere sphere2;

	// the refractive index on the lens material
	float glassN;

	// pass a ray through a lens; ray is assumed to intersect the lens; if it
	// doesn't the behavior is undefined.
	// ray is overwritten, and its new value is the point where it exits the
	// lens and the new direction it's traveling
	void passRayThrough(RTTypesRay * ray);
};

#endif // _LENS___H_