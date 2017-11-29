#pragma once

#include "template.h"

class Primitive
{
public:
	Material* mat;

	virtual ~Primitive()
	{
	}

	Material* getMaterial() const
	{
		return this->mat;
	}
    
    virtual float3 getNormal(float3 point) = 0;
    
    virtual float3 intersect( Ray ray ) = 0;
    
};
