#pragma once

#include "template.h"

class Sphere :
	public Primitive
{
public:
	Sphere();

	Sphere(float3 c, float r, Material* mat);
	~Sphere();

	bool IntersectSphere( Ray ray );
	
	float3 getNormal(float3 point);

	float3 center;
	float radius;
	float radius2;
    Material* mat;
};
