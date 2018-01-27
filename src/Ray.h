#pragma once
#include "template.h"

class Primitive;
class Ray
{
public:
	Ray() {}
	Ray(vec3 origin, vec3 direction)
	{
		this->orig = origin;
		this->dir = direction;
		this->t = INFINITY;
		this->u = INFINITY;
		this->v = INFINITY;

		// BVH helpers
		this->invDir = 1.0f / this->dir;
	}
	
	float t, u, v;
	vec3 orig;
	vec3 dir;
	Primitive* hit;

	// BVH helpers
	int sign[3];
	vec3 invDir;
};