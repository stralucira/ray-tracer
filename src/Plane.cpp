#include "template.h"
#include "Plane.h"

bool Plane::intersect(Ray* ray)
{
	float denominator = dot(normal, ray->dir);

	if (abs(denominator) > EPSILON)
	{
		float t = dot((this->centroid - ray->orig), normal) / denominator;
		
		if (t >= EPSILON)
		{
			ray->t = t;
			return true;
		}
	}
	return false;
}

vec2 Plane::getTexCoord(Ray* ray)
{
	return vec2(0.0f);
}

bool Plane::getIsLight()
{
	return this->isLight;
}

vec3 Plane::getNormal(vec3 point)
{
	return this->normal;
}

AABB* Plane::calculateAABB()
{
	return new AABB(vec3(-INFINITY, -INFINITY, -INFINITY), vec3(INFINITY, INFINITY, INFINITY));
}

float Plane::calculateArea()
{
	return INFINITY;
}

vec3 Plane::randomPointOnPrimitive(vec3 point)
{
	return point;
}