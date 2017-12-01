#include "template.h"

Plane::Plane()
{

	this->mat = new Material(float4(0.1, 0.1, 0.1, 1));
	this->position = float3(0,0,20);
	this->normal = float3(0,0,-1);

}

Plane::Plane(float3 position, float3 normal)
{
	this->position = position;
	this->normal = normal;
	this->mat = new Material(float4(1,0,1,1));
}

Plane::~Plane()
{
}

float3 Plane::getNormal(float3 point) {
	return this->normal;
}

float Plane::intersect( Ray* ray )
{

	// denominator in the vector-plane intersection 
	// equation after solving for t
	float denominator = dot(normal,ray->dir);

	if(abs(denominator) > 0.00001f)
	{
		float tHit = dot((position - ray->orig), normal) / denominator;

		if (tHit>0.00001f) {
			ray->t = tHit;
			return tHit;
		}
	}
	return -1;
}
