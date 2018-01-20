#include "template.h"
#include "Sphere.h"

// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool Sphere::intersect(Ray* ray)
{
	float t0, t1; // solutions for t if the ray intersects

	vec3 L = this->centroid - ray->orig;
	float tca = dot(L, ray->dir);
	if (tca < 0) return false;
	float d2 = dot(L, L) - tca * tca;
	if (d2 > radius2) return false;
	float thc = sqrt(radius2 - d2);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0)
	{
		t0 = t1; // if t0 is negative, let's use t1 instead
		if (t0 < 0) return false; // both t0 and t1 are negative
	}
	ray->t = t0;

	return true;
}

vec2 Sphere::getTexCoord(Ray* ray)
{
	return vec2(0.0f);
}

bool Sphere::getIsLight()
{
	return this->isLight;
}

vec3 Sphere::getNormal(vec3 point)
{
	return normalize(point - this->centroid);
}

AABB* Sphere::calculateAABB()
{
	return new AABB(this->centroid - radius, this->centroid + radius);
}

vec3 Sphere::randomPointOnSphere(vec3 p) {
		
		vec3 e2 = vec3(1.0f,0.0f,0.0f);
	  	vec3 e3 = cross((this->centroid-p) , e2);
	  	normalize(e3);
	  	e3 = e3*radius;
	  	normalize(e2);
	  	e2 = e2*radius;

	  	//std::cout << e2 << ", " << e3 << std::endl;
	  	//srand(time(NULL));
	  	float x, y;
	  	do {
	   		x = ((float)rand())/((float)RAND_MAX)*2 - 1;
	    	y = ((float)rand())/((float)RAND_MAX)*2 - 1;
	  	} while (x*x + y*y > 1);

	  	return this->centroid + (x * e2 + e3*y);
	}