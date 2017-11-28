#include "template.h"

Ray::Ray()
{
}

Ray::Ray(float3 orig, float3 dir)
{
	this->dir = dir.normalized();
	this->orig = orig;
	this->t = 0; 
}

Ray::~Ray()
{
}
