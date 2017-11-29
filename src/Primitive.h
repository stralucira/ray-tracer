#pragma once

#include "template.h"

class Primitive
{
protected:
	Material* mat;
public:
	virtual ~Primitive()
	{
	}

	Material* getMaterial() const
	{
		return this->mat;
	}
    
    float3 getNormal(float3 point);
};
