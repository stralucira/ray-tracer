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
	bool Intersect(AABB bounds);
	float IntersectT(AABB bounds);

	int sign[3];
	vec3 invDir;
	Primitive* hit;
};