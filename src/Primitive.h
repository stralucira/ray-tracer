#pragma once

#include "template.h"

class Primitive
{
public:
	Material* mat = nullptr;



	virtual ~Primitive()
	{
	}

	Material* getMaterial() const
	{
		return this->mat;
	}

	virtual bool intersect(Ray ray) = 0;
    virtual float3 getNormal(float3 point) = 0;
};
