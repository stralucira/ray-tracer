#pragma once
#include "Primitive.h"

class Plane : public Primitive
{
public:
	Plane(vec3 pos, vec3 normal) : Primitive(pos)
	{
		this->normal = normal;
		this->pos = pos;
	}

	bool intersect(Ray* ray);
	vec3 getNormal(vec3 point);

	vec3 pos;
	vec3 normal;
};