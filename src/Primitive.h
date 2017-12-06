#pragma once
#include "Ray.h"
#include "Material.h"

class Primitive
{
public:
	Primitive(vec3 pos);
	vec3 GetPosition();

	// default material color and shader
	Material material = Material(vec3(1.0f, 0.0f, 0.0f), Material::Shader::DIFFUSE);

	virtual bool intersect(Ray* ray) = 0;
	virtual vec3 getNormal(vec3 point) = 0;

	vec3 pos;
};
