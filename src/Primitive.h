#pragma once
//#include "template.h"
#include "Ray.h"
#include "Material.h"

class Primitive
{
public:
	Material material = Material(float3(1.0f, 0.0f, 0.0f), Material::Shader::DIFFUSE);

	virtual bool intersect(Ray* ray) = 0;
    virtual float3 getNormal(float3 point) = 0;

	//void Init(float3 pos);
};
