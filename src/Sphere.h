#pragma once
#include "Primitive.h"

class Sphere : public Primitive
{
public:
	Sphere(float3 c, float r)
	{
		this->center = c;
		this->radius = r;
		this->radius2 = r*r;
	}

	bool intersect(Ray* ray);
	float3 getNormal(float3 point);

	float3 center;
	float radius;
	float radius2;
};
