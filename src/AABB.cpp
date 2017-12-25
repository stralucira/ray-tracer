#include "template.h"
#include "AABB.h"

vec3 AABB::CalculateCentroid()
{
	return ((this->max - this->min) * 0.5f);
}

float AABB::CalculateArea()
{
	vec3 length = this->max - this->min;
	return (length.x * length.y + length.x * length.z + length.y * length.z) * 2.0f;
}

float AABB::CalculateVolume()
{
	vec3 length = this->max - this->min;
	return length.x * length.y * length.z;
}