#include "template.h"
#include "Sphere.h"

// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool Sphere::intersect(Ray* ray)
{
	float t, t0, t1; // solutions for t if the ray intersects

	float3 L = center - ray->orig;
	float tca = L.dot(ray->dir);
	if (tca < 0) return false;
	float d2 = L.dot(L) - tca * tca;
	if (d2 > radius2) return false;
	float thc = sqrtf(radius2 - d2);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0)
	{
		t0 = t1; // if t0 is negative, let's use t1 instead
		if (t0 < 0) return false; // both t0 and t1 are negative
	}

	t = t0;

	return true;
}

float3 Sphere::getNormal(float3 point)
{
    return (point - center).normalized();
}
