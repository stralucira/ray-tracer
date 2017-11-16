#include "Ray.h"


Ray::Ray(float3 orig, float3 dir)
{
	this->direction = dir.normalized();
	this->origin = orig;
}


Ray::~Ray()
{
}

float3 Ray::getDirection() const
{
	return this->direction;
}

float3 Ray::getOrigin() const
{
	return this->origin;
}