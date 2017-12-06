#pragma once
#include "Primitive.h"

class Cylinder : public Primitive
{
public:
	Cylinder(vec3 position, vec3 up, float radius, float height) : Primitive(position)
	{
		this->pos = position;
		this->up = up;
		this->radius = radius;
		this->height = height;
	}

	bool intersect(Ray* ray);
	vec3 getNormal(vec3 point);

	vec3 pos;
	vec3 up;
	float radius;
	float height;
};