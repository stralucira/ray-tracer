﻿#pragma once
#include "Ray.h"
#include "Material.h"
#include "AABB.h"

class Primitive
{
public:
	Primitive(vec3 pos);

	// default material color and shader
	Material material = Material(Material::Shader::DIFFUSE);

	virtual bool intersect(Ray* ray) = 0;
	virtual vec3 getNormal(vec3 point) = 0;
	virtual AABB* calculateAABB() = 0;

	vec3 centroid;
	AABB* bounds;
	int index;
};
