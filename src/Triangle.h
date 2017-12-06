#pragma once
#include "Primitive.h"

class Triangle : public Primitive
{
public:
	Triangle(vec3 position, vec3 b, vec3 c) : Primitive(position)
	{
		this->a = position;
		this->b = b;
		this->c = c;
		this->normal = normalize(cross(position - b, b - c));
	}

	bool intersect(Ray* ray);
	vec3 getNormal(vec3 point);

	vec3 a, b, c;
	vec3 normal;
};