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
	return vec3((a.x + b.x + c.x) / 3.0f, (a.y + b.y + c.y) / 3.0f, (a.z + b.z + c.z) / 3.0f);
}

AABB* Triangle::calculateAABB()
{
	float maxX = -INFINITY;
	float maxY = -INFINITY;
	float maxZ = -INFINITY;

	float minX = INFINITY;
	float minY = INFINITY;
	float minZ = INFINITY;

	if (a.x >= maxX) { maxX = a.x; }
	if (b.x >= maxX) { maxX = b.x; }
	if (c.x >= maxX) { maxX = c.x; }

	if (a.y >= maxY) { maxY = a.y; }
	if (b.y >= maxY) { maxY = b.y; }
	if (c.y >= maxY) { maxY = c.y; }

	if (a.z >= maxZ) { maxZ = a.z; }
	if (b.z >= maxZ) { maxZ = b.z; }
	if (c.z >= maxZ) { maxZ = c.z; }

	if (a.x <= minX) { minX = a.x; }
	if (b.x <= minX) { minX = b.x; }
	if (c.x <= minX) { minX = c.x; }

	if (a.y <= minY) { minY = a.y; }
	if (b.y <= minY) { minY = b.y; }
	if (c.y <= minY) { minY = c.y; }

	if (a.z <= minZ) { minZ = a.z; }
	if (b.z <= minZ) { minZ = b.z; }
	if (c.z <= minZ) { minZ = c.z; }

	return new AABB(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));
}