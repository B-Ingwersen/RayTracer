#ifndef _LENS___H_
#define _LENS___H_

#include "RTTypes.h"

struct Lens_Descriptor {
	RTTypesSphere sphere1;
	RTTypesSphere sphere2;
	float glassN;

	void passRayThrough(RTTypesRay * ray);
};

#endif // _LENS___H_