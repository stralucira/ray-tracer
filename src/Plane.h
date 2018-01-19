#pragma once
#include "Primitive.h"

class Plane : public Primitive
{
public:
	Plane(vec3 position, vec3 normal) : Primitive(position)
	{
		this->normal = normal;
		
		this->centroid = position;
		this->bounds = calculateAABB();
	}
	~Plane();

	bool intersect(Ray* ray);
	vec2 getTexCoord(Ray * ray);
	bool getIsLight();
	vec3 getNormal(vec3 point);

	AABB* calculateAABB();

	vec3 normal;
};