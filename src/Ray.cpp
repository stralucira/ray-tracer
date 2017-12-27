#include "template.h"
#include "Ray.h"

Ray::Ray(vec3 origin, vec3 direction)
{
	this->orig = origin;
	this->dir = direction;
	this->t = INFINITY;

	// BVH helpers
	this->invDir = 1.0f / this->dir;
}