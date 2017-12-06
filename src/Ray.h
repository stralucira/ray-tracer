#pragma once

class Ray
{
public:
	Ray();
	Ray(vec3 origin, vec3 direction);

	vec3 orig;
	vec3 dir;	
	
	float t;
};