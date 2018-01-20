#pragma once
#include "Ray.h"
#include "RayPacket.h"
#include "Material.h"
#include "AABB.h"

class Primitive
{
public:
	Primitive(vec3 pos);

	// default material color and shader
	Material* material;

	virtual bool intersect(Ray* ray) = 0;
	virtual bool getIsLight() = 0;
	virtual vec3 getNormal(vec3 point) = 0;
	virtual vec2 getTexCoord(Ray* ray) = 0;
	virtual AABB* calculateAABB() = 0;

	vec3 centroid;
	AABB* bounds;
};
