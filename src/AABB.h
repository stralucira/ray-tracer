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
	vec3 CalculateCentroid();
	float CalculateArea();
	float CalculateVolume();

	// Variables
	vec3 min;	// 12 bytes ( 3 x 4 bytes )
	vec3 max;	// 12 bytes ( 3 x 4 bytes )
};