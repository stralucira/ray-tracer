#pragma once

struct AABB
{
public:
	// Constructors
	AABB() {};
	AABB(vec3 min, vec3 max)
	{
		this->min = min;
		this->max = max;
	}

	// Functions
	float AABB::CalculateVolume();
	vec3 AABB::CalculateCentroid();

	// Variables
	vec3 min;
	vec3 max;
};