#pragma once
#include "Primitive.h"
#include "quarticsolver.h"

class Torus : public Primitive
{
public:
	Torus(vec3 position, vec3 axis, float R, float r) : Primitive(position)
	{
		this->R = R;
		this->r = r;
		this->axis = normalize(axis);

		this->center = position;
		this->boundingBox = calculateAABB();
	}

	bool intersect(Ray* ray);
	vec3 getNormal(vec3 point);

	AABB* calculateAABB();

	float R, r;
	vec3 axis;
};