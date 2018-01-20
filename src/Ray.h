#pragma once

class Primitive;
class Ray
{
public:
	Ray();
	Ray(vec3 origin, vec3 direction);
	
	float t, u, v;
	vec3 orig;
	vec3 dir;
	Primitive* hit;

	// BVH helpers
	int sign[3];
	vec3 invDir;
};