#pragma once
#include "AABB.h"

class Primitive;
class Ray
{
public:
	Ray(vec3 origin, vec3 direction);
	
	float t;
	float tHit;
	vec3 orig;
	vec3 dir;	

	// BVH helpers
	float Intersect(AABB bounds);

	int sign[3];
	vec3 invDir;
	Primitive* hit;
};