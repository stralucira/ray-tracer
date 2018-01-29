#pragma once
#include "Primitive.h"

class Sphere : public Primitive
{
public:
	Sphere(vec3 position, float radius, bool isLight = false) : Primitive(position)
	{
		this->radius = radius;
		this->radius2 = radius * radius;
		
		this->centroid = position;
		this->bounds = calculateAABB();

		this->isLight = isLight;
	}
	~Sphere();

	bool intersect(Ray* ray);
	vec2 getTexCoord(Ray * ray);
	bool getIsLight();
	vec3 getNormal(vec3 point);

	AABB* calculateAABB();
	float calculateArea();
	vec3 randomPointOnPrimitive(vec3 point);

	float radius, radius2;

	bool isLight = false;
};