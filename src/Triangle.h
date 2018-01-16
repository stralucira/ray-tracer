#pragma once
#include "Primitive.h"

class Triangle : public Primitive
{
public:
	Triangle(vec3 a, vec3 b, vec3 c, vec3 normal) : Primitive(a)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->normal = normal;
	
		this->centroid = calculateCentroid();
		this->bounds = calculateAABB();
	}
	Triangle(vec3 a, vec3 b, vec3 c, bool sign = false, bool isLight = false) : Primitive(a)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		
		if (sign) this->normal = -normalize(cross(b - a, c - a));
		else this->normal = normalize(cross(b - a, c - a));

		this->centroid = calculateCentroid();
		this->bounds = calculateAABB();

		this->isLight = isLight;
	}
	~Triangle();

	bool intersect(Ray* ray);
	bool getIsLight();
	vec3 getNormal(vec3 point);

	AABB* calculateAABB();
	vec3 calculateCentroid();

	vec3 a, b, c;
	vec3 normal;
	float area;
	bool isLight;
};