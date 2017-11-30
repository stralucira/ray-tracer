#include "template.h"
#include <math.h>

Sphere::Sphere()
{
	this->center = float3(0, 0, 10);
	this->radius = 1;
	this->radius2 = 1*1;
    this->mat = new Material(float4(0,1,1,1));
}

Sphere::Sphere(float3 c, float r, Material* mat)
{
	this->center = c;
	this->radius = r;
	this->radius2 = r*r;
	this->mat = mat;
}

Sphere::~Sphere()
{
}

//float3 Sphere::intersect( Ray ray )
//{
//float3 c = this->center - ray.orig;
//
//float t = dot( c, ray.dir );
//float3 q = c - ray.dir * t;
//
//float p2 = dot( q, q );
//if (p2 > this->radius2) return -1; // r2 = r * r
//t -= sqrtf(this->radius2-p2);
//
//if ((t < ray.t) && (t > 0)){
//    ray.t = t;
//    return (ray.orig + -ray.dir*ray.t);
//} else return -1;
//// or: ray.t = min( ray.t, max( 0, t ) );
//}

float Sphere::intersect( Ray* ray )
{
	float t0, t1; // solutions for t if the ray intersects

	float3 L = center - ray->orig;
	float tca = L.dot(ray->dir);
	if (tca < 0) return -1;
	float d2 = L.dot(L) - tca * tca;
	if (d2 > radius2) return -1;
	float thc = sqrtf(radius2 - d2);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0)
	{
		t0 = t1; // if t0 is negative, let's use t1 instead
		if (t0 < 0) return -1; // both t0 and t1 are negative
	}

	ray->t = t0;

	return t0;
}

float3 Sphere::getNormal(float3 point)
{
    return (point - center).normalized();
}
