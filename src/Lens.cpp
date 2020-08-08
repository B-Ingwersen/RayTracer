#include "Lens.h"
#include "Ray.h"

void Lens_Descriptor::passRayThrough(RTTypesRay * ray) {
	RTTypesVector intersect;
	intersectSphereRay(&intersect, &this -> sphere2, ray);

	RTTypesVector normal = this -> sphere2.center - intersect;
	RTTypesVector refractedDirection;
	refractRay(&(ray -> direction), &normal, 1 / this -> glassN,
		&refractedDirection);
	ray -> point = intersect;
	ray -> direction = refractedDirection;

	intersectSphereRay(&intersect, &this -> sphere1, ray);
	normal = this -> sphere1.center - intersect;
	refractRay(&(ray -> direction), &normal, this -> glassN,
		&refractedDirection);
	ray -> point = intersect;
	ray -> direction = refractedDirection;
}
