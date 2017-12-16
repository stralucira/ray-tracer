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
	float CalculateVolume();
	vec3 CalculateCentroid();

	// Variables
	vec3 min;
	vec3 max;
};