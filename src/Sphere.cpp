#include "Sphere.h"
#include <math.h>

Sphere::Sphere()
{
	this->center = float3(0, 0, 0);
	this->radius = 5;
}

Sphere::Sphere(float3 c, float r, Material mat)
{
	this->center = c;
	this->radius = r;
	this->mat = mat;
}

Sphere::~Sphere()
{
}