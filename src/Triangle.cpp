#include "Triangle.h"


Triangle::Triangle()
{
	this->a = float3(0, 1, 0);
	this->b = float3(1, 0, 0);
	this->c = float3(-1, 0, 0);
}

Triangle::Triangle(float3 a, float3 b, float3 c, Material mat)
{
	this->a = a;
	this->b = b;
	this->c = c;
	this->normal = ((a - b).cross(b - c)).normalized();
	this->mat = mat;
}

Triangle::~Triangle()
{
}

// from https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
void Triangle::IntersectTriangle( Ray ray ) {

	float3 ab = b - a;
	float3 ac = c - a;
	float3 pvec = ray.dir.cross(ac);
	float det = ab.dot(pvec);
};