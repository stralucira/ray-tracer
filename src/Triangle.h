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
		this->boundingBox = calculateAABB();
	}
	Triangle(vec3 a, vec3 b, vec3 c) : Primitive(a)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->normal = normalize(cross(b - a, c - a));

		this->centroid = calculateCentroid();
		this->boundingBox = calculateAABB();
	}
	Triangle(vec3 a, vec3 b, vec3 c, int sign) : Primitive(a)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->normal = -normalize(cross(b - a, c - a));

		this->centroid = calculateCentroid();
		this->boundingBox = calculateAABB();
	}
	~Triangle();

	bool intersect(Ray* ray);
	vec3 getNormal(vec3 point);

	AABB* calculateAABB();
	vec3 calculateCentroid();

	vec3 a, b, c;
	vec3 normal;
};