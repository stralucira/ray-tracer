#pragma once
#include "Ray.h"
#include "Primitive.h"

class BVH
{
public:
	void Traverse(Ray* ray);
};