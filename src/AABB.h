#pragma once

struct AABB
{
	AABB() {};

	AABB(vec3 min, vec3 max)
	{
		this->min = min;
		this->max = max;
	}

	float AABB::CalculateVolume();
	vec3 AABB::CalculateCentroid();

	vec3 min;
	vec3 max;
};