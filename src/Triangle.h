#pragma once
#include "Primitive.h"

class Triangle : public Primitive
{
public:
	Triangle(float3 a, float3 b, float3 c)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->normal = ((a - b).cross(b - c)).normalized();
	}
	
	bool intersect(Ray* ray);
	float3 getNormal(float3 point);

	float3 a, b, c;
	float3 normal;
};
