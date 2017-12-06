#pragma once
#include "Primitive.h"
#include "quarticsolver.h"

class Torus : public Primitive
{
public:
	Torus(float R, float r, vec3 pos, vec3 axis) : Primitive(pos)
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