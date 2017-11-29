#pragma once
#include "template.h"

#include <stdlib.h>

class Triangle : public Primitive
{
public:
	
	Triangle();

	Triangle(float3 a, float3 b, float3 c, Material mat);
	~Triangle();
	

	bool IntersectTriangle( Ray ray );
	float3 getNormal(float3 point);

private:
	float3 a;
	float3 b;
	float3 c;
	float3 normal;
};
