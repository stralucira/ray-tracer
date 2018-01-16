#pragma once
#include "Primitive.h"

class Cylinder : public Primitive
{
public:
	Cylinder(vec3 position, vec3 up, float radius, float height) : Primitive(position)
	{
		this->up = up;
		this->radius = radius;
		this->height = height;

		this->centroid = position;
		this->bounds = calculateAABB();
	}
	~Cylinder();

	bool intersect(Ray* ray);
	bool getIsLight();
	vec3 getNormal(vec3 point);

	AABB* calculateAABB();

	vec3 up;
	float radius;
	float height;
};