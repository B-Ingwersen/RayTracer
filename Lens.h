struct Lens_Descriptor {
	RTTypesSphere sphere1;
	RTTypesSphere sphere2;
	float glassN;

	void passRayThrough(RTTypesRay * ray) {
		RTTypesVector intersect;
		intersectSphereRay(&intersect, &sphere2, ray);

		RTTypesVector normal = sphere2.center - intersect;
		RTTypesVector refractedDirection;
		refractRay(&(ray -> direction), &normal, 1 / glassN, &refractedDirection);
		ray -> point = intersect;
		ray -> direction = refractedDirection;

		intersectSphereRay(&intersect, &sphere1, ray);
		normal = sphere1.center - intersect;
		refractRay(&(ray -> direction), &normal, glassN, &refractedDirection);
		ray -> point = intersect;
		ray -> direction = refractedDirection;
	}
};