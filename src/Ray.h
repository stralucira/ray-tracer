#pragma once

class Ray
{
public:
	Ray(vec3 orig, vec3 dir);

	vec3 orig;
	vec3 dir;	
	
	float t;
};