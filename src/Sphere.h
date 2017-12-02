#pragma once
#include "Primitive.h"

class Sphere : public Primitive
{
public:
	Sphere(vec3 pos, float r) : Primitive(pos)
	{
		this->center = pos;
		this->radius = r;
		this->radius2 = r*r;
	}

	bool intersect(Ray* ray);
	vec3 getNormal(vec3 point);

	vec3 center;
	float radius, radius2;
};