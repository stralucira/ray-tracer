#pragma once
#include "Primitive.h"

class Triangle : public Primitive
{
public:
	Triangle(vec3 pos, vec3 b, vec3 c) : Primitive(pos)
	{
		this->a = pos;
		this->b = b;
		this->c = c;
		this->normal = normalize(cross(pos - b, b - c));
	}

	bool intersect(Ray* ray);
	vec3 getNormal(vec3 point);

	vec3 a, b, c;
	vec3 normal;

};