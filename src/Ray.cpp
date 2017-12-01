#include "template.h"
#include "Ray.h"

Ray::Ray(float3 orig, float3 dir)
{
	this->dir = dir;
	this->orig = orig;
	this->t = INFINITY; 
}
