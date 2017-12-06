#pragma once
#include "Primitive.h"
#include "quarticsolver.h"

class Torus : public Primitive
{
public:
	Torus(vec3 pos, vec3 axis, float R, float r) : Primitive(pos)
	{
		this->position = pos;
		this->R = R;
		this->r = r;
		this->axis = normalize(axis);
	}

	bool intersect(Ray* ray);
	vec3 getNormal(vec3 point);

	float R, r;
	vec3 position, axis;
};