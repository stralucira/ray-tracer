#pragma once
#include "Primitive.h"

class Sphere : public Primitive
{
public:
	Sphere(vec3 position, float radius) : Primitive(position)
	{
		this->radius = radius;
		this->radius2 = radius * radius;
		
		this->centroid = position;
		this->boundingBox = calculateAABB();
	}

	bool intersect(Ray* ray);
	vec3 getNormal(vec3 point);

	AABB* calculateAABB();

	float radius, radius2;
};