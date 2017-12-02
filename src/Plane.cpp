#include "template.h"
#include "Plane.h"

bool Plane::intersect(Ray* ray)
{
	float denominator = dot(normal, ray->dir);

	if (abs(denominator) > 0.00001f)
	{
		float t = dot((pos - ray->orig), normal) / denominator;
		
		if (t >= 0.00001f)
		{
			ray->t = t;
			return true;
		}
	}
	return false;
}

vec3 Plane::getNormal(vec3 point)
{
	return this->normal;
}