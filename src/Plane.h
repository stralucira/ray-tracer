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
	bool getIsTriangle() { return false; }
	vec3 getNormal(vec3 point);

	AABB* calculateAABB();
	float calculateArea();
	vec3 randomPointOnPrimitive(vec3 point);

	vec3 normal;

	bool isLight = false;
};