#include "template.h"
#include "Triangle.h"

#define MOLLER_TRUMBORE

// from https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
bool Triangle::intersect(Ray* ray)
{
	vec3 ab = b - a;
	vec3 ac = c - a; 
	vec3 pvec = cross(ray->dir, ac);
	float det = dot(ab, pvec);

	//if (det > -0.0001f && det < 0.0001f) return false;
	
	float invDet = 1.0f / det;
	
	vec3 tvec = ray->orig - a;
	float u = dot(tvec, pvec) * invDet;
	if (u < 0.0f || u > 1.0f) return false;

	vec3 qvec = cross(tvec, ab);
	float v = dot(ray->dir, qvec) * invDet;
	if (v < 0.0f || u + v > 1.0f) return false;

	float t = dot(ac, qvec) * invDet;

	if (t > 0.0001f && t < ray->t)
	{
		ray->t = t;
		ray->u = u;
		ray->v = v;
		return true;
	}

	return false;
}

vec2 Triangle::getTexCoord(Ray* ray)
{
	return uv0 + ray->u * (uv1 - uv0) + ray->v * (uv2 - uv0);
}

bool Triangle::getIsLight()
{
	return this->isLight;
}

vec3 Triangle::getNormal(vec3 point)
{
	return this->normal0;
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

vec3 Triangle::randomPointOnPrimitive(vec3 point)
{
	
	float x = (float)rand() / (RAND_MAX);
	float y = (float)rand() / (RAND_MAX);
	if (x + y >= 1) {
		x = 1 - x;
		y = 1 - y;
	}

	return a + (b - a) * x + (c - a) * y;
}

float Triangle::calculateArea()
{
	vec3 ab = normalize(b-a);
	vec3 ac = normalize(c-a);
	float theta = acos(dot(ab,ac));

	return glm::length(ab) * glm::length(ac) * sin(theta) * 0.5f;
}

