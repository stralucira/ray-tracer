#include "template.h"
#include "AABB.h"

float AABB::CalculateVolume()
{
	vec3 length = this->max - this->min;
	return length.x * length.y * length.z;
}

vec3 AABB::CalculateCentroid()
{
	return ((this->max - this->min) * 0.5f);
}