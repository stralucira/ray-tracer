#pragma once
#include "Primitive.h"

class Sphere : public Primitive
{
public:
	Sphere(vec3 position, float radius) : Primitive(position)
	{
		this->center = position;
		this->radius = radius;
		this->radius2 = radius * radius;
	}

	bool intersect(Ray* ray);
	vec3 getNormal(vec3 point);

	vec3 center;
	float radius, radius2;
};