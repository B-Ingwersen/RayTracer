#include "Lens.h"
#include "Ray.h"

// pass a ray through a lens; ray is assumed to intersect the lens; if it
// doesn't the behavior is undefined.
// ray is overwritten, and its new value is the point where it exits the
// lens and the new direction it's traveling
void Lens_Descriptor::passRayThrough(RTTypesRay * ray) {

	// find the point where the ray intersects the second sphere
	RTTypesVector intersect;
	intersectSphereRay(&intersect, &this -> sphere2, ray);

	// get the normal vector at the first point of intersect
	RTTypesVector normal = this -> sphere2.center - intersect;

	// calculate the direction that the ray travels after it enters the lens
	RTTypesVector refractedDirection;
	refractRay(&(ray -> direction), &normal, 1 / this -> glassN,
		&refractedDirection);
	ray -> point = intersect;
	ray -> direction = refractedDirection;

	// calculate the locaiton where the ray exits ths lens
	intersectSphereRay(&intersect, &this -> sphere1, ray);

	// calculate the normal vector on the lens at the second point of
	// intersection
	normal = this -> sphere1.center - intersect;

	// calculate the direction that the ray travels after it exits the lens
	refractRay(&(ray -> direction), &normal, this -> glassN,
		&refractedDirection);
	ray -> point = intersect;
	ray -> direction = refractedDirection;
}
