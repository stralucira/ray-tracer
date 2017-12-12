#include "template.h"
#include "Sphere.h"

// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool Sphere::intersect(Ray* ray)
{
	float t0, t1; // solutions for t if the ray intersects

	vec3 L = this->center - ray->orig;
	float tca = dot(L, ray->dir);
	if (tca < 0) return false;
	float d2 = dot(L, L) - tca * tca;
	if (d2 > radius2) return false;
	float thc = sqrt(radius2 - d2);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0)
	{
		t0 = t1; // if t0 is negative, let's use t1 instead
		if (t0 < 0) return false; // both t0 and t1 are negative
	}
	ray->t = t0;

	return true;
}

vec3 Sphere::getNormal(vec3 point)
{
	return normalize(point - this->center);
}

AABB* Sphere::calculateAABB()
{
	return new AABB(this->center - radius, this->center + radius);
}