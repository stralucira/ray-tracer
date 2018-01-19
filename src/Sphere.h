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
		this->bounds = calculateAABB();
	}
	~Sphere();

	bool intersect(Ray* ray);
	vec2 getTexCoord(Ray * ray);
	bool getIsLight();
	vec3 getNormal(vec3 point);

	AABB* calculateAABB();

	float radius, radius2;
};