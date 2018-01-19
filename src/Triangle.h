#pragma once
#include "Primitive.h"

class Triangle : public Primitive
{
public:
	Triangle(vec3 a, vec3 b, vec3 c, vec3 normal) : Primitive(a)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->normal = normal;
	
		this->centroid = calculateCentroid();
		this->bounds = calculateAABB();
	}
	Triangle(vec3 a, vec3 b, vec3 c, vec3 normal, vec3 normal1, vec3 normal2, vec2 uv0, vec2 uv1, vec2 uv2) : Primitive(a)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->normal = normal;
		this->normal = normal1;
		this->normal = normal2;
		this->uv0 = uv0;
		this->uv1 = uv1;
		this->uv2 = uv2;

		this->centroid = calculateCentroid();
		this->bounds = calculateAABB();
	}
	Triangle(vec3 a, vec3 b, vec3 c, bool sign = false, bool isLight = false) : Primitive(a)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		
		if (sign) this->normal = -normalize(cross(b - a, c - a));
		else this->normal = normalize(cross(b - a, c - a));

		this->centroid = calculateCentroid();
		this->bounds = calculateAABB();

		this->isLight = isLight;
	}
	~Triangle();

	bool intersect(Ray* ray);
	vec2 getTexCoord(Ray * ray);
	bool getIsLight();
	vec3 getNormal(vec3 point);

	AABB* calculateAABB();
	vec3 calculateCentroid();

	vec3 a, b, c;
	vec3 normal, normal1, normal2;
	vec2 uv0, uv1, uv2;
	float area;
	bool isLight;
};