#pragma once

#include "template.h"

class Plane :
	public Primitive
{
public:
	
	Plane();
	Plane(float3 position, float3 normal);
	~Plane();

	float intersect( Ray ray );
	
	float3 getNormal(float3 point);

	float3 position;
	float3 normal;
};
