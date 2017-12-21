#include "template.h"
#include "Triangle.h"

// from https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
bool Triangle::intersect(Ray* ray)
{
	vec3 ab = b - a;
	vec3 ac = c - a; 
	vec3 pvec = cross(ray->dir, ac);
	float det = dot(ab, pvec);

	if (det > -0.00001f && det < 0.00001f) return false;
	
	float invDet = 1.0f / det;
	vec3 tvec = ray->orig - a;
	float u = dot(tvec, pvec) * invDet;

	if (u < 0.0f || u > 1.0f) return false;

	vec3 qvec = cross(tvec, ab);
	float v = dot(ray->dir, qvec) * invDet;

	if (v < 0.0f || u + v > 1.0f) return false;

	float t = dot(ac, qvec) * invDet;

	if (t > 0.00001f)
	{
		ray->t = t;
		return true;
	}

	return false;
}

vec3 Triangle::getNormal(vec3 point)
{
	return this->normal;
}

vec3 Triangle::calculateCentroid()
{
	return (a + b + c) / 3.0f;
}

AABB* Triangle::calculateAABB()
{
	vec3 max = glm::max(glm::max(a, b), c);
	vec3 min = glm::min(glm::min(a, b), c);
	return new AABB(min, max);
}