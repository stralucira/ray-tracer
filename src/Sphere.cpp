#include "template.h"
#include <math.h>

Sphere::Sphere()
{
	this->center = float3(0, 0, 0);
	this->radius = 5;
	this->radius2 = 5*5;
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

float3 Sphere::intersect( Ray ray )
{
float3 c = this->center - ray.orig;

float t = dot( c, ray.dir );
float3 q = c - ray.dir * t;

float p2 = dot( q, q );
if (p2 > this->radius2) return -1; // r2 = r * r
t -= sqrtf(this->radius2-p2);

if ((t < ray.t) && (t > 0)){
	ray.t = t;
	return (ray.orig + ray.dir*t);
} else return -1;
// or: ray.t = min( ray.t, max( 0, t ) );
}

float3 Sphere::getNormal(float3 point){
    return (point - center).normalized();
}
