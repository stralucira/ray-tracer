#include "template.h"
#include <math.h> 

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
bool Triangle::IntersectTriangle( Ray ray ) {

	float t,u,v;

	float3 ab = b - a;
	float3 ac = c - a;
	float3 pvec = ray.dir.cross(ac);
	float det = ab.dot(pvec);

	float invDet = 1 / det; 

	float3 tvec = ray.orig - a;
	u = tvec.dot(pvec)*invDet;

	if (u < 0 || u > 1) return false;

	float3 qvec = tvec.cross(ab);
	v = ray.dir.dot(qvec) * invDet;
	if (v < 0 || u + v > 1) return false;

	t = ac.dot(qvec) * invDet;
	return true;
};

float3 Triangle::getNormal(float3 point) {
    return (a - c).cross(b - c).normalized();
}
