#include "template.h"
#include "Ray.h"

Ray::Ray(vec3 orig, vec3 dir)
{
	this->orig = orig;
	this->dir = dir;
	this->t = INFINITY;
}
