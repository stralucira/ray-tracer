#include "template.h"
#include "Ray.h"

Ray::Ray(vec3 origin, vec3 direction)
{
	this->orig = origin;
	this->dir = direction;
	this->t = INFINITY;

	// BVH helpers
	this->invDir = (1.0f / this->dir);
	this->sign[0] = (invDir.x < 0);
	this->sign[1] = (invDir.y < 0);
	this->sign[2] = (invDir.z < 0);
}

bool Ray::Intersect(AABB bounds)
{
	float tmin, tmax, txmin, txmax, tymin, tymax, tzmin, tzmax;

	txmin = (bounds.min.x - this->orig.x) * this->invDir.x;
	txmax = (bounds.max.x - this->orig.x) * this->invDir.x;
	tymin = (bounds.min.y - this->orig.y) * this->invDir.y;
	tymax = (bounds.max.y - this->orig.y) * this->invDir.y;

	tmin = min(txmin, txmax);
	tmax = max(txmin, txmax);

	tmin = max(tmin, min(tymin, tymax));
	tmax = min(tmax, max(tymin, tymax));

	tzmin = (bounds.min.z - this->orig.z) * this->invDir.z;
	tzmax = (bounds.max.z - this->orig.z) * this->invDir.z;

	tmin = max(tmin, min(tzmin, tzmax));
	tmax = min(tmax, max(tzmin, tzmax));

	return tmax >= tmin && tmax >= 0;

}